
#include "guiglue.h"
#include "workspace.h"
class MockWorkspaceGui : public WorkspaceGui {
public:
	virtual void postUpdate() {
	};
};


WorkspaceGui * setupWorkspaceGui(Workspace & doc)
{
	return new MockWorkspaceGui();
}
