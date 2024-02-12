#include "simpleEngineCore/application.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

namespace SimpleEngine {
	Application::Application() {

	}

	Application::~Application() {

	}

	int Application::start(unsigned int windowWidth, unsigned int windowHeight, const char* title) {
        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(windowWidth, windowHeight, title, nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
           // glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
            onUpdate();

        }

        glfwTerminate();
        return 0;
	}

}