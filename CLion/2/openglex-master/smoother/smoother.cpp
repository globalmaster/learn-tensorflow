#include "gltools.h"
#include <math.h>
#include "math3d.h"

//��Ļ�Ŀ���
#define  SCREEN_X 800
#define  SCREEN_Y 600

//����С���ǵ�����
#define LARGE_NUM 20
#define MEDIUM_NUM 30
#define SMALL_NUM 40

//���ǵ�����
M3DVector2f smallStars[SMALL_NUM];
M3DVector2f mediumStars[MEDIUM_NUM];
M3DVector2f largeStars[LARGE_NUM];

void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //����Ϊ2D����ͶӰ,ʹ���������Ļ�����½ǿ�ʼ
  gluOrtho2D(0.0, SCREEN_X, 0.0, SCREEN_Y);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutPostRedisplay();
}

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //�����ȡ���ǵ�λ�� 
  for (int i = 0; i < SMALL_NUM; ++i)
  {
    smallStars[i][0] = (GLfloat)(rand() % SCREEN_X);
    smallStars[i][1] = (GLfloat)(rand() % SCREEN_Y);
  }
  for (int i = 0; i < MEDIUM_NUM; ++i)
  {
    mediumStars[i][0] = (GLfloat)(rand() % SCREEN_X);
    mediumStars[i][1] = (GLfloat)((rand() % SCREEN_Y) + 50);
  }

  for (int i = 0; i < LARGE_NUM; ++i)
  {
    largeStars[i][0] = (GLfloat)(rand() % SCREEN_X);
    largeStars[i][1] = (GLfloat)(rand() % SCREEN_Y);
  }
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor3f(1.0f, 1.0f, 1.0f);

  //��С����
  glPointSize(1.5);
  glBegin(GL_POINTS);
    for (int i = 0; i < SMALL_NUM; ++i)
      glVertex2fv(smallStars[i]);
  glEnd();

  //���еȴ�С������
  glPointSize(3.5);
  glBegin(GL_POINTS);
    for (int i = 0; i < MEDIUM_NUM; ++i)
    {
      glVertex2fv(mediumStars[i]);
    }
  glEnd();

  //������
  glPointSize(5.5);
  glBegin(GL_POINTS);
  for (int i = 0; i < LARGE_NUM; ++i)
  {
    glVertex2fv(largeStars[i]);
  }
  glEnd();

  //������
  GLfloat angle = 0.0;

  GLfloat xCircle = 650.0f;
  GLfloat yCircle = 400.0f;
  GLfloat r = 80.0f;
  glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xCircle, yCircle);
    for (angle = 0.0f; angle < 2.0f * 3.14159f; angle += 0.1f)
    {
      glVertex2f(xCircle + (float)cos(angle) * r, yCircle + (float)sin(angle) * r);
    }
    glVertex2f(xCircle + r, yCircle);
  glEnd();

  //��������
  glLineWidth(3.0);
  glBegin(GL_LINE_STRIP);
    glVertex2f(0.0f, 50.0f);
    glVertex2f(50.0f, 150.0f);
    glVertex2f(100.0f, 20.0f);
    glVertex2f(300.0f, 300.0f);
    glVertex2f(450.0f, 100.0f);
    glVertex2f(600.0f, 200.0f);
    glVertex2f(800.0f, 30.0f);
  glEnd();
  glutSwapBuffers();
}

void ProcessMenu(int value)
{
  switch (value)
  {
  case 1:
    {
      //�������
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
      glEnable(GL_POINT_SMOOTH);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
      glEnable(GL_LINE_SMOOTH);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      glEnable(GL_POLYGON_SMOOTH);
      glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
      break;
    }
  case 2:
    {
      //�رջ��
      glDisable(GL_BLEND);
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_POLYGON_SMOOTH);
      break;
    }
  default:
  	break;
  }

  glutPostRedisplay();
}
int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(SCREEN_X, SCREEN_Y);
  glutCreateWindow("smoother");

  //�Ҽ��˵�
  int menuID = glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("antialiasing", 1);
  glutAddMenuEntry("normal", 2);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();

  return 0;
}