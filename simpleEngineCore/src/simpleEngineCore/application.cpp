#include "simpleEngineCore/application.hpp"
#include "simpleEngineCore/log.hpp"
#include "simpleEngineCore/window.hpp"
#include "SimpleEngineCore/Event.hpp"

#include <iostream>

namespace SimpleEngine {
	Application::Application() {
		LOG_INFO("Starting application");
	}

	Application::~Application() {
		LOG_INFO("Closing application");
	}

	int Application::start(unsigned int windowWidth, unsigned int windowHeight, const char* title) {
        
		pWindow = std::make_unique<Window>(title, windowWidth, windowHeight);

		eventDispatcher.addEventListener<EventMouseMoved>(
			[](EventMouseMoved& e) {
				LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", e.x, e.y);
			}
		);

		eventDispatcher.addEventListener<EventWindowResize>(
			[](EventWindowResize& e) {
				LOG_INFO("[Resized] changed size to {0}x{1}", e.width, e.height);
			}
		);

		eventDispatcher.addEventListener<EventWindowClose>(
			[&](EventWindowClose& e) {
				LOG_INFO("[WindowClose]");
				bCloseWindow = true;
			}
		);

		pWindow->setEventCallback(
			[&](BaseEvent& e) {
				eventDispatcher.dispatch(e);
			}
		);

		while (!bCloseWindow) {
			pWindow->onUpdate();
			onUpdate();
		}
		pWindow = nullptr;

        return 0;
	}

}