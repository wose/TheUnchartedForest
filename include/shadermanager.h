#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#include <string>
#include <vector>
#include <map>

#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

class CShaderManager
{
 public:
  CShaderManager();
  ~CShaderManager();

 private:
  std::map<std::string, GLuint> m_mapShaderPrograms;
  std::vector<GLuint> m_vShaderList;

 public:
  void CompileAndLinkShaderProgram(const std::string &strShaderProgramName);

  void LoadVertexShader(const std::string &strFile);
  void LoadGeometryShader(const std::string &strFile);
  void LoadFragmentShader(const std::string &strFile);

  GLuint GetProgram(const std::string &strShaderProgramName) {
    return m_mapShaderPrograms[strShaderProgramName];
  }

 private:
  GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
  GLuint CreateProgram();
};

#endif
