#include "gltools.h"
#include <stdio.h>

//����곣��
#define CEILING 0
#define BRICK 1
#define FLOOR 2
#define TEXTURENUM 3

//����ͼ���·��
const char* texFileName[] = {"..\\ceiling.tga","..\\brick.tga","..\\floor.tga"};

//�����������
static GLuint textureName[TEXTURENUM];

//��ת���ƶ�
static GLfloat yRot = 0.0f;
static GLfloat zPos = 0.0f;

//�л���ͬ������ģʽ
void ProcessMenu(int value)
{
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
  switch (value)
  {
  case 0:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  	break;
  case 1:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    break;
  case 2:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    break;
  case 3:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    break;
  case 4:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    break;
  case 5:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    break;
  case 6:
    if (gltIsExtSupported("GL_EXT_texture_filter_anisotropic"))
    {

      //�����������Թ���
      GLfloat fLargest;
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
      printf("anisotropic:%f\n", fLargest);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
    }
    break;
  default:
    break;
  }

  glutPostRedisplay();
}

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //������Ȳ���,���������棬����󻭵�ǽ����ǰ����
  glEnable(GL_DEPTH_TEST);

  //����ͼ�����Ϣ
  GLint iWidth, iHeight, iComponents;
  GLenum eFormat;

  //����������
  glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV, GL_REPLACE);

  //�����������
  glGenTextures(TEXTURENUM, textureName);

  for (int i = 0; i < TEXTURENUM; ++i)
  {
    void *pImage = gltLoadTGA(texFileName[i], &iWidth, &iHeight, &iComponents, &eFormat);
    
    if (pImage)
    {

      //���������,����mipmap
      glBindTexture(GL_TEXTURE_2D, textureName[i]);
      gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    free(pImage);
  }

  glEnable(GL_TEXTURE_2D);

}


void ShutdownRC()
{
  //���ɾ���������
  glDeleteTextures(TEXTURENUM, textureName);
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

    //�ƶ�����ת
    glTranslatef(0.0f, 0.0f, zPos);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
   
    for(GLfloat z = -60.0f; z <= 0.0f; z += 10.0f)
    {
      //�󶨵ذ�������Ƶذ壬ע��glBeindTexture��glBegin��glEnd������Ч��
      glBindTexture(GL_TEXTURE_2D, textureName[FLOOR]);
      glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-10.0f, -10.0f, z);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-10.0f, -10.0f, z + 10.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(10.0f, -10.0f, z + 10.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(10.0f, -10.0f, z);
       
      glEnd();

      //���컨������
      glBindTexture(GL_TEXTURE_2D, textureName[CEILING]);
      glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-10.0f, 10.0f, z);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-10.0f, 10.0f, z + 10.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(10.0f, 10.0f, z + 10.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(10.0f, 10.0f, z);
      glEnd();

      //��שǽ������
      glBindTexture(GL_TEXTURE_2D, textureName[BRICK]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-10.0f, -10.0f, z);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-10.0f, 10.0f, z);

      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-10.0f, 10.0f, z + 10.0f);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-10.0f, -10.0f, z + 10.0f);

      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(10.0f, -10.0f, z);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(10.0f, 10.0f, z);

      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(10.0f, 10.0f, z + 10.0f);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(10.0f, -10.0f, z + 10.0f);
      glEnd();
    }

  GLclampf prioritize[TEXTURENUM] = {0.0f, 0.0f, 1.0f};
  glPrioritizeTextures(TEXTURENUM, textureName, prioritize);
  GLboolean isResident[TEXTURENUM];
  if (glAreTexturesResident(TEXTURENUM, textureName, isResident))
  {
    printf("all texture is resident\n");
  }
  else
  {
    printf("texture resident is : %d %d %d", isResident[0], isResident[1], isResident[2]);
  }
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
  glutCreateWindow("tunel");

  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  glutSpecialFunc(SpecialKey);
  glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("GL_NEAREST", 0);
  glutAddMenuEntry("GL_LINEAR", 1);
  glutAddMenuEntry("GL_NEAREST_MIPMAP_NEAREST", 2);
  glutAddMenuEntry("GL_LINEAR_MIPMAP_NEAREST", 3);
  glutAddMenuEntry("GL_NEAREST_MIPMAP_LINEAR", 4);
  glutAddMenuEntry("GL_LINEAR_MIPMAP_LINEAR", 5);
  glutAddMenuEntry("ANISOTROPIC", 6);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  SetupRC();
  glutMainLoop();
  ShutdownRC();
  return 0;
}