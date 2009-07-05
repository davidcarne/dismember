/*
 *  guiglue.cpp
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "guiglue.h"
#include "dummygui.h"
#include "document.h"

DocumentGui *setupDocumentGui(Document &)
{ 
	return new DummyGUI();
}
