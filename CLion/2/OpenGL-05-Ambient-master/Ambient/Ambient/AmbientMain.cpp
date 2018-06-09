
//#include "../HeadFile/gltools.h"
#include<GL/glut.h>
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

// ������Ⱦ״̬
void SetupRC()
{
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);	
	// ������ʱ�뻷�ƵĶ����Ϊ����
	glFrontFace(GL_CCW);
	// ���ñ����޳�
	glEnable(GL_CULL_FACE);
	// ���ù��ռ���
	glEnable(GL_LIGHTING);
	// ָ��������ǿ�ȣ�RGBA��
	GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// ���ù���ģ�ͣ���ambientLight��ָ����RGBAǿ��ֵӦ�õ�������
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	// ������ɫ׷��
	glEnable(GL_COLOR_MATERIAL);
	// ���ö��������Ļ������ɢ���������ԣ�׷��glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// ָ�����ɫ
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
}

// ���Ʒɻ�ģ��
void RenderPlane()
{
	// ������ͼԪ
	glBegin(GL_TRIANGLES);

		// ��һ����ɫ����ɫ��
		glColor3ub(0, 255, 0);

		glVertex3f(0.0f, 0.0f, 60.0f);
		glVertex3f(-15.0f, 0.0f, 30.0f);
		glVertex3f(15.0f,0.0f,30.0f);

		glVertex3f(15.0f,0.0f,30.0f);
		glVertex3f(0.0f, 15.0f, 30.0f);
		glVertex3f(0.0f, 0.0f, 60.0f);

		glVertex3f(0.0f, 0.0f, 60.0f);
		glVertex3f(0.0f, 15.0f, 30.0f);
		glVertex3f(-15.0f,0.0f,30.0f);

		// �ڶ�����ɫ����ɫ��
		glColor3ub(192,192,192);

		glVertex3f(-15.0f,0.0f,30.0f);
		glVertex3f(0.0f, 15.0f, 30.0f);
		glVertex3f(0.0f, 0.0f, -56.0f);

		glVertex3f(0.0f, 0.0f, -56.0f);
		glVertex3f(0.0f, 15.0f, 30.0f);
		glVertex3f(15.0f,0.0f,30.0f);	

		glVertex3f(15.0f,0.0f,30.0f);
		glVertex3f(-15.0f, 0.0f, 30.0f);
		glVertex3f(0.0f, 0.0f, -56.0f);

		// ��������ɫ����ң�
		glColor3ub(64,64,64);

		glVertex3f(0.0f,2.0f,27.0f);
		glVertex3f(-60.0f, 2.0f, -8.0f);
		glVertex3f(60.0f, 2.0f, -8.0f);

		glVertex3f(60.0f, 2.0f, -8.0f);
		glVertex3f(0.0f, 7.0f, -8.0f);
		glVertex3f(0.0f,2.0f,27.0f);

		glVertex3f(60.0f, 2.0f, -8.0f);
		glVertex3f(-60.0f, 2.0f, -8.0f);
		glVertex3f(0.0f,7.0f,-8.0f);

		glVertex3f(0.0f,2.0f,27.0f);
		glVertex3f(0.0f, 7.0f, -8.0f);
		glVertex3f(-60.0f, 2.0f, -8.0f);

		// ��������ɫ����ɫ��
		glColor3ub(255,255,0);

		glVertex3f(-30.0f, -0.50f, -57.0f);
		glVertex3f(30.0f, -0.50f, -57.0f);
		glVertex3f(0.0f,-0.50f,-40.0f);

		glVertex3f(0.0f,-0.5f,-40.0f);
		glVertex3f(30.0f, -0.5f, -57.0f);
		glVertex3f(0.0f, 4.0f, -57.0f);

		glVertex3f(0.0f, 4.0f, -57.0f);
		glVertex3f(-30.0f, -0.5f, -57.0f);
		glVertex3f(0.0f,-0.5f,-40.0f);

		glVertex3f(30.0f,-0.5f,-57.0f);
		glVertex3f(-30.0f, -0.5f, -57.0f);
		glVertex3f(0.0f, 4.0f, -57.0f);

		// ��������ɫ����ɫ��
		glColor3ub(255,0,0);

		glVertex3f(0.0f,0.5f,-40.0f);
		glVertex3f(3.0f, 0.5f, -57.0f);
		glVertex3f(0.0f, 25.0f, -65.0f);

		glVertex3f(0.0f, 25.0f, -65.0f);
		glVertex3f(-3.0f, 0.5f, -57.0f);
		glVertex3f(0.0f,0.5f,-40.0f);

		glVertex3f(3.0f,0.5f,-57.0f);
		glVertex3f(-3.0f, 0.5f, -57.0f);
		glVertex3f(0.0f, 25.0f, -65.0f);

	glEnd();
}

// ���Ƴ���
void RenderScene()
{
	// �����ɫ����������Ȼ�����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �������״̬
	glPushMatrix();
	// ��ת��ǰ����
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	// ���Ʒɻ�ģ��
	RenderPlane();
	// �ָ�����״̬
	glPopMatrix();
	// ��������������ʾ����
	glutSwapBuffers();
}

// ���ڴ�С�ı�ʱ�Ĵ���
void ChangeSize(int w, int h)
{
	GLfloat nRange = 80.0f;
	// �����0
	if(h == 0)
		h = 1;

	// �����ӿڴ�С
	glViewport(0, 0, w, h);

	// ����ͶӰ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ���òü�����
	if (w <= h) 
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
	else 
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

	// ����ģ����ͼ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// ������Ӧ�Ĵ���
void SpecialKeys(int key, int x, int y)
{
	if(key == GLUT_KEY_UP)
		xRot-= 5.0f;

	if(key == GLUT_KEY_DOWN)
		xRot += 5.0f;

	if(key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	if(key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	if(key > 356.0f)
		xRot = 0.0f;

	if(key < -1.0f)
		xRot = 355.0f;

	if(key > 356.0f)
		yRot = 0.0f;

	if(key < -1.0f)
		yRot = 355.0f;

	// ˢ�´���
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutCreateWindow("Ambient Light Demo");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();

	glutMainLoop();

	return 0;
}