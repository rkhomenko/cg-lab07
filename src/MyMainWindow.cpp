// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#include <MyMainWindow.hpp>
#include <MyOpenGLWidget.hpp>

#include <QHBoxLayout>
#include <QLabel>
#include <QSurfaceFormat>
#include <QTabWidget>
#include <QVBoxLayout>

MyMainWindow::MyMainWindow(QWidget* parent) : QMainWindow(parent) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    OpenGLWidget = new MyOpenGLWidget;
    OpenGLWidget->setFormat(format);

    setCentralWidget(CreateCentralWidget());
}

QWidget* MyMainWindow::CreateCentralWidget() {
    const auto fixedSizePolicy =
        QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto widget = new QWidget;
    auto mainLayout = new QVBoxLayout;
    auto toolLayout = new QHBoxLayout;
    auto label = new QLabel(VARIANT_DESCRIPTION);

    label->setSizePolicy(fixedSizePolicy);
    toolLayout->addWidget(label);


    mainLayout->addLayout(toolLayout);
    mainLayout->addWidget(OpenGLWidget);
    widget->setLayout(mainLayout);

    return widget;
}
