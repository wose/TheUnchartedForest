#ifndef TUF_GUI_H
#define TUF_GUI_H

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include "GL3/gl3.h"

#include <vector>
#include <glm/glm.hpp>

#include "guielement.h"

class CGUI
{
 public:
  CGUI();
  ~CGUI();

 private:
  std::vector<CGUIElement*> m_vGuiElements;
  GLuint m_nVBO;
  GLuint m_nVAO;
  CTexture m_Texture;

 private:

 public:
  void Init();
  void Draw();
};

#endif
