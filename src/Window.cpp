/**
 * @file Window.cpp
 * @brief Code for the Window class, which is your portal to the graphics world!
 *
 * This file is all about setting up and managing the main window of your application.
 * Think of a Window as the canvas where all your 3D graphics are drawn.
 * It's the thing you see on your screen - the frame that contains your game or application.
 * This class uses GLFW (Graphics Library Framework) to handle the low-level window creation and event handling,
 * so you can focus on the fun stuff: rendering and game logic!
 */

 #include <Window.h>


 /**
  * @brief Constructor for the Window class. This is the "window builder"!
  *
  * When you create a Window object, this constructor is like laying the foundation and building the frame of your application's window.
  * It's where all the initial setup for the window happens, getting it ready to be displayed on your screen.
  *
  * @param width  The initial width of the window, in pixels. Think of this as setting the horizontal size of your canvas.
  * @param height The initial height of the window, in pixels.  Think of this as setting the vertical size of your canvas.
  * @param title  The text that will appear in the window's title bar.  This is like naming your window so you know what it is!
  *
  * @note This constructor performs several crucial steps to get your window up and running:
  *       - **Initializes GLFW:**  GLFW is the library we're using to create and manage windows. This step gets GLFW ready to work.
  *       - **Sets GLFW Window Hints:**  These hints tell GLFW about the OpenGL context we want. We're specifying that we want OpenGL version 3.3 (Core Profile), which is a modern and widely supported version.
  *       - **Creates the GLFW Window:** This is where the actual window is created using `glfwCreateWindow`. If this fails, it means something went wrong, and we can't create a window.
  *       - **Makes the Context Current:** OpenGL commands need to be executed in the context of a specific window. This step makes our newly created window the "active" window for OpenGL commands.
  *       - **Initializes GLAD:** GLAD is a library that helps us access OpenGL functions. This step makes sure we can use all the cool features of OpenGL.
  *       - **Sets the Viewport:** The viewport is the area within the window where OpenGL will draw. We're setting it to cover the entire window initially.
  *       - **Sets the Framebuffer Size Callback:** We set up a function (`framebuffer_size_callback`) that GLFW will call whenever the window is resized. This allows us to update the viewport when the window size changes, so our drawing always fits the window.
  *
  * @throws std::runtime_error If GLFW initialization, window creation, or GLAD initialization fails.
  */
 Window::Window(int width, int height, const char* title)
     : width(width), height(height), title(title) ///< Initialize member variables with provided parameters.
 {
     // ====[ Initialize GLFW -  "Get the Window-Making Tools Ready" ]====
     if (!glfwInit()) // Initialize GLFW. Returns GLFW_TRUE if successful, GLFW_FALSE if failed.
     {
         throw std::runtime_error("Failed to initialize GLFW"); // If GLFW initialization fails, throw an error.  We can't continue without GLFW.
     }

     // ====[ Set GLFW Window Hints -  "Tell GLFW what kind of Window we want" ]====
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);   // Request OpenGL context version 3.x (major version 3)
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);   // Request OpenGL context version x.3 (minor version 3), so we are asking for version 3.3
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use the modern Core Profile of OpenGL (no outdated features)

     // ====[ Create the GLFW Window -  "Actually Build the Window Frame" ]====
     window = glfwCreateWindow(width, height, title, nullptr, nullptr);
     // glfwCreateWindow: Creates the window.
     // - width, height: Initial window dimensions.
     // - title:  Window title bar text.
     // - nullptr, nullptr:  For fullscreen mode and window sharing (not used here).
     // Returns: GLFWwindow* pointer to the created window object, or nullptr if creation failed.

     if (window == nullptr) // Check if window creation was successful.
     {
         glfwTerminate(); // If window creation failed, clean up GLFW resources (in case GLFW partially initialized).
         throw std::runtime_error("Failed to create GLFW window"); // Throw an error to indicate window creation failure.
     }

     // ====[ Make Context Current - "Focus OpenGL on our New Window" ]====
     glfwMakeContextCurrent(window); // Make the OpenGL context of our 'window' the current context.
     // All subsequent OpenGL commands will now be directed to this window's context.

     // ====[ Initialize GLAD - "Load OpenGL Function Pointers" ]====
     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Initialize GLAD using GLFW's function pointer loading mechanism.
     {
         throw std::runtime_error("Failed to initialize GLAD"); // If GLAD initialization fails, throw an error.  We need GLAD to use OpenGL functions.
     }

     // ====[ Set Initial Viewport -  "Define the Drawing Area inside the Window" ]====
     glViewport(0, 0, width, height); // Set the initial OpenGL viewport to cover the entire window.
     // (x, y, width, height): Defines the rectangular area within the window where rendering will occur. (0,0) is bottom-left.

     // ====[ Set Framebuffer Size Callback - "Prepare for Window Resizing" ]====
     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Set the callback function to handle window resize events.
     // Whenever the window's framebuffer size changes (e.g., user resizes the window), GLFW will call 'framebuffer_size_callback'.
 }


 /**
  * @brief Checks if the window should close. "Is it time to close the portal?"
  *
  * This function asks GLFW whether the user has requested to close the window (e.g., by clicking the close button).
  * It's how you check if your application should shut down.
  *
  * @return `true` if the window should close, `false` otherwise.
  */
 bool Window::windowShouldClose()
 {
     return glfwWindowShouldClose(window); // Query GLFW if the window has been instructed to close.
     // glfwWindowShouldClose(window) returns GLFW_TRUE if the close button was pressed, GLFW_FALSE otherwise.
 }

 /**
  * @brief Swaps the front and back buffers. "Show the completed frame!"
  *
  * In double-buffered rendering (which is the standard), you draw to a "back buffer" while the "front buffer" is being displayed on the screen.
  * Once you've finished drawing a frame, you "swap buffers". This makes the back buffer (which now contains your newly drawn frame) become the front buffer,
  * and the old front buffer becomes the new back buffer, ready for the next frame to be drawn.
  * This swapping prevents flickering and tearing in your animation.
  */
 void Window::swapBuffers()
 {
     glfwSwapBuffers(window); // Swap the front and back buffers of the window.  Makes the rendered image visible.
 }

 /**
  * @brief Polls for and processes events. "Listen for user actions!"
  *
  * This function tells GLFW to check for any events that have occurred (like keyboard presses, mouse movements, window resizing, etc.)
  * and process them.  It's essential to call this regularly in your main loop to keep your application responsive to user input and window events.
  */
 void Window::pollEvents()
 {
     glfwPollEvents(); // Process pending GLFW events.  Handles user input and window events.
 }


 /**
  * @brief Framebuffer size callback function.  "Resize the drawing area!"
  *
  * This is a static function that gets called by GLFW whenever the window's framebuffer size changes (e.g., due to user resizing).
  * It's crucial for ensuring that your OpenGL rendering always matches the current window dimensions.
  *
  * @param window  The GLFW window that was resized (we don't actually use this parameter in this simple callback, but it's required by GLFW).
  * @param width   The new width of the framebuffer, in pixels.
  * @param height  The new height of the framebuffer, in pixels.
  *
  * @note  Inside this callback, we update the OpenGL viewport to match the new window dimensions. This ensures that OpenGL knows the new drawing area.
  *        It's important to set the viewport whenever the framebuffer size changes, otherwise, your rendering might be stretched or cropped.
  */
 // sort of like utils - This callback function is a utility for handling window resizing.
 void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
 {
     glViewport(0, 0, width, height); // Update the OpenGL viewport to the new window dimensions.
     // When the window is resized, we need to tell OpenGL to adjust its drawing area to match the new size.
 }

 /**
  * @brief Processes user input. "React to user commands!"
  *
  * This function checks for keyboard input and performs actions based on it.
  * In this case, it checks if the Escape key is pressed. If it is, it tells GLFW to close the window.
  * You would typically expand this function to handle various keyboard and mouse inputs for your application.
  */
 void Window::processInput()
 {
     if (window && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Check if window exists AND Escape key is currently pressed.
     {
         glfwSetWindowShouldClose(window, true); // If Escape is pressed, tell GLFW to close the window.
         // Setting windowShouldClose to true will cause glfwWindowShouldClose() to return true in the main loop, ending the application.
     }
 }

 /**
  * @brief Destructor for the Window class. "Close the portal and clean up!"
  *
  * When a Window object is no longer needed, this destructor is automatically called.
  * It's responsible for cleaning up all the resources associated with the window, especially GLFW resources.
  * This prevents memory leaks and ensures a clean shutdown of your application.
  *
  * @note This destructor does the following cleanup:
  *       - **Destroys the GLFW window:**  Releases the window resources managed by GLFW.
  *       - **Terminates GLFW:**  Shuts down the GLFW library, releasing any resources GLFW itself is using.
  */
 // Destructor - Called automatically when a Window object goes out of scope or is deleted.
 Window::~Window()
 {
     // ====[ Destroy GLFW Window - "Close the Window Frame" ]====
     glfwDestroyWindow(window); // Destroy the GLFW window object, releasing window resources.

     // ====[ Terminate GLFW - "Shutdown the Window-Making Tools" ]====
     glfwTerminate(); // Terminate GLFW, releasing all allocated GLFW resources.  Clean shutdown of GLFW library.
 }