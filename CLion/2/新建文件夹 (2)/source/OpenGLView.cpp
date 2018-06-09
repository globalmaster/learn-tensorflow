/* ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
 * ┃     ┏━┣━┣┓　　┏┓┏┓┳┓┏━━┓┣┣━┓　┓　┓┣┳━┓       ┃
 * ┃     ┏┏┏╯━┓┳┳━┛┏╯┃┃┃　　┃┣┣━┓┃┃　┃┃┃　　       ┃
 * ┃     ┃┃┏━╮┃┗┗┏╯┗┃┃╯┃　　┃┏┣━┓┃┃　┃╯┣━┓       ┃
 * ┃     ╰┫┏━┻╯┗┳┣┛┏┛┃┃┣━━┫┃┃　┃┃┃┗╯　┃　　       ┃
 * ┃     ┏┫━┳━┫┏┃┣┓┗┃┃╯┃　　┃┃┃　┃　┃　┃　┣━┓       ┃
 * ┃     ┗┗┗━━╯┗┛┛╯┗╯╰　┗━━╯　┛　┛┗╯　╰┛┗　　       ┃
 * ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 * ┃                     Copyright (c) 2013 jiangcaiyang                    ┃
 * ┃ This software is provided 'as-is', without any express or implied      ┃
 * ┃ warranty. In no event will the authors be held liable for any damages  ┃
 * ┃ arising from the use of this software.                                 ┃
 * ┃                                                                        ┃
 * ┃ Permission is granted to anyone to use this software for any purpose,  ┃
 * ┃ including commercial applications, and to alter it and redistribute it ┃
 * ┃ freely, subject to the following restrictions:                         ┃
 * ┃                                                                        ┃
 * ┃ 1. The origin of this software must not be misrepresented; you must    ┃
 * ┃    not claim that you wrote the original software. If you use this     ┃
 * ┃    software in a product, an acknowledgment in the product             ┃
 * ┃    documentation would be appreciated but is not required.             ┃
 * ┃ 2. Altered source versions must be plainly marked as such, and must    ┃
 * ┃    not be misrepresented as being the original software.               ┃
 * ┃ 3. This notice may not be removed or altered from any source           ┃
 * ┃    distribution.                                                       ┃
 * ┃                                                                        ┃
 * ┃ jiangcaiyang jiangcaiyang123@163.com                                   ┃
 * ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 * ┃ file name: OpenGLView.cpp                                              ┃
 * ┃ create date: 2013年9月24日星期二 19时33分16秒                          ┃
 * ┃ last modified date: 2013年9月24日星期二 22时12分23秒                   ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#include <cmath>
#include <QMatrix4x4>
#include <QtOpenGL>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QMessageBox>
#include "OpenGLView.h"
/*---------------------------------------------------------------------------*/
bool g_ShowAxis = false;
/*---------------------------------------------------------------------------*/
void OpenGLView::PaintGL( void )
{
    glClear( GL_COLOR_BUFFER_BIT |
             GL_DEPTH_BUFFER_BIT );

    glPushMatrix( );
    m_Camera.Apply( );
    m_3DS.RenderGL( );
    if ( g_ShowAxis ) DrawAxis( );
    glPopMatrix( );
}
/*---------------------------------------------------------------------------*/
void OpenGLView::DrawAxis( void )
{
    static const qint32 s_Scale = 64;

    m_pShaderProgram->release( );// 绘制坐标轴不需要着色器
    glPushAttrib( GL_LINE_BIT );
    glLineWidth( 1.0f );
    glBegin( GL_LINES );

    // X轴
    glColor3ub( 255, 0, 0 );
    glVertex3i( 0, 0, 0 );
    glVertex3i( s_Scale, 0, 0 );

    // Y轴
    glColor3ub( 0, 255, 0 );
    glVertex3i( 0, 0, 0 );
    glVertex3i( 0, s_Scale, 0 );

    // Z轴
    glColor3ub( 0, 0, 255 );
    glVertex3i( 0, 0, 0 );
    glVertex3i( 0, 0, s_Scale );

    glEnd( );
    glPopAttrib( );
    m_pShaderProgram->bind( );// 开启着色器
}
/*---------------------------------------------------------------------------*/
void PopupError( const QString& info )
{
    QString header = QObject::tr( "Sorry but there are some errors in using shaders.\n"
                                  "Here are some useful information:\n" );
    QMessageBox::warning(
                Q_NULLPTR,
                QObject::tr( "Shader program error" ),
                header + info );
}
#define CHECK( stmt, info ) \
    if ( !stmt )\
{\
    PopupError( info );\
    return false;\
    }
