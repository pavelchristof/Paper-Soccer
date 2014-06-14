#ifndef PS_CONTROLLERS_WELCOMECONTROLLER_HPP
#define PS_CONTROLLERS_WELCOMECONTROLLER_HPP

#include "controller.hpp"

namespace ps
{

class WelcomeController : public Controller
{
	Q_OBJECT

public:
	void setup(Application* app) override;
	void activate(Application* app) override;
	void deactivate(Application* app) override;
};

}

#endif // PS_CONTROLLERS_WELCOMECONTROLLER_HPP
