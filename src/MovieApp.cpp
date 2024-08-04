#include "MovieApp.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


std::set<std::string> favorites;

// Function to add a movie to favorites
// Function to add a movie to favorites with a limit of 10
void addMovieToFavorites(const Movie& movie) {
    if (favorites.size() >= 10) {
        std::cerr << "Favorites list is full. Cannot add more movies." << std::endl;
        return;
    }
    if (favorites.find(movie.title) == favorites.end()) {
        favorites.insert(movie.title);
        saveFavorites(); // Save updated favorites to file
    }
}


// Function to remove a movie from favorites
void removeMovieFromFavorites(const Movie& movie) {
    if (favorites.find(movie.title) != favorites.end()) {
        favorites.erase(movie.title);
        saveFavorites(); // Save updated favorites to file
    }
}

// Function to save favorites to a file
void saveFavorites() {
    std::ofstream outfile("favorites.txt");
    for (const auto& title : favorites) {
        outfile << title << std::endl;
    }
}

// Function to load favorites from a file
void loadFavorites() {
    std::ifstream infile("favorites.txt");
    std::string title;
    while (std::getline(infile, title)) {
        favorites.insert(title);
    }
}

// Function to fetch movies by genre from the API
std::vector<Movie> getMoviesByGenre(int genreId, const std::string& api_key, int limit) {
    httplib::Client cli("api.themoviedb.org");
    std::vector<Movie> movies;
    std::string url = "/3/discover/movie?with_genres=" + std::to_string(genreId) +
        "&api_key=" + api_key + "&page=1";

    std::cout << "Requesting URL: " << url << std::endl;

    auto res = cli.Get(url.c_str());
    if (res && res->status == 200) {
        try {
            auto data = nlohmann::json::parse(res->body);
            int totalFetched = 0;

            for (const auto& item : data["results"]) {
                if (totalFetched >= limit) break;

                Movie movie;
                movie.title = item.value("title", "Unknown");
                movie.releaseDate = item.value("release_date", "N/A");
                movie.overview = item.value("overview", "No overview available.");
                movie.rating = item.value("vote_average", 0.0);
                movie.posterPath = item.value("poster_path", "");
                movie.favorite = false;

                movies.push_back(movie);
                totalFetched++;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
        }
    }
    else {
        std::cerr << "Failed to fetch movies by genre: " << (res ? res->status : -1) << std::endl;
        if (res) {
            std::cerr << "Response body: " << res->body << std::endl;
        }
    }

    return movies;
}

// Function to search for movies by query
std::vector<Movie> searchMovies(const std::string& query, const std::string& api_key) {
    httplib::Client cli("api.themoviedb.org");
    std::string encodedQuery = httplib::detail::encode_url(query);
    std::string url = "/3/search/movie?query=" + encodedQuery + "&api_key=" + api_key;

    std::cout << "Requesting URL: " << url << std::endl; // Debugging statement

    auto res = cli.Get(url.c_str());
    std::vector<Movie> movies;
    if (res && res->status == 200) {
        try {
            auto data = nlohmann::json::parse(res->body);
            for (const auto& item : data["results"]) {
                movies.push_back({
                    item.value("title", "Unknown"),
                    item.value("release_date", "N/A"),
                    item.value("overview", "No overview available."),
                    item.value("vote_average", 0.0),
                    item.value("poster_path", ""),
                    false
                    });
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
        }
    }
    else {
        std::cerr << "Failed to fetch movies: " << (res ? res->status : -1) << std::endl;
        if (res) {
            std::cerr << "Response body: " << res->body << std::endl;
        }
    }
    return movies;
}

// Function to load textures using stb_image
GLuint loadTexture(const std::string& filePath) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return 0;
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGB;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return textureId;
}

// Function to download images
bool downloadImage(const std::string& url, const std::string& filePath) {
    httplib::Client cli("image.tmdb.org");
    auto res = cli.Get(url.c_str());
    if (res && res->status == 200) {
        std::ofstream file(filePath, std::ios::binary);
        file.write(res->body.c_str(), res->body.size());
        file.close();
        return true;
    }
    std::cerr << "Failed to download image: " << url << std::endl;
    return false;
}

// Function to fetch movie posters
bool fetchMoviePoster(const std::string& posterPath) {
    std::string localFilePath = "assets/images/" + posterPath.substr(posterPath.find_last_of("/") + 1);
    if (std::ifstream(localFilePath)) {
        return true; // File already exists
    }
    std::string url = "/t/p/w500" + posterPath; // Adjust the URL path as per TMDb guidelines
    return downloadImage(url, localFilePath);
}

// Function to display movie posters
void displayMoviePoster(const std::string& posterPath) {
    std::string localFilePath = "assets/images/" + posterPath.substr(posterPath.find_last_of("/") + 1);
    GLuint textureId = loadTexture(localFilePath);
    if (textureId) {
        ImGui::Image((void*)(intptr_t)textureId, ImVec2(100, 150)); // Adjust size as needed
    }
}

// Function to display the search interface
void displaySearchInterface(const std::string& api_key) {
    static char searchQuery[256] = "";
    static std::vector<Movie> searchResults;
    static bool loading = false;
    static std::mutex dataMutex;

    ImGui::Begin("Movie Search");
    ImGui::InputText("Search for Movies", searchQuery, IM_ARRAYSIZE(searchQuery));

    if (ImGui::Button("Search") && !loading) {
        loading = true;
        std::thread([&]() {
            std::vector<Movie> results = searchMovies(searchQuery, api_key);
            {
                std::lock_guard<std::mutex> lock(dataMutex);
                searchResults = std::move(results);
            }
            loading = false;
            }).detach();
    }

    if (loading) {
        ImGui::Text("Loading...");
    }
    else {
        ImGui::Text("Search Results:");
        std::lock_guard<std::mutex> lock(dataMutex);
        for (const auto& movie : searchResults) {
            ImGui::Text("Title: %s", movie.title.c_str());
            ImGui::Text("Release Date: %s", movie.releaseDate.c_str());
            ImGui::Text("Rating: %.1f", movie.rating);
            ImGui::TextWrapped("Overview: %s", movie.overview.c_str());

            if (fetchMoviePoster(movie.posterPath)) {
                displayMoviePoster(movie.posterPath);
            }

            if (ImGui::Button(("Add to Favorites##" + movie.title).c_str())) {
                addMovieToFavorites(movie);
            }
            ImGui::Separator();
        }
    }
    ImGui::End();
}

// Function to display the genre interface
void displayGenreInterface(const std::string& api_key) {
    static int selectedGenre = 28; // Action as default
    static std::vector<Movie> genreResults;

    // Define a map of genre ids to their names for easy lookup
    static const std::map<int, std::string> genreMap = {
        {28, "Action"},
        {35, "Comedy"},
        {18, "Drama"},
        {878, "Science Fiction"},
        {27, "Horror"}
    };

    // Set the current label using the selected genre
    const char* currentGenreName = genreMap.at(selectedGenre).c_str();

    ImGui::Begin("Movies by Genre");

    // Dropdown for genre selection
    if (ImGui::BeginCombo("Select Genre", currentGenreName)) {
        if (ImGui::Selectable("Action", selectedGenre == 28)) selectedGenre = 28;
        if (ImGui::Selectable("Comedy", selectedGenre == 35)) selectedGenre = 35;
        if (ImGui::Selectable("Drama", selectedGenre == 18)) selectedGenre = 18;
        if (ImGui::Selectable("Science Fiction", selectedGenre == 878)) selectedGenre = 878;
        if (ImGui::Selectable("Horror", selectedGenre == 27)) selectedGenre = 27;
        ImGui::EndCombo();
    }

    if (ImGui::Button("Fetch Movies")) {
        genreResults = getMoviesByGenre(selectedGenre, api_key);
    }

    ImGui::Text("Movies in Genre:");
    for (const auto& movie : genreResults) {
        ImGui::Text("Title: %s", movie.title.c_str());
        ImGui::Text("Release Date: %s", movie.releaseDate.c_str());
        ImGui::Text("Rating: %.1f", movie.rating);
        ImGui::TextWrapped("Overview: %s", movie.overview.c_str());
        if (fetchMoviePoster(movie.posterPath)) {
            displayMoviePoster(movie.posterPath);
        }
        if (ImGui::Button(("Add to Favorites##" + movie.title).c_str())) {
            addMovieToFavorites(movie);
        }
        ImGui::Separator();
    }

    ImGui::End();
}
void displayFavoritesInterface() {
    ImGui::Begin("Favorites List");

    if (favorites.empty()) {
        ImGui::Text("No favorites yet.");
    }
    else {
        std::vector<std::string> favoritesToRemove; // To store titles to remove
        for (const auto& title : favorites) {
            ImGui::Text("Title: %s", title.c_str());
            if (ImGui::Button(("Remove from Favorites##" + title).c_str())) {
                favoritesToRemove.push_back(title); // Mark for removal
            }
            ImGui::Separator();
        }
        // Perform removal outside the loop to avoid iterator invalidation
        for (const auto& title : favoritesToRemove) {
            favorites.erase(title);
        }
        if (!favoritesToRemove.empty()) {
            saveFavorites(); // Save updated list to file
        }
    }

    ImGui::End(); // Close the favorites window
}

void displayHomeInterface(std::string& currentInterface, bool& showFavoritesWindow) {
    ImGui::Begin("Home Screen");
    ImGui::Text("Welcome to the Movie App!");

    if (ImGui::Button("Search Movies")) {
        currentInterface = "Search";
    }
    if (ImGui::Button("Favorites")) {
        currentInterface = "Favorites";
        showFavoritesWindow = true; // Set to true when navigating to favorites
    }
    if (ImGui::Button("Genres")) {
        currentInterface = "Genres";
    }

    ImGui::End();
}

// Function to initialize the application and load favorites
void initializeApplication() {
    loadFavorites(); // Load the favorites when the application starts
}
void displayMainInterface(const std::string& api_key) {
    static std::string currentInterface = "Home"; // Default to Home
    static bool showFavoritesWindow = false;

    if (currentInterface == "Home") {
        displayHomeInterface(currentInterface, showFavoritesWindow);
    }
    else if (currentInterface == "Search") {
        displaySearchInterface(api_key);
        if (ImGui::Button("Back to Home")) {
            currentInterface = "Home";
        }
    }
    else if (currentInterface == "Favorites") {
        displayFavoritesInterface();
        if (ImGui::Button("Back to Home")) {
            currentInterface = "Home";
            showFavoritesWindow = false; // Reset to false when navigating back
        }
    }
    else if (currentInterface == "Genres") {
        displayGenreInterface(api_key);
        if (ImGui::Button("Back to Home")) {
            currentInterface = "Home";
        }
    }
}