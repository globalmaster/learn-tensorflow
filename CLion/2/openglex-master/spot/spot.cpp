#include "gltools.h"

GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat specref[] = {1.0f, 1.0f, 1.0f, 1.0f};
//��Դλ��
GLfloat lightPos[] = {0.0f, 0.0f, 70.0f, 1.0f};
//�۹�ƹ��շ���z�Ḻ����
GLfloat spotDir[] = {0.0f, 0.0f, -1.0f};
//������ת
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

#define FLAT_MODE 1
#define SMOOTH_MODE 2
#define LOW_LEVEL 3
#define MEDIUM_LEVEL 4
#define HIGH_LEVEL 5

int iShade = FLAT_MODE;
int iLevel = LOW_LEVEL;

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //������Ȳ���
  glEnable(GL_DEPTH_TEST);
  //�޳�������Ҫ�ı���
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  //��������
  glEnable(GL_LIGHTING);
  //����light0���ղ���
  glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambientLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  //���ù�Դλ�úͷ���
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 80.0f);
  //����ȫ�ֻ�����
  glLightModelfv(GL_AMBIENT, ambientLight);
  //����light0
  glEnable(GL_LIGHT0);
  
  //������������ɫ׷��
  glEnable(GL_COLOR_MATERIAL);
  //���ö��������Ĳ��ϵĻ�����������������Ϊ��ɫ׷��
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  //���þ����ķ�������
  glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  glEnable(GL_NORMALIZE);
}

void RenderScene()
{
  if (iShade == FLAT_MODE)
  {
    glShadeModel(GL_FLAT);
  }
  else
  {
    glShadeModel(GL_SMOOTH);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //����ѹջ
  glPushMatrix();

    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    //��ת���������ù�Դλ��
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    glColor3ub(255, 0, 0);
    glutSolidCone(4.0, 6.0, 15, 15);

    glPushAttrib(GL_LIGHTING_BIT);
      //�رչ��ջ�С����
      glDisable(GL_LIGHTING);
      glColor3ub(255, 255, 0);
      glutSolidSphere(3.0, 15, 15);
    glPopAttrib();

  //�����ջ
  glPopMatrix();
  glColor3ub(0, 0, 255);
  if (iLevel == LOW_LEVEL)
  {
    glutSolidSphere(30.0f, 8, 8);
  }
  else if (iLevel == MEDIUM_LEVEL)
  {
    glutSolidSphere(30.0f, 10, 10);
  }
  else
  {
    glutSolidSphere(30.0f, 15, 15);
  }

  glutSwapBuffers();
}

void ChangeSize(int w, int h)
{
  if (h == 0)
  {
    h = 1;
  }
  //�����ӿ�
  glViewport(0, 0, w, h);
  GLfloat faspect = (GLfloat)w/(GLfloat)h;

  //͸��ͶӰ�任
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(35.0, faspect, 1.0, 350.0);

  //ģ����ͼ����
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //������ʾ����ƽ��
  glTranslatef(0.0f, 0.0f, -250.0f);

  glutPostRedisplay();
}

void SpecialKeys(int keys, int x, int y)
{
  //�ı���ת�Ƕ�
  if (keys == GLUT_KEY_UP)
  {
    xRot -= 5.0f;
  }
  if (keys == GLUT_KEY_DOWN)
  {
    xRot += 5.0f;
  }

  if (keys == GLUT_KEY_LEFT)
  {
    yRot += 5.0f;
  }

  if (keys == GLUT_KEY_RIGHT)
  {
    yRot -= 5.0f;
  }

  if (xRot > 356.0f)
  {
    xRot = 0.0f;
  }
  else if (xRot < -1.0f)
  {
    xRot = 355.0f;
  }

  if (yRot > 356.0f)
  {
    yRot = 0.0f;
  }
  else if (yRot < -1.0f)
  {
    yRot = 355.0f;
  }

  glutPostRedisplay();
}

void ProcessMenu(int key)
{
  switch(key)
  {
  case 1:
    iShade = FLAT_MODE;
    break;
  case 2:
    iShade = SMOOTH_MODE;
    break;
  case 3:
    iLevel = LOW_LEVEL;
    break;
  case 4:
    iLevel = MEDIUM_LEVEL;
    break;
  case 5:
    iLevel = HIGH_LEVEL;
    break;
  default:
    break;
  }
  glutPostRedisplay();
}
int main(int args, char *arv[])
{
  glutInit(&args, arv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("spot light");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  glutSpecialFunc(SpecialKeys);
  //�����˵�
  int menuID = glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("Flag Mode", 1);
  glutAddMenuEntry("SMOOTH Mode", 2);
  glutAddMenuEntry("Low Level", 3);
  glutAddMenuEntry("Midum Level", 4);
  glutAddMenuEntry("High Level", 5);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  SetupRC();
  glutMainLoop();
  return 0;
}