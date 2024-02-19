#include "simpleEngineCore/window.hpp"
#include "simpleEngineCore/log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace SimpleEngine {

    static bool GLFW_initialized = false;

    GLfloat points[] = {
        0.0f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f
    };

    GLfloat colors[] = {
        1.0f,  0.5f, 0.0f,
        0.0f,  1.0f, 0.0f,
        0.0f,  0.0f, 1.0f
    };

    const char* vertexShader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position, 1.0);"
        "}";

    const char* fragmentShader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        " frag_color = vec4(color, 1.0);"
        "}";

    GLuint shaderProgram;
    GLuint vao;

    Window::Window(
        std::string title,
        const unsigned int width,
        const unsigned int height)
        : m_data({ std::move(title), width, height }) {
		    int resCode = init();
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui_ImplOpenGL3_Init();
            ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
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

        glfwSetFramebufferSizeCallback(pWindow, [](GLFWwindow* pwindow, int width, int height) {
            glViewport(0, 0, width, height);
        });



        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShader, nullptr);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShader, nullptr);
        glCompileShader(fs);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);

        glLinkProgram(shaderProgram);

        glDeleteShader(vs);
        glDeleteShader(fs);

        GLuint pointsVbo = 0;
        glGenBuffers(1, &pointsVbo);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colorsVbo = 0;
        glGenBuffers(1, &colorsVbo);
        glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        return 0;
        
	}
	void Window::shutdown() {
        glfwDestroyWindow(pWindow);
        glfwTerminate();
	}

	void Window::onUpdate() {

        glClearColor(backgoundColor[0], backgoundColor[1], backgoundColor[2], backgoundColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("Background color window");
        ImGui::ColorEdit4("Background color", backgoundColor);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
	}
}