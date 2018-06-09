#include "gltools.h"
#include "math3d.h"

//��Դλ��
GLfloat lightPos[4] = { -100.0f, 100.0f, 50.0f, 1.0f };
GLfloat lightPosMirror[4] = {-100.0f, -100.0f, 50.0f, 1.0f};

GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void DrawGround()
{
  //������
  GLfloat fExtent = 20.0f;
  GLfloat fStep = 0.5f;
  GLfloat y = 0.0f;
  GLfloat fColor;
  GLfloat iStrip, iRun;
  GLint iBounce = 0;

  glShadeModel(GL_FLAT);
  for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
  {
    glBegin(GL_TRIANGLE_STRIP);
    for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
    {
      if((iBounce %2) == 0)
        fColor = 1.0f;
      else
        fColor = 0.0f;

      glColor4f(fColor, fColor, fColor, 0.5f);
      glVertex3f(iStrip, y, iRun);
      glVertex3f(iStrip + fStep, y, iRun);

      iBounce++;
    }
    glEnd();
  }
  glShadeModel(GL_SMOOTH);
}

void DrawWorld()
{
  glColor3f(1.0f, 0.0f, 0.0f);
  glPushMatrix();

    glTranslatef(0.0f, 0.5f, -3.5f);
    gltDrawTorus(0.25, 0.08, 68, 37);

  glPopMatrix();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glPushMatrix();
    //���ù�Դ�ĵ�Ӱ
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosMirror);

    glPushMatrix();
      //��Ϊ�Ǿ�������Ҫ����������
      glFrontFace(GL_CW);          
      glScalef(1.0f, -1.0f, 1.0f);
      DrawWorld();
      glFrontFace(GL_CCW);
    glPopMatrix();
    //������ʱ���رչ�Դ, ����ɼ���������ɫ��
    glDisable(GL_LIGHTING);
    //�򿪻�ϣ����û������
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //������
    DrawGround();
    //�رջ��Ч��
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

  glPopMatrix();
  //���ù�Դ�����Ͻ�
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  DrawWorld();

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

  gluPerspective(35.0, faspect, 1.0f, 50.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //��Y�Ḻ����ƽ��һ��
  glTranslatef(0.0f, -0.4f, 0.0f);
  glutPostRedisplay();

}

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //������Ȳ��ԣ��޳����屳��
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  //��ʱ�뷽����Ƶ���Ϊ����
  glFrontFace(GL_CCW);
  
  //�������գ����ù�Դ����
  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
  glEnable(GL_LIGHT0);

  //������ɫ׷�٣����þ������Ч��
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

}

int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Reflection");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();
  return 0;
}