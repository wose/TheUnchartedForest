#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include "GL3/gl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <cmath>

#include "console.h"
#include "shader.h"

CConsole::CConsole() :
  m_bVisible(false),
  m_bGlow(false),
  m_vFontColor(0.9f, 0.9f, 0.9f, 1.0f),
  m_vBackgroundColor(0.1f, 0.1f, 0.1f, 0.7f),
  m_Font("Inconsolata", 18),
  m_fCursorX(16.0),
  m_fCursorY(530.0),
  m_nFontSize(18)
{
  m_matOrtho = glm::ortho( 0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);

  glGenVertexArrays(1, &m_nVAO); 
  glBindVertexArray(m_nVAO);

  glGenBuffers(1, &m_nVBO);

  CSDFFont::glyph_t glyph = m_Font.m_mapGlyphs['O'];
  float cursor[] = {(float)glyph.xoffset / 55 * 18, - (float)glyph.yoffset / 55 * 18 +(float)glyph.height / 55 * 18,  0, 0, 0,
                    (float)glyph.xoffset / 55 * 18, - (float)glyph.yoffset / 55 * 18, 0, 0, 0,
                    (float)glyph.xoffset / 55 * 18 + (float)glyph.width / 55 * 18, - (float)glyph.yoffset / 55 * 18, 0, 0, 0,
                    (float)glyph.xoffset / 55 * 18,  - (float)glyph.yoffset / 55 * 18 +(float)glyph.height / 55 * 18,  0, 0, 0,
                    (float)glyph.xoffset / 55 * 18 + (float)glyph.width / 55 * 18, - (float)glyph.yoffset / 55 * 18,  0, 0, 0,
                    (float)glyph.xoffset / 55 * 18 + (float)glyph.width / 55 * 18, - (float)glyph.yoffset / 55 * 18 +(float)glyph.height / 55 * 18, 0, 0, 0
  };


  float background[] = { 0,    0,      0, 0, 0,
                         0,    1080/2, 0, 0, 0,
                         1920, 1080/2, 0, 0, 0,
                         0,    0,      0, 0, 0,
                         1920, 1080/2, 0, 0, 0,
                         1920, 0,      0, 0, 0
  };

  glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
  //  glBufferData(GL_ARRAY_BUFFER, 24000*sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, 24000*sizeof(float), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, 30 * sizeof(float), cursor);
  glBufferSubData(GL_ARRAY_BUFFER, 30 * sizeof(float), 30 * sizeof(float), background);
  //  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //  glGenVertexArrays(1, &m_nVAO); 
  //  glBindVertexArray(m_nVAO);

  //  glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
 
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
  glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*) (3 * sizeof(float)) );
  glBindVertexArray(0);
}

CConsole::~CConsole()
{
}

////////////////////////////////////////////////////////////////////////////////
void CConsole::ResetCursor()
{
  m_fCursorX = 16.0;
  m_fCursorY = 530.0;
}

void CConsole::AddLineFormat(const char *cFmt, ...)
{
}

void CConsole::AddChar(const char cChar)
{
  if(m_bVisible)
    {
      float fTexT, fTexB, fTexL, fTexR;
      float fVertT, fVertB, fVertL, fVertR;
      CSDFFont::glyph_t glyph = m_Font.m_mapGlyphs[cChar];

      fTexL = (float)glyph.x / 512;
      fTexT = (float)glyph.y / 512;
      fTexR = (float)glyph.x / 512 + (float)glyph.width / 512;
      fTexB = (float)glyph.y / 512 + (float)glyph.height / 512;

      fVertL = m_fCursorX + (float)glyph.xoffset / 55 * 18;
      fVertT = m_fCursorY - (float)glyph.yoffset / 55 * 18;
      fVertR = fVertL + (float)glyph.width / 55 * 18;
      fVertB = fVertT + (float)glyph.height / 55 * 18;

      float aVertexData[] =
        {
          fVertL, fVertB, 0.0, fTexL, fTexB,
          fVertL, fVertT, 0.0, fTexL, fTexT,
          fVertR, fVertT, 0.0, fTexR, fTexT,
          fVertL, fVertB, 0.0, fTexL, fTexB,
          fVertR, fVertT, 0.0, fTexR, fTexT,
          fVertR, fVertB, 0.0, fTexR, fTexB
        };
      m_vVertexData.insert(m_vVertexData.end(), aVertexData, aVertexData + 30);
      
      glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
      glBufferSubData(GL_ARRAY_BUFFER, (m_vVertexData.size() + 30) * sizeof(float),
                      30 * sizeof(float), aVertexData);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      m_fCursorX += glyph.xadvance / 55 * 18;
    }
}

