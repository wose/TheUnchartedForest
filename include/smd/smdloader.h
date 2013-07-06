#ifndef SMD_SMDLOADER_H_
#define SMD_SMDLOADER_H_

#include <string>
#include <vector>


struct bone_t {
  unsigned int id;
  std::string strName;
  int nParent;
};

class CSMDLoader {
 public:
  CSMDLoader();
  ~CSMDLoader();

 private:
  enum parsing_state_t {
    VERSION = 1,
    NODES,
    SKELETON,
    TRIANGLES
  };

 public:
  std::vector<short> m_vVertexIDs;
  std::vector<float> m_vVertices;
  std::vector<float> m_vUV;
  std::vector<float> m_vNormals;
  std::vector<bone_t> m_vBones;

 public:
  bool LoadModel(const std::string &strFileName);
};

#endif
