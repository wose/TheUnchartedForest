#include <iostream>

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include <GL3/gl3.h>

#include <SDL.h>
#define PROGRAM_NAME "TheUnchartedForest"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "tuf.h"
#include "shader.h"
#include "mesh.h"

#define MAX_SPEED 2.0f

CTUF theApp;

CTUF::CTUF() :
  m_bQuit(false),
  m_bFullScreen(false),
  m_nWindowWidth(1024),
  m_nWindowHeight(768),
  m_fzNear(1.0),
  m_fzFar(10.0),
  m_fFrustumScale(1.0),
  m_nFrames(0),
  m_nTicks(0)
{
}

CTUF::~CTUF()
{
  TearDown();
}

////////////////////////////////////////////////////////////////////////////////
void CTUF::Die(const char *cMsg)
{
  std::cerr << cMsg << " (" << SDL_GetError() << ")" << std::endl;
  SDL_Quit();
  exit(1);
}

void CTUF::CheckSDLError(int nLine /* = -1 */)
{
#ifndef TUF_DEBUG
  const char *cError = SDL_GetError();
  if(*cError != '\0')
    {
      std::cerr << "SDL Error: " << cError;
      if(nLine >= 0)
        std::cerr << " on line " << nLine;

      std::cerr << std::endl;
    }
#endif
}

bool CTUF::InitSDL()
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
    Die("Unable to initialize SDL");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  uint32_t nSDLFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
  if(m_bFullScreen)
  {
    // TODO set width and height to screen resolution
    nSDLFlags |= SDL_WINDOW_FULLSCREEN;
  }

  m_pMainWindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, m_nWindowWidth,
                                   m_nWindowHeight, nSDLFlags);

  if(!m_pMainWindow)
    Die("Unable to create window");

  CheckSDLError(__LINE__);

  m_MainContext = SDL_GL_CreateContext(m_pMainWindow);
  CheckSDLError(__LINE__);


  SDL_GL_SetSwapInterval(1);
  //  SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
  SDL_SetRelativeMouseMode(SDL_TRUE);

  return true;
}

bool CTUF::InitShader()
{
  bool bResult = true;

  // simple shader
  m_mapShader["simple"] = CShader();

  bResult &=
    m_mapShader["simple"].CompileShaderFromFile("resources/shader/simple.vert", Shader::VERTEX) &&
    m_mapShader["simple"].CompileShaderFromFile("resources/shader/simple.frag", Shader::FRAGMENT) &&
    m_mapShader["simple"].Link();


  m_mapShader["phong"] = CShader();

  bResult &=
    m_mapShader["phong"].CompileShaderFromFile("resources/shader/phong.vert", Shader::VERTEX) &&
    m_mapShader["phong"].CompileShaderFromFile("resources/shader/phong.frag", Shader::FRAGMENT) &&
    m_mapShader["phong"].Link();


  m_mapShader["sdf"] = CShader();

  bResult &=
    m_mapShader["sdf"].CompileShaderFromFile("resources/shader/sdf.vert", Shader::VERTEX) &&
    m_mapShader["sdf"].CompileShaderFromFile("resources/shader/sdf.frag", Shader::FRAGMENT) &&
    m_mapShader["sdf"].Link();

  return bResult;
}

bool CTUF::InitViewPort()
{
  //  m_matPerspective[0][0] = m_fFrustumScale;
  // m_matPerspective[1][1] = m_fFrustumScale;
  // m_matPerspective[2][2] =  (m_fzFar + m_fzNear) / (m_fzNear - m_fzFar);
  // m_matPerspective[2][3] = (2 * m_fzFar * m_fzNear) / (m_fzNear - m_fzFar);
  // m_matPerspective[3][2] = -1.0f;

  m_matProjection = glm::perspective(45.0f, (float)m_nWindowWidth/m_nWindowHeight, 0.1f, 100.0f);
  m_matView = glm::lookAt(
                          glm::vec3(4, 3, 3),
                          glm::vec3(0, 0, 0),
                          glm::vec3(0, 1, 0)
                          );

  m_Cam.SetPosition(glm::vec3(4, 0, 0));
  m_Cam.SetOrientation(m_matView);

  for(auto &it : m_mapShader)
    if(it.second.IsLinked())
      {
        it.second.Use();
        //        it.second.SetUniform("perspectiveMatrix", m_matProjection);
      }

  glUseProgram(0);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 3.0f);

  return true;
}

bool CTUF::InitMap()
{
  m_vObjects.push_back(CMesh("foo"));

  m_pConsole = new CConsole(m_nWindowHeight, m_nWindowWidth);

  return true;
}

void CTUF::TearDown()
{
  if(m_pConsole)
    {
      delete m_pConsole;
      m_pConsole = nullptr;
    }

  SDL_GL_DeleteContext(m_MainContext);
  SDL_DestroyWindow(m_pMainWindow);
  m_pMainWindow = nullptr;
  SDL_Quit();
}

