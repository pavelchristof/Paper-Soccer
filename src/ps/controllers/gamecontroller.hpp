#ifndef PS_CONTROLLERS_GAMECONTROLLER_HPP
#define PS_CONTROLLERS_GAMECONTROLLER_HPP

#include "controller.hpp"

namespace ps
{

class GameController : public Controller
{
public:
	void setup(Application* app) override;
	void activate(Application* app) override;
	void deactivate(Application* app) override;
};

}

#endif // PS_CONTROLLERS_GAMECONTROLLER_HPP
