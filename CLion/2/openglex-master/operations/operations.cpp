#include "gltools.h"
#include <math.h>

static GLbyte *pImage = NULL;
static GLint iRenderMode = 0;
static GLint iWidth, iHeight, iComponents;
static GLenum eFormat;

void ProcessMenu(int value)
{
	if (value == 1)
	{

    //����ͼ��Ŀ���
		gltWriteTGA("screenshot.tga");
	}
	else
	{
		iRenderMode = value;
	}

	glutPostRedisplay();

}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  

	if (!pImage)
	{
		return;
	}

  //���ù�դλ��
  glRasterPos2i(0, 0);

  //�޸ĵ�ͼ�񣬺����ڷ�ת��ӳ������
  GLbyte *pModifyImage = NULL;
  GLfloat invertMap[256];

	switch (iRenderMode)
	{
  case 2:

    //ͼ��ת
    glPixelZoom(-1.0f, -1.0f);

    //��ת���ͼ���,��Ⱦ����Ҳ��ת�����ˣ�����˴����Ͻǿ�ʼ�����½���Ⱦ���������õ�ת��Ĺ�դλ��Ϊͼ��Ŀ�ߡ�
    glRasterPos2i(iWidth, iHeight);
    break;
  case 3:

    //��ͼ���������Ļ
    GLint viewport[4];

    //ȡ���ӿڵĴ�С
    glGetIntegerv(GL_VIEWPORT, viewport);

    //����������
    glPixelZoom((GLfloat)viewport[2]/iWidth, (GLfloat)viewport[3]/iHeight);
    break;

  case 4:

    //ֻ������ɫ
    glPixelTransferf(GL_RED_SCALE, 1.0f);
    glPixelTransferf(GL_GREEN_SCALE, 0.0f);
    glPixelTransferf(GL_BLUE_SCALE, 0.0f);
    break;
  case 5:
    
    //ֻ������ɫ
    glPixelTransferf(GL_RED_SCALE, 0.0f);
    glPixelTransferf(GL_GREEN_SCALE, 1.0f);
    glPixelTransferf(GL_BLUE_SCALE, 0.0f);
    break;
  case 6:

    //ֻ������ɫ
    glPixelTransferf(GL_RED_SCALE, 0.0f);
    glPixelTransferf(GL_GREEN_SCALE, 0.0f);
    glPixelTransferf(GL_BLUE_SCALE, 1.0f);
    break;
  case 7:
    
    //����Ⱦͼ����ɫ��������
    glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

    //NTSC��׼,ת�ɺڰ�ͼ�� 
    glPixelTransferf(GL_RED_SCALE, 0.3f);
    glPixelTransferf(GL_GREEN_SCALE, 0.59f);
    glPixelTransferf(GL_BLUE_SCALE, 0.11f);

    //������ʱ�ռ��������޸ĺ��ͼ��
    pModifyImage = (GLbyte *)malloc(iWidth * iHeight * 3);
    if (!pModifyImage)
    {
      return;
    }

    //����ɫ�������ж�ȡͼ�����������
    glReadPixels(0, 0, iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, pModifyImage);

    //��ԭ
    glPixelTransferf(GL_RED_SCALE, 1.0f);
    glPixelTransferf(GL_GREEN_SCALE, 1.0f);
    glPixelTransferf(GL_BLUE_SCALE, 1.0f);
    break;
  case 8:

    //���÷�ת����ɫӳ��
    invertMap[0] = 1.0f;
    for(int i = 0; i < 256; ++i)
    {
      invertMap[i] = 1.0f - (1.0f / 255.0f * (GLfloat)i);
    }

    //ӳ��
    glPixelMapfv(GL_PIXEL_MAP_R_TO_R, 255, invertMap);
    glPixelMapfv(GL_PIXEL_MAP_G_TO_G, 255, invertMap);
    glPixelMapfv(GL_PIXEL_MAP_B_TO_B, 255, invertMap);

    //������ɫӳ��
    glPixelTransferi(GL_MAP_COLOR, GL_TRUE);
	default:
			break;
	}

  if (pModifyImage)
  {

    //���ڰ�ͼ��
    glDrawPixels(iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, pModifyImage);
    free(pModifyImage);
  }
  else
  {
  	glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);
  }

  //��ԭ
  glPixelTransferf(GL_RED_SCALE, 1.0f);
  glPixelTransferf(GL_GREEN_SCALE, 1.0f);
  glPixelTransferf(GL_BLUE_SCALE, 1.0f);
  glPixelZoom(1.0f, 1.0f);
  glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
	glutSwapBuffers();
}

void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //�������صĴ洢��ʽ
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //����ͼ������
  pImage = gltLoadTGA("horse.tga", &iWidth, &iHeight, &iComponents, &eFormat);

}

//�ͷ�ͼ������ռ�õ��ڴ�ռ�
void ShutdownRC()
{
	if (pImage)
	{
		free(pImage);
		pImage = NULL;
	}
}

void ChangeSize(GLsizei w, GLsizei h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

int main(int args, char **argv)
{
	glutInit(&args, argv);
	glutInitDisplayMode(GL_RGB | GL_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("pixel operation");
	
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

  //���ò˵�
	int menuID = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Save Image", 1);
	glutAddMenuEntry("Flip", 2);
	glutAddMenuEntry("zoom pixel fill window", 3);
  glutAddMenuEntry("Just Red", 4);
  glutAddMenuEntry("Just Green", 5);
  glutAddMenuEntry("Just Blue", 6);
  glutAddMenuEntry("black & white", 7);
  glutAddMenuEntry("invert map", 8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	SetupRC();
	glutMainLoop();
	ShutdownRC();

	return 0;
}
