// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 130

out vec4 vFragColor1;


uniform sampler2D textureA;  
uniform sampler2D textureB; 

smooth in vec3 vVaryingNormal;
smooth in vec3 vVaryingLightDir;
smooth in vec2 vTexCoords;
smooth in vec4 vFragColor;
void main(void)
    { 



    vFragColor1 = vFragColor;;
    vFragColor1 =  mix(texture(textureA, vTexCoords), texture(textureB, vTexCoords), 0.5);

 
    }
    