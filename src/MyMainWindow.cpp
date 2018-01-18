// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#include <MyMainWindow.hpp>
#include <MyOpenGLWidget.hpp>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
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

    std::vector<Point> points = {
        Point(-0.5, 0, 0.25), Point(-0.5, 0.5, 0.3),
        Point(0, 0, 0.8), Point(0, 0.5, 0.1),
        Point(0.5, 0, 0.5), Point(0.5, 0.5, 0.2)
    };

    OpenGLWidget = new MyOpenGLWidget(points);
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

    auto scaleUpButton = new QPushButton;
    scaleUpButton->setSizePolicy(fixedSizePolicy);
    scaleUpButton->setIcon(QIcon(":/icons/scaleUpIcon"));
    toolLayout->addWidget(scaleUpButton);

    auto scaleDownButton = new QPushButton;
    scaleDownButton->setSizePolicy(fixedSizePolicy);
    scaleDownButton->setIcon(QIcon(":/icons/scaleDownIcon"));
    toolLayout->addWidget(scaleDownButton);

    connect(scaleUpButton, &QPushButton::pressed, OpenGLWidget,
            &MyOpenGLWidget::ScaleUpSlot);
    connect(scaleDownButton, &QPushButton::pressed, OpenGLWidget,
            &MyOpenGLWidget::ScaleDownSlot);

    label->setSizePolicy(fixedSizePolicy);
    toolLayout->addWidget(label);

    toolLayout->addStretch();

    mainLayout->addLayout(toolLayout);
    mainLayout->addWidget(OpenGLWidget);
    widget->setLayout(mainLayout);

    return widget;
}
