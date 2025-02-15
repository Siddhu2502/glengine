#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
    public:
        Window(int width, int height, const char* title);
        ~Window();

        bool windowShouldClose();  
        void swapBuffers();        
        void pollEvents();         
        void processInput();       

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    private:
        int width, height;
        const char* title;
        GLFWwindow* window;
};

#endif