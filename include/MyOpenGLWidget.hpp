// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#ifndef CG_LAB_MYOPENGLWIDGET_HPP_
#define CG_LAB_MYOPENGLWIDGET_HPP_

#include <Point.hpp>
#include <vector>

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#ifdef EIGEN3_INCLUDE_DIR
#include <Eigen/Dense>
#else
#include <eigen3/Eigen/Dense>
#endif

using Vec3 = Eigen::Matrix<float, 1, 3>;
using Vec4 = Eigen::Matrix<float, 1, 4>;
using Mat4x4 = Eigen::Matrix<float, 4, 4>;
using Map4x4 = Eigen::Map<Eigen::Matrix<float, 4, 4, Eigen::RowMajor>>;

using SizeType = std::size_t;
using LenghtType = float;

class QOpenGLShaderProgram;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    using FloatType = float;

    explicit MyOpenGLWidget(std::vector<Point>& p, QWidget* parent = nullptr);
    ~MyOpenGLWidget();

public slots:
    void ScaleUpSlot();
    void ScaleDownSlot();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private slots:
    void CleanUp();

private:
    static constexpr auto WIDGET_DEFAULT_SIZE = QSize(350, 350);
    static constexpr auto IMAGE_DEFAULT_SIZE = QSize(300, 300);

    static constexpr auto VERTEX_SHADER = ":/shaders/vertexShader.glsl";
    static constexpr auto FRAGMENT_SHADER = ":/shaders/fragmentShader.glsl";
    static constexpr auto POSITION = "position";
    static constexpr auto COLOR = "color";
    static constexpr auto TRANSFORM_MATRIX = "transformMatrix";

    static constexpr auto SCALE_FACTOR_PER_ONCE = 1.15f;

    void UpdateOnChange(int width, int height);
    void OnWidgetUpdate();

    Mat4x4 GenerateScaleMatrix(int width, int height) const;

    QOpenGLShaderProgram* ShaderProgram;
    QOpenGLBuffer* Buffer;
    QOpenGLVertexArrayObject* VertexArray;
    FloatType ScaleFactor;
    std::vector<Point> P;
    std::vector<Point> Points;
    int Step;
    bool NeedRegeneratePoints;
    float CurrentScale;
};

#endif  // CG_LAB_MYOPENGLWIDGET_HPP_
