#include <iostream>
#include <memory>
#include <GameEngineCore/Application.hpp>

class MyApp : public mge::Application {
public:
	MyApp() : frame_(0) {}
	virtual void OnUpdate() override {
		// std::cout << "Update frame: " << ++frame_ << std::endl;
	}

private:
	int frame_;
};

int main() {
	auto app = std::make_unique<MyApp>();
	app->Start(1024, 780, "Program");
	std::cin.get();
}