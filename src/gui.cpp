#include <glog/logging.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "gui.h"

CGUI::CGUI() :
  m_bVisible(false),
  m_pConsole(nullptr)
{
}

CGUI::~CGUI()
{
  if(m_pConsole)
  {
  	delete m_pConsole;
  	m_pConsole = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////
void CGUI::Init(const uint nWidth, const uint nHeight)
{
  m_nWidth   = nWidth;
  m_nHeight  = nHeight;
  m_pConsole = new CConsole(nWidth, nHeight);
  m_ConsoleLogger.SetConsole(m_pConsole);
  google::AddLogSink(&m_ConsoleLogger);
  LOG(INFO) << "console logger added";
}

void CGUI::Draw(CShader &shader, const uint nTime)
{
  if(m_bVisible)
  {
  	for(auto guielement : m_vGuiElements)
  	{
  	  guielement->Draw();
  	}
  }

  if(m_pConsole)
    m_pConsole->Draw(shader, nTime);
}

void CGUI::Resize(const uint nWidth, const uint nHeight)
{
  m_matOrtho = glm::ortho(0.0f, (float)nWidth, (float)nHeight, 0.0f, -1.0f, 1.0f);
  if(m_pConsole)
  	m_pConsole->Resize(nWidth, nHeight);
}

void CGUI::Toggle()
{
  m_bVisible = !m_bVisible;
}

bool CGUI::OnKeyDown(const SDL_Keycode nKeyCode)
{
  switch(nKeyCode)
  {
    case SDLK_BACKSPACE:
      if(m_pConsole)
        return m_pConsole->Backspace();
      break;
    case SDLK_RETURN:
      if(m_pConsole)
        return m_pConsole->Enter();
      break;
    case SDLK_UP:
      if(m_pConsole)
        m_pConsole->Up();
      return true;
      break;
    case SDLK_DOWN:
      if(m_pConsole)
        m_pConsole->Down();
      return true;
      break;
    case SDLK_PAGEUP:
      if(m_pConsole)
        m_pConsole->PageUp();
      return true;
      break;
    case SDLK_PAGEDOWN:
      if(m_pConsole)
        m_pConsole->PageDown();
      return true;
      break;
  }

  return false;
}

bool CGUI::OnMouseMotion(const int xrel, const int yrel)
{
  return true;
}

bool CGUI::OnTextInput(const char cChar)
{
  if(cChar == '`' && m_pConsole)
  {
    m_pConsole->Toggle();
    return true;
  }

  if(m_pConsole && m_pConsole->IsVisible())
  	return m_pConsole->HandleText(cChar);

  return false;
}