/*---------------------------------------------------------------------------*/
OpenGLView::OpenGLView( QWidget* pParent ):
    QGraphicsView( pParent )
{
    m_Camera.SetPos( QVector3D( 0.0, 0.0, 20.0 ) );// 设置摄像机坐标
    m_3DS.Load( ":/Model/Various.3ds" );
}
/*---------------------------------------------------------------------------*/
OpenGLView::~OpenGLView( void )
{
    m_pShaderProgram->release( );
    m_pShaderProgram->removeShader( m_pVertexShader );
    if ( m_pVertexShader != Q_NULLPTR ) delete m_pVertexShader;
    if ( m_pShaderProgram != Q_NULLPTR ) delete m_pShaderProgram;
}
/*---------------------------------------------------------------------------*/
void OpenGLView::setScene( GraphicsScene* pScene )
{
    pScene->SetCamera( &m_Camera );
    QGraphicsView::setScene( pScene );
}
/*---------------------------------------------------------------------------*/
bool OpenGLView::SetupShaders( void )
{
    // 初始化着色器
    m_pShaderProgram = new QOpenGLShaderProgram( viewport( ) );
    m_pVertexShader = new QOpenGLShader( QOpenGLShader::Vertex,
                                         m_pShaderProgram );

    // 先假装添加一下着色器，然后调用SwitchShader()函数走常规路线
    CHECK( m_pShaderProgram->addShader( m_pVertexShader ),
           m_pShaderProgram->log( ) );

    return SwitchShader( ":/Shader/Diffuse.vert" );
}
/*---------------------------------------------------------------------------*/
bool OpenGLView::SwitchShader( const QString& shaderFileName )
{
    m_pShaderProgram->release( );
    m_pShaderProgram->removeShader( m_pVertexShader );

    // 重新编译着色器、连接至着色器程序最后使用它
    CHECK( m_pVertexShader->compileSourceFile( shaderFileName ),
           m_pVertexShader->log( ) );

    CHECK( m_pShaderProgram->addShader( m_pVertexShader ),
           m_pShaderProgram->log( ) );

    CHECK( m_pShaderProgram->link( ),
           m_pShaderProgram->log( ) );

    SetLightPos( );

    return true;
}
/*---------------------------------------------------------------------------*/
void OpenGLView::SetLightPos( const QVector3D& lightPos )
{
    static QVector3D s_LightPos( 30.0f, 30.0f, 30.0f );
    if ( !lightPos.isNull( ) )
    {
        s_LightPos = lightPos;
    }
    if ( m_pShaderProgram != Q_NULLPTR &&
         m_pShaderProgram->isLinked( ) )
    {
        m_pShaderProgram->bind( );
        m_pShaderProgram->setUniformValue( "lightPos", s_LightPos );

        m_pShaderProgram->release( );
    }
}
/*---------------------------------------------------------------------------*/
void OpenGLView::InitGL( void )
{
    glEnable( GL_MULTISAMPLE );
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );

    static bool runOnce = SetupShaders( );
    Q_UNUSED( runOnce );
    m_pShaderProgram->bind( );
}
/*---------------------------------------------------------------------------*/
void OpenGLView::resizeEvent( QResizeEvent* pEvent )
{
    scene( )->setSceneRect( 0.0,
                            0.0,
                            pEvent->size( ).width( ),
                            pEvent->size( ).height( ) );
    pEvent->accept( );
}
/*---------------------------------------------------------------------------*/
static void MyPerspective( GLdouble fov,
                           GLdouble aspectRatio,
                           GLdouble zNear,
                           GLdouble zFar )
{
    using namespace std;
    // 使用OpenGL函数，但是需要添加math.h头文件
    GLdouble rFov = fov * M_PI / 180.0;
    GLdouble up = -zNear * tan( rFov / 2.0 );
    GLdouble down = -up;
    GLdouble left = up * aspectRatio;
    GLdouble right = -left;
    glFrustum( left, right, up, down, zNear, zFar );
}
/*---------------------------------------------------------------------------*/
void OpenGLView::ResizeGL( int w, int h )
{
    GLdouble width = GLdouble( w );
    GLdouble height = GLdouble( h );
    static GLdouble s_AspectRatio = 16.0 / 9.0;
    GLdouble aspectRatio = width / height;

    if ( aspectRatio < s_AspectRatio )
    {
        GLint smallHeight = GLint( width / s_AspectRatio );
        GLint heightBlank = ( GLint( height ) - smallHeight ) / 2;
        glViewport( 0, heightBlank, GLint( width ), smallHeight );
    }
    else
    {
        GLint smallWidth = GLint( height * s_AspectRatio );
        GLint widthBlank = ( GLint( width ) - smallWidth ) / 2;
        glViewport( widthBlank, 0, smallWidth, GLint( height ) );
    }

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    MyPerspective( 45.0, s_AspectRatio, 0.5, 1000.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}
/*---------------------------------------------------------------------------*/
void OpenGLView::drawBackground( QPainter* pPainter,
                                 const QRectF& )
{
    pPainter->beginNativePainting( );
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    InitGL( );
    ResizeGL( pPainter->device( )->width( ),
              pPainter->device( )->height( ) );
    PaintGL( );

    glPopAttrib( );
    pPainter->endNativePainting( );
}
/*---------------------------------------------------------------------------*/
void OpenGLView::mousePressEvent( QMouseEvent* pEvent )
{
    QGraphicsView::mousePressEvent( pEvent );
}
/*---------------------------------------------------------------------------*/
void OpenGLView::mouseMoveEvent( QMouseEvent* pEvent )
{
    QGraphicsView::mouseMoveEvent( pEvent );
    viewport( )->repaint( );
}
/*---------------------------------------------------------------------------*/
void OpenGLView::mouseReleaseEvent( QMouseEvent* pEvent )
{
    QGraphicsView::mouseReleaseEvent( pEvent );
    viewport( )->repaint( );
}
/*---------------------------------------------------------------------------*/
void OpenGLView::wheelEvent( QWheelEvent* pEvent )
{
    QGraphicsView::wheelEvent( pEvent );
    viewport( )->repaint( );
}
