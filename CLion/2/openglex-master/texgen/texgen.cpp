#include "gltools.h"
#include <stdio.h>

#define ENV 0
#define STRIPES 1
#define TEXTURENUM 2

const char* texFileName[] = {"..\\Environment.tga","..\\stripes.tga"};

static GLuint textureName[TEXTURENUM];

static GLfloat yRot = 0.0f;
static GLfloat zPos = -2.0f;

static GLint iRenderMode = 3;

void ProcessMenu(int value)
{
  //ͶӰƽ��
  GLfloat zPlane[] = {0.0f, 0.0f, 1.0f, 0.0f};
  //��Ⱦģʽ
  iRenderMode = value;

  switch(value)
  {
  case 1:

    //��������
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_PLANE, zPlane);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, zPlane);
    break;
  case 2:

    //�Ӿ�����
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, zPlane);
    glTexGenfv(GL_T, GL_EYE_PLANE, zPlane);
    break;
  case 3:
  default:

    //������ͼ
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  }

  glutPostRedisplay();
}

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);

  GLint iWidth, iHeight, iComponents;
  GLenum eFormat;

  //����������
  glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV, GL_REPLACE);
  //������������
  glGenTextures(TEXTURENUM, textureName);
  for (int i = 0; i < TEXTURENUM; ++i)
  {
    //��������ͼ��
    void *pImage = gltLoadTGA(texFileName[i], &iWidth, &iHeight, &iComponents, &eFormat);

    if (pImage)
    {

      //������
      glBindTexture(GL_TEXTURE_2D, textureName[i]);

      //����mimap
      gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

      //�����������
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    }
    free(pImage);
  }

  if (gltIsExtSupported("GL_EXT_texture_filter_anisotropic"))
  {
    GLfloat fLargest;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
  }
  glEnable(GL_TEXTURE_2D);

  //����Ϊ������ͼ
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  //����S��T�����������������
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);

}


void ShutdownRC()
{
  glDeleteTextures(TEXTURENUM, textureName);
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);

  //����ͼ,ʹ������ͶӰ
  glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glDepthMask(GL_FALSE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, textureName[ENV]);

    //�ر��������������
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(0.0f, 0.0f);

      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(1.0f, 0.0f);

      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(1.0f, 1.0f);
      
      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(0.0f, 1.0f);
    glEnd();
    //��ԭͶӰ���� 
    glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);

  glDepthMask(GL_TRUE);

  if (iRenderMode != 3)
  {
    glBindTexture(GL_TEXTURE_2D, textureName[STRIPES]);
  }
  glPushMatrix();
    glTranslatef(0.0f, 0.0f, zPos);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    gltDrawTorus(0.35, 0.15, 61, 37);
  glPopMatrix();

  glutSwapBuffers();
}
void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 1)
    h = 0;

  glViewport(0, 0, w, h);

  GLfloat aspect = (GLfloat)w/(GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.5, aspect, 1.0, 150.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutPostRedisplay();
}

void SpecialKey(int value, int x, int y)
{
  if (value == GLUT_KEY_LEFT)
  {
    yRot += 0.5f;
  }

  if (value == GLUT_KEY_RIGHT)
  {
    yRot -= 0.5f;
  }

  if (value == GLUT_KEY_UP)
  {
    zPos += 0.5f;
  }

  if (value == GLUT_KEY_DOWN)
  {
    zPos -= 0.5f;
  }

  if (yRot > 365.5f)
  {
    yRot = 0.0f;
  }

  glutPostRedisplay();
}

int main(int arg, char **argv)
{
  glutInit(&arg, argv);
  glutInitDisplayMode(GL_RGB | GL_DOUBLE | GL_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("TEXGEN");

  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  glutSpecialFunc(SpecialKey);
  glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("Object Linear", 1);
  glutAddMenuEntry("Eye linear", 2);
  glutAddMenuEntry("sphere map", 3);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  SetupRC();
  glutMainLoop();
  ShutdownRC();
  return 0;
}