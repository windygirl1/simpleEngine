#pragma once

#include "simpleEngineCore/event.hpp"
 
#include <string>
#include <functional>

struct GLFWwindow;

namespace SimpleEngine {

	class Window {
	public:
		using eventCallback = std::function<void(BaseEvent&)>;

		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void onUpdate();

		unsigned int getWidth() const { return m_data.width; }
		unsigned int getHeight() const { return m_data.height; }

		void setEventCallback(const eventCallback& callback) {
			m_data.eventCallbackfn = callback;
		}

	private:
		struct windowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			eventCallback eventCallbackfn;
		};


		int init();
		void shutdown();

		GLFWwindow* pWindow = nullptr;
		
		windowData m_data;

	};

}