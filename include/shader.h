#ifndef TUF_SHADER_H_
#define TUF_SHADER_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include "GL3/gl3.h"

namespace Shader {
  enum ShaderType {
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    TESS_CONTROL,
    TESS_EVALUATION
  };
};

class CShader {
 public:
  CShader();
  ~CShader();

 private:
  int m_nHandle;
  bool m_bLinked;
  std::string m_strLog;
  std::vector<GLuint> m_vShaderList;

 private:
  int GetUniformLocation(const char *cName);

 public:
  bool CompileShaderFromFile(const char *cName, Shader::ShaderType eType);
  bool CompileShaderFromString(const std::string &strSource, Shader::ShaderType eType);
  bool Link();
  void Use();
  std::string Log();
  int GetHandle();
  bool IsLinked();
  
  void BindAttributeLocation(GLuint nLocation, const char *cName);
  void BindFragDataLocation(GLuint nLocation, const char *cName);

  void SetUniform(const char *cName, float x, float y, float z);
  void SetUniform(const char *cName, const glm::vec3 &v);
  void SetUniform(const char *cName, const glm::vec4 &v);
  void SetUniform(const char *cName, const glm::mat3 &m);
  void SetUniform(const char *cName, const glm::mat4 &m);
  void SetUniform(const char *cName, float fVal);
  void SetUniform(const char *cName, int nVal);
  void SetUniform(const char *cName, bool bVal);

  void PrintActiveUniforms();
  void PrintActiveAttribs();
};

#endif
