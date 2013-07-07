#include "gui.h"

CGUI::CGUI()
{
}

CGUI::~CGUI()
{
}

////////////////////////////////////////////////////////////////////////////////
void CGUI::Init()
{
}

void CGUI::Draw()
{
  for(auto guielement : m_vGuiElements)
  {
  	guielement->Draw();
  }
}
