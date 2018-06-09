#include "gltools.h"
#include "math3d.h"

//���Ƶ�
GLint numOfPoints = 4;
static GLfloat controlPoints[4][3] = {{-4.0f, 0.0f, 0.0f},
{-6.0f, 4.0f, 0.0f},
{6.0f, -4.0f, 0.0f},
{4.0f, 0.0f, 0.0f}};

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glColor3f(1.0f, 0.0f, 1.0f);
}

//�����Ƶ�
void DrawPoints()
{
  glPointSize(2.5f);
  glBegin(GL_POINTS);
    for (int i = 0; i < numOfPoints; ++i)
    {
      glVertex3fv(controlPoints[i]);
    }
  glEnd();
}

void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
  {
    h = 1;
  }

  glViewport(0, 0, w, h);

  //ʹ������ͶӰ
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(-10.0f, 10.0f, -10.0f, 10.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  //���ñ��������ߣ����������ʵֻ��Ҫ����һ�Σ����Է���SetupRC������
  glMap1f(GL_MAP1_VERTEX_3, //���ɵ���������
    0.0f, //uֵ���½�
    100.0f, //uֵ���Ͻ�
    3, //�����������еļ����x,y,z���Լ����3
    numOfPoints, //u�����ϵĽף������Ƶ�ĸ���
    &controlPoints[0][0] //ָ����Ƶ����ݵ�ָ��
  );

  //�����ڻ��ƶ���֮ǰ����
  glEnable(GL_MAP1_VERTEX_3);
  //ʹ�û��ߵķ�ʽ�����ӵ�
  /*glBegin(GL_LINE_STRIP);
  for (int i = 0; i <= 100; i++)
  {
    glEvalCoord1f((GLfloat)i);
  }
  glEnd();*/

  glMapGrid1f(100, 0.0f, 100.0f);

  glEvalMesh1(GL_LINE, 0, 100);
  DrawPoints();

  glutSwapBuffers();

}

int main (int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Bezier");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();

  return 0;

}