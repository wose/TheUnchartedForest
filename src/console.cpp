#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <cmath>

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include "GL3/gl3.h"

#include "console.h"
#include "shader.h"

CConsole::CConsole(const uint nWidth, const uint nHeight) :
  m_bVisible(false),
  m_bGlow(false),
  m_bColoringMode(false),
  m_nHeight(nHeight/2),
  m_nWidth(nWidth),
  m_vFontColor(0.9f, 0.9f, 0.9f, 1.0f),
  m_vCurrentFontColor(0.9f, 0.9f, 0.9f),
  m_vBackgroundColor(0.1f, 0.1f, 0.1f, 0.7f),
  m_Font("Inconsolata", 18),
  m_fCursorX(16.0),
  m_fCursorY(nHeight/2 - 20),
  m_nFontSize(18)
{
  m_matOrtho = glm::ortho( 0.0f, (float)nWidth, (float)nHeight, 0.0f, -1.0f, 1.0f);

  glGenVertexArrays(1, &m_nVAO);
  glBindVertexArray(m_nVAO);

  glGenBuffers(1, &m_nVBO);

  CSDFFont::glyph_t glyph = m_Font.m_mapGlyphs['O'];
  float cursor[] =
    {
      (float)glyph.xoffset / 55 * 18, - (float)glyph.yoffset / 55 * 18 +(float)glyph.height / 55 * 18,  0, 0, 0,  0.1f, 0.1f, 0.1f,
      (float)glyph.xoffset / 55 * 18, - (float)glyph.yoffset / 55 * 18, 0, 0, 0,  0.1f, 0.1f, 0.1f,
      (float)glyph.xoffset / 55 * 18 + (float)glyph.width / 55 * 18, - (float)glyph.yoffset / 55 * 18, 0, 0, 0, 0.1f, 0.1f, 0.1f,
      (float)glyph.xoffset / 55 * 18,  - (float)glyph.yoffset / 55 * 18 +(float)glyph.height / 55 * 18,  0, 0, 0, 0.1f, 0.1f, 0.1f,
      (float)glyph.xoffset / 55 * 18 + (float)glyph.width / 55 * 18, - (float)glyph.yoffset / 55 * 18,  0, 0, 0, 0.1f, 0.1f, 0.1f,
      (float)glyph.xoffset / 55 * 18 + (float)glyph.width / 55 * 18, - (float)glyph.yoffset / 55 * 18 +(float)glyph.height / 55 * 18, 0, 0, 0,  0.1f, 0.1f, 0.1f
    };

  float fHeight = (float)m_nHeight;
  float fWidth  = (float)m_nWidth;
  float background[] =
    {//x       y         z  u  v  r     g     b
      0,       0,        0, 0, 0, 0.1f, 0.1f, 0.1f,
      0,       fHeight,  0, 0, 0, 0.1f, 0.1f, 0.1f,
      fWidth,  fHeight,  0, 0, 0, 0.1f, 0.1f, 0.1f,
      0,       0,        0, 0, 0, 0.1f, 0.1f, 0.1f,
      fWidth,  fHeight,  0, 0, 0, 0.1f, 0.1f, 0.1f,
      fWidth,  0,        0, 0, 0, 0.1f, 0.1f, 0.1f
    };

  glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
  glBufferData(GL_ARRAY_BUFFER, 48000*sizeof(float), NULL, GL_DYNAMIC_DRAW);

  glBufferSubData(GL_ARRAY_BUFFER, 0, 48 * sizeof(float), cursor);
  glBufferSubData(GL_ARRAY_BUFFER, 48 * sizeof(float), 48 * sizeof(float),
    background);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
  glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),
    (void*) (3 * sizeof(float)));
  glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
    (void*) (5 * sizeof(float)));
  glBindVertexArray(0);
}

CConsole::~CConsole()
{
}

