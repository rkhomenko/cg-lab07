// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#ifndef CG_LAB_MYMAINWINDOW_HPP_
#define CG_LAB_MYMAINWINDOW_HPP_

#include <QMainWindow>

#include <array>

class MyOpenGLWidget;

class MyMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MyMainWindow(QWidget* parent = nullptr);
    ~MyMainWindow() = default;

    static constexpr auto VARIANT_DESCRIPTION =
        "Computer grapics lab 7\n"
        "Variant 20: NURBS n = 5, k = 4, uniform knot vector\n"
        "Made by Roman Khomenko (8O-308)";

private:
    QWidget* CreateCentralWidget();

    MyOpenGLWidget* OpenGLWidget;
};

#endif  // CG_LAB_MYMAINWINDOW_HPP_
