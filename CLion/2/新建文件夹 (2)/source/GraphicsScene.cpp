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
 * ┃ file name: GraphicsScene.cpp                                           ┃
 * ┃ create date: 2013年9月24日星期二 23时3分11秒                           ┃
 * ┃ last modified date: 2013年9月24日星期二 23时3分11秒                    ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#include <QDebug>
#include <QtOpenGL>
#include <QApplication>
#include <QGraphicsSceneHoverEvent>
#include "GraphicsScene.h"
#include "ShaderOptionDialog.h"
#include "Feedback/FeedbackDialog.h"
/*---------------------------------------------------------------------------*/
ClickableTextItem::ClickableTextItem( QGraphicsItem* pParent ):
    QGraphicsTextItem( pParent )
{

}
/*---------------------------------------------------------------------------*/
void ClickableTextItem::hoverEnterEvent( QGraphicsSceneHoverEvent* pEvent )
{
    QApplication::setOverrideCursor( Qt::PointingHandCursor );
    QGraphicsTextItem::hoverEnterEvent( pEvent );
}
/*---------------------------------------------------------------------------*/
void ClickableTextItem::hoverLeaveEvent( QGraphicsSceneHoverEvent* pEvent )
{
    QApplication::restoreOverrideCursor( );
    QGraphicsTextItem::hoverLeaveEvent( pEvent );
}
/*---------------------------------------------------------------------------*/
void ClickableTextItem::mousePressEvent( QGraphicsSceneMouseEvent* pEvent )
{
    QGraphicsTextItem::mousePressEvent( pEvent );
    emit Clicked( );
}
/*---------------------------------------------------------------------------*/
GraphicsScene::GraphicsScene( QObject* pParent ):
    QGraphicsScene( pParent ), m_pCamera( Q_NULLPTR )
{
    ClickableTextItem* pTextItem = new ClickableTextItem( Q_NULLPTR );
    pTextItem->setPos( 10.0, 10.0 );
    pTextItem->setHtml( tr( "<font color=white>"
                            "Made By Jiangcaiyang<br>"
                            "Created in September<br>"
                            "Click for feedback."
                            "</font>" ) );
    connect( pTextItem, SIGNAL( Clicked( ) ),
             this, SLOT( Feedback( ) ) );
    addItem( pTextItem );

    ShaderOptionDialog* pDialog = new ShaderOptionDialog;
    connect( pDialog, SIGNAL( SwitchShader( const QString& ) ),
             this, SIGNAL( SwitchShader( const QString& ) ) );
    connect( pDialog, SIGNAL( SetLightPos( const QVector3D& ) ),
             this, SIGNAL( SetLightPos( const QVector3D& ) ) );

    QGraphicsProxyWidget* pProxy = addWidget( pDialog, Qt::Window | Qt::WindowTitleHint );
    pProxy->setPos( 100, 200 );
}
/*---------------------------------------------------------------------------*/
void GraphicsScene::SetCamera( Camera *pCamera )
{
    m_pCamera = pCamera;
}
/*---------------------------------------------------------------------------*/
void GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent* pEvent )
{
    QGraphicsScene::mousePressEvent( pEvent );
    if ( pEvent->isAccepted( ) ) return;
    m_LastPos = pEvent->scenePos( );
    switch ( pEvent->button( ) )
    {
    case Qt::LeftButton:
    case Qt::RightButton:

        break;
    case Qt::MiddleButton:
    default: break;
    }
    pEvent->accept( );
}
/*---------------------------------------------------------------------------*/
void GraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent* pEvent )
{
    QGraphicsScene::mouseMoveEvent( pEvent );
    if ( pEvent->isAccepted( ) ) return;
    QVector2D delta( pEvent->scenePos( ) - m_LastPos );
    if ( pEvent->buttons( ) & Qt::LeftButton ||
         pEvent->buttons( ) & Qt::RightButton )
    {
        m_pCamera->SetRotateH( -delta.y( ), true );
        m_pCamera->SetRotateY( delta.x( ), true );
    }
    else if ( pEvent->buttons( ) & Qt::MiddleButton )
    {
        m_pCamera->SetPos( QVector3D( -delta.x( ) / 5, delta.y( ) / 5, 0 ), true );
    }
    pEvent->accept( );
}
/*---------------------------------------------------------------------------*/
void GraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* pEvent )
{
    QGraphicsScene::mouseReleaseEvent( pEvent );
    if ( pEvent->isAccepted( ) ) return;
    QVector2D delta( pEvent->scenePos( ) - m_LastPos );
    switch ( pEvent->button( ) )
    {
    case Qt::LeftButton:
    case Qt::RightButton:
        m_pCamera->SetRotateH( -delta.y( ) );
        m_pCamera->SetRotateY( delta.x( ) );
        break;
    case Qt::MiddleButton:
        m_pCamera->SetPos( QVector3D( -delta.x( ) / 5, delta.y( ) / 5, 0 ) );
        break;
    default: break;
    }
    pEvent->accept( );
}
/*---------------------------------------------------------------------------*/
void GraphicsScene::wheelEvent( QGraphicsSceneWheelEvent* pEvent )
{
    QGraphicsScene::wheelEvent( pEvent );
    if ( pEvent->isAccepted( ) ) return;
    m_pCamera->ZoomIn( pEvent->delta( ) / 50 );
    pEvent->accept( );
}
/*---------------------------------------------------------------------------*/
void GraphicsScene::Feedback( void )
{
    FeedbackDialog feedback( tr( "Light for shader" ),
                             Q_NULLPTR );
    feedback.exec( );
}