////////////////////////////////////////////////////////////////////////////////
void CConsole::ResetCursor()
{
  m_fCursorX = 16.0;
  m_fCursorY = m_nHeight - 20;
}

void CConsole::AddChar(const char cChar)
{
  CSDFFont::glyph_t glyph = m_Font.m_mapGlyphs[cChar];

  float fTexL = (float)glyph.x / 512;
  float fTexT = (float)glyph.y / 512;
  float fTexR = (float)glyph.x / 512 + (float)glyph.width / 512;
  float fTexB = (float)glyph.y / 512 + (float)glyph.height / 512;

  float fVertL = m_fCursorX + (float)glyph.xoffset / 55 * 18;
  float fVertT = m_fCursorY - (float)glyph.yoffset / 55 * 18;
  float fVertR = fVertL + (float)glyph.width / 55 * 18;
  float fVertB = fVertT + (float)glyph.height / 55 * 18;

  float aVertexData[] =
    {
      fVertL, fVertB, 0.0, fTexL, fTexB, m_vCurrentFontColor.r, m_vCurrentFontColor.g, m_vCurrentFontColor.b,
      fVertL, fVertT, 0.0, fTexL, fTexT, m_vCurrentFontColor.r, m_vCurrentFontColor.g, m_vCurrentFontColor.b,
      fVertR, fVertT, 0.0, fTexR, fTexT, m_vCurrentFontColor.r, m_vCurrentFontColor.g, m_vCurrentFontColor.b,
      fVertL, fVertB, 0.0, fTexL, fTexB, m_vCurrentFontColor.r, m_vCurrentFontColor.g, m_vCurrentFontColor.b,
      fVertR, fVertT, 0.0, fTexR, fTexT, m_vCurrentFontColor.r, m_vCurrentFontColor.g, m_vCurrentFontColor.b,
      fVertR, fVertB, 0.0, fTexR, fTexB, m_vCurrentFontColor.r, m_vCurrentFontColor.g, m_vCurrentFontColor.b
    };
  m_vVertexData.insert(m_vVertexData.end(), aVertexData, aVertexData + 48);

  glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
  glBufferSubData(GL_ARRAY_BUFFER, (m_vVertexData.size() + 48) * sizeof(float),
    48 * sizeof(float), aVertexData);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  m_fCursorX += glyph.xadvance / 55 * 18;
}

void CConsole::SetCurrentColor()
{
  if(m_strColorCode.size() == 2)
  {
    if(m_strColorCode == "^1")
      m_vCurrentFontColor = glm::vec3(1.0f, 0.0f, 0.0f);
    else if(m_strColorCode == "^2")
      m_vCurrentFontColor = glm::vec3(0.0f, 1.0f, 0.0f);
    else if(m_strColorCode == "^3")
      m_vCurrentFontColor = glm::vec3(1.0f, 1.0f, 0.0f);
    else if(m_strColorCode == "^4")
      m_vCurrentFontColor = glm::vec3(0.0f, 0.0f, 1.0f);
    else if(m_strColorCode == "^5")
      m_vCurrentFontColor = glm::vec3(0.0f, 1.0f, 1.0f);
    else if(m_strColorCode == "^6")
      m_vCurrentFontColor = glm::vec3(1.0f, 0.0f, 1.0f);
    else if(m_strColorCode == "^7")
      m_vCurrentFontColor = glm::vec3(1.0f, 1.0f, 1.0f);
    else if(m_strColorCode == "^N")
      m_vCurrentFontColor = glm::vec3(m_vFontColor.r, m_vFontColor.g, m_vFontColor.b);

    m_strColorCode.clear();
    m_bColoringMode = false;
  }
  else if(m_strColorCode.size() > 2)
  {
    m_strColorCode.clear();
    m_bColoringMode = false;
  }
}

////////////////////////////////////////////////////////////////////////////////
void CConsole::AddLine(const std::string& strLine)
{
  m_vOutput.push_back(strLine);
  Invalidate();
}

