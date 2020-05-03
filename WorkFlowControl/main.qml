import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Styles 1.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Shapes 1.11
import an.qml.Controls 1.0
Window {
    property int flowPosition: 0
    visible: true
   // width: 640
    //height: 480
    //title: qsTr("Hello World")
    minimumWidth: 600;
    minimumHeight: 800;

    signal qmlSignal(string msg);
    ACompactControllerItem {
        id: controller;

    }

    Rectangle {
        id: workFlow;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;
        signal qmlSignal(string msg);
        Component{
            id: btnStyle;
            ButtonStyle {
                background: Rectangle {
                    implicitWidth: 70;
                    implicitHeight: 28;
                    border.width: control.hovered ? 2 : 1;
                    border.color: "#888";
                    radius: 4;
                    gradient: Gradient {
                        GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                        GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                    }
                }
/*
                label: Text {
                    text: control.text;
                    font.pointSize: 12;
                    color: "blue";
                    horizontalAlignment: Text.AlignHCenter;
                    verticalAlignment: Text.AlignVCenter;
                }*/

            }
        }

        Button {
            id: start;
            anchors.top: parent.top;
            anchors.topMargin: 4;
            anchors.horizontalCenter: parent.horizontalCenter;
            //anchors.verticalCenter: splitter2.verticalCenter;
            width: 70;
            height: 28;
            Text {
                text: "start";
                font.pointSize: 12;
                color: flowPosition==0? "red":"blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }

            style: btnStyle;

            onClicked: controller.start_work();
        }

        Rectangle {
            id: arrow1;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: start.bottom;
            width: 2;
            height: 20;
        }

        Button {
            id: scatter;
            anchors.top: arrow1.bottom;
            anchors.topMargin: 0;
            anchors.horizontalCenter: parent.horizontalCenter;
            width: 70;
            height: 28;
            //text: "scatter";
            Text {
                text: "scatter";
                font.pointSize: 12;
                color: flowPosition==1? "red":"blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
            //onClicked: painter.clear();
        }
        Rectangle {
            id: arrow2;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: scatter.bottom;
            width: 2;
            height: 20;
        }

        Shape {
            id: _shape
            width: 70
            height: 60
            //anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: arrow2.bottom;
            antialiasing: true
            containsMode: Shape.FillContains

            ShapePath {
                id: _shapePath
                strokeWidth: 2
                strokeColor: "gray"
                fillColor: "#ccc"

                strokeStyle: ShapePath.SolidLine
                //dashPattern: [ 1, 4 ]
                startX: 35; startY: 0
                //Text: "Running?"
                PathLine { x: -10; y: 30 }
                PathLine { x: 35; y: 60 }
                PathLine { x: 80; y: 30 }
                PathLine { x: 35; y: 0 }
            }

            Text {
                anchors.fill:parent
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.verticalCenter: parent.verticalCenter;
                anchors.leftMargin: 6;
                anchors.topMargin: 20;
                id: myText
                font.family: "Helvetica"
                font.pointSize: 10
                color: flowPosition==2? "red":"blue"
                text:  qsTr("running?")
            }

            MouseArea{
                anchors.fill: parent
                containmentMask: parent

                onPressed: {
                    _shapePath.fillColor = "#476ce3";
                }

                onReleased: {
                     _shapePath.fillColor = "#e3cb47";
                }

                onClicked: {
                    console.debug("clicking ...")
                }
            }
        }

        Rectangle {
            id: arrow0;
            border.width: 1;
            border.color: "gray";
            anchors.verticalCenter: _shape.verticalCenter;
            anchors.leftMargin: 10;
            anchors.left: _shape.right;
            Text {
                text: "n";
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            width: 40;
            height: 2;
        }

        Button {
            id: motorStart;
            anchors.leftMargin: 0;
            anchors.left: arrow0.right;
            anchors.verticalCenter: _shape.verticalCenter;
            width: 100;
            height: 28;
            //text: "motor start";
            Text {
                text: "motor start";
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
            //onClicked: painter.clear();
        }

        Rectangle {
            id: arrow3;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: _shape.bottom;
            Text {
                text: "y";
                anchors.left: arrow3.right;
                anchors.leftMargin: 2;
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            width: 2;
            height: 20;
        }

        Button {
            id: feedingMotorStop;
            anchors.top: arrow3.bottom;
            anchors.topMargin: 0;
            anchors.horizontalCenter: parent.horizontalCenter;
            width: 140;
            height: 28;
            Text {
                text: "Feeding Motor Stop";
                font.pointSize: 12;
                color: flowPosition==3? "red":"blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
            //onClicked: painter.clear();
        }

        Rectangle {
            id: arrow4;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: feedingMotorStop.bottom;
            width: 2;
            height: 20;
        }

        Button {
            id: armPickup;
            anchors.top: arrow4.bottom;
            anchors.topMargin: 0;
            anchors.horizontalCenter: parent.horizontalCenter;
            width: 100;
            height: 28;
            Text {
                text: "Arm pickup";
                font.pointSize: 12;
                color: flowPosition==4? "red":"blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
            //onClicked: painter.clear();
        }

        Rectangle {
            id: arrow5;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: armPickup.bottom;
            width: 2;
            height: 20;
        }

        Shape {
            id: detectPowder
            width: 70
            height: 60
            //anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: arrow5.bottom;
            antialiasing: true
            containsMode: Shape.FillContains

            ShapePath {
                id: detectPowderPath
                strokeWidth: 2
                strokeColor: "gray"
                fillColor: "#ccc"

                strokeStyle: ShapePath.SolidLine
                //dashPattern: [ 1, 4 ]
                startX: 35; startY: 0
                //Text: "Running?"
                PathLine { x: -30; y: 30 }
                PathLine { x: 35; y: 60 }
                PathLine { x: 100; y: 30 }
                PathLine { x: 35; y: 0 }
            }

            Text {
                anchors.fill:parent
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.verticalCenter: parent.verticalCenter;
                anchors.leftMargin: 0;
                anchors.topMargin: 20;
                id: detectPowderText
                font.family: "Helvetica"
                font.pointSize: 10
                color: flowPosition==5? "red":"blue"
                text:  qsTr("detect Powder?")
            }

            MouseArea{
                anchors.fill: parent
                containmentMask: parent

                onPressed: {
                    detectPowder.fillColor = "#476ce3";
                }

                onReleased: {
                     detectPowder.fillColor = "#e3cb47";
                }

                onClicked: {
                    console.debug("clicking detectPowder...");
                    controller.powderConditionSwitch();
                }
            }
        }

        Rectangle {
            id: arrow001;
            border.width: 1;
            border.color: "gray";
            anchors.verticalCenter: detectPowder.verticalCenter;
            anchors.rightMargin: 30;
            anchors.right: detectPowder.left;
            Text {
                text: "        n";
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignRight;
                verticalAlignment: Text.AlignVCenter;
            }
            width: 50;
            height: 2;
        }

        Button {
            id: rejectDeck;
            anchors.rightMargin: 0;
            anchors.right: arrow001.left;
            anchors.verticalCenter: detectPowder.verticalCenter;
            width: 60;
            height: 28;
            Text {
                text: "Reject";
                font.pointSize: 12;
                color: flowPosition==11? "red":"blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
        }


        Rectangle {
            id: arrow003;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: rejectDeck.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: rejectDeck.bottom;
            width: 2;
            height: 12;
        }


        Button {
            id: rejectEnd;
            //anchors.rightMargin: 0;
            anchors.top: arrow003.bottom;
            anchors.horizontalCenter: rejectDeck.horizontalCenter;
            width: 60;
            height: 28;
            Text {
                text: "end";
                font.pointSize: 12;
                color: flowPosition==12? "red":"blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
        }
        Rectangle {
            id: arrow6;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: detectPowder.bottom;
            Text {
                text: " y";
                anchors.left: arrow3.right;
                anchors.leftMargin: 2;
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            width: 2;
            height: 20;
        }

        Shape {
            id: detectCompact
            width: 70
            height: 60
            //anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: arrow6.bottom;
            antialiasing: true
            containsMode: Shape.FillContains

            ShapePath {
                id: detectCompactPath
                strokeWidth: 2
                strokeColor: "gray"
                fillColor: "#ccc"

                strokeStyle: ShapePath.SolidLine
                //dashPattern: [ 1, 4 ]
                startX: 35; startY: 0
                //Text: "Running?"
                PathLine { x: -30; y: 30 }
                PathLine { x: 35; y: 60 }
                PathLine { x: 100; y: 30 }
                PathLine { x: 35; y: 0 }
            }

            Text {
                anchors.fill:parent
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.verticalCenter: parent.verticalCenter;
                anchors.leftMargin: 0;
                anchors.topMargin: 20;
                id: detectCompactText
                font.family: "Helvetica"
                font.pointSize: 10
                color: flowPosition==6? "red":"blue"
                text:  qsTr("detect Compact?")
            }

            MouseArea{
                anchors.fill: parent
                containmentMask: parent

                onPressed: {
                    detectCompact.fillColor = "#476ce3";
                }

                onReleased: {
                     detectCompact.fillColor = "#e3cb47";
                }

                onClicked: {
                    console.debug("clicking detectCompact...")
                }
            }
        }

        Rectangle {
            id: arrow002;
            border.width: 1;
            border.color: "gray";
            anchors.verticalCenter: detectCompact.verticalCenter;
            anchors.rightMargin: 30;
            anchors.right: detectCompact.left;
            Text {
                text: "        n";
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignRight;
                verticalAlignment: Text.AlignVCenter;
            }
            width: 50;
            height: 2;
        }

        Button {
            id: rejectCompact;
            anchors.rightMargin: 0;
            anchors.right: arrow002.left;
            anchors.verticalCenter: detectCompact.verticalCenter;
            width: 60;
            height: 28;
            Text {
                text: "Reject";
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
        }

        Rectangle {
            id: arrow004;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: rejectCompact.horizontalCenter;
            anchors.topMargin: 0;
            anchors.bottom: rejectCompact.top;
            width: 2;
            height: 12;
        }
        Rectangle {
            id: arrow7;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: detectCompact.bottom;
            Text {
                text: " y";
                anchors.left: arrow3.right;
                anchors.leftMargin: 2;
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            width: 2;
            height: 20;
        }

        Button {
            id: calculateCompactPosition;
            anchors.top: arrow7.bottom;
            anchors.topMargin: 0;
            anchors.horizontalCenter: parent.horizontalCenter;
            width: 210;
            height: 28;
            Text {
                text: "Calculate Compact Position";
                font.pointSize: 12;
                color: flowPosition==7? "red":"blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
            onClicked: controller.stop();//workFlow.qmlSignal("stop from QML");
        }

        Rectangle {
            id: arrow70;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: calculateCompactPosition.bottom;
            width: 2;
            height: 20;
        }
        Shape {
            id: receivePosIn3Times
            width: 70
            height: 60
            //anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: arrow70.bottom;
            antialiasing: true
            containsMode: Shape.FillContains

            ShapePath {
                id: receivePosIn3TimesPath
                strokeWidth: 2
                strokeColor: "gray"
                fillColor: "#ccc"

                strokeStyle: ShapePath.SolidLine
                //dashPattern: [ 1, 4 ]
                startX: 35; startY: 0
                //Text: "Running?"
                PathLine { x: -50; y: 30 }
                PathLine { x: 35; y: 60 }
                PathLine { x: 120; y: 30 }
                PathLine { x: 35; y: 0 }
            }

            Text {
                anchors.fill:parent
                //anchors.horizontalCenter: parent.horizontalCenter;
                //anchors.verticalCenter: parent.verticalCenter;
                horizontalAlignment: Text.AlignLeft;
                anchors.leftMargin: 0;
                anchors.topMargin: 20;
                id: receivePosIn3TimesText
                font.family: "Helvetica"
                font.pointSize: 10
                color: flowPosition==8? "red":"blue"
                text:  qsTr("Rec pos in 3 times?")
            }

            MouseArea{
                anchors.fill: parent
                containmentMask: parent

                onPressed: {
                    receivePosIn3Times.fillColor = "#476ce3";
                }

                onReleased: {
                     receivePosIn3Times.fillColor = "#e3cb47";
                }

                onClicked: {
                    console.debug("clicking ...")
                }
            }
        }

        Rectangle {
            id: arrow8;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: receivePosIn3Times.bottom;
            Text {
                text: " y";
                anchors.left: arrow3.right;
                anchors.leftMargin: 2;
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            width: 2;
            height: 20;
        }

        Button {
            id: armPlace;
            anchors.top: arrow8.bottom;
            anchors.topMargin: 0;
            anchors.horizontalCenter: parent.horizontalCenter;
            width: 100;
            height: 28;
            Text {
                text: "Arm Place";
                font.pointSize: 12;
                color: flowPosition==9? "red":"blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
            onClicked: controller.stop();//workFlow.qmlSignal("stop from QML");
        }
        Rectangle {
            id: arrow15;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: armPlace.bottom;
            width: 2;
            height: 20;
        }

        Button {
            id: end;
            anchors.top: arrow15.bottom;
            anchors.topMargin: 0;
            anchors.horizontalCenter: parent.horizontalCenter;
            width: 70;
            height: 28;
            Text {
                text: "end";
                font.pointSize: 12;
                color: flowPosition==10? "red":"blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
            onClicked: controller.stop();//workFlow.qmlSignal("stop from QML");
        }

        Rectangle {
            id: arrow01;
            border.width: 1;
            border.color: "gray";
            anchors.verticalCenter: receivePosIn3Times.verticalCenter;
            anchors.leftMargin: 50;
            anchors.left: receivePosIn3Times.right;
            Text {
                text: "n";
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            width: 60;
            height: 2;
        }

        Button {
            id: alarm;
            anchors.leftMargin: 0;
            anchors.left: arrow01.right;
            anchors.verticalCenter: receivePosIn3Times.verticalCenter;
            width: 100;
            height: 28;
            Text {
                text: "Alarm";
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
        }
        Rectangle {
            id: arrow02;
            border.width: 1;
            border.color: "gray";
            anchors.horizontalCenter: alarm.horizontalCenter;
            anchors.topMargin: 0;
            anchors.top: alarm.bottom;
            width: 2;
            height: 20;
        }

        Button {
            id: emergencyStop;
            anchors.top: arrow02.bottom;
            anchors.topMargin: 0;
            anchors.horizontalCenter: alarm.horizontalCenter;
            width: 120;
            height: 28;
            Text {
                text: "Emergency Stop";
                font.pointSize: 12;
                color: "blue";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            style: btnStyle;
            onClicked: controller.stop();//workFlow.qmlSignal("stop from QML");
        }

    }

    function myQmlFunction(msg) {
        console.log("Got message:", msg)
        if (flowPosition===0)
        {
            //highligh position start

        }

        return "some return value"
    }
}
