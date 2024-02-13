#include "simpleEngineCore/application.hpp"
#include "simpleEngineCore/log.hpp"
#include "simpleEngineCore/window.hpp"

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

		while (true) {
			pWindow->onUpdate();
			onUpdate();
		}

        return 0;
	}

}