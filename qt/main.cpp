#include <QApplication>
#include <stdexcept>
#include "exception.h"
#include "application.h"
#include "document.h"
#include "app_main.h"

int main(int argc, char **argv)
{
	QApplication Q(argc, argv);
	app_main();

	QTApplication *D = new QTApplication();
	D->show();
	
	int ret = 1;
	try {
		ret = Q.exec();
	} catch (Exception &e) {
		fprintf(stderr, "Fatal error: %s\n", e.getMessage());
		e.printStackTrace();
	} catch (std::exception &e) {
		fprintf(stderr, "Fatal exception %s\n", e.what());
	}
	return ret;
}
