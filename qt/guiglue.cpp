/*
 *  guiglue.cpp
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "guiglue.h"
#include "documentwindow.h"
#include "document.h"

DocumentGui * setupDocumentGui(Document & doc)
{ 
	DocumentWindow *frame = new DocumentWindow(doc);
	frame->show();
	return frame;
}
