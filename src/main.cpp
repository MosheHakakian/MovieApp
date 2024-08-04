#include "MovieApp.h"

void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

// Function to read API key from a file
std::string readApiKeyFromFile(const std::string& filePath) {
    std::ifstream keyFile(filePath);
    std::string key;
    if (keyFile.is_open()) {
        std::getline(keyFile, key);
        keyFile.close();
    }
    else {
        std::cerr << "Unable to open API key file: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }
    return key;
}

// Main function
int main(int, char**) {
    // Initialize GLFW library
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a GLFW window with an OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Movie App", NULL, NULL);

    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Make the OpenGL context current
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW library
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsLight();

    // Setup ImGui binding for GLFW and OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Read API key from file
    std::string api_key = readApiKeyFromFile("assets/config/config.txt");

    // Initialize the application
    initializeApplication();

    // Main application loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // Process all pending events

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Retrieve the size of the GLFW window
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        // Fit ImGui window to GLFW window size
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(display_w), static_cast<float>(display_h)));

        // Create ImGui window
        ImGui::Begin("Movie App", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        // Display the main interface
        displayMainInterface(api_key);

        ImGui::End(); // End the main window

        // Render the ImGui frame
        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); // Swap front and back buffers
    }

    // Clean up and terminate the application
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
