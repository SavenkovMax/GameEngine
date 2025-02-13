#include <iostream>
#include <memory>

#include "core/Application.hpp"

class MyApp : public engine::Application {
	virtual void OnUpdate() override {
		//    std::cout << "Update frame: " << frame++ << std::endl;
	}

	int frame = 0;
};


int main() {
	auto myApp = std::make_unique<MyApp>();

	int returnCode = myApp->Start(1024, 768, "My first App");

	std::cin.get();

	return returnCode;
}