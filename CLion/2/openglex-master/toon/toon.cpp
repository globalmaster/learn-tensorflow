#include "gltools.h"
#include "math3d.h"
#include <math.h>

//ָ����ߵ�����
M3DVector3f vLightDir = {-1.0f, 1.0f, 1.0f};


void SetupRC()
{
  //һά����,�𽥼���ĺ�ɫ
  GLbyte toonMap[4][3] = {{32, 0, 0},
  {64, 0, 0}, {128, 0, 0}, {192, 0, 0}};

  glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

  //����������ü�����Ȳ���
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  //�������������������
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexEnvf(GL_TEXTURE_1D, GL_TEXTURE_ENV_MODE, GL_DECAL);

  //��������
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, toonMap);

  glEnable(GL_TEXTURE_1D);
}
//������Ȧ
void DrawDoughnut(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor, M3DVector3f vLightDir)
{
  M3DMatrix44f mInvertLight;
  M3DMatrix44f mModelView;
  M3DVector3f vNewLight;
  M3DVector3f vNormal;

  glGetFloatv(GL_MODELVIEW_MATRIX, mModelView);
  
  double majorStep = 2.0 * M3D_PI / numMajor;
  double minorStep = 2.0 * M3D_PI / numMinor;

  //�ѹ�任����������ռ���,�����Է�ת��ģ����ͼ����
  m3dInvertMatrix44(mInvertLight, mModelView);
  m3dTransformVector3(vNewLight, vLightDir, mInvertLight);
  vNewLight[0] -= mInvertLight[12];
  vNewLight[1] -= mInvertLight[13];
  vNewLight[2] -= mInvertLight[14];
  m3dNormalizeVector(vNewLight);

  for (int i = 0; i < numMajor; ++i)
  {
    double a0 = i * majorStep;
    double a1 = a0 + majorStep;
    GLfloat x0 = (GLfloat)cos(a0);
    GLfloat y0 = (GLfloat)sin(a0);
    GLfloat x1 = (GLfloat)cos(a1);
    GLfloat y1 = (GLfloat)sin(a1);

  glBegin(GL_TRIANGLE_STRIP);
    for (int j = 0; j <= numMinor; ++j)
    {
      double s = j * minorStep;
      GLfloat c = (GLfloat)cos(s);
      GLfloat r = minorRadius * c + majorRadius;
      GLfloat z = minorRadius * (GLfloat)sin(s);

      //��һ����
      vNormal[0] = x0*c;
      vNormal[1] = y0*c;
      vNormal[2] = z/minorRadius;
      m3dNormalizeVector(vNormal);
  
      //������������Ϊ���ǿ��
      //��ѧ����������������ж����������ļнǵĴ�С
      glTexCoord1f(m3dDotProduct(vNewLight, vNormal));
      glVertex3f(x0*r, y0*r, z);

      //�ڶ�����
      vNormal[0] = x1*c;
      vNormal[1] = y1*c;
      vNormal[2] = z/minorRadius;
      m3dNormalizeVector(vNormal);

      //������������Ϊ���ǿ��
      glTexCoord1f(m3dDotProduct(vNewLight, vNormal));
      glVertex3f(x1*r, y1*r, z);
    }
  glEnd();
  }

}

void RenderScene()
{
  static GLfloat yRot = 0.0f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
    glTranslatef(0.0f, 0.0f, -2.5f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    DrawDoughnut(0.35f, 0.15f, 50, 25, vLightDir);
  glPopMatrix();

  glutSwapBuffers();

  yRot += 0.5f;
}

void TimerFunc(int value)
{
  glutPostRedisplay();
  glutTimerFunc(50, TimerFunc, 1);
}

void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
    h = 1;

  GLfloat fAspect = (GLfloat)w/(GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.5, fAspect, 1.0, 50.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glutPostRedisplay();
}

int main(int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GL_DOUBLE | GL_RGB | GL_DEPTH);
  glutCreateWindow("CARTOON");

  SetupRC();
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  glutTimerFunc(50, TimerFunc, 1);

  glutMainLoop();
  return 0;
}