#ifndef PS_CONTROLLERS_CONTROLLER_HPP
#define PS_CONTROLLERS_CONTROLLER_HPP

#include <QtCore/QObject>

namespace ps {

class Application;

class Controller : public QObject
{
	Q_OBJECT

public:
	virtual void setup(Application* app) = 0;
	virtual void activate(Application* app) = 0;
	virtual void deactivate(Application* app) = 0;
};

}

#endif // PS_CONTROLLERS_CONTROLLER_HPP
