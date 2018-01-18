// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#include <MyMainWindow.hpp>
#include <MyOpenGLWidget.hpp>
#include <NURBS.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>

#include <QApplication>
#include <QDebug>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QResizeEvent>

MyOpenGLWidget::MyOpenGLWidget(std::vector<Point>& p, QWidget* parent)
    : QOpenGLWidget(parent),
      ScaleFactor{1.0f},
      P{p},
      Step{100},
      NeedRegeneratePoints{true},
      CurrentScale{0.0f} {
    setMinimumSize(WIDGET_DEFAULT_SIZE);
}

MyOpenGLWidget::~MyOpenGLWidget() {}

void MyOpenGLWidget::ScaleUpSlot() {
    ScaleFactor *= SCALE_FACTOR_PER_ONCE;
    Step = static_cast<int>(Step * 1.5f);
    NeedRegeneratePoints = true;
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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this,
            &MyOpenGLWidget::CleanUp);

    if (!context()->hasExtension("GL_ARB_arrays_of_arrays")) {
        qDebug() << "GL_ARB_arrays_of_arrays is not supported!";
        QApplication::quit();
    }

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

    Buffer->allocate(Points.data(), sizeof(Point) * Points.size());

    VertexArray = new QOpenGLVertexArrayObject;
    VertexArray->create();
    VertexArray->bind();

    int posAttr = ShaderProgram->attributeLocation(POSITION);
    ShaderProgram->enableAttributeArray(posAttr);
    ShaderProgram->setAttributeBuffer(posAttr, GL_FLOAT, Point::GetOffset(),
                                      Point::GetTupleSize(),
                                      Point::GetStride());

    ShaderProgram->disableAttributeArray(posAttr);

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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Buffer->destroy();
    if (!Buffer->create()) {
        qDebug() << "Cannot create buffer";
    }

    if (!Buffer->bind()) {
        qDebug() << "Cannot bind buffer";
    }

    Buffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    Buffer->allocate(Points.data(), sizeof(Point) * Points.size());

    VertexArray->destroy();
    VertexArray->create();
    VertexArray->bind();

    int posAttr = ShaderProgram->attributeLocation(POSITION);
    ShaderProgram->enableAttributeArray(posAttr);
    ShaderProgram->setAttributeBuffer(posAttr, GL_FLOAT, Point::GetOffset(),
                                      Point::GetTupleSize(),
                                      Point::GetStride());

    glEnable(GL_POINT_SMOOTH);
    glDrawArrays(GL_LINE_STRIP, 0, Points.size());

    ShaderProgram->disableAttributeArray(posAttr);

    Buffer->release();
    VertexArray->release();
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
    Mat4x4 scaleMatrix = GenerateScaleMatrix(width, height);
    ShaderProgram->bind();
    ShaderProgram->setUniformValue(TRANSFORM_MATRIX,
                                   QMatrix4x4(scaleMatrix.data()));
    ShaderProgram->release();

    if (NeedRegeneratePoints && CurrentScale < ScaleFactor) {
        CurrentScale = ScaleFactor;
        std::vector<float> t(Step + 1);
        for (auto i = 0; i <= Step; i++) {
            t[i] = i * 1.0f / Step;
        }

        Points.clear();
        NURBS nurbs(5, 4, P);
        for (auto t_i : t) {
            auto r = nurbs.R(t_i);
            Points.push_back(r);
        }

        Points.erase(std::unique(std::begin(Points), std::end(Points)),
                     std::end(Points));
        Points.erase(
            std::remove(std::begin(Points), std::end(Points), Point(0, 0, 0)),
            std::end(Points));

        NeedRegeneratePoints = false;
    }
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
