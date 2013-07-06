#ifndef TUF_TUF_H_
#define TUF_TUF_H_

#include <map>
#include <vector>
#include <string>

#include <SDL.h>

#include <glm/glm.hpp>

#include "camera.h"
#include "console.h"

class CShader;
class CMesh;

class CTUF
{
 public:
  CTUF();
  ~CTUF();

 private:
  bool m_bQuit;
  std::map<std::string, CShader> m_mapShader;
  std::vector<CMesh> m_vObjects;

  unsigned int m_nWindowWidth;
  unsigned int m_nWindowHeight;

  float m_fzNear;
  float m_fzFar;
  float m_fFrustumScale;
  float m_fElapsedSeconds;

  SDL_Window *m_pMainWindow;
  SDL_GLContext m_MainContext;

  unsigned int m_nFrames;
  unsigned int m_nTicks;
  unsigned int m_nLastTicks;
  float m_fFPS;

  glm::mat4 m_matProjection;
  glm::mat4 m_matView;
  glm::mat4 m_matMVP;

  CCamera m_Cam;
  CConsole *m_pConsole;

 private:
  void Die(const char *cMsg);
  void CheckSDLError(int nLine = -1);
  bool InitSDL();
  bool InitShader();
  bool InitViewPort();
  bool InitMap();

  void TearDown();

  void HandleEvents();
  void Update();
  void UpdateFPS();

 public:
  bool Initialize();
  void Run();
  
};

extern CTUF theApp;
#endif
