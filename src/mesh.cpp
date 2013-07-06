#include "mesh.h"
#include "utils.h"
#include "smd/smdloader.h"


CMesh::CMesh(const std::string &strFile)
{
  CSMDLoader smd;
  smd.LoadModel("resources/mesh/Torus.smd");

  m_VertexCount= smd.m_vVertexIDs.size();


#define GREEN_COLOR 0.75f, 0.75f, 1.0f, 1.0f
#define BLUE_COLOR      0.0f, 0.5f, 0.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

  std::vector<float> vPosNorm = smd.m_vVertices;
  vPosNorm.insert( vPosNorm.end(), smd.m_vNormals.begin(), smd.m_vNormals.end() );

  //float *vertexData= &smd.m_vVertices[0];
  float *vertexData= &vPosNorm[0];
  short *indexData = &smd.m_vVertexIDs[0];

  glGenBuffers(1, &m_vbo);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vPosNorm.size(), vertexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &m_ibo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short)*smd.m_vVertexIDs.size(), indexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  size_t colorDataOffset = sizeof(float) * 3 * m_VertexCount;
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

  glBindVertexArray(0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

CMesh::~CMesh()
{
}

void CMesh::Draw()
{
  glBindVertexArray(m_vao);

  //glUniform3f(offsetUniform, 0.0f, 0.0f, 0.0f);
  glDrawElements(GL_TRIANGLES, 576 * 6, GL_UNSIGNED_SHORT, 0);

  // glUniform3f(offsetUniform, 0.0f, 0.0f, -1.0f);
  //    glDrawElementsBaseVertex(GL_TRIANGLES, 8,
  //                         GL_UNSIGNED_SHORT, 0, m_VertexCount / 2);

  glBindVertexArray(0);
}
