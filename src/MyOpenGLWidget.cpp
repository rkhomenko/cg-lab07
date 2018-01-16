// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#include <MyMainWindow.hpp>
#include <MyOpenGLWidget.hpp>

#include <cmath>

#include <QApplication>
#include <QDebug>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QResizeEvent>

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setMinimumSize(WIDGET_DEFAULT_SIZE);
}

MyOpenGLWidget::~MyOpenGLWidget() {}

void MyOpenGLWidget::ScaleUpSlot() {
    ScaleFactor *= SCALE_FACTOR_PER_ONCE;
    UpdateOnChange(width(), height());
    OnWidgetUpdate();
}

void MyOpenGLWidget::ScaleDownSlot() {
    ScaleFactor /= SCALE_FACTOR_PER_ONCE;
    UpdateOnChange(width(), height());
    OnWidgetUpdate();
}

void MyOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this,
            &MyOpenGLWidget::CleanUp);

    ShaderProgram = new QOpenGLShaderProgram(this);
    ShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           VERTEX_SHADER);
    ShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           FRAGMENT_SHADER);

    if (!ShaderProgram->link()) {
        qDebug() << ShaderProgram->log();
        QApplication::quit();
    }

    Buffer = new QOpenGLBuffer;
    Buffer->create();
    Buffer->bind();
    Buffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    UpdateOnChange(width(), height());

    VertexArray = new QOpenGLVertexArrayObject;
    VertexArray->create();
    VertexArray->bind();

    VertexArray->release();
    Buffer->release();
}

void MyOpenGLWidget::resizeGL(int width, int height) {
    UpdateOnChange(width, height);
}

void MyOpenGLWidget::paintGL() {
    if (!ShaderProgram->bind()) {
        qDebug() << "Cannot bind program";
        QApplication::quit();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    ShaderProgram->release();
}

void MyOpenGLWidget::CleanUp() {
    VertexArray->destroy();
    Buffer->destroy();
    delete VertexArray;
    delete Buffer;
    delete ShaderProgram;
}

void MyOpenGLWidget::UpdateOnChange(int width, int height) {
    const Mat4x4 scaleMatrix = GenerateScaleMatrix(width, height);
}

void MyOpenGLWidget::OnWidgetUpdate() {
    auto event = new QResizeEvent(size(), size());
    QOpenGLWidget::event(event);
}

Mat4x4 MyOpenGLWidget::GenerateScaleMatrix(int width, int height) const {
    const auto DEFAULT_WIDTH = IMAGE_DEFAULT_SIZE.width();
    const auto DEFAULT_HEIGHT = IMAGE_DEFAULT_SIZE.height();

    auto xScaleFactor = 1.0f * DEFAULT_WIDTH / width;
    auto yScaleFactor = 1.0f * DEFAULT_HEIGHT / height;

    GLfloat matrixData[] = {
        xScaleFactor * ScaleFactor,
        0.0f,
        0.0f,
        0.0f,  // first line
        0.0f,
        yScaleFactor * ScaleFactor,
        0.0f,
        0.0f,  // second line
        0.0f,
        0.0f,
        1.0f,
        0.0f,  // third line
        0.0f,
        0.0f,
        0.0f,
        1.0f  // fourth line
    };

    return Map4x4(matrixData);
}
