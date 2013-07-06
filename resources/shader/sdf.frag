#version 330

out vec4 vFragColor;

uniform sampler2D fontTexture;
uniform vec4 vFontColor;
uniform vec4 vBackgroundColor;
uniform int nConsolePart;
uniform float fSine;

smooth in vec2 vUV;

const float SmoothCenter = 0.5;
const float SmoothWidth = 0.25;

void main(void)
{
    if(nConsolePart == 2)
    {
        vFragColor = vec4(1.0, 0.0, 0.0, fSine);
    }
    else if(nConsolePart == 0)
    {
        vFragColor = vBackgroundColor;
    }
    else
    {
        vec4 color = texture2D(fontTexture, vUV);
        float distance = color.r;
        float alpha = smoothstep(SmoothCenter - SmoothWidth,
                                 SmoothCenter + SmoothWidth,
                                 distance);

        vFragColor = vec4(vFontColor.xyz, color.a * alpha);
    }
}