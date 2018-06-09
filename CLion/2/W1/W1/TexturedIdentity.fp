// The TexturedIdentity Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 130


uniform sampler2D textureA;  
uniform sampler2D textureB; 

out vec4 vFragColor;
smooth in vec2 vVaryingTexCoords;


void main(void)
   { 
   vFragColor =  mix(texture(textureA, vVaryingTexCoords), texture(textureB, vVaryingTexCoords), 0.5);
  
   }