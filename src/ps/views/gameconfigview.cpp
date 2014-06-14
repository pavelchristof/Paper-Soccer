#include "gameconfigview.hpp"
#include "evenspinbox.hpp"
#include "../models/gameconfig.hpp"
#include "ui_gameconfig.h"

namespace ps {

GameConfigView::GameConfigView(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
	, ui(new Ui_GameConfig)
{
	ui->setupUi(this);
	ui->widthBox->setMinimum(GameConfig::minSize);
	ui->widthBox->setMaximum(GameConfig::maxSize);
	ui->heightBox->setMinimum(GameConfig::minSize);
	ui->heightBox->setMaximum(GameConfig::maxSize);

	connect(ui->widthBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this] (int value) {
		if (config()) {
			config()->setWidth(value);
		}
	});
	connect(ui->heightBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this] (int value) {
		if (config()) {
			config()->setHeight(value);
		}
	});

	connect(ui->playerOneHuman, &QRadioButton::clicked, [this] () {
		if (config()) {
			config()->setPlayerHuman(Player::One, true);
		}
	});
	connect(ui->playerOneAI, &QRadioButton::clicked, [this] () {
		if (config()) {
			config()->setPlayerHuman(Player::One, false);
		}
	});
	connect(ui->playerTwoHuman, &QRadioButton::clicked, [this] () {
		if (config()) {
			config()->setPlayerHuman(Player::Two, true);
		}
	});
	connect(ui->playerTwoAI, &QRadioButton::clicked, [this] () {
		if (config()) {
			config()->setPlayerHuman(Player::Two, false);
		}
	});

	connect(ui->cancelButton, &QPushButton::clicked, this, &GameConfigView::cancelClicked);
	connect(ui->startGameButton, &QPushButton::clicked, this, &GameConfigView::startGameClicked);
}

GameConfig* GameConfigView::config() const
{
	return config_;
}

void GameConfigView::setConfig(GameConfig* config)
{
	config_ = config;
	if (config) {
		ui->widthBox->setValue(config->width());
		ui->heightBox->setValue(config->height());
		ui->playerOneHuman->setChecked(config->isPlayerHuman(Player::One));
		ui->playerOneAI->setChecked(!config->isPlayerHuman(Player::One));
		ui->playerTwoHuman->setChecked(config->isPlayerHuman(Player::Two));
		ui->playerTwoAI->setChecked(!config->isPlayerHuman(Player::Two));
	}
}

} // namespace ps
