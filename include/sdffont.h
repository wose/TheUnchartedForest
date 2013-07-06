#ifndef TUF_SDFFONT_H
#define TUF_SDFFONT_H

#include <string>
#include <map>

#include "texture.h"

class CSDFFont :
public CTexture
{
 public:
  CSDFFont(const std::string &strName, float fSize);
  ~CSDFFont();

  public:
  typedef struct {
    char id;
    int x;
    int y;
    int width;
    int height;
    float xoffset;
    float yoffset;
    float xadvance;
    int page;
    int chnl;
  } glyph_t;
  std::map<char, glyph_t> m_mapGlyphs;

 private:
  void LoadFont(const std::string &strName);

 public:

};

#endif
