 # ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
 # ┃     ┏━┣━┣┓　　┏┓┏┓┳┓┏━━┓┣┣━┓　┓　┓┣┳━┓       ┃
 # ┃     ┏┏┏╯━┓┳┳━┛┏╯┃┃┃　　┃┣┣━┓┃┃　┃┃┃　　       ┃
 # ┃     ┃┃┏━╮┃┗┗┏╯┗┃┃╯┃　　┃┏┣━┓┃┃　┃╯┣━┓       ┃
 # ┃     ╰┫┏━┻╯┗┳┣┛┏┛┃┃┣━━┫┃┃　┃┃┃┗╯　┃　　       ┃
 # ┃     ┏┫━┳━┫┏┃┣┓┗┃┃╯┃　　┃┃┃　┃　┃　┃　┣━┓       ┃
 # ┃     ┗┗┗━━╯┗┛┛╯┗╯╰　┗━━╯　┛　┛┗╯　╰┛┗　　       ┃
 # ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 # ┃                     Copyright (c) 2013 jiangcaiyang                    ┃
 # ┃ This software is provided 'as-is', without any express or implied      ┃
 # ┃ warranty. In no event will the authors be held liable for any damages  ┃
 # ┃ arising from the use of this software.                                 ┃
 # ┃                                                                        ┃
 # ┃ Permission is granted to anyone to use this software for any purpose,  ┃
 # ┃ including commercial applications, and to alter it and redistribute it ┃
 # ┃ freely, subject to the following restrictions:                         ┃
 # ┃                                                                        ┃
 # ┃ 1. The origin of this software must not be misrepresented; you must    ┃
 # ┃    not claim that you wrote the original software. If you use this     ┃
 # ┃    software in a product, an acknowledgment in the product             ┃
 # ┃    documentation would be appreciated but is not required.             ┃
 # ┃ 2. Altered source versions must be plainly marked as such, and must    ┃
 # ┃    not be misrepresented as being the original software.               ┃
 # ┃ 3. This notice may not be removed or altered from any source           ┃
 # ┃    distribution.                                                       ┃
 # ┃                                                                        ┃
 # ┃ jiangcaiyang jiangcaiyang123@163.com                                   ┃
 # ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 # ┃ file name: QtGLSL_LightShader_5.pro                                    ┃
 # ┃ create date: 2013年9月24日星期二 23时45分21秒                          ┃
 # ┃ last modified date: 2013年9月24日星期二 23时45分21秒                   ┃
 # ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 # 
TARGET = ShaderForLight
RC_ICONS += appIcon.ico

DEFINES += _MODEL_OPENGL_
QT += core gui widgets opengl

# 由于作者jiangcaiyang最近比较忙，没有尝试在Linux上静态编译Feedback模块，
# 所以如果想在Linux上构建该例子，注释掉下面两句。
include ( Feedback/Feedback.pri )
CONFIG += feedback

HEADERS += \
    ShaderOptionDialog.h \
    ShaderKeywordHighlight.h \
    OpenGLView.h \
    MainWindow.h \
    GraphicsScene.h \
    Format3DS.h \
    Camera.h

SOURCES += \
    ShaderOptionDialog.cpp \
    ShaderKeywordHighlight.cpp \
    OpenGLView.cpp \
    MainWindow.cpp \
    main.cpp \
    GraphicsScene.cpp \
    Camera.cpp

FORMS += \
    ShaderOptionDialog.ui

RESOURCES += \
    Translation.qrc \
    Shader.qrc \
    Model.qrc

TRANSLATIONS += zh_CN.ts

LIBS += $${PWD}/lib/lib3ds.a -lopengl32