void CConsole::Resize(const uint nWidth, const uint nHeight)
{
  m_matOrtho = glm::ortho(0.0f, (float)nWidth, (float)nHeight, 0.0f, -1.0f, 1.0f);

  m_nWidth  = nWidth;
  m_nHeight = nHeight / 2;

  float fHeight = (float)m_nHeight;
  float fWidth  = (float)m_nWidth;
  float background[] =
    {
      0,       0,        0, 0, 0, 0.1f, 0.1f, 0.1f,
      0,       fHeight,  0, 0, 0, 0.1f, 0.1f, 0.1f,
      fWidth,  fHeight,  0, 0, 0, 0.1f, 0.1f, 0.1f,
      0,       0,        0, 0, 0, 0.1f, 0.1f, 0.1f,
      fWidth,  fHeight,  0, 0, 0, 0.1f, 0.1f, 0.1f,
      fWidth,  0,        0, 0, 0, 0.1f, 0.1f, 0.1f
    };

  glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
  glBufferSubData(GL_ARRAY_BUFFER, 48 * sizeof(float), 48 * sizeof(float),
    background);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  Invalidate();
}

void CConsole::Invalidate()
{
  m_vVertexData.clear();

  m_fCursorX = 16.0;
  m_fCursorY = 20.0;
  int nOutputLines = (m_nHeight - 40) / 20;

  auto it = m_vOutput.size() >= nOutputLines ?
            m_vOutput.end() - nOutputLines : m_vOutput.begin();

  for(; it != m_vOutput.end(); ++it)
    {
      m_vCurrentFontColor = glm::vec3(m_vFontColor);
      for(char &c : *it)
        {
          if(c == '^' or m_bColoringMode)
          {
            m_bColoringMode = true;
            m_strColorCode += c;
            SetCurrentColor();
          }
          else
            AddChar(c);
        }
      m_fCursorX = 16.0;
      m_fCursorY += 20;
    }

  m_fCursorX = 16.0;
  m_fCursorY = m_nHeight - 20;
  m_vCurrentFontColor = glm::vec3(m_vFontColor);

  if(m_strCommand.size() > 0)
    for(char &c : m_strCommand)
      {
        AddChar(c);
      }
}

bool CConsole::HandleText(const char cChar)
{
  if(m_bVisible and m_Font.m_mapGlyphs.find(cChar) != m_Font.m_mapGlyphs.end())
    {
      m_strCommand += cChar;
      if(cChar == '^' or m_bColoringMode)
      {
        m_bColoringMode = true;
        m_strColorCode += cChar;
        SetCurrentColor();
      }
      else
        AddChar(cChar);

      std::cout << "vertex count (48000 max): " << m_vVertexData.size() << std::endl;
      return true;
    }
  return false;
}

bool CConsole::Backspace()
{
  if(m_bVisible && m_strCommand.size() > 0)
    {
      m_fCursorX -= 27.5 / 55 * 18;
      m_strCommand.pop_back();
      m_vVertexData.erase(m_vVertexData.end() - 48, m_vVertexData.end());

      return true;
    }
  else
    return false;
}

bool CConsole::Enter()
{
  if(m_bVisible && m_strCommand.size() > 0)
    {
      std::cout << "command: " << m_strCommand << std::endl;
      m_vHistory.push_back(m_strCommand);
      m_vOutput.push_back(m_strCommand);
      m_strCommand.clear();
      m_strCommand = "";
      Invalidate();

      return true;
    }
  return false;
}

void CConsole::Draw(CShader &shader, const uint nTime)
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
    glDrawArrays(GL_TRIANGLES, 12, m_vVertexData.size() / 8);

    shader.SetUniform("nConsolePart", 2);
    shader.SetUniform("opMatrix",
      glm::translate(m_matOrtho, glm::vec3(m_fCursorX, m_fCursorY, 0)));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBindVertexArray(0);
  }
}

void CConsole::Toggle()
{
  m_bVisible = !m_bVisible;
}
