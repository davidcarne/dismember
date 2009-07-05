#include "dummygui.h"

DummyGUI::DummyGUI()
 : m_real(0)
{ }

void DummyGUI::hookUpdate(DocumentGui *real)
{
	m_real = real;
}

void DummyGUI::postUpdate()
{
	if (m_real)
		m_real->postUpdate();
}
