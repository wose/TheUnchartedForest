#ifndef TUF_TEXTURE_H
#define TUF_TEXTURE_H

#include <string>

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include "GL3/gl3.h"

class CTexture
{
 public:
  CTexture();
  CTexture(const std::string &strFileName);
  ~CTexture();

 private:
  GLuint m_nTextureID;

 protected:
  void LoadPNG(const std::string &strFileName);

 public:
  int GetTextureID();
};

#endif
