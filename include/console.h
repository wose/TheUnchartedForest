#ifndef TUF_CONSOLE_H
#define TUF_CONSOLE_H

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include "GL3/gl3.h"

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <functional>

#include <glm/glm.hpp>

#include "sdffont.h"

class CShader;

class CConsole
{
 public:
  CConsole(const uint nWidth, const uint nHeight);
  ~CConsole();

 private:
  bool m_bVisible;
  bool m_bGlow;
  bool m_bColoringMode;

  const size_t m_nHistorySize = 1000;
  uint m_nWidth;
  uint m_nHeight;
  CSDFFont m_Font;

  glm::mat4 m_matOrtho;
  glm::vec4 m_vFontColor;
  glm::vec4 m_vBackgroundColor;
  glm::vec4 m_vGlowColor;
  glm::vec3 m_vCurrentFontColor;

  std::string m_strCommand;
  std::string m_strColorCode;

  std::vector<std::string> m_vHistory;
  std::vector<std::string>::iterator m_itHistory;
  std::deque<std::string> m_vOutput;
  std::vector<float> m_vVertexData;

  GLuint m_nVBO;
  GLuint m_nVAO;

  float m_fCursorX;
  float m_fCursorY;

  int m_nFontSize;
//  std::map<std::string, std::function<std::string ()> func> m_Commands;

 private:
  void AddChar(const char cChar);
  void SetCurrentColor();
  void ResetCursor();

 public:
  void AddLine(const std::string& strLine);
  bool Backspace();
  void Down();
  void Draw(CShader &shader, const uint nTime);
  bool Enter();
  void Invalidate();
  bool IsVisible() const { return m_bVisible; }
  bool HandleText(const char cChar);
  void Toggle();
  void Resize(const uint nWidth, const uint nHeight);
  void Up();
};

#endif
