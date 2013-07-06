#ifndef TUF_CAMERA_H
#define TUF_CAMERA_H

#include <glm/glm.hpp>

class CCamera
{
 public:
  CCamera();
  ~CCamera();

 private:
  float m_fFieldOfView;
  float m_fAspectRatio;
  float m_fNearPlane;
  float m_fFarPlane;

  float m_fVertAngle;
  float m_fHorzAngle;

  glm::vec3 m_vPosition;
  glm::mat4 m_matOrientation;

 public:
  glm::vec3 Up();
  glm::vec3 Right();
  glm::vec3 Forward();

 public:
  void SetPosition(glm::vec3 vPosition);
  void SetOffsetPosition(glm::vec3 vOffset);
  void SetOrientation(glm::mat4 &matOrientation);
  glm::mat4 GetOrientation();
  glm::mat4 GetMatrix();
  void MoveForward(float fDist);
  void MoveUp(float fDist);
  void MoveRight(float fDist);
  void RotateRight(float fAngle);
  void RotateUp(float fAngle);
};

#endif
