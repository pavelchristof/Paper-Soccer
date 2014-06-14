#ifndef PS_CONTROLLERS_GAMECONFIGCONTROLLER_HPP
#define PS_CONTROLLERS_GAMECONFIGCONTROLLER_HPP

#include "controller.hpp"

namespace ps
{

class GameConfigController : public Controller
{
	Q_OBJECT

public:
	void setup(Application* app) override;
	void activate(Application* app) override;
	void deactivate(Application* app) override;
};

}

#endif // PS_CONTROLLERS_GAMECONFIGCONTROLLER_HPP
