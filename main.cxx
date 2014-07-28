/*
Copyright 2013 BIT OTISMED
*/

// QT includes
#include <QApplication>
#include <QCleanlooksStyle>
#include <QDesktopWidget>

#include "SimpleView.h"

extern int qInitResources_icons();

int main( int argc, char** argv )
{
	// QT Stuff
	QApplication app( argc, argv );
	QApplication::setStyle(new QCleanlooksStyle);
	QDesktopWidget *pDesk = QApplication::desktop();
	qInitResources_icons();
	SimpleView mySimpleView;
	mySimpleView.show();
	mySimpleView.move ((QApplication::desktop()->width() - mySimpleView.width())/2,(QApplication::desktop()->height() - mySimpleView.height())/2);
	return app.exec();
}
