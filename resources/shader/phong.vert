#version 330

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;

smooth out vec4 theColor;

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform vec3 vLightPosition;

smooth out vec3 vVaryingNormal;
smooth out vec3 vVaryingLightDir;

void main()
{
    vVaryingNormal = (mvMatrix * vec4(vNormal, 0.0)).xyz;

    vec4 vPosition4 = mvMatrix * vVertex;
    vec3 vPosition3 = vPosition4.xyz / vPosition4.w;

    vVaryingLightDir = vLightPosition - vPosition3;

    gl_Position = mvpMatrix * vVertex;
}

