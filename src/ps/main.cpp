#include "application.hpp"
#include "controllers/welcomecontroller.hpp"

int main(int argc, char** argv)
{
	QCoreApplication::setOrganizationName("No organization");
	QCoreApplication::setOrganizationDomain("pn347193.students.mimuw.edu.pl");
	QCoreApplication::setApplicationName("Paper soccer");
	QApplication qtApp(argc, argv);
	ps::Application app;
	return qtApp.exec();
}
