#ifndef PS_VIEWS_EDITORVIEW_HPP
#define PS_VIEWS_EDITORVIEW_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QCommandLinkButton>

class Ui_EditorView;

namespace ps
{

class GameConfig;
class PlayerSwitch;
class BoardView;

class EditorView : public QWidget
{
public:
	EditorView(QWidget* parent = nullptr, Qt::WindowFlags f = 0);

	BoardView* boardView();
	PlayerSwitch* playerSwitch();
	QCommandLinkButton* playButton();

	GameConfig* config() const;
	void setConfig(GameConfig* config);

private:
	Ui_EditorView* ui;
	GameConfig* config_;
};

} // namespace ps

#endif // PS_VIEWS_EDITORVIEW_HPP
