#version 330

out vec4 vFragColor;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

smooth in vec3 vVaryingNormal;
smooth in vec3 vVaryingLightDir;

void main()
{
    float diff = max(0.0, dot(normalize(vVaryingNormal),
                              normalize(vVaryingLightDir)));
    vFragColor = diff * diffuseColor;
//    vFragColor = vec4(vVaryingNormal, 1.0);
    vFragColor += ambientColor;
    vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),
                                         normalize(vVaryingNormal)));
    float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));

    if(diff > 0)
    {
        float fSpec = pow(spec, 128.0);
        vFragColor.rgb += vec3(fSpec, fSpec, fSpec);
    }
}