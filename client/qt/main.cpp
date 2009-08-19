/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QApplication>
#include <stdexcept>
#include "exception.h"
#include "application.h"
#include "workspace.h"
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
