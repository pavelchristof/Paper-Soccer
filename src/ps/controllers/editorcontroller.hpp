#ifndef PS_CONTROLLERS_EDITORCONTROLLER_HPP
#define PS_CONTROLLERS_EDITORCONTROLLER_HPP

#include "controller.hpp"
#include "../maybe.hpp"
#include "../models/edge.hpp"

namespace ps
{

class Board;
class EditorView;

class EditorController : public Controller
{
	Q_OBJECT
	
public:
	void setup(Application* app) override;
	void activate() override;
	void deactivate() override;

private:
	Application* app;
	EditorView* view;

	// -----------------
	// Utility functions

	Board* board();

	/**
	 * Returns the edge from @a start to @a end if it is valid and flippable.
	 */
	Maybe<Edge> flippableEdge(QPoint start, QPoint end);

	void flipEdge(QPoint start, QPoint end);

	// ---------------
	// Signal handlers

	void switchPlayer();
	void play();
	void pointClicked(QPoint point, Qt::MouseButton button);
	void pointMouseEnter(QPoint point);
	void pointMouseLeave(QPoint point);
};

} // namespace ps

#endif // PS_CONTROLLERS_EDITORCONTROLLER_HPP