////////////////////////////////////////////////////////////////////////////////
void CConsole::Invalidate()
{
  m_vVertexData.clear();

  m_fCursorX = 16.0;
  m_fCursorY = 20.0;

  auto it = m_vOutput.size() >= 25 ? m_vOutput.end() - 25 : m_vOutput.begin();

  for(; it != m_vOutput.end(); ++it)
    {
      for(char &c : *it)
        {
          AddChar(c);
        }
      m_fCursorX = 16.0;
      m_fCursorY += 20;
    }

  m_fCursorX = 16.0;
  m_fCursorY = 530.0;
  if(m_strCommand.size() > 0)
    for(char &c : m_strCommand)
      {
        AddChar(c);
      }
}

bool CConsole::HandleText(const char cChar)
{
  if(cChar == '`')
    {
      Toggle();
      return true;
    }
  if(m_bVisible)
    {
      if(m_Font.m_mapGlyphs.find(cChar) != m_Font.m_mapGlyphs.end())
        {
          m_strCommand += cChar;
          AddChar(cChar);
          return true;
        }
    }
  return false;
}

bool CConsole::Backspace()
{
  if(m_bVisible && m_strCommand.size() > 0)
    {
      m_fCursorX -= 27.5 / 55 * 18;
      m_strCommand.pop_back();
      m_vVertexData.erase(m_vVertexData.end() - 30, m_vVertexData.end());
      return true;
    }
  else
    return false;
}

bool CConsole::Enter()
{
  if(m_bVisible && m_strCommand.size() > 0)
    {
      std::cout << m_strCommand << std::endl;
      m_vHistory.push_back(m_strCommand);
      m_vOutput.push_back(m_strCommand);
      //      m_vVertexData.erase(m_vVertexData.begin() , m_vVertexData.end());
      m_strCommand.clear();
      m_strCommand = "";
      std::cout << m_strCommand << std::endl;
      //      ResetCursor();
      Invalidate();
      
      return true;
    }
  else
    return false;

}

void CConsole::Draw(CShader &shader, unsigned int nTime)
{
  if(m_bVisible)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);

      shader.Use();
      shader.SetUniform("fSine", (float)sin((float)nTime / 300));
      shader.SetUniform("nConsolePart", 0);
      shader.SetUniform("fontTexture", 0);
      shader.SetUniform("vFontColor", m_vFontColor);
      shader.SetUniform("vBackgroundColor", m_vBackgroundColor);
      shader.SetUniform("opMatrix", m_matOrtho);

      glBindVertexArray(m_nVAO);
      glBindTexture(GL_TEXTURE_2D, m_Font.GetTextureID());

      glDrawArrays(GL_TRIANGLES, 6, 6);

      shader.SetUniform("nConsolePart", 1);
      glDrawArrays(GL_TRIANGLES, 12, m_vVertexData.size() / 5);

      shader.SetUniform("nConsolePart", 2);
      shader.SetUniform("opMatrix", glm::translate(m_matOrtho, glm::vec3(m_fCursorX, m_fCursorY, 0)));
      glDrawArrays(GL_TRIANGLES, 0, 6);

      glDisable(GL_BLEND);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);
      glBindVertexArray(0);
    }
}

void CConsole::Toggle()
{
  m_bVisible = m_bVisible == false;
}
