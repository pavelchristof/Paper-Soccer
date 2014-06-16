#include "gameview.hpp"
#include "boardview.hpp"
#include "historyview.hpp"
#include "playerswitch.hpp"
#include "ui_gameview.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtSvg/QSvgWidget>

namespace ps {

GameView::GameView(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
	, ui(new Ui_GameView)
{
	ui->setupUi(this);
}

BoardView* GameView::boardView()
{
	return ui->boardView;
}

HistoryView* GameView::historyView()
{
	return ui->historyView;
}

PlayerSwitch* GameView::playerSwitch()
{
	return ui->playerSwitch;
}

QWidget* GameView::hintBox()
{
	return ui->hintBox;
}

QPushButton* GameView::startHintButton()
{
	return ui->startHintButton;
}

QPushButton* GameView::stopHintButton()
{
	return ui->stopHintButton;
}

QWidget* GameView::aiBox()
{
	return ui->aiBox;
}

QPushButton* GameView::startAiButton()
{
	return ui->startAiButton;
}

QPushButton* GameView::stopAiButton()
{
	return ui->stopAiButton;
}

QCommandLinkButton* GameView::editButton()
{
	return ui->editButton;
}

} // namespace ps
