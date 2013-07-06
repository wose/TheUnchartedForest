#version 330

layout(location = 0) in vec3		vVertex;
layout(location = 1) in vec2		vVertexUV;

uniform mat4		opMatrix;

smooth out vec2 vUV;

void main(void) {
    vUV = vVertexUV;
    gl_Position = opMatrix * vec4(vVertex, 1);
}
