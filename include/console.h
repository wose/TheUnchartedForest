#ifndef TUF_CONSOLE_H
#define TUF_CONSOLE_H

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include "GL3/gl3.h"

#include <string>
#include <vector>
#include <map>
#include <functional>

#include <glm/glm.hpp>

#include "sdffont.h"

class CShader;

class CConsole
{
 public:
  CConsole();
  ~CConsole();

 private:
  bool m_bVisible;
  bool m_bGlow;
  CSDFFont m_Font;

  glm::mat4 m_matOrtho;
  glm::vec4 m_vFontColor;
  glm::vec4 m_vBackgroundColor;
  glm::vec4 m_vGlowColor;

  std::string m_strCommand;

  std::vector<std::string> m_vHistory;
  std::vector<std::string> m_vOutput;
  std::vector<float> m_vVertexData;

  GLuint m_nVBO;
  GLuint m_nVAO;

  float m_fCursorX;
  float m_fCursorY;

  int m_nFontSize;
//  std::map<std::string, std::function<std::string ()> func> m_Commands;

 private:
  void ResetCursor();
  void AddLineFormat(const char *cFmt, ...);
  void AddChar(const char cChar);

 public:
  void Invalidate();
  bool HandleText(const char cChar);
  bool Backspace();
  bool Enter();
  void Draw(CShader &shader, unsigned int nTime);
  void Toggle();
  bool IsVisible() { return m_bVisible; }
};

#endif
