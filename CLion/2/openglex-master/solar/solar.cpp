#include <gl/glut.h>
#include <gl/GL.h>
#include <math.h>

static GLfloat whiteLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
static GLfloat sourceLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
static GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};

static void SetupRC()
{
  //������Ȳ���
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CCW);
  //���òü���
  glEnable(GL_CULL_FACE);

  //��������
  glEnable(GL_LIGHTING);
  //���ù��ղ���
  glLightModelfv(GL_AMBIENT_AND_DIFFUSE, whiteLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  //������ԴLIGHT0
  glEnable(GL_LIGHT0);

  //���ò�������
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

static void RenderScene()
{
  static GLfloat fEarth = 0.0f;
  static GLfloat fMoon = 0.0f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  
  //����Ļ����ƽ��
  glTranslatef(0.0f, 0.0f, -300.0f);
  //�ȹرչ���
  glDisable(GL_LIGHTING);
  //����
  glColor3ub(255, 255, 0);
  glutSolidSphere(20.0, 15, 15);
  //��������
  glEnable(GL_LIGHTING);
  //LIGHT0�ķ����
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  //������
  glRotatef(fEarth, 0.0f, 1.0f, 0.0f);
  glColor3ub(0,0,255);
  glTranslatef(105.0f, 0.0f, 0.0f);
  glutSolidSphere(15.0, 15, 15);

  //������
  glColor3ub(200, 200, 200);
  glRotatef(fMoon, 0.0f, 1.0f, 0.0f);
  glTranslatef(30.0f, 0.0f, 0.0f);
  glutSolidSphere(10.0, 15, 15);
  //�������ת
  fMoon += 15.0f;
  if (fMoon > 360.0f)
  {
    fMoon = 0.0f;
  }
  //�������ת
  fEarth += 10.0f;
  if (fEarth > 360.0f)
  {
    fEarth = 0.0f;
  }
  glPopMatrix();

  glutSwapBuffers();
}

static void ChangeSize(GLsizei w, GLsizei h)
{

  if (h == 0)
  {

    h = 1;
  }
  //�����ӿ�
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //���ÿ�������
  GLfloat aspect = (GLfloat)w / (GLfloat)h;
  gluPerspective(45.0, aspect, 1.0, 400.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void TimerFunc(int value)
{
  glutPostRedisplay();
  glutTimerFunc(100, TimerFunc, 1);
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(400,400);
  glutCreateWindow("solar Test");
  glutReshapeFunc(ChangeSize);
  glutTimerFunc(100, TimerFunc, 1);
  glutDisplayFunc(RenderScene);
  SetupRC();
  glutMainLoop();
  return 0;
} 