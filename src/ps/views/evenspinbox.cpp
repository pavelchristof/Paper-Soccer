#include "evenspinbox.hpp"
#include <QtWidgets/QLineEdit>

namespace ps {

EvenSpinBox::EvenSpinBox(QWidget* parent)
	: QSpinBox(parent)
{
	setSingleStep(2);
}

QValidator::State EvenSpinBox::validate(QString& input, int& pos) const
{
	// Filter out obviously unacceptable states.
	QValidator::State state = QSpinBox::validate(input, pos);
	if (state != QValidator::Acceptable) {
		return state;
	}

	// Filter out obviously acceptable states.
	int value = valueFromText(input);
	if (value % 2 == 0) {
		return QValidator::Acceptable;
	}

	// Now we are left with either an invalid even number or 
	// an intermediate number that is a prefix of some valid number.
	if (value * 10 > maximum()) {
		return QValidator::Invalid;
	}

	return QValidator::Intermediate;
}

}
