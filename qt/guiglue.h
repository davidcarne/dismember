/*
 *  guiglue.h
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _guiglue_H_
#define _guiglue_H_

/**
 * Empty class that is the interface between the core logic and the gui
 */
class WorkspaceGui {
public:
	/**
	 * Notify the gui that an update may be necessary. The gui is free to rate-limit these.
	 */
	virtual void postUpdate() = 0;
	virtual ~WorkspaceGui() = 0;
};

class Workspace;
WorkspaceGui * setupWorkspaceGui(Workspace & doc);

#endif


