#ifndef PS_CONTROLLERS_GAMECONFIGCONTROLLER_HPP
#define PS_CONTROLLERS_GAMECONFIGCONTROLLER_HPP

#include "controller.hpp"

namespace ps
{

class GameConfigView;

class GameConfigController : public Controller
{
	Q_OBJECT

public:
	void setup(Application* app) override;
	void activate() override;
	void deactivate() override;

private:
	Application* app;
	GameConfigView* view;
};

}

#endif // PS_CONTROLLERS_GAMECONFIGCONTROLLER_HPP
