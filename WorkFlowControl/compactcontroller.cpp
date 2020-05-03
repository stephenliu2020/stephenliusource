#include "compactcontroller.h"
#include <QDebug>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickView>
extern  QObject *object;
CompactController::CompactController(QObject *parent) : QObject(parent),componentCreated(false)
  ,bCompactDetectCondition(true)
{
    cntTimer = new QTimer();


}

void CompactController::start_work()
{
     qDebug() << "CompactController::start work";
     /*if (!componentCreated)
     {
         QQuickView view;
         view.setSource(QUrl::fromLocalFile("C:/Users/Crystalclaire.HP-LAPTOP-04302/Documents/WorkFlowControl/main.qml"));
         view.show();
         object = reinterpret_cast<QObject*>(view.rootObject());
         //object = component.create();
         componentCreated = true;
     }*/
    // QObject::connect(object, SIGNAL(qmlSignal(QString)),  this, SLOT(stop(QString)));
     qDebug() << "Property value:" << QQmlProperty::read(object, "flowPosition").toInt();
     QQmlProperty::write(object, "flowPosition", 1);
     start();
}

void CompactController::stop(){
    qDebug() << "Stop triggered" ;
    cntTimer->stop();
    disconnect(cntTimer, nullptr, nullptr, nullptr);
}

void CompactController::powderConditionSwitch()
{
    bCompactDetectCondition=!bCompactDetectCondition;
}
void CompactController::pause(){
    cntTimer->stop();
}

void CompactController::resume(){
    cntTimer->start(500);
}

void CompactController::start(){
    qDebug() << "CompactController::start";
    //emit startSuccess();
    QQmlProperty::write(object, "flowPosition", startNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(scatter()));
    cntTimer->start(500); // Put This in a better function
}


void CompactController::scatter(){
    qDebug() << "CompactController::scatter";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", scatterNode);
    setNodeColor("#f00");
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(isMotorRunning()));
}

void CompactController::isMotorRunning()
{
    qDebug() << "CompactController::isMotorRunning";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", motorRunningNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(feedingMotorStop()));
}

void CompactController::feedingMotorStop()
{
    qDebug() << "CompactController::feedingMotorStop";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", motorStopNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(armPick()));
}
void CompactController::armPick()
{
    qDebug() << "CompactController::armPick";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", armPickNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(detectPowder()));
}

void CompactController::motorStart()
{

}

void CompactController::detectPowder()
{
    qDebug() << "CompactController::detectPowder";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", powderDetectNode);
    if (bCompactDetectCondition)
       QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(detectCompact()));
    else
        QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(rejectDeck()));
}

void CompactController::rejectDeck()
{
    qDebug() << "CompactController::rejectDeck";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", rejectDeckNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(rejectEnd()));
}

void CompactController::rejectEnd()
{
    qDebug() << "CompactController::rejectEnd";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", rejectEndNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(start()));
}
void CompactController::detectCompact()
{
    qDebug() << "CompactController::detectCompact";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", compactDetectNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(calculateCompactPosition()));
}
void CompactController::calculateCompactPosition()
{
    qDebug() << "CompactController::calculateCompactPosition";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", calculateCompactPosNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(recPosIn3Times()));
}
void CompactController::recPosIn3Times()
{
    qDebug() << "CompactController::recPosIn3Times";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", recPosIn3TimesNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(armPlace()));
}
void CompactController::armPlace()
{
    qDebug() << "CompactController::armPlace";
    disconnect(cntTimer, nullptr, nullptr, nullptr);
    QQmlProperty::write(object, "flowPosition", armPlaceNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(end()));
}

void CompactController::end()
{
    qDebug() << "CompactController::end";
    QQmlProperty::write(object, "flowPosition", endNode);
    QObject::connect(cntTimer, SIGNAL(timeout()), this, SLOT(start()));
}
