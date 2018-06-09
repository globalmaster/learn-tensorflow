#include "gltools.h"

static GLfloat angle = 0.0f;

static int iMode = 0;

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);

}

void DrawQuad()
{
  glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, -5.0f);
    glVertex3f(-1.0f, -1.0f, -5.0f);
    glVertex3f(1.0f, -1.0f, -5.0f);
    glVertex3f(1.0f, 1.0f, -5.0f);
    glEnd();
  glPopMatrix();
}

void DrawNormal()
{
  glPushMatrix();
  //�����ƶ�����ת
  glTranslatef(0.0f, 0.0f, -5.0f);
  glRotatef(angle, 1.0f, 1.0f, 0.0f);

  //���߿�������
  glColor3f(1.0f, 0.0f, 0.0f);
  glLineWidth(3.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glutSolidCube(1);

  //��ʵ��������
  glColor3f(0.0f, 0.0f, 0.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glutSolidCube(1);

  glPopMatrix();
}

void UseOffset()
{
  //����֮ǰ�ľ��������
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  
  //�����ƶ�����ת
  glTranslatef(0.0f, 0.0f, -5.0f);
  glRotatef(angle, 1.0f, 1.0f, 0.0f);

  //���߿�������,��������
  glColor3f(1.0f, 0.0f, 0.0f);
  //�����ߵĿ��Ϊ3
  glLineWidth(3.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glutSolidCube(1);

  //��ʵ��������
  glColor3f(0.0f, 0.0f, 0.0f);
  //���ö����ƫ�ƣ�����Ļ�⿿���۲�����ƫ��
  glPolygonOffset(-1.5, -1.0f);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glutSolidCube(1);

  glPopAttrib();
  glPopMatrix();
}

void UseStencil()
{
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  //���ģ�建����
  glClearStencil(0);
  glClear(GL_STENCIL_BUFFER_BIT);
  //һ��ʼ����Ϊ����ͨ��ģ����ԣ�����ģ��
  glStencilFunc(GL_ALWAYS, 1, 0xFFFF);
  //ģ�����ʧ��ʱ��ģ�����ͨ������Ȳ���ʧ��ʱ��ģ���ֵ���ֲ��޸�.
  //ͨ��ģ���������Ȳ���ʱ��ʹ������glStencilFuncָ����ref��1���滻��
  glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
  glEnable( GL_STENCIL_TEST );


  glTranslatef(0.0f, 0.0f, -5.0f);
  glRotatef(angle, 1.0f, 1.0f, 0.0f);

  //����ʵ���������
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glColor3f(0.0f, 0.0f, 0.0f);
  glutSolidCube(1);

  //����ֻ��ģ�建����ֵ��Ϊ1�ģ��ط���ͨ��ģ�����
  glStencilFunc(GL_NOTEQUAL, 1, 0xFFFF);
  //�����߿�������
  glLineWidth(3.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(1.0f, 0.0f, 0.0f);
  glutSolidCube(1);
  
  glPopAttrib();
  glPopMatrix();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if (iMode == 1)
  {
    UseOffset();
  }
  else if (iMode == 2)
  {
    UseStencil();
  }
  else
  {
    DrawNormal();
  }
  
  glutSwapBuffers();

}

void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  GLfloat aspect = (GLfloat)w/(GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.0, aspect, 1.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

void TimerFunc(int value)
{
  angle += 0.5f;
  if (angle > 360.5f)
  {
    angle = 0.0f;
  }

  glutPostRedisplay();
  glutTimerFunc(50, TimerFunc, 1);
}

void ProcessMenu(int value)
{
  iMode = value;
  glutPostRedisplay();
}

int main(int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
  glutCreateWindow("outline");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();

  glutTimerFunc(50, TimerFunc, 1);

  glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("normal", 0);
  glutAddMenuEntry("polygon offset", 1);
  glutAddMenuEntry("stencil", 2);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutMainLoop();

  return 0;
}