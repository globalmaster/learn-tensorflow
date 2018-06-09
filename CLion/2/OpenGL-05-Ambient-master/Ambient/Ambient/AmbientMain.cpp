
//#include "../HeadFile/gltools.h"
#include<GL/glut.h>
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

// 设置渲染状态
void SetupRC()
{
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);	
	// 设置逆时针环绕的多边形为正面
	glFrontFace(GL_CCW);
	// 启用背面剔除
	glEnable(GL_CULL_FACE);
	// 启用光照计算
	glEnable(GL_LIGHTING);
	// 指定环境光强度（RGBA）
	GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	// 启用颜色追踪
	glEnable(GL_COLOR_MATERIAL);
	// 设置多边形正面的环境光和散射光材料属性，追踪glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// 指定清除色
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
}

// 绘制飞机模型
void RenderPlane()
{
	// 三角形图元
	glBegin(GL_TRIANGLES);

		// 第一种颜色（绿色）
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

		// 第二种颜色（灰色）
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

		// 第三种颜色（深灰）
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

		// 第四种颜色（黄色）
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

		// 第五种颜色（红色）
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

// 绘制场景
void RenderScene()
{
	// 清除颜色缓冲区和深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 保存矩阵状态
	glPushMatrix();
	// 旋转当前矩阵
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	// 绘制飞机模型
	RenderPlane();
	// 恢复矩阵状态
	glPopMatrix();
	// 交换缓冲区，显示画面
	glutSwapBuffers();
}

// 窗口大小改变时的处理
void ChangeSize(int w, int h)
{
	GLfloat nRange = 80.0f;
	// 避免除0
	if(h == 0)
		h = 1;

	// 设置视口大小
	glViewport(0, 0, w, h);

	// 重置投影矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 设置裁剪区域
	if (w <= h) 
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
	else 
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

	// 重置模型视图矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// 按键响应的处理
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

	// 刷新窗口
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