import QtQuick 1.1

Rectangle {

    id:blackboard
    radius:4
    property alias anagramText:anagram.text;
    property alias hint:anagramHint.text;
    property alias showHintTimeInterval:hintButton.countDownTimerValue;

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
        text: kanagramEngineHelper.categoryName();
        font.pixelSize: parent.width/19.5
    }

    Text{
        id:anagram
        text:kanagramEngineHelper.createNextAnagram();
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
        property int countDownTimerValue:0;

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:timerButton.state="onEntered"
            onExited:timerButton.state="onExited"
            onClicked:{
                timerButton.countDownTimerValue=15;
                scoreTimer.repeat=true;
                scoreTimer.start();
                timerText.text="Stop Timer"
            }
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

    Timer {
        id: scoreTimer;
        interval: 1000;
        repeat: true;
        running: false;
        triggeredOnStart: false;

        onTriggered: {
             if (--timerButton.countDownTimerValue == 0) {
                 stop();
                 timerSection.opacity=0;
                 timeRemaining.opacity=0;
                 timerText.text="Start Timer";
             }
             else{
                 timerSection.opacity=0.35;
                 if(timerButton.countDownTimerValue>9)
                 {
                     timeRemaining.text='00:'+timerButton.countDownTimerValue;
                 }
                 else
                 {
                     timeRemaining.text='00:0'+timerButton.countDownTimerValue;
                 }
                 timeRemaining.opacity=1;
             }
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
        property int countDownTimerValue:0;

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:hintButton.state="onEntered"
            onExited:hintButton.state="onExited"
            onClicked:{
                hintButton.countDownTimerValue=5;
                hintTimer.repeat=true;
                hintTimer.start();
            }
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

        transitions: Transition {
                PropertyAnimation { properties: "x,y,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
            }
    }

    Rectangle{
        id: timerSection
        width:blackboard.width/9;height:blackboard.height/7
        anchors{top:blackboard.top;topMargin:blackboard.height/7;right: blackboard.left;rightMargin:blackboard.width/68.5}
        color:"black"
        opacity:0
    }

    Text{
        id:timeRemaining
        anchors{verticalCenter: timerSection.verticalCenter;horizontalCenter:timerSection.horizontalCenter}
        opacity:0
        text: " "
        color:"white"
        font.pixelSize: parent.width/32
    }

    Rectangle {
        id: hintSection
        width: parent.width/3; height: parent.height/3
        opacity:0
        color: "black"
        anchors{verticalCenter: blackboard.verticalCenter;right: blackboard.left;rightMargin:blackboard.width/68.5}
    }

    Text{
        id:anagramHint
        anchors{verticalCenter: hintSection.verticalCenter;horizontalCenter:hintSection.horizontalCenter}
        color:"white"
        text: kanagramEngineHelper.showHint();
        opacity:0
        font.pixelSize: hintSection.width/10
    }

    Timer {
        id: hintTimer;
        interval: 1000;
        repeat: true;
        running: false;
        triggeredOnStart: false;

        onTriggered: {
             if (--hintButton.countDownTimerValue == 0) {
                 hintSection.opacity=0;
                 anagramHint.opacity=0;
                 stop();
             }
             else{
                 hintSection.opacity=0.35;
                 anagramHint.opacity=1;
             }
        }
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
            onClicked:anagram.text=kanagramEngineHelper.anagramOriginalWord();
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
