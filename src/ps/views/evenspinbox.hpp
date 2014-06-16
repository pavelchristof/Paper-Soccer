#ifndef PS_VIEWS_EVENSPINBOX_HPP
#define PS_VIEWS_EVENSPINBOX_HPP

#include "../maybe.hpp"

#include <QtWidgets/QSpinBox>

namespace ps
{

class EvenSpinBox : public QSpinBox
{
public:
	 EvenSpinBox(QWidget* parent = nullptr);

protected:
	QValidator::State validate(QString& input, int& pos) const override;
};

}

#endif // PS_VIEWS_EVENSPINBOX_HPP
