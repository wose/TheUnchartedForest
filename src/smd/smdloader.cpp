#include "smd/smdloader.h"

#include <iostream>
#include <fstream>
#include <sstream>

CSMDLoader::CSMDLoader()
{
}

CSMDLoader::~CSMDLoader()
{
}

bool CSMDLoader::LoadModel(const std::string &strFileName)
{
  int nCurrentState= VERSION;

  std::ifstream ifs(strFileName.c_str(), std::ifstream::in);

  std::string line;
  std::string strSection;
  std::string strKeyword;

  char ws;
  float x, y, z;
  bone_t sBone;
  unsigned int nBoneID;
  unsigned short nVertexCount= 0;

  std::cout << "start parsing" << std::endl;
  while (ifs.good() && !ifs.eof() && std::getline(ifs, line)) {
    std::stringstream str(line);
    str >> strKeyword;
    if (strKeyword.compare("version") == 0) {
      ++nCurrentState;
    } else if (strKeyword.compare("nodes") == 0) {
    } else if (strKeyword.compare("skeleton") == 0) {
    } else if (strKeyword.compare("triangles") == 0) {
    } else if (strKeyword.compare("end") == 0) {
      ++nCurrentState;
    } else if (strKeyword.compare(line) == 0 && nCurrentState == TRIANGLES) {
      // Material gefunden
      //      std::cout << "Material found: " << strKeyword << std::endl;
    } else {
      str.clear();
      str.seekg ( 0, std::ios::beg );
      switch(nCurrentState)
        {
        case VERSION:
          break;
        case NODES:
          str >> sBone.id >> ws >> sBone.strName >> ws >> sBone.nParent;
          //          std::cout << "added Bone: " << sBone.strName << std::endl;
          m_vBones.push_back(sBone);
          break;
        case SKELETON:
          break;
        case TRIANGLES:
          str >> nBoneID  >> x >> y >> z;
          m_vVertices.push_back(x);
          m_vVertices.push_back(y);
          m_vVertices.push_back(z);


          m_vVertexIDs.push_back(nVertexCount);
          nVertexCount++;
          //          std::cout << "added Vertex (" << nVertexCount << "): " << std::fixed << x << "/" << y << "/" << z << std::endl;

          str >> x >> y >> z;
          m_vNormals.push_back(x);
          m_vNormals.push_back(y);
          m_vNormals.push_back(z);
          //          std::cout << "added Normals: " << x << "/" << y << "/" << z << std::endl;
          break;
        default:
          break;
        }
    }
  }  

  return true;
}
