import QtQuick 1.1

Rectangle {

    id:blackboard
    radius:4

    Item {
        width: parent.width
        height: parent.height

        Image {
            id: background
            smooth:true
            anchors.fill: parent
            source: "../ui/images/chalkboard.png"
        }
    }

    Rectangle {
        id: categoryBar
        width: parent.width; height: parent.height/5
        opacity: .25
        color: "black"
        anchors.top: parent.top
    }

    Text{
        id:categoryName
        anchors{verticalCenter: categoryBar.verticalCenter;horizontalCenter: categoryBar.horizontalCenter }
        color:"white"
        text: "Category"
        font.pixelSize: parent.width/19.5
    }

    Text{
        id:anagram
        text:"manraga"
        anchors{verticalCenter: parent.verticalCenter;horizontalCenter: parent.horizontalCenter}
        color:"white"
        font.pixelSize: parent.width/12.5
    }

    Rectangle {
        id: optionsBar
        width: parent.width; height: parent.height/5
        opacity: .25
        color: "black"
        anchors.bottom: parent.bottom
    }

    Image{
        id:timerButton
        smooth:true
        height:optionsBar.height
        anchors{verticalCenter: optionsBar.verticalCenter;left:optionsBar.left}
        source: "../ui/icons/timer.png"
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:timerButton.state="onEntered"
            onExited:timerButton.state="onExited";
        }

        states: State {
               name: "onEntered"
               PropertyChanges {
                   target: timerText
                   opacity:1
               }
               PropertyChanges {
                   target: timerBar
                   opacity:0.25
               }
           }
        State{
            name:"onExited"
        }

        transitions: Transition {
                PropertyAnimation { properties: "x,y,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
            }
    }

    Rectangle {
        id: timerBar
        width: parent.width/6; height: parent.height/15
        opacity: 0
        radius: 4
        color: "black"
        anchors{bottom:optionsBar.top;horizontalCenter: timerButton.horizontalCenter}
    }

    Text{
        id:timerText
        anchors{verticalCenter: timerBar.verticalCenter;horizontalCenter:timerBar.horizontalCenter}
        color:"white"
        text: "Start Timer"
        opacity: 0
        font.pixelSize: parent.width/40
    }

    Image{
        id: hintButton
        smooth:true
        height:optionsBar.height
        anchors{verticalCenter: optionsBar.verticalCenter;horizontalCenter:optionsBar.horizontalCenter}
        source: "../ui/icons/hint.png"
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            hoverEnabled: true
            onClicked: console.log("Button clicked!")
            onEntered:hintButton.state="onEntered"
            onExited:hintButton.state="onExited"

        }

        states: State {
               name: "onEntered"
               PropertyChanges {
                   target: hintText
                   opacity:1
               }
               PropertyChanges {
                   target: hintBar
                   opacity:0.25
               }
           }

        State{
            name:"onExited"
        }

        State{
            name:"clicked"
            PropertyChanges {
                target: hintSection
                opacity:0.35
            }
        }

        transitions: Transition {
                PropertyAnimation { properties: "x,y,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
            }
    }

    Rectangle {
        id: hintSection
        width: parent.width/3; height: parent.height/3
        opacity:0
        radius: 4
        color: "black"
        anchors{verticalCenter: parent.verticalCenter;right: parent.left}
    }

    Rectangle {
        id: hintBar
        width: parent.width/6; height: parent.height/15
        opacity: 0
        radius: 4
        color: "black"
        anchors{bottom:optionsBar.top;horizontalCenter: hintButton.horizontalCenter}
    }

    Text{
        id:hintText
        anchors{verticalCenter: hintBar.verticalCenter;horizontalCenter:hintBar.horizontalCenter}
        color:"white"
        text: "Hint"
        opacity: 0
        font.pixelSize: parent.width/40
    }

    Image{
        id: revealButton
        smooth:true
        height:optionsBar.height
        anchors{verticalCenter: optionsBar.verticalCenter;right:optionsBar.right}
        source: "../ui/icons/reveal.png"
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:revealButton.state="onEntered"
            onExited:revealButton.state="onExited";
        }

        states: State {
               name: "onEntered"
               PropertyChanges {
                   target: revealText
                   opacity:1
               }
               PropertyChanges {
                   target: revealBar
                   opacity:0.25
               }
           }
        State{
            name:"onExited"
        }

        transitions: Transition {
                PropertyAnimation { properties: "x,y,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
            }
    }

    Rectangle {
        id: revealBar
        width: parent.width/6; height: parent.height/15
        opacity: 0
        radius: 4
        color: "black"
        anchors{bottom:optionsBar.top;horizontalCenter: revealButton.horizontalCenter}
    }

    Text{
        id:revealText
        anchors{verticalCenter: revealBar.verticalCenter;horizontalCenter:revealBar.horizontalCenter}
        color:"white"
        text: "Reveal Word"
        opacity: 0
        font.pixelSize: parent.width/40
    }
}
