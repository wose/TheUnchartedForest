#version 330

layout(location = 0) in vec3  vVertex;
layout(location = 1) in vec2  vVertexUV;
layout(location = 2) in vec3  vVertexColor;

uniform mat4		opMatrix;

smooth out vec2 vUV;
smooth out vec3 vColor;

void main(void) {
    vUV = vVertexUV;
    vColor = vVertexColor;
    gl_Position = opMatrix * vec4(vVertex, 1);
}
