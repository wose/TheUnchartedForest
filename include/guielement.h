#ifndef TUF_GUIELEMENT_H
#define TUF_GUIELEMENT_H

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include "GL3/gl3.h"

#include <glm/glm.hpp>

#include "texture.h"

class CGUIElement
{
 public:
  CGUIElement();
  ~CGUIElement();

 private:
  glm::vec2 m_vPosition;
  float m_fWidth;
  float m_fHeight;
  bool m_bVisible;

  GLuint m_nVBO;
  unsigned int m_nVertexOffset;

 public:
  virtual unsigned int Init(const GLuint nVBO, const unsigned int nVertexOffset);
  virtual void Draw();
  bool IsVisible() { return m_bVisible; }
};

#endif
