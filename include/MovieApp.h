#ifndef MOVIEAPP_H
#define MOVIEAPP_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <mutex>
#include <thread>
#include <json.hpp> // JSON parsing library
#include <httplib.h> // HTTP client library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image.h"


// Struct for storing movie details
struct Movie {
    std::string title;
    std::string releaseDate;
    std::string overview;
    double rating;
    std::string posterPath;
    bool favorite;
};

// Function prototypes
void addMovieToFavorites(const Movie& movie);
void removeMovieFromFavorites(const Movie& movie);
void saveFavorites();
void loadFavorites();
std::vector<Movie> getMoviesByGenre(int genreId, const std::string& api_key, int limit = 10);
std::vector<Movie> searchMovies(const std::string& query, const std::string& api_key);
void displaySearchInterface(const std::string& api_key);
void displayGenreInterface(const std::string& api_key);
void displayFavoritesInterface();
void initializeApplication();
void displayMainInterface(const std::string& api_key);
GLuint loadTexture(const std::string& filePath);
bool fetchMoviePoster(const std::string& posterPath);
void displayMoviePoster(const std::string& posterPath);

#endif // MOVIEAPP_H
