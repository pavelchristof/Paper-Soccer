#include <QtWidgets/QApplication>
#include "shape.hpp"
#include <iostream>

using namespace ps;
using namespace std;

int main(int argc, char** argv)
{
	Shape<int, int> shape(make_tuple(make_pair(-5, 3), make_pair(8, 16)));
	cout << shape.size() << endl;
	
	for (int x = -5; x <= 3; ++x) {
		for (int y = 8; y <= 16; ++y) {
			cout << shape.map(make_tuple(x, y)) << endl;
		}
	}

    QApplication app(argc, argv);
    return app.exec();
}
