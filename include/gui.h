#ifndef TUF_GUI_H
#define TUF_GUI_H

#include <vector>
#include <glm/glm.hpp>

#include <SDL.h>

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include "GL3/gl3.h"

#include "console.h"
#include "guielement.h"
#include "shader.h"

class CGUI
{
 public:
  CGUI();
  ~CGUI();

 private:
  std::vector<CGUIElement*> m_vGuiElements;

  bool      m_bVisible;
  uint      m_nWidth;
  uint      m_nHeight;

  GLuint    m_nVBO;
  GLuint    m_nVAO;
  CTexture  m_Texture;
  CConsole* m_pConsole;

  glm::mat4 m_matOrtho;

 private:

 public:
  void Draw(CShader &shader, const uint nTime);
  void Init(const uint nWidth, const uint nHeight);
  bool IsConsoleVisible() const { return m_pConsole && m_pConsole->IsVisible(); }
  bool IsVisible() const { return m_bVisible; }
  void Resize(const uint nWidth, const uint nHeight);
  void Toggle();

  bool OnKeyDown(const SDL_Keycode nKeyCode);
  bool OnMouseMotion(const int xrel, const int yrel);
  bool OnTextInput(const char cChar);
};

#endif
