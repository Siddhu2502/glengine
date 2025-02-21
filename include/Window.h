#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * @file Window.h
 * @brief Header file for the Window class, managing the application window.
 *
 * Defines the `Window` class, responsible for creating and managing the application window using GLFW.
 * This class encapsulates window creation, event handling (like resizing and input), and buffer swapping,
 * providing a simple interface for managing the main application window.
 */

/**
 * @class Window
 * @brief Manages the application window using GLFW.
 *
 * The `Window` class handles the creation and management of the application window using the GLFW library.
 * It initializes GLFW, creates a window with specified dimensions and title, sets up OpenGL context,
 * and provides methods for checking window state, swapping buffers, polling events, and processing input.
 * It also sets up a framebuffer size callback to handle window resizing.
 */
class Window
{
public:
    /**
     * @brief Constructor for the Window class. Creates and initializes the application window.
     *
     * This constructor sets up the application window using GLFW. It performs the following steps:
     *  - Initializes GLFW.
     *  - Sets GLFW window hints for OpenGL context version and profile.
     *  - Creates a GLFW window with the specified width, height, and title.
     *  - Makes the OpenGL context current for the newly created window.
     *  - Initializes GLAD to load OpenGL function pointers.
     *  - Sets the initial viewport size to match the window dimensions.
     *  - Sets the framebuffer size callback function to handle window resizing.
     *
     * @param width The initial width of the window in pixels.
     * @param height The initial height of the window in pixels.
     * @param title The title to be displayed in the window's title bar.
     *
     * @throws std::runtime_error If GLFW initialization, window creation, or GLAD initialization fails.
     *
     * @note After successful construction, the `Window` object represents a fully initialized and ready-to-use application window.
     */
    Window(int width, int height, const char* title);

    /**
     * @brief Destructor for the Window class. Destroys the window and terminates GLFW.
     *
     * The destructor is responsible for releasing resources associated with the `Window` object.
     * It destroys the GLFW window and terminates the GLFW library, cleaning up any resources allocated by GLFW.
     */
    ~Window();

    /**
     * @brief Checks if the window has been requested to close.
     *
     * This method queries GLFW to determine if the window should close, typically because the user has clicked the close button
     * or initiated a close action.
     *
     * @return bool `true` if the window should close, `false` otherwise.
     */
    bool windowShouldClose();

    /**
     * @brief Swaps the front and back buffers to display the rendered frame.
     *
     * In double-buffered rendering, drawing is done to a back buffer. This method swaps the back buffer with the front buffer,
     * making the rendered frame visible on the screen. This is essential for animation and smooth rendering.
     */
    void swapBuffers();

    /**
     * @brief Polls for and processes window events.
     *
     * This method tells GLFW to check for and process any pending window events, such as keyboard and mouse input,
     * window resizing, and other system events. Calling this method regularly in the main loop is crucial for
     * the application to be responsive to user input and window events.
     */
    void pollEvents();

    /**
     * @brief Processes user input for the window.
     *
     * This method handles user input, such as keyboard presses. Currently, it checks for the Escape key press, and if pressed,
     * sets the window to close, signaling the application to exit. This method can be expanded to handle more complex input scenarios.
     *
     * @note Currently, it only handles the Escape key to close the window.
     */
    void processInput();

    /**
     * @brief Framebuffer size callback function. Handles window resize events.
     *
     * This static callback function is registered with GLFW to be called whenever the window's framebuffer size changes (e.g., due to user resizing).
     * It updates the OpenGL viewport to match the new framebuffer dimensions, ensuring that rendering is scaled correctly to the window size.
     *
     * @param window  The GLFW window that was resized.
     * @param width   The new width of the framebuffer in pixels.
     * @param height  The new height of the framebuffer in pixels.
     *
     * @note This is a static function as required by GLFW callback function signatures.
     */
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
    int width, height;       ///< Window width and height in pixels.
    const char* title;       ///< Window title bar text.
    GLFWwindow* window;      ///< GLFW window object handle.
};

#endif