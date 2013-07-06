#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>

#include "shader.h"

CShader::CShader() :
  m_bLinked(false)
{
}

CShader::~CShader()
{
  if(m_bLinked)
    glDeleteProgram(m_nHandle);

  if(m_vShaderList.size() > 0)
    for(auto shader : m_vShaderList)
      glDeleteShader(shader);
}

////////////////////////////////////////////////////////////////////////////////
int CShader::GetUniformLocation(const char *cName)
{
  return glGetUniformLocation(m_nHandle, cName);
}

////////////////////////////////////////////////////////////////////////////////
bool CShader::CompileShaderFromFile(const char *cName, Shader::ShaderType eType)
{
  std::ifstream in(cName, std::ios::in | std::ios::binary);
  if(in)
    {
      std::string strSource;
      in.seekg(0, std::ios::end);
      strSource.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&strSource[0], strSource.size());
      in.close();

      return CompileShaderFromString(strSource, eType);
    }
  else
    {
      std::cerr << "Error loading shader file: " << cName << " errno: " << errno << std::endl;
      return false;
    }
}

bool CShader::CompileShaderFromString(const std::string &strSource,
                                      Shader::ShaderType eType)
{
  GLuint nShader;

  switch(eType)
    {
    case Shader::VERTEX:
      nShader = glCreateShader(GL_VERTEX_SHADER);
      break;
    case Shader::FRAGMENT:
      nShader = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    case Shader::GEOMETRY:
      nShader = glCreateShader(GL_GEOMETRY_SHADER);
      break;
    case Shader::TESS_CONTROL:
      nShader = glCreateShader(GL_TESS_CONTROL_SHADER);
      break;
    case Shader::TESS_EVALUATION:
      nShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
      break;
    default:
      std::cerr << "Error creating shader. Unknown shader type: " << eType << std::endl;
      return false;
      break;
    }

  if(nShader == 0)
    {
      std::cerr << "Error creating shader object (" << eType << ")" << std::endl;
      return false;
    }

  const GLchar *cSource = strSource.c_str();
  glShaderSource(nShader, 1, &cSource, nullptr);

  glCompileShader(nShader);

  GLint nResult;
  glGetShaderiv(nShader, GL_COMPILE_STATUS, &nResult);

  if(nResult == GL_FALSE)
    {
      GLint nLogLength;
      std::cerr << "Shader compilation faled!" << std::endl;

      glGetShaderiv(nShader, GL_INFO_LOG_LENGTH, &nLogLength);
      if(nLogLength > 0)
        {
          GLchar *cLog = new GLchar[nLogLength + 1];
          GLsizei nWritten;

          glGetShaderInfoLog(nShader, nLogLength, &nWritten, cLog);
          std::cerr << "Shader log:" << std::endl << cLog << std::endl;
          delete cLog;
        }
      return false;
    }

  m_vShaderList.push_back(nShader);
  return true;
}

bool CShader::Link()
{
  GLint nStatus;

  m_nHandle = glCreateProgram();

  if(m_nHandle == 0)
    {
      std::cerr << "Error creating program object!" << std::endl;
      return false;
    }

  for(auto shader : m_vShaderList)
    glAttachShader(m_nHandle, shader);

  glLinkProgram(m_nHandle);
  glGetProgramiv(m_nHandle, GL_LINK_STATUS, &nStatus);

  if(nStatus == GL_FALSE)
    {
      std::cerr << "Failed to link shader program!" << std::endl;

      GLint nLogLength;
      glGetProgramiv(m_nHandle, GL_INFO_LOG_LENGTH, &nLogLength);

      if(nLogLength > 0)
        {
          GLchar *cLog = new GLchar[nLogLength];
          GLsizei nWritten;
          glGetProgramInfoLog(m_nHandle, nLogLength, &nWritten, cLog);
          std::cerr << "Program log: " << std::endl << cLog << std::endl;
          delete cLog;
        }
      return false;
    }
  m_bLinked = true;
  return true;
}

void CShader::Use()
{
  if(m_bLinked)
    glUseProgram(m_nHandle);
}

std::string CShader::Log()
{
  return m_strLog;
}

int CShader::GetHandle()
{
  return m_nHandle;
}

bool CShader::IsLinked()
{
  return m_bLinked;
}

void CShader::BindAttributeLocation(GLuint nLocation, const char *cName)
{
  glBindAttribLocation(m_nHandle, nLocation, cName);
}

void CShader::BindFragDataLocation(GLuint nLocation, const char *cName)
{
  glBindFragDataLocation(m_nHandle, nLocation, cName);
}

void CShader::SetUniform(const char *cName, float x, float y, float z)
{
  GLuint nLocation = glGetUniformLocation(m_nHandle, cName);

  if(nLocation >= 0)
    glUniform3f(nLocation, x, y, z);
}

void CShader::SetUniform(const char *cName, const glm::vec3 &v)
{
  GLuint nLocation = glGetUniformLocation(m_nHandle, cName);

  if(nLocation >= 0)
    glUniform3fv(nLocation, 1, &v[0]);
}

void CShader::SetUniform(const char *cName, const glm::vec4 &v)
{
  GLuint nLocation = glGetUniformLocation(m_nHandle, cName);
  
  if(nLocation >= 0)
    glUniform4fv(nLocation, 1, &v[0]);
}

void CShader::SetUniform(const char *cName, const glm::mat3 &m)
{
  GLuint nLocation = glGetUniformLocation(m_nHandle, cName);

  if(nLocation >= 0)
    glUniformMatrix3fv(nLocation, 1, GL_FALSE, &m[0][0]);
}

void CShader::SetUniform(const char *cName, const glm::mat4 &m)
{
  GLuint nLocation = glGetUniformLocation(m_nHandle, cName);
  
  if(nLocation >= 0)
    glUniformMatrix4fv(nLocation, 1, GL_FALSE, &m[0][0]);
}

void CShader::SetUniform(const char *cName, float fVal)
{
  GLuint nLocation = glGetUniformLocation(m_nHandle, cName);

  if(nLocation >= 0)
    glUniform1f(nLocation, fVal);
}

void CShader::SetUniform(const char *cName, int nVal)
{
  GLuint nLocation = glGetUniformLocation(m_nHandle, cName);

  if(nLocation >= 0)
    glUniform1i(nLocation, nVal);
}

void CShader::SetUniform(const char *cName, bool bVal)
{
  GLuint nLocation = glGetUniformLocation(m_nHandle, cName);

  if(nLocation >= 0)
    glUniform1i(nLocation, bVal ? 1 : 0);
}

void CShader::PrintActiveUniforms()
{
  GLint nUniforms, nMaxLenght, nSize, nLocation;
  GLsizei nWritten;
  GLenum eType;
  
  glGetProgramiv(m_nHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nMaxLenght);
  glGetProgramiv(m_nHandle, GL_ACTIVE_UNIFORMS, &nUniforms);

  GLchar *cName = new GLchar[nMaxLenght + 1];

  std::cout << " Location | Name" << std::endl;
  std::cout << "------------------------------------------------------------" << std::endl;

  for(int nIndex = 0; nIndex < nUniforms; ++nIndex)
    {
      glGetActiveUniform(m_nHandle, nIndex, nMaxLenght, &nWritten, &nSize, &eType, cName);
      nLocation = glGetUniformLocation(m_nHandle, cName);
      std::cout << nLocation << " | " << cName << std::endl;
    }
  delete cName;
}

void CShader::PrintActiveAttribs()
{
  GLint nAttribs, nMaxLenght, nSize, nLocation;
  GLsizei nWritten;
  GLenum eType;

  glGetProgramiv(m_nHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &nMaxLenght);
  glGetProgramiv(m_nHandle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

  GLchar *cName = new GLchar[nMaxLenght + 1];

  std::cout << " Location | Name" << std::endl;
  std::cout << "------------------------------------------------------------" << std::endl;

  for(int nIndex = 0; nIndex < nAttribs; ++nIndex)
    {
      glGetActiveAttrib(m_nHandle, nIndex, nMaxLenght, &nWritten, &nSize, &eType, cName);
      nLocation = glGetAttribLocation(m_nHandle, cName);
      std::cout << nLocation << " | " << cName << std::endl;
    }
  delete cName;
}
