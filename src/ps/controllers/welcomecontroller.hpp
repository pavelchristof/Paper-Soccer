#ifndef PS_CONTROLLERS_WELCOMECONTROLLER_HPP
#define PS_CONTROLLERS_WELCOMECONTROLLER_HPP

#include "controller.hpp"

namespace ps
{

class WelcomeView;

class WelcomeController : public Controller
{
	Q_OBJECT

public:
	void setup(Application* app) override;
	void activate() override;
	void deactivate() override;

private:
	Application* app;
	WelcomeView* view;
};

}

#endif // PS_CONTROLLERS_WELCOMECONTROLLER_HPP
