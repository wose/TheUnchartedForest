#ifndef TUF_GUIBAR_H
#define TUF_GUIBAR_H

#include "guielement.h"

class CGUIBar :
  public CGUIElement
{
 public:
  CGUIBar();
  ~CGUIBar();

 private:
 public:
  virtual unsigned int Init(const GLuint nVBO, const unsigned int nVertexOffset);
  virtual void Draw();
};

#endif
