// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 130

out vec4 vFragColor;

uniform vec4      ambientColor;
uniform vec4      diffuseColor;   
uniform vec4      specularColor;


uniform sampler2D textureA;  
uniform sampler2D textureB; 

smooth in vec3 vVaryingNormal;
smooth in vec3 vVaryingLightDir;
smooth in vec2 vTexCoords;

void main(void)
    { 

    float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir)));

  
    vFragColor = diff * diffuseColor;


    vFragColor += ambientColor;


    vFragColor *= mix(texture(textureA, vTexCoords), texture(textureB, vTexCoords), 0.6);


 
    vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir), normalize(vVaryingNormal)));
    float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
    if(diff != 0) {
        float fSpec = pow(spec, 128.0);
        vFragColor.rgb += vec3(fSpec, fSpec, fSpec);
        }
    }
    