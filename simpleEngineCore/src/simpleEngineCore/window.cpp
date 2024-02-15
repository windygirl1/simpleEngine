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
        : m_data({ std::move(title), width, height }) {
		    int resCode = init();
	}

	Window::~Window() {
		shutdown();
	}

	int Window::init() {
        LOG_INFO("Creating window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);

        if (!GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRIT("Can`t initialize GLFW");
                return -1;
           }
            GLFW_initialized = true;
        }

        pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!pWindow)
        {
            LOG_CRIT("Can`t create the window{0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRIT("FAILED TO INITIALIZE GLAD");
            glfwTerminate();
            return -3;
        }

        glfwSetWindowUserPointer(pWindow, &m_data);

        glfwSetWindowSizeCallback(pWindow, [](GLFWwindow* pWindow, int width, int height) {

            windowData& data = *static_cast<windowData*>(glfwGetWindowUserPointer(pWindow));

            data.width = width;
            data.height = height;

            EventWindowResize wR(width, height);
            data.eventCallbackfn(wR);
        });

        glfwSetCursorPosCallback(pWindow, [](GLFWwindow* pWindow, double x, double y) {
            windowData& data = *static_cast<windowData*>(glfwGetWindowUserPointer(pWindow));

            EventMouseMoved mM(x, y);
            data.eventCallbackfn(mM);
        });

        glfwSetWindowCloseCallback(pWindow, [](GLFWwindow* pWindow) {
            windowData& data = *static_cast<windowData*>(glfwGetWindowUserPointer(pWindow));

            EventWindowClose wC;
            data.eventCallbackfn(wC);
        });

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