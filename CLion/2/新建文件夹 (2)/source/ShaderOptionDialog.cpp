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
 * ┃ file name: ShaderOptionDialog.cpp                                      ┃
 * ┃ create date: 2013年9月24日星期二 19时34分31秒                          ┃
 * ┃ last modified date: 2013年9月24日星期二 22时12分23秒                   ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#include <QVector3D>
#include <QFile>
#include <QGraphicsSceneMouseEvent>
#include "ShaderOptionDialog.h"
#include "ShaderKeywordHighlight.h"
#include "ui_ShaderOptionDialog.h"
/*---------------------------------------------------------------------------*/
extern bool g_ShowAxis;
/*---------------------------------------------------------------------------*/
ShaderOptionDialog::ShaderOptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShaderOptionDialog)
{
    ui->setupUi(this);
    ToggleShaderText( false );
    new ShaderKeywordHighlight( ui->pShaderTextEdit->document( ) );

    InitShaderList( );
    InitEdit( ui->pEditX );
    InitEdit( ui->pEditY );
    InitEdit( ui->pEditZ );

    // 设定初始值
    setWindowOpacity( 0.9 );

    // 设定连接
    connect( ui->pOpacitySlider, SIGNAL( valueChanged( int ) ),
             this, SLOT( SetSpinValue( int ) ) );
    connect( ui->pOpacitySpin, SIGNAL( valueChanged( double ) ),
             this, SLOT( SetSliderValue( double ) ) );
    connect( ui->pShowAxisCheckBox, SIGNAL( toggled( bool ) ),
             this, SLOT( ShowAxis( bool ) ) );
    connect( ui->pShowShaderCheckBox, SIGNAL( toggled( bool ) ),
             this, SLOT( ToggleShaderText( bool ) ) );
}
/*---------------------------------------------------------------------------*/
ShaderOptionDialog::~ShaderOptionDialog()
{
    delete ui;
}
/*---------------------------------------------------------------------------*/
void ShaderOptionDialog::SetSpinValue( int value )
{
    ui->pOpacitySpin->setValue( double( value ) / 100.0 );
    setWindowOpacity( ui->pOpacitySpin->value( ) );
}
/*---------------------------------------------------------------------------*/
void ShaderOptionDialog::SetSliderValue( double value )
{
    ui->pOpacitySlider->setValue( int( value * 100 ) );
    setWindowOpacity( ui->pOpacitySpin->value( ) );
}
/*---------------------------------------------------------------------------*/
void ShaderOptionDialog::ShowAxis( bool show )
{
    g_ShowAxis = show;
}
/*---------------------------------------------------------------------------*/
void ShaderOptionDialog::SwitchSelectedShaderText( int row )
{
    QString key = ui->pListWidget->item( row )->text( );

    // 打开文件，放入着色器编辑器中
    QFile file( m_ShaderTextMap[key] );
    if ( file.open( QIODevice::ReadOnly ) )
    {
        ui->pShaderTextEdit->setPlainText( file.readAll( ) );
        file.close( );
    }
    emit SwitchShader( m_ShaderTextMap[key] );
}
/*---------------------------------------------------------------------------*/
void ShaderOptionDialog::MakeLightPos( void )
{
    QVector3D pos( ui->pEditX->text( ).toDouble( ),
                   ui->pEditY->text( ).toDouble( ),
                   ui->pEditZ->text( ).toDouble( ) );
    emit SetLightPos( pos );
}
/*---------------------------------------------------------------------------*/
void ShaderOptionDialog::ToggleShaderText( bool show )
{
    static int s_minWidth = ui->pListWidget->minimumWidth( );
    ui->pShaderTextEdit->setVisible( show );
    if ( !show )// 在不勾选显示代码的时候，界面自动收缩
    {
        adjustSize( );
        resize( s_minWidth, height( ) );
    }
}
/*---------------------------------------------------------------------------*/
void ShaderOptionDialog::InitShaderList( void )
{
    // 初始化map
    m_ShaderTextMap.clear( );
    m_ShaderTextMap[tr( "diffuse" )] = ":/Shader/Diffuse.vert";
    m_ShaderTextMap[tr( "specular" )] = ":/Shader/Specular.vert";
    m_ShaderTextMap[tr( "specular Opt" )] = ":/Shader/Specular_Opt.vert";

    // 将可用的着色器列在列表上
    foreach ( const QString key, m_ShaderTextMap.keys( ) )
    {
        ui->pListWidget->addItem( key );
    }
    ui->pListWidget->setSelectionMode( QAbstractItemView::SingleSelection );
    ui->pListWidget->setEditTriggers( QAbstractItemView::NoEditTriggers );

    connect( ui->pListWidget, SIGNAL( currentRowChanged( int ) ),
             this, SLOT( SwitchSelectedShaderText( int ) ) );
}
/*---------------------------------------------------------------------------*/
void ShaderOptionDialog::InitEdit( QLineEdit* pEdit )
{
    pEdit->setValidator( new QDoubleValidator( this ) );// 仅允许double数字
    connect( pEdit, SIGNAL( textChanged( QString ) ),
             this, SLOT( MakeLightPos( ) ) );
}
