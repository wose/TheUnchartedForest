#include "shadermanager.h"
#include "utils.h"

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
}

void CShaderManager::CompileAndLinkShaderProgram(const std::string &strShaderProgramName)
{
  m_mapShaderPrograms[strShaderProgramName]= CreateProgram();

  for( auto &shader : m_vShaderList)
    {
      glDeleteShader(shader);
    }

  m_vShaderList.clear();
  //GLuint loopDurationUnf = glGetUniformLocation(theProgram, "loopDuration");
  //GLuint fragLoopDurUnf = glGetUniformLocation(m_ShaderProgram, "fragLoopDuration");
    
    
  //glUseProgram(m_ShaderProgram);
  //glUniform1f(loopDurationUnf, 5.0f);
  //glUniform1f(fragLoopDurUnf, 10.0f);
  //glUseProgram(0);
}

GLuint CShaderManager::CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
  GLuint shader = glCreateShader(eShaderType);
  const char *strFileData = strShaderFile.c_str();
  glShaderSource(shader, 1, &strFileData, NULL);
    
  glCompileShader(shader);
    
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE)
    {
      GLint infoLogLength;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        
      GLchar *strInfoLog = new GLchar[infoLogLength + 1];
      glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
        
      const char *strShaderType = NULL;
      switch(eShaderType)
        {
        case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }
        
      fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
      delete[] strInfoLog;
    }

  return shader;
}

GLuint CShaderManager::CreateProgram()
{
  GLuint program = glCreateProgram();

  for(auto shader : m_vShaderList)
    {
      glAttachShader(program, shader);
    }
    
  glLinkProgram(program);
    
  GLint status;
  glGetProgramiv (program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE)
    {
      GLint infoLogLength;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        
      GLchar *strInfoLog = new GLchar[infoLogLength + 1];
      glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
      fprintf(stderr, "Linker failure: %s\n", strInfoLog);
      delete[] strInfoLog;
    }

  for(auto shader : m_vShaderList)
    {
      glDetachShader(program, shader);
    }

  return program;
}

void CShaderManager::LoadVertexShader(const std::string &strFile)
{
  std::string strShaderContent= LoadFile(strFile);
  m_vShaderList.push_back(CreateShader(GL_VERTEX_SHADER, strShaderContent));
}

void CShaderManager::LoadGeometryShader(const std::string &strFile)
{
  std::string strShaderContent= LoadFile(strFile);
  m_vShaderList.push_back(CreateShader(GL_GEOMETRY_SHADER, strShaderContent));
}

void CShaderManager::LoadFragmentShader(const std::string &strFile)
{
  std::string strShaderContent= LoadFile(strFile);
  m_vShaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strShaderContent));
}

