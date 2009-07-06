#include <QApplication>
#include "application.h"
#include "document.h"
#include "app_main.h"

int main(int argc, char **argv)
{
	QApplication Q(argc, argv);
	app_main();

	QTApplication *D = new QTApplication();
	D->show();
	
	return Q.exec();
}
