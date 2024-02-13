#include "simpleEngineCore/window.hpp"
#include "simpleEngineCore/log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace SimpleEngine {

    static bool GLFW_initialized = false;

	Window::Window(
		std::string title, 
		const unsigned int width, 
		const unsigned int height) 
			: m_title(std::move(title)), 
			m_width(width), 
			m_height(height) {

		int resCode = init();
	}

	Window::~Window() {
		shutdown();
	}

	int Window::init() {
        LOG_INFO("Creating window {0} width size {1}x{2}", m_title, m_width, m_height);

        if (!GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRIT("Can`t initialize GLFW");
                return -1;
           }
            GLFW_initialized = true;
        }

        pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!pWindow)
        {
            LOG_CRIT("Can`t create the window{0} width size {1}x{2}", m_title, m_width, m_height);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRIT("FAILED TO INITIALIZE GLAD");
            glfwTerminate();
            return -3;
        }

        return 0;
        
	}
	void Window::shutdown() {
        glfwDestroyWindow(pWindow);
        glfwTerminate();
	}

	void Window::onUpdate() {

        glClearColor(0.1, 0.2, 0.3, 1.0);


        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(pWindow);

        glfwPollEvents();
	}
}