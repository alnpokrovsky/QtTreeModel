#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "src/treemodel/treemodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    TreeModel treeModel;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("treeModel", &treeModel);
    //qmlRegisterType<CustomType>("CustomType", 1, 0, "CustomType");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
