#ifndef TUF_MATERIAL_H_
#define TUF_MATERIAL_H_

#include <string>

#include <glm/glm.hpp>

class CMaterial
{
 public:
  CMaterial();
  ~CMaterial();

 private:
  float m_fNs;             ///< Specular exponent
  glm::vec3 m_vKa;         ///< Ambient reflectivity
  glm::vec3 m_vKd;         ///< Diffuse reflectivity
  glm::vec3 m_vKs;         ///< Specular reflectivity
  float m_fNi;             ///< Optical density/index of refraction
  float m_fd;              ///< Dissolve Factor (1.0 completely opaque)
  unsigned short m_nIllum; ///< Illumination Model
        /*
          illumination models:

          0 Color on and Ambient off
          1 Color on and Ambient on
          2 Highlight on
          3 Reflection on and Ray trace on
          4 Transparency: Glass on
            Reflection: Ray trace on
          5 Reflection: Fresnel on and Ray trace on
          6 Transparency: Refraction on
            Reflection: Fresnel off and Ray trace on
          7 Transparency: Refraction on
            Reflection: Fresnel on and Ray trace on
          8 Reflection on and Ray trace off
          9 Transparency: Glass on
            Reflection: Ray trace off
          10 Casts shadows onto invisible surfaces 
        */

 private:

 public:
};

#endif
