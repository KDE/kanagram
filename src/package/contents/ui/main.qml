import QtQuick 1.1

Rectangle {

    id:screen

    width: 1366 ; height: 768

    Item {
        width: parent.width
        height: parent.height

        Image {
            id: background
            anchors.fill : parent
            source: "../ui/images/background.jpg"
            smooth:true
            fillMode: Image.PreserveAspectCrop
        }
    }

    Rectangle {
        id: toolBarTop
        width: parent.width; height: parent.height/6
        opacity: .25
        color: "black"
        anchors.top: screen.top
    }

    Image {
            id: header
            width:toolBarTop.width/3.5
            height:toolBarTop.height/1.5
            anchors{verticalCenter: toolBarTop.verticalCenter;horizontalCenter: toolBarTop.horizontalCenter }
            source: "../ui/images/header.png"
            smooth:true
            fillMode: Image.PreserveAspectCrop
    }

    Image{
            id: powerButton
            smooth:true
            height:toolBarTop.height/3
            anchors{verticalCenter: toolBarTop.verticalCenter;right:toolBarTop.right;rightMargin: toolBarTop.width/24}
            source: "../ui/icons/on-off-light.svgz"
            fillMode: Image.PreserveAspectFit

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered:powerButton.state="onEntered"
                onExited:powerButton.state="onExited";
            }

            states: State {
                   name: "onEntered"
                   PropertyChanges {
                       target: quitText
                       opacity:1
                   }
               }
            State{
                name:"onExited"
            }

            transitions: Transition {
                    //AnchorAnimation { duration: 200 }
                    PropertyAnimation { properties: "x,y,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
                }
    }

    Text{
        id:quitText
        anchors{top:powerButton.bottom;horizontalCenter:powerButton.horizontalCenter}
        color:"white"
        text:"Quit"
        opacity:0
        font.pixelSize: parent.width/91
    }

        Rectangle{
            id: nextAnagramButton
            width:blackboard.width/9;height:blackboard.height/7
            anchors{top:blackboard.top;topMargin:blackboard.height/7;left:blackboard.right}
            radius:8
            color:"black"
            opacity:0.5

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered:nextAnagramButton.state="onEntered"
                onExited:nextAnagramButton.state="onExited";
                onClicked:nextAnagramButton.state="onClicked";
            }

            states: State {
                   name: "onEntered"
                    PropertyChanges {
                       target: nextAnagramButton
                       width:blackboard.width/3
                   }
                   PropertyChanges {
                       target: nextAnagramText
                       opacity:1
                   }
                   AnchorChanges{target:nextAnagramIcon;
                       anchors.horizontalCenter: undefined;
                       anchors.right:nextAnagramButton.right}
               }
               State{
                   name:"onClicked"
                   AnchorChanges{target:nextAnagramIcon;
                       anchors.horizontalCenter: undefined;
                       anchors.right:nextAnagramButton.right}
                   PropertyChanges {
                       target: nextAnagramButton
                       width:blackboard.width/3
                   }
                   PropertyChanges{
                       target:nextAnagramText
                       opacity: 1
                       text:kanagramEngineHelper.createNextAnagram();
                   }
               }
            State{
                name:"onExited"
            }

            transitions: Transition {
                    AnchorAnimation { duration: 250 }
                    PropertyAnimation { properties: "x,y,width,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
                }
        }

        Text{
            id:nextAnagramText
            anchors{verticalCenter: nextAnagramButton.verticalCenter;horizontalCenter:nextAnagramButton.horizontalCenter}
            color:"white"
            text: "Next Anagram"
            opacity: 0
            font.pixelSize: parent.width/68.5
        }

        Image{
                id: nextAnagramIcon
                smooth:true
                height:nextAnagramButton.height/2; width:nextAnagramText.width/4
                anchors{verticalCenter: nextAnagramButton.verticalCenter;horizontalCenter: nextAnagramButton.horizontalCenter}
                source: "../ui/icons/arrow-light.svgz"
                fillMode: Image.PreserveAspectFit
        }

    Rectangle{
        id: configureButton
        width:blackboard.width/9;height:blackboard.height/7
        anchors{verticalCenter:blackboard.verticalCenter;left:blackboard.right}
        radius:8
        color:"black"
        opacity:0.5

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:configureButton.state="onEntered"
            onExited:configureButton.state="onExited";
        }

        states: State {
               name: "onEntered"
               PropertyChanges {
                   target: configureButton
                   width:blackboard.width/3
               }
               PropertyChanges {
                   target: configureText
                   opacity:1
               }
               AnchorChanges{target:configureIcon;
                   anchors.horizontalCenter: undefined;
                   anchors.right:configureButton.right}
           }
        State{
            name:"onExited"
        }

        transitions: Transition {
                AnchorAnimation { duration: 250 }
                PropertyAnimation { properties: "x,y,width,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
            }
    }

    Text{
        id:configureText
        anchors{verticalCenter: configureButton.verticalCenter;horizontalCenter:configureButton.horizontalCenter}
        color:"white"
        text:"Configure"
        opacity:0
        font.pixelSize: parent.width/68.5
    }

    Image{
            id: configureIcon
            smooth:true
            height:configureButton.height/2;width:configureText.width/3
            anchors{verticalCenter: configureButton.verticalCenter;horizontalCenter: configureButton.horizontalCenter}
            source: "../ui/icons/spanner-light.svgz"
            fillMode: Image.PreserveAspectFit
    }

    Rectangle{
        id: helpButton
        width:blackboard.width/9;height:blackboard.height/7
        anchors{bottom:blackboard.bottom;bottomMargin: blackboard.height/7;left:blackboard.right}
        radius:8
        color:"black"
        opacity:0.5

        property bool flag:true

        function buttonClicked()
        {
            if(flag)
            {
                moreOptionsButton.opacity=0.5;
                moreOptionsButton.width=blackboard.width/3;
                kanagramHandbookIcon.visible=true;
                aboutKdeIcon.visible=true;
                aboutKanagramIcon.visible=true;
                flag=false;
            }
            else
            {
                moreOptionsButton.opacity=0;
                moreOptionsTextBar.opacity=0;
                kanagramHandbookIcon.visible=false;
                aboutKdeIcon.visible=false;
                aboutKanagramIcon.visible=false;
                flag=true;
            }
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:helpButton.state="onEntered"
            onExited:helpButton.state="onExited";
            onClicked:helpButton.buttonClicked();
        }

        states: State {
               name: "onEntered"
               PropertyChanges {
                   target: helpButton
                   width:blackboard.width/3
               }
               PropertyChanges{
                   target:helpText
                   text:"About"
                   opacity:1
               }
               AnchorChanges{target:helpIcon;
                   anchors.horizontalCenter: undefined;
                   anchors.right:helpButton.right}
        }
        State{
            name:"onExited"
        }

        transitions: Transition {
                AnchorAnimation { duration: 250 }
                PropertyAnimation { properties: "x,y,width,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
            }
    }

    Rectangle{
        id: moreOptionsButton
        width:blackboard.width/9;height:blackboard.height/7
        anchors{top:helpButton.bottom;topMargin: blackboard.height/50;left:blackboard.right}
        radius:8
        color:"black"
        opacity:0
    }

    Rectangle{
        id: moreOptionsTextBar
        width:blackboard.width/4;height:blackboard.height/16
        anchors{top:moreOptionsButton.bottom;horizontalCenter:moreOptionsButton.horizontalCenter}
        radius:8
        color:"black"
        opacity:0
    }

    Text{
        id:moreOptioinsText
        anchors{verticalCenter: moreOptionsTextBar.verticalCenter;horizontalCenter:moreOptionsTextBar.horizontalCenter}
        color:"white"
        text: "  "
        opacity: 0
        font.pixelSize: parent.width/91
    }

    Text{
        id:helpText
        anchors{verticalCenter: helpButton.verticalCenter;horizontalCenter:helpButton.horizontalCenter}
        color:"white"
        text: "About"
        opacity: 0
        font.pixelSize: parent.width/68.5
    }

    Image{
        id: helpIcon
        smooth:true
        height:blackboard.height/14
        anchors{verticalCenter: helpButton.verticalCenter;horizontalCenter:helpButton.horizontalCenter}
        source: "../ui/icons/question-light.svgz"
        visible:true
        fillMode: Image.PreserveAspectFit
    }

    Image{
        id: aboutKdeIcon
        smooth:true
        height:blackboard.height/14;
        anchors{verticalCenter: moreOptionsButton.verticalCenter;right:moreOptionsButton.right;rightMargin:moreOptionsButton.width/8}
        source: "../ui/icons/about-kde.png"
        visible:false
        fillMode: Image.PreserveAspectFit

         MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:aboutKdeIcon.state="onEntered"
            onExited:aboutKdeIcon.state="onExited"
            preventStealing:true
        }

        states: State {
               name: "onEntered"
               PropertyChanges {
                   target: moreOptionsTextBar
                   opacity:0.5
               }
               PropertyChanges {
                   target: moreOptioinsText
                   text:"About KDE"
                   opacity:1
               } 
        }
        State{
            name:"onExited"
        }

        transitions: Transition {
                AnchorAnimation { duration: 250 }
                PropertyAnimation { properties: "x,y,width,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
            }
    }

    Image{
        id: aboutKanagramIcon
        height:blackboard.height/14
        anchors{verticalCenter: moreOptionsButton.verticalCenter;horizontalCenter:moreOptionsButton.horizontalCenter}
        source: "../ui/icons/about-kanagram.png"
        smooth:true
        visible:false
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:aboutKanagramIcon.state="onEntered"
            onExited:aboutKanagramIcon.state="onExited"
            preventStealing:true
        }

        states: State {
               name: "onEntered"
               PropertyChanges {
                   target: moreOptionsTextBar
                   opacity:0.5
               }
               PropertyChanges {
                   target: moreOptioinsText
                   text:"About Kanagram"
                   opacity:1
               } 
        }
        State{
            name:"onExited"
        }

        transitions: Transition {
                AnchorAnimation { duration: 250 }
                PropertyAnimation { properties: "x,y,width,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
            }
    }

    Image{
        id: kanagramHandbookIcon
        smooth:true
        height:blackboard.height/14
        anchors{verticalCenter: moreOptionsButton.verticalCenter;left:moreOptionsButton.left;leftMargin:moreOptionsButton.width/8}
        source: "../ui/icons/kanagram-handbook.png"
        visible:false
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:kanagramHandbookIcon.state="onEntered"
            onExited:kanagramHandbookIcon.state="onExited"
            preventStealing:true
        }

        states: State {
               name: "onEntered"
               PropertyChanges {
                   target:moreOptionsTextBar
                   opacity:0.5
               }
               PropertyChanges {
                   target: moreOptioinsText
                   text:"Kanagram Handbook"
                   opacity:1
               } 
        }
        State{
            name:"onExited"
        }

        transitions: Transition {
                AnchorAnimation { duration: 250 }
                PropertyAnimation { properties: "x,y,width,opacity"; easing.type: Easing.Linear; easing.amplitude: 5.0; easing.period: 1 }
            }
    }

    Blackboard{
        id:blackboard
        border{width:parent.width/68.5;color:"#613529"}
        width:parent.width/2; height: parent.height/1.5
        anchors {horizontalCenter:parent.horizontalCenter; top:toolBarTop.bottom}
    }

    Rectangle{
        id: inputField
        width:blackboard.width;height:parent.height/10
        anchors{horizontalCenter: blackboard.horizontalCenter;bottom:parent.bottom;bottomMargin:parent.height/35}
        radius:8
        color:"black"
        opacity:0.35
    }

    TextInput{
         id: input
         color: "white"; selectionColor: "white";selectedTextColor: "black"   //DEB204,0B7BB8
         font.pixelSize: parent.width/65; font.bold: true
         width: inputField.width
         anchors.centerIn: inputField
         focus: true
     }

}
