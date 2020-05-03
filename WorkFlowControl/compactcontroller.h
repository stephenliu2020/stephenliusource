#ifndef COMPACTCONTROLLER_H
#define COMPACTCONTROLLER_H

#include <QObject>
#include <QTimer>

class CompactController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nodeColor READ nodeColor WRITE setNodeColor NOTIFY nodeColorChanged)
public:
    enum {startNode=0, scatterNode, motorRunningNode, motorStopNode, armPickNode,powderDetectNode,
          compactDetectNode, calculateCompactPosNode, recPosIn3TimesNode, armPlaceNode, endNode };
    enum {rejectDeckNode=11, rejectEndNode};
    explicit CompactController(QObject *parent = nullptr);

    QString nodeColor() {return _nodeColor;}
    void setNodeColor(QString value) {     _nodeColor = value;
                                            emit nodeColorChanged();  }
    // Function to stop the process flow


    // Pauses the process flow
    void pause();

    // Resumes the process flow
    void resume();
    Q_INVOKABLE void start_work();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void powderConditionSwitch();
signals:
    void startSuccess();
    void waitCompactSuccess();
    void compactCheckingSuccess();
    void provideCompensationSuccess();
    void alarmSuccess();
    void manualTakeoutSuccess();
    void nodeColorChanged();
    void qmlSignal(QString);
private slots:
    void start();
    //void stop(QString);
    void scatter();
    void isMotorRunning();
    void feedingMotorStop();
    void armPick();
    void detectPowder();
    void rejectEnd();
    void detectCompact();
    void calculateCompactPosition();
    void recPosIn3Times();
    void armPlace();
    void end();
    void motorStart();
    void rejectDeck();
private:
    QTimer * cntTimer;
    QString _nodeColor;
    //QObject *object;
    bool componentCreated;
    bool bCompactDetectCondition;
};

#endif // COMPACTCONTROLLER_H