void CTUF::HandleEvents()
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
    {
      switch(event.type)
        {
        case SDL_MOUSEMOTION:
          m_Cam.RotateUp(m_fElapsedSeconds * MAX_SPEED * 10 * event.motion.yrel);
          m_Cam.RotateRight(m_fElapsedSeconds * MAX_SPEED * 10 * event.motion.xrel);
          break;
        case SDL_TEXTINPUT:
          if(m_pConsole->HandleText(*event.text.text))
            return;
          break;
        case SDL_KEYDOWN:
          switch(event.key.keysym.sym)
            {
            case SDLK_BACKSPACE:
              if(m_pConsole->Backspace())
                return;
              break;
            case SDLK_RETURN:
              if(m_pConsole->Enter())
                return;
              break;
            case SDLK_ESCAPE:
              m_bQuit = true;
              break;
            case SDLK_w:
            case SDLK_s:
            case SDLK_a:
            case SDLK_d:
              //m_Cam.SetOffsetPosition(m_fElapsedSeconds * MAX_SPEED
              //* m_Cam.Right());
              break;
            case SDLK_UP:
              //m_Cam.RotateUp(1);
              break;
            case SDLK_DOWN:
              //m_Cam.RotateUp(-1);
              break;
            case SDLK_RIGHT:
              //m_Cam.RotateRight(1);
              break;
            case SDLK_LEFT:
              //m_Cam.RotateRight(-1);
              break;
            case SDLK_c:
              break;
            default:
              break;
            }
            break;
          case SDL_QUIT:
            m_bQuit = true;
            break;
          default:
            break;
        }
    }

  if(!m_pConsole->IsVisible())
    {
      Uint8 *aKeyState = SDL_GetKeyboardState(nullptr);

      if(aKeyState[SDL_SCANCODE_W])
        m_Cam.SetOffsetPosition(m_fElapsedSeconds * MAX_SPEED * m_Cam.Forward());
      if(aKeyState[SDL_SCANCODE_S])
        m_Cam.SetOffsetPosition(m_fElapsedSeconds * MAX_SPEED * -m_Cam.Forward());
      if(aKeyState[SDL_SCANCODE_A])
        m_Cam.SetOffsetPosition(m_fElapsedSeconds * MAX_SPEED * -m_Cam.Right());
      if(aKeyState[SDL_SCANCODE_D])
        m_Cam.SetOffsetPosition(m_fElapsedSeconds * MAX_SPEED * m_Cam.Right());

      if(aKeyState[SDL_SCANCODE_UP])
        m_Cam.RotateUp(m_fElapsedSeconds * MAX_SPEED * 10);
      if(aKeyState[SDL_SCANCODE_DOWN])
        m_Cam.RotateUp(-m_fElapsedSeconds * MAX_SPEED * 10);
      if(aKeyState[SDL_SCANCODE_RIGHT])
        m_Cam.RotateRight(m_fElapsedSeconds * MAX_SPEED * 10);
      if(aKeyState[SDL_SCANCODE_LEFT])
        m_Cam.RotateRight(-m_fElapsedSeconds * MAX_SPEED * 10);
    }

}

void CTUF::Update()
{
  glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //  m_mapShader["simple"].Use();
  //m_mapShader["simple"].SetUniform("offset", 0.0f, 0.0f, -2.0f);
  m_mapShader["phong"].Use();

  float fAngle = (SDL_GetTicks() / 10) % 360;
  glm::mat4 matModel(1.0f);

  matModel = glm::rotate(matModel, fAngle, glm::vec3(0, 1, 0));

  m_matMVP = m_matProjection * m_Cam.GetMatrix() * matModel; // letzte
                                                            // ist
                                                            // model
                                                            // matrix
  m_mapShader["phong"].SetUniform("mvMatrix", /* m_Cam.GetMatrix() */ matModel);
  m_mapShader["phong"].SetUniform("mvpMatrix", m_matMVP);
  //  m_mapShader["phong"].SetUniform("normalMatrix",
  //  glm::mat3(m_matMVP));
  m_mapShader["phong"].SetUniform("normalMatrix", glm::mat3(m_Cam.GetMatrix() * matModel));
  m_mapShader["phong"].SetUniform("vLightPosition", glm::vec3(5.0f, 5.0f, 10.0));

  m_mapShader["phong"].SetUniform("ambientColor", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
  m_mapShader["phong"].SetUniform("diffuseColor", glm::vec4(0.9f, 0.1f, 0.1f, 1.0f));
  m_mapShader["phong"].SetUniform("specularColor", glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));


  for(auto mesh : m_vObjects)
    mesh.Draw();

  glUseProgram(0);

  m_pConsole->Draw(m_mapShader["sdf"], m_nLastTicks);

  SDL_GL_SwapWindow(m_pMainWindow);
}

void CTUF::UpdateFPS()
{
  ++m_nFrames;

  if(SDL_GetTicks() - m_nTicks > 1000)
    {
      m_fFPS = (m_nFrames/(float)(SDL_GetTicks() - m_nTicks)) * 1000;
      std::cout << "FPS: " << m_fFPS << std::endl;
      std::cout << "LastTicks: " << m_nLastTicks << std::endl;
      std::cout << "ElapsedSeconds:" << m_fElapsedSeconds << std::endl;

      m_nFrames = 0;
      m_nTicks = SDL_GetTicks();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool CTUF::Initialize()
{
  return InitSDL() && InitShader() && InitViewPort() && InitMap();
}

void CTUF::Run()
{
  m_nTicks = m_nLastTicks = SDL_GetTicks();
  while(!m_bQuit)
    {
      m_fElapsedSeconds = (float)(SDL_GetTicks() - m_nLastTicks) / 1000;
      m_nLastTicks = SDL_GetTicks();
      HandleEvents();
      Update();
      UpdateFPS();
      SDL_Delay(1);
    }
}
