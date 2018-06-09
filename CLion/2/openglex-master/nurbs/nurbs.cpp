#include "gltools.h"	

static GLUnurbsObj *pNurb = NULL;

GLint nNumPoints = 4; // 4 X 4

//                 u  v  (x,y,z)	
GLfloat ctrlPoints[4][4][3]= {{{  -6.0f, -6.0f, 0.0f},	// u = 0,	v = 0
{	  -6.0f, -2.0f, 0.0f},	//			v = 1
{   -6.0f,  2.0f, 0.0f},	//			v = 2	
{   -6.0f,  6.0f, 0.0f}}, //			v = 3

{{  -2.0f, -6.0f, 0.0f},	// u = 1	v = 0
{   -2.0f, -2.0f, 8.0f},	//			v = 1
{   -2.0f,  2.0f, 8.0f},	//			v = 2
{   -2.0f,  6.0f, 0.0f}},	//			v = 3

{{   2.0f, -6.0f, 0.0f }, // u =2		v = 0
{    2.0f, -2.0f, 8.0f }, //			v = 1
{    2.0f,  2.0f, 8.0f },	//			v = 2
{    2.0f,  6.0f, 0.0f }},//			v = 3

{{   6.0f, -6.0f, 0.0f},	// u = 3	v = 0
{    6.0f, -2.0f, 0.0f},	//			v = 1
{    6.0f,  2.0f, 0.0f},	//			v = 2
{    6.0f,  6.0f, 0.0f}}};//			v = 3

// Knot sequence for the NURB
GLfloat Knots[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};


void DrawPoints(void)
{
  glColor3ub(255, 0, 0);
  glPointSize(5.0f);
  glBegin(GL_POINTS);
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        glVertex3fv(ctrlPoints[i][j]);
      }
    }
  glEnd();
}

// NURBS ����ʱ�Ļص����� 
void CALLBACK NurbsErrorHandler(GLenum nErrorCode)
{
  char cMessage[100] = {0,};

  strcpy(cMessage, "NURBS error : ");
  strcat(cMessage, (char*)gluErrorString(nErrorCode));

  glutSetWindowTitle(cMessage);
}


void RenderScene(void)
{
  glColor3ub(0,0,220);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glRotatef(330.0f, 1.0f, 0.0f, 0.0f);
  //�޼�����һ���պϵĻ�
  //���޼���
  GLfloat outSidePts[5][2] = 
  {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}};
  //���޼���
  GLfloat inSidePts[4][2] =
  {{0.25f, 0.25f}, { 0.5f, 0.5f}, {0.75f, 0.25f},{0.25f, 0.25f} };

  gluBeginSurface(pNurb);
  //����NURBS����
  gluNurbsSurface(pNurb,
    8, Knots, //u�������ڵĽ��������Լ��������
    8, Knots,//v�������ڵĽ��������Լ��������
    4 * 3,  //u�����ϵĿ��Ƶ���
    3,  //v�����ϵĿ��Ƶ���
    &ctrlPoints[0][0][0], //���Ƶ�����
    4, 4, //u v�Ĵ���
    GL_MAP2_VERTEX_3);//����������

  //��ʼ�޼�
  gluBeginTrim(pNurb);
  gluPwlCurve(pNurb,
    5,  //�޼���ĸ���
    &outSidePts[0][0], //�޼�������
    2, //��֮��ļ��
    GLU_MAP1_TRIM_2);//�޼�������
  gluEndTrim(pNurb);

  gluBeginTrim(pNurb);
  gluPwlCurve(pNurb, 4, &inSidePts[0][0], 2, GLU_MAP1_TRIM_2);
  gluEndTrim(pNurb);
  gluEndSurface(pNurb);

  DrawPoints();
  glPopMatrix();

  glutSwapBuffers();
}

void SetupRC()
{
  GLfloat  whiteLight[] = {0.7f, 0.7f, 0.7f, 1.0f };
  GLfloat  specular[] = { 0.7f, 0.7f, 0.7f, 1.0f};
  GLfloat  shine[] = { 100.0f };

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f );

  //��������
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // ������ɫ׷��
  glEnable(GL_COLOR_MATERIAL);

  //���ò�������
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, shine);

  //�Զ����ɷ���
  glEnable(GL_AUTO_NORMAL);

  //����NURBS��Ⱦ��
  pNurb = gluNewNurbsRenderer();
  //����NURBS��������
  gluNurbsCallback(pNurb, GLU_ERROR, (CallBack)NurbsErrorHandler);
  //���ò����ݲ�
  gluNurbsProperty(pNurb, GLU_SAMPLING_TOLERANCE, 25.0f);
  //������ʾģʽ
  gluNurbsProperty(pNurb, GLU_DISPLAY_MODE, GLU_FILL);
}


void ChangeSize(int w, int h)
{
  if(h == 0)
    h = 1;

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective (45.0f, (GLdouble)w/(GLdouble)h, 1.0, 40.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef (0.0f, 0.0f, -20.0f);
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("NURBS Surface");
  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  SetupRC();
  glutMainLoop();

  return 0;
}
