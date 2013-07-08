#ifndef TUF_TUF_H_
#define TUF_TUF_H_

#include <map>
#include <vector>
#include <string>

#include <SDL.h>

#include <glm/glm.hpp>

#include "camera.h"
#include "gui.h"

class CShader;
class CMesh;

class CTUF
{
 public:
  CTUF();
  ~CTUF();

 private:
  bool m_bQuit;
  bool m_bFullScreen;
  bool m_bVisible;
  bool m_bHasFocus;
  std::map<std::string, CShader> m_mapShader;
  std::vector<CMesh> m_vObjects;

  float m_fzNear;
  float m_fzFar;
  float m_fFrustumScale;
  float m_fElapsedSeconds;

  SDL_Window *m_pMainWindow;
  SDL_GLContext m_MainContext;

  uint  m_nWindowWidth;
  uint  m_nWindowHeight;
  uint  m_nFrames;
  uint  m_nTicks;
  uint  m_nLastTicks;
  float m_fFPS;

  glm::mat4 m_matProjection;
  glm::mat4 m_matView;
  glm::mat4 m_matMVP;

  CCamera  m_Cam;
  CGUI     m_GUI;

 private:
  void CheckSDLError(int nLine = -1);
  void Die(const char *cMsg);

  bool InitMap();
  bool InitSDL();
  bool InitShader();
  bool InitViewPort();
  bool InitGUI();

  void OnEvent();
  void OnMouseMotion(const int xrel, const int yrel);
  void OnResize(const uint nWidth, const uint nHeight);
  void OnTextInput(const char cChar);
  void OnUpdate();

  void TearDown();
  void UpdateFPS();

 public:
  bool Initialize();
  void Run();

};

extern CTUF theApp;
#endif
