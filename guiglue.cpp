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
	DocumentWindow *codeview_frame = new DocumentWindow(_T("Untitled"), doc);
	codeview_frame->Show(true);
	return codeview_frame;
}