#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <math.h>
#include <GLMatrixStack.h>
#include "ObjLoader.h"
#include <GLGeometryTransform.h>
#include <StopWatch.h>
#include <math.h>
#include <stdio.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define GLUT_DISABLE_ATEXIT_HACK 
#include <GL/glut.h>
#endif


#include<stdio.h>
#ifdef __cplusplus
extern"C"
#endif 
FILE __iob_func[3] = { __acrt_iob_func(0),__acrt_iob_func(2),__acrt_iob_func(3) };


GLFrame             viewFrame;
GLFrustum           viewFrustum;
GLTriangleBatch     sphereBatch;
GLMatrixStack       modelViewMatrix;
GLMatrixStack       projectionMatrix;
GLGeometryTransform transformPipeline;
GLShaderManager     shaderManager;

GLuint	ADSTextureShader;	// The textured diffuse light shader
GLint	locAmbient;			// The location of the ambient color
GLint   locDiffuse;			// The location of the diffuse color
GLint   locSpecular;		// The location of the specular color
GLint	locLight;			// The location of the Light in eye coordinates
GLint	locMVP;				// The location of the ModelViewProjection matrix uniform
GLint	locMV;				// The location of the ModelView matrix uniform
GLint	locNM;				// The location of the Normal matrix uniform
GLint   locTexture;
GLuint  uiTextures[2];

// Load a TGA as a 2D Texture. Completely initialize the state
bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if (pBits == NULL)
		return false;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
		eFormat, GL_UNSIGNED_BYTE, pBits);

	free(pBits);

	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC(void)
{
	// Background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	shaderManager.InitializeStockShaders();
	viewFrame.MoveForward(4.0f);

	// Make the sphere
	gltMakeSphere(sphereBatch, 1.0f, 104, 52);

	ADSTextureShader = gltLoadShaderPairWithAttributes("ADSTexture.vp", "ADSTexture.fp", 3, GLT_ATTRIBUTE_VERTEX, "vVertex",
		GLT_ATTRIBUTE_NORMAL, "vNormal", GLT_ATTRIBUTE_TEXTURE0, "vTexture0");

	locAmbient = glGetUniformLocation(ADSTextureShader, "ambientColor");
	locDiffuse = glGetUniformLocation(ADSTextureShader, "diffuseColor");
	locSpecular = glGetUniformLocation(ADSTextureShader, "specularColor");
	locLight = glGetUniformLocation(ADSTextureShader, "vLightPosition");
	locMVP = glGetUniformLocation(ADSTextureShader, "mvpMatrix");
	locMV = glGetUniformLocation(ADSTextureShader, "mvMatrix");
	locNM = glGetUniformLocation(ADSTextureShader, "normalMatrix");
	locTexture = glGetUniformLocation(ADSTextureShader, "colorMap");

	glGenTextures(2, uiTextures);
	glBindTexture(GL_TEXTURE_2D, uiTextures[0]);
	LoadTGATexture("CoolTexture.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);


	// Load Mars
	glBindTexture(GL_TEXTURE_2D, uiTextures[1]);
	LoadTGATexture("1.tga", GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR, GL_CLAMP_TO_EDGE);
}

// Cleanup
void ShutdownRC(void)
{
	glDeleteTextures(2, uiTextures);
}


// Called to draw scene
void RenderScene(void)
{
	static CStopWatch rotTimer;

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelViewMatrix.PushMatrix(viewFrame);
	modelViewMatrix.Rotate(rotTimer.GetElapsedSeconds() * 10.0f, 0.0f, 1.0f, 0.0f);

	GLfloat vEyeLight[] = { -100.0f, 100.0f, 100.0f };
	GLfloat vAmbientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat vDiffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat vSpecularColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glBindTexture(GL_TEXTURE_2D, uiTextures[0]);     // 使用纹理A  									
	GLint textureALocation = glGetUniformLocation(ADSTextureShader, "textureA");
	glUniform1i(textureALocation, 0);   // 将纹理A传至Shader的纹理采样器中  

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, uiTextures[1]);
	glUniform1i(glGetUniformLocation(ADSTextureShader, "textureB"), 1);


	glUseProgram(ADSTextureShader);
	glUniform4fv(locAmbient, 1, vAmbientColor);
	glUniform4fv(locDiffuse, 1, vDiffuseColor);
	glUniform4fv(locSpecular, 1, vSpecularColor);
	glUniform3fv(locLight, 1, vEyeLight);


	glUniformMatrix4fv(locMVP, 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(locMV, 1, GL_FALSE, transformPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(locNM, 1, GL_FALSE, transformPipeline.GetNormalMatrix());
	glUniform1i(locTexture, 0);
	sphereBatch.Draw();

	modelViewMatrix.PopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();
}



void ChangeSize(int w, int h)
{
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 100.0f);

	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lit Texture");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	SetupRC();
	glutMainLoop();
	ShutdownRC();
	return 0;
}

/*
GLMatrixStack		modelViewMatrix;		// Modelview Matrix
GLTriangleBatch		sphereBatch;

GLShaderManager	shaderManager;

GLint	myTexturedIdentityShader;
GLuint	uiTextures[2];

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);
}




// Load a TGA as a 2D Texture. Completely initialize the state
bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if (pBits == NULL)
		return false;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
		eFormat, GL_UNSIGNED_BYTE, pBits);

	free(pBits);

	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}


///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	shaderManager.InitializeStockShaders();

	// Load up a triangle
	GLfloat vVerts[] = { -0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f };

	GLfloat vTexCoords[] = { 0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f };



	myTexturedIdentityShader = gltLoadShaderPairWithAttributes("TexturedIdentity.vp", "TexturedIdentity.fp", 2,
		GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "vTexCoords");

	glGenTextures(2, uiTextures);
	glBindTexture(GL_TEXTURE_2D, uiTextures[0]);
	LoadTGATexture("2.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

	// Load Mars
	glBindTexture(GL_TEXTURE_2D, uiTextures[1]);
	LoadTGATexture("1.tga", GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR, GL_CLAMP_TO_EDGE);

	gltMakeSphere(sphereBatch, 1.0f, 84, 52);
}


///////////////////////////////////////////////////////////////////////////////
// Cleanup
void ShutdownRC()
{
	glDeleteProgram(myTexturedIdentityShader);
	glDeleteTextures(2, uiTextures);
}


///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{

	static CStopWatch	rotTimer;
	float yRot = rotTimer.GetElapsedSeconds() * 60.0f;

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glUseProgram(myTexturedIdentityShader);

	/*  glBindTexture(GL_TEXTURE_2D, uiTextures[0]);
	GLint iTextureUniform = glGetUniformLocation(myTexturedIdentityShader, "colorMap");
	glUniform1i(iTextureUniform, 0);

	modelViewMatrix.PushMatrix();


	modelViewMatrix.Rotate(yRot * -2.0f, 0.0f, 1.0f, 0.0f);
	modelViewMatrix.Translate(0.8f, 0.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, uiTextures[0]);     // 使用纹理A  									
	GLint textureALocation = glGetUniformLocation(myTexturedIdentityShader, "textureA");
	glUniform1i(textureALocation, 0);   // 将纹理A传至Shader的纹理采样器中  

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, uiTextures[1]);
	glUniform1i(glGetUniformLocation(myTexturedIdentityShader, "textureB"), 1);



	sphereBatch.Draw();


	modelViewMatrix.PopMatrix();
	//triangleBatch.Draw();

	// Perform the buffer swap to display back buffer
	glutSwapBuffers();
}


///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Textured Triangle");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	SetupRC();

	glutMainLoop();

	ShutdownRC();

	return 0;
}
*/