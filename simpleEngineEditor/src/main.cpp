#include <iostream>
#include <memory>
#include <simpleEngineCore/application.hpp>

class myAppClass : public SimpleEngine::Application {
	virtual void onUpdate() override {
		//std::cout << "Update frame: " << frame++ << std::endl;
	}

	int frame = 0;
};


int main() {
	
	auto myApp = std::make_unique<myAppClass>();

	
	int returnCode = myApp->start(800, 600, "HIHIHIHIH");

	return returnCode;
}