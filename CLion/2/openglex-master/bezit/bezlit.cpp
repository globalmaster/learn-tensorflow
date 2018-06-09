#include "gltools.h"
#include "math3d.h"

//���Ƶ� 
GLint nNumPoints = 3;

GLfloat ctrlPoints[3][3][3]= {{{  -4.0f, 0.0f, 4.0f},	
{ -2.0f, 4.0f, 4.0f},	
{  4.0f, 0.0f, 4.0f }},

{{  -4.0f, 0.0f, 0.0f},	
{ -2.0f, 4.0f, 0.0f},	
{  4.0f, 0.0f, 0.0f }},

{{  -4.0f, 0.0f, -4.0f},	
{ -2.0f, 4.0f, -4.0f},	
{  4.0f, 0.0f, -4.0f }}};


//�����Ƶ� 
void DrawPoints(void)
{
  int i,j;	

  glColor3f(1.0f, 0.0f, 0.0f);
  //�ѵ�Ŵ�һ�㣬���ø���� 
  glPointSize(5.0f);

  glBegin(GL_POINTS);
  for(i = 0; i < nNumPoints; i++)
    for(j = 0; j < 3; j++)
      glVertex3fv(ctrlPoints[i][j]);
  glEnd();
}




void RenderScene(void)
{
  // Clear the window with current clearing color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ����ģ����ͼ����
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  //��תһ���ĽǶȷ���۲�
  glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(60.0f, 1.0f, 0.0f, 0.0f);


  glColor3f(0.0f, 0.0f, 1.0f);
  //����ӳ�䷽ʽ��ֻ��Ҫ����һ�ο�����SetupRC�е��á�
  glMap2f(GL_MAP2_VERTEX_3,	//���ɵ���������
    0.0f,						// u���½�
    10.0f,						//u���Ͻ�
    3,              //�����е�ļ��
    3,              //u�����ϵĽ�
    0.0f,           //v���½�
    10.0f,          //v���Ͻ�
    9,							// ���Ƶ�֮��ļ��
    3,							// v�����ϵĽ�
    &ctrlPoints[0][0][0]);		//���Ƶ�����

  //������ֵ�� 
  glEnable(GL_MAP2_VERTEX_3);


  //��0��10ӳ��һ������10��������� 
  glMapGrid2f(10,0.0f,10.0f,10,0.0f,10.0f);

  // ��������
  glEvalMesh2(GL_FILL,0,10,0,10);

  //�����Ƶ�
  DrawPoints();
  
  glPopMatrix();

  glutSwapBuffers();
}

void SetupRC()
{
  // ���ù�Դ
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // ������ɫ׷��
  glEnable(GL_COLOR_MATERIAL);

  // ���ò�������
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  // �Զ����ɷ���
  glEnable(GL_AUTO_NORMAL);

}


void ChangeSize(int w, int h)
{
  // Prevent a divide by zero
  if(h == 0)
    h = 1;

  // Set Viewport to window dimensions
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);

  // Modelview matrix reset
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Lit 3D Bezier Surface");
  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  SetupRC();
  glutMainLoop();

  return 0;
}