// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#include <MyMainWindow.hpp>

#include <iostream>

#include <QApplication>
#include <QByteArray>
#include <QCommandLineParser>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

void Init() {
    Q_INIT_RESOURCE(resources);

    QCoreApplication::setApplicationName("CG Lab07 by Roman Khomenko (8O-308)");
    QCoreApplication::setOrganizationName("Khomenko and Co");
    QCoreApplication::setApplicationVersion("0.1.0");
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    Init();

    QCommandLineParser parser;
    parser.setApplicationDescription("CG Lab07 by Roman Khomenko (8O-308)");
    QCommandLineOption fileOption(QStringList() << "f"
                                                << "file",
                                  "Set config file.", "config");
    parser.addOption(fileOption);
    parser.addVersionOption();
    parser.addHelpOption();

    if (argc < 2) {
        std::cout << parser.helpText().toStdString();
        return 1;
    }

    parser.process(a);

    if (!parser.isSet(fileOption)) {
        std::cout << "Bad command line options. Use --help to details!"
                  << std::endl;
        return 1;
    }

    auto fileName = parser.value(fileOption);
    auto file = QFile(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Cannot open file \"" << fileName.toStdString() << "\"!"
                  << std::endl;
        return 1;
    }

    auto byteArray = file.readAll();
    file.close();
    auto jsonError = QJsonParseError();
    auto jsonDocument = QJsonDocument::fromJson(byteArray, &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        std::cout << "Cannot parse JSON file: "
                  << jsonError.errorString().toStdString() << std::endl;
        return 1;
    }

    auto obj = jsonDocument.object();
    auto points = obj.value("points").toArray();
    if (points.size() != 6) {
        std::cout << "Bad configuration file: you must set six points!"
                  << std::endl;
    }

    std::vector<Point> ps;
    for (auto&& point : points) {
        auto pointObj = point.toObject();
        auto p = Point(pointObj["x"].toDouble(), pointObj["y"].toDouble(),
                       pointObj["w"].toDouble());
        ps.push_back(p);
    }

    MyMainWindow w(ps);
    w.show();

    return a.exec();
}
