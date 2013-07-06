#ifndef TUF_MESH_H_
#define TUF_MESH_H_

#include <string>

#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES
#endif
#include <GL3/gl3.h>

class CMesh
{
 public:
  CMesh(const std::string &strFile);
  ~CMesh();

 private:
  int m_VertexCount;
  GLuint m_vbo;
  GLuint m_ibo;
  GLuint m_vao;

 public:
  void Draw();
};

#endif
