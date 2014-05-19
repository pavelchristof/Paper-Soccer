#include "Paper Soccer.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>

Paper_Soccer::Paper_Soccer()
{
    QLabel* label = new QLabel( this );
    label->setText( "Hello World!" );
    setCentralWidget( label );
    QAction* action = new QAction(this);
    action->setText( "Quit" );
    connect(action, SIGNAL(triggered()), SLOT(close()) );
    menuBar()->addMenu( "File" )->addAction( action );
}

Paper_Soccer::~Paper_Soccer()
{}

#include "Paper Soccer.moc"
