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
 * ┃ file name: GraphicsScene.h                                             ┃
 * ┃ create date: 2013年9月24日星期二 22时51分31秒                          ┃
 * ┃ last modified date: 2013年9月24日星期二 22时51分31秒                   ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "Camera.h"

class ClickableTextItem: public QGraphicsTextItem
{
    Q_OBJECT
public:
    ClickableTextItem( QGraphicsItem* pParent = 0 );
signals:
    void Clicked( void );
protected:
    void hoverEnterEvent( QGraphicsSceneHoverEvent* pEvent );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent* pEvent );
    void mousePressEvent( QGraphicsSceneMouseEvent* pEvent );
};

class GraphicsScene: public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene( QObject* pParent = 0 );
    void SetCamera( Camera* pCamera );
signals:
    void SwitchShader( const QString& shaderFileName );
    void SetLightPos( const QVector3D& pos );
protected:
    void mousePressEvent( QGraphicsSceneMouseEvent* pEvent );
    void mouseMoveEvent( QGraphicsSceneMouseEvent* pEvent );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* pEvent );
    void wheelEvent( QGraphicsSceneWheelEvent* pEvent );
private slots:
    void Feedback( void );
private:
    // 鼠标事件需要
    QPointF         m_LastPos;

    Camera*         m_pCamera;
};

#endif // GRAPHICSSCENE_H
