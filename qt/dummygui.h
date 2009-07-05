#ifndef _DUMMYGUI_H_
#define _DUMMYGUI_H_

#include "guiglue.h"

/* This class represents an inversion of stupidity.
 * I've inverted the control in this way to prevent breaking other users of
 * the Document class. You can bitch n' complain all you want about this,
 * but there's no way a document should own a GUI.
 * I've simply rectified the situation as a temporary solution.
 *  -courtc
 */ 

class DummyGUI : public DocumentGui
{
 public:
	DummyGUI();
	void hookUpdate(DocumentGui *real);
	void postUpdate();

 private:
	DocumentGui *m_real;
};

#endif
