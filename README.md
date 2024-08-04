# MovieApp
Overview
MovieApp is a desktop application that allows users to search for movies, view movies by genre, and manage a list of favorite movies. The app utilizes the The Movie Database (TMDb) API for fetching movie details and posters.

Features
  * Search Movies: Search for movies by title.
  * View by Genre: Browse movies by genre.
  * Favorites Management: Add or remove movies from a favorites list,         with a maximum of 10 favorites.
  * Fetch and Display Posters: Download and display movie posters.
Technologies Used
  * C++: Core programming language.
  * GLFW: Library for creating windows and handling input.
  * GLEW: OpenGL Extension Wrangler Library.
  * ImGui: Immediate Mode Graphical User Interface.
  * JSON for Modern C++: For parsing JSON responses from the TMDb API.
  * httplib: Simple HTTP client.
  * stb_image: For loading images.
    
Getting Started
Prerequisites
  * C++ compiler (e.g., g++, clang++)
  * CMake
  * Libraries: GLFW, GLEW, ImGui, stb_image, httplib, JSON for Modern C++
  
  Installation
  1.Clone the Repository
    bash
    git clone https://github.com/yourusername/MovieApp.git
    cd MovieApp
  
  2.Configure and Build
    bash
    mkdir build
    cd build
    cmake ..
    make
  
  3.Run the Application
    bash
    ./MovieApp

Configuration
  * API Key: Obtain an API key from TMDb and save it in a file named     
              config.txt under the assets/config/ directory.

Usage
Main Interface
  * Home Screen: Navigate to different sections: Search, Favorites,   Genres.
  * Search Movies: Enter a movie title and search.
  * Favorites: View, add, or remove favorite movies.
  * Genres: Select a genre and view movies.
  
Key Functions
  * addMovieToFavorites(const Movie& movie): Adds a movie to the       favorites list.
  * removeMovieFromFavorites(const Movie& movie): Removes a movie from the favorites list.
  * getMoviesByGenre(int genreId, const std::string& api_key, int limit = 10): Fetches movies by genre.
  * searchMovies(const std::string& query, const std::string& api_key): Searches for movies by title.
  * fetchMoviePoster(const std::string& posterPath): Downloads a movie poster.
  * displayMainInterface(const std::string& api_key): Displays the main application interface.
  
Code Structure
  * MovieApp.h: Header file containing includes, struct definitions, and function prototypes.
  * MovieApp.cpp: Implementation file containing the main logic and functions.
  * main.cpp: Entry point of the application, sets up the environment and runs the main loop.
  
Contributing
  Fork the repository.
  Create a new branch.
  Make your changes.
  Submit a pull request.
  
License
  This project is licensed under the MIT License.

Acknowledgements
  TMDb API
  ImGui
  GLFW
  GLEW
  JSON for Modern C++
  httplib
  stb_image
  
  Contact
    For any inquiries or feedback, please contact moshehakakian@gmail.com

Video presentation of the project

https://github.com/user-attachments/assets/b7b0512a-a1b7-4608-bccd-86cb6933f1ea

