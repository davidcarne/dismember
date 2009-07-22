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


