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
 * ┃ file name: OpenGLView.h                                                ┃
 * ┃ create date: 2013年9月24日星期二 12时32分33秒                          ┃
 * ┃ last modified date: 2013年9月24日星期二 22时12分23秒                   ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H

#include <QVector3D>
#include <QGraphicsView>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "Camera.h"
#include "Format3DS.h"
#include "GraphicsScene.h"

class OpenGLView: public QGraphicsView,
        protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLView( QWidget* pParent = 0 );
    virtual ~OpenGLView( void );
    void setScene( GraphicsScene* pScene );
public slots:
    bool SwitchShader( const QString& shaderFileName );
    void SetLightPos( const QVector3D& lightPos = QVector3D( ) );
protected:
    void resizeEvent( QResizeEvent* pEvent );
    void mousePressEvent( QMouseEvent* pEvent );
    void mouseReleaseEvent( QMouseEvent* pEvent );
    void mouseMoveEvent( QMouseEvent* pEvent );
    void wheelEvent( QWheelEvent* pEvent );
    void drawBackground( QPainter* pPainter, const QRectF& rect );
private:
    void InitGL( void );
    void ResizeGL( int width, int height );
    void PaintGL( void );

    void DrawAxis( void );

    bool SetupShaders( void );

    Camera          m_Camera;
    Format3DS       m_3DS;

    // 着色器
    QOpenGLShader*  m_pVertexShader;
    QOpenGLShaderProgram* m_pShaderProgram;
};

#endif // OPENGLVIEW_H
