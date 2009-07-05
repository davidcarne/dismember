#include <QApplication>
#include "documentwindow.h"
#include "document.h"
#include "../app_main.h"

int main(int argc, char **argv)
{
	QApplication Q(argc, argv);
	app_main();

	DocumentWindow *D = new DocumentWindow();

	// according to the docs, this should be done before Q's construction,
	// but the application disagrees, and proceeds to spit out errors
	//Q.setStyle("plastique");

	D->show();
	
	// We own the document, rather than vise-versa. HAH!
	//Document *d = new Document();

	return Q.exec();
}
