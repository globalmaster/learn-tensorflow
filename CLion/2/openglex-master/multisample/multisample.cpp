#include "gltools.h"
#include "math3d.h"
#include "glframe.h"
#include <math.h>

#define  SPHERE_NUM 30
GLfloat fNoLight[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat fLowLight[] = {0.25f, 0.25f, 0.25f, 1.0f};
GLfloat fBrightLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat fLightPos[4] = {-100.0f, 100.0f, 50.0f, 1.0f};

GLFrame camara;
GLFrame sphere[SPHERE_NUM];

M3DMatrix44f mShadowMatrix;


void SetupRC()
{
  glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);

  M3DVector3f vPoints[3] = {{0.0f, -0.4f, 0.0f},
  { 10.0f, -0.4f, 0.0f  }, { 5.0f, -0.4f, -5.0f} };

  int iSphere;

  //�޳�����α���
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH);

  //���ù���
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  //��ƽ���ϵ�3������ȡ��ƽ��ľ���
  M3DVector4f vPlaneEquation;
  m3dGetPlaneEquation(vPlaneEquation, vPoints[0], vPoints[1], vPoints[2]);
  //����ͶӰ����
  m3dMakePlanarShadowMatrix(mShadowMatrix, vPlaneEquation, fLightPos);
  
  //������ɫ׷��
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  //������������λ��
  for (iSphere = 0; iSphere < SPHERE_NUM; iSphere++)
  {
    sphere[iSphere].SetOrigin((float)(((rand() % 400) - 200) * 0.1f), 0.0f,
      (float)((rand() % 400) - 200) * 0.1f);
  }
  //�������ز�����Ĭ���ǿ�����
  glEnable(GL_MULTISAMPLE);
}


void DrawGround()
{
  GLfloat fExtent = 20.0f;
  GLfloat step = 1.0f;
  GLfloat y = -0.4f;

  GLfloat x, z;
  for (x = -fExtent; x <= fExtent; x += step)
  {
    glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(0.0f, 1.0f, 0.0f);
      for (z = fExtent; z >= -fExtent; z -= step)
      {
        glVertex3f(x, y, z);
        glVertex3f(x + step, y, z);
      }
    glEnd();
  }
}

void DrawInhabitants(GLint nShadow)
{
  static GLfloat yRot = 0.0f;
  GLint i;

  //�ж��Ƿ�����Ӱ
  if (nShadow == 0)
  {
    yRot += 0.5f;
    glColor3f(0.0f, 1.0f, 0.0f);
  }
  else
  {
    glColor3f(0.0f, 0.0f, 0.0f);
  }
  //������
  for (i = 0; i < SPHERE_NUM; i++)
  {
    glPushMatrix();
    sphere[i].ApplyActorTransform();
    glutSolidSphere(0.3f, 17, 9);
    glPopMatrix();
  }

  glPushMatrix();
    //ƽ��
    glTranslatef(0.0f, 0.1f, -2.5f);

    if (nShadow == 0)
    {
      glColor3f(0.0f, 0.0f, 1.0f);
    }
    //��ת������
    glPushMatrix();
      glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
      glTranslatef(1.0f, 0.0f, 0.0f);
      glutSolidSphere(0.1f, 17, 9);
    glPopMatrix();
    //����Ӱ����������ȫ����
    if (nShadow == 0)
    {
      glColor3f(1.0f, 0.0f, 0.0f);
      glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
    }
    //������
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    gltDrawTorus(0.35, 0.15, 61, 37);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
  glPopMatrix();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
    //Ӧ��������任
    camara.ApplyCameraTransform();
    //���ù�Դλ��
    glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
    //������ɫ
    glColor3f(0.6f, 0.4f, 0.1f);
    //������
    DrawGround();

    //����Ӱ, �رչ���
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glPushMatrix();
      //������Ӱ����
      glMultMatrixf(mShadowMatrix);
      DrawInhabitants(1);
    glPopMatrix();
    //��������
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    DrawInhabitants(0);

  glPopMatrix();

  glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  GLfloat faspect = (GLfloat)w/(GLfloat)h;


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective(35.0f, faspect, 1.0f, 50.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, -0.4f, 0.0f);
  glutPostRedisplay();
}

void TimerFunc(int value)
{
  glutPostRedisplay();
  glutTimerFunc(10, TimerFunc, 1);
}


void ProcessMenu(int value)
{
  if (value == 1)
  {
    glEnable(GL_MULTISAMPLE);
  }
  else
  {
    glDisable(GL_MULTISAMPLE);
  }

  glutPostRedisplay();
}

int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowSize(800, 600);
  glutCreateWindow("multisample");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);

  glutTimerFunc(30, TimerFunc, 1);
  SetupRC();

  int menuID = glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("enable multisample", 1);
  glutAddMenuEntry("disable multisample", 2);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutMainLoop();
  return 0;
}