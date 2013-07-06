#include <iostream>

#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

CCamera::CCamera() :
  m_fVertAngle(0),
  m_fHorzAngle(0)
{
}

CCamera::~CCamera()
{
}

////////////////////////////////////////////////////////////////////////////////
glm::vec3 CCamera::Up()
{
  glm::vec4 up = glm::inverse(GetOrientation()) * glm::vec4(0,1,0,1);
  return glm::vec3(up);
}

glm::vec3 CCamera::Right()
{
  glm::vec4 right = glm::inverse(GetOrientation()) * glm::vec4(1,0,0,1);
  return glm::vec3(right);
}

glm::vec3 CCamera::Forward()
{
  glm::vec4 forward = glm::inverse(GetOrientation()) * glm::vec4(0,0,-1,1);
  return glm::vec3(forward);
}

////////////////////////////////////////////////////////////////////////////////
void CCamera::SetPosition(glm::vec3 vPosition)
{
  m_vPosition = vPosition;
}

void CCamera::SetOffsetPosition(glm::vec3 vOffset)
{
  m_vPosition += vOffset;
}

void CCamera::SetOrientation(glm::mat4 &matOrientation)
{
  m_matOrientation = matOrientation;
}

glm::mat4 CCamera::GetOrientation()
{
  glm::mat4 matOrientation = glm::rotate(glm::mat4(1.0), m_fVertAngle, glm::vec3(1.0f, 0.0f, 0.0f));
  matOrientation = glm::rotate(matOrientation, m_fHorzAngle, glm::vec3(0.0f, 1.0f, 0.0f));

  return matOrientation;
}

glm::mat4 CCamera::GetMatrix()
{
  return GetOrientation() * glm::translate(glm::mat4(1.0f), -m_vPosition);
}

void CCamera::MoveForward(float fDist)
{
  m_vPosition.x += fDist;
}

void CCamera::MoveUp(float fDist)
{
}

void CCamera::MoveRight(float fDist)
{
  m_vPosition.z += fDist;
}

void CCamera::RotateRight(float fAngle)
{
  m_fHorzAngle += fAngle;
  //  std::cout << "HorzAngle: " << m_fHorzAngle << std::endl;
  while(m_fHorzAngle > 360.0f) m_fHorzAngle -= 360.0;
  while(m_fHorzAngle < 0.0f) m_fHorzAngle += 360.0;
}

void CCamera::RotateUp(float fAngle)
{
  m_fVertAngle += fAngle;
  //  std::cout << "VertAngle: " << m_fVertAngle << std::endl;
  if(m_fVertAngle > 80) m_fVertAngle = 80;
  if(m_fVertAngle < -80) m_fVertAngle = -80;
}
