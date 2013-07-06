#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "sdffont.h"

CSDFFont::CSDFFont(const std::string &strName, float fSize)
{
  LoadFont(strName);
  LoadPNG("resources/fonts/" + strName + ".png");
}

CSDFFont::~CSDFFont()
{
}

////////////////////////////////////////////////////////////////////////////////
void CSDFFont::LoadFont(const std::string &strName)
{
  std::string strFileName = "resources/fonts/" + strName + ".sdf";
  std::ifstream ifs(strFileName.c_str(), std::ifstream::in);

  std::string strLine;
  std::string strKeyword;

  std::cout << strFileName << std::endl;
  while(ifs.good() && !ifs.eof() && std::getline(ifs, strLine))
    {
      std::stringstream ss(strLine);
      ss >> strKeyword;
      if(strKeyword.compare("info") == 0)
        {
          // load face name
        }
      else if(strKeyword.compare("chars") == 0)
        {
        }
      else if(strKeyword.compare("char") == 0)
        {
          int id;
          glyph_t glyph;
          ss.ignore(256, '=');
          ss >> id;             ss.ignore(256, '=');
          ss >> glyph.x;        ss.ignore(256, '=');
          ss >> glyph.y;        ss.ignore(256, '=');
          ss >> glyph.width;    ss.ignore(256, '=');
          ss >> glyph.height;   ss.ignore(256, '=');
          ss >> glyph.xoffset;  ss.ignore(256, '=');
          ss >> glyph.yoffset;  ss.ignore(256, '=');
          ss >> glyph.xadvance; ss.ignore(256, '=');
          ss >> glyph.page;     ss.ignore(256, '=');
          ss >> glyph.chnl;
          glyph.id = id;
          m_mapGlyphs[id] = glyph;
        }
      else
        {
          std::cout << "Unknown keyword: " << strKeyword << std::endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
