#include "gltools.h"
#include "glFrame.h"
#include <math.h>


#define SPHERE_NUM 20
//����Ľ�ɫ֡
GLFrame spheres[SPHERE_NUM];
//�����֡
GLFrame camara;

static void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //���ö����ģʽΪ����ģʽ
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  GLfloat x;
  GLfloat z;
  //�����������λ�� ��-20 ��20֮��
  for (unsigned int i = 0; i < SPHERE_NUM; ++i)
  {
    x = (GLfloat)(rand() % 400 - 200) * 0.1f;
    z = (GLfloat)(rand() % 400 - 200) * 0.1f;
    spheres[i].SetOrigin(x, 0.0f, z);
  }

  glEnable(GL_DEPTH_TEST);
}

static void DrawGround()
{
  GLfloat y = -4.0f;
  GLfloat fStep = 2.0f;
  GLfloat fExtent = 40.0f;

  //��������
  glBegin(GL_LINES);
    for (GLfloat line = -fExtent; line < fExtent; line += fStep)
    {
      glVertex3f(line, y, -fExtent);
      glVertex3f(line, y, fExtent);

      glVertex3f(-fExtent, y, line);
      glVertex3f(fExtent, y, line);
    }
  glEnd();

}

static void RenderScene()
{
  static GLfloat yRot = 0.0f;
  yRot += 0.5f;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glColor3f(0.0f, 1.0f, 0.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    //������任
    camara.ApplyCameraTransform();
    //��������
    DrawGround();
    //������
    for (int i = 0; i < SPHERE_NUM; ++i) 
    {
      glPushMatrix();
        //Ӧ�ý�ɫ�任,���������λ��
        spheres[i].ApplyActorTransform();
        glutSolidSphere(0.5f, 13, 26);
      glPopMatrix();
    }

    glPushMatrix();
    //����ƽ��
      glTranslatef(0.0f, 0.0f, -25.0f);
    //��ת
      glRotatef(yRot, 0.0f, 1.0f, 0.0f);
      //��Բ��
      gltDrawTorus(0.95, 0.15, 40, 20);
    glPopMatrix();
  glPopMatrix();

  glutSwapBuffers();
}

static void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
  {
    h = 1;
  }

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //͸��ͶӰ 
  gluPerspective(35.0, (GLdouble)w/h, 1.0, 50.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glutPostRedisplay();
  
}

static void TimerFun(int value)
{
  //��ʱˢ�³���
  glutPostRedisplay();
  glutTimerFunc(50, TimerFun, 1);
}

void SpecialKey(int type, int x, int y)
{
  //�����ƶ������
  if (GLUT_KEY_UP == type)
  {
    camara.MoveUp(0.5f);
  }
  //�����ƶ������ 
  if (GLUT_KEY_DOWN == type)
  {
    camara.MoveUp(-0.5f);
  }
  //����ƶ�
  if (GLUT_KEY_LEFT == type)
  {
    camara.MoveForward(-0.5f);
  }
  //��ǰ�ƶ�
  if (GLUT_KEY_RIGHT == type)
  {
    camara.MoveForward(0.5f);
  }

  glutPostRedisplay();
}

int main(int args, char** arv)
{
  glutInit(&args, arv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("sphere world");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  glutTimerFunc(50, TimerFun, 1);
  glutSpecialFunc(SpecialKey);
  SetupRC();
  glutMainLoop();
  return 0;
}