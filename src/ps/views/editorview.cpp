#include "editorview.hpp"
#include "../models/gameconfig.hpp"
#include "ui_editorview.h"

namespace ps {

EditorView::EditorView(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
	, ui(new Ui_EditorView)
	, config_(nullptr)
{
	ui->setupUi(this);

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
}

BoardView* EditorView::boardView()
{
	return ui->boardView;
}

PlayerSwitch* EditorView::playerSwitch()
{
	return ui->playerSwitch;
}

QCommandLinkButton* EditorView::playButton()
{
	return ui->playButton;
}

void EditorView::setConfig(GameConfig* config)
{
	config_ = config;
	if (config) {
		ui->playerOneHuman->setChecked(config->isPlayerHuman(Player::One));
		ui->playerOneAI->setChecked(!config->isPlayerHuman(Player::One));
		ui->playerTwoHuman->setChecked(config->isPlayerHuman(Player::Two));
		ui->playerTwoAI->setChecked(!config->isPlayerHuman(Player::Two));
	}
}

GameConfig* EditorView::config() const
{
	return config_;
}

} // namespace ps
