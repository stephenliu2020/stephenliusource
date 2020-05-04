/*******************************************************************/
/**************         main.cpp         ***************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickView>
#include <QtWidgets/QApplication>
#include "compactcontroller.h"
QObject *object;
int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    qmlRegisterType<CompactController>("an.qml.Controls", 1, 0, "ACompactControllerItem");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    if (engine.rootObjects().isEmpty())
        return -1;
    object=engine.rootObjects()[0];
   // QQmlComponent component(qobject_cast<QObject*>(&engine));
   // object = component.create();


    return app.exec();
}
