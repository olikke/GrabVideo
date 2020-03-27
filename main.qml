import QtQuick 2.11
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.2
import "."

Window {
    visible: true
    width: 1920
    height: 1200
    color: primary
    title: qsTr("Hello World")

    Component.onCompleted: {
        Material.theme=Material.Dark
        Material.accent=accent
    }

    Connections{
        target: iProvider
        onImageChanged: image.reload()
    }

    Connections{
        target: littleProvider
        onImageChanged: littleImage.reload()
    }

    Connections{
        target: imageFront
        onTrackingAreaChanged: {
            trackingAreaScal=scal
        }
        onTargetPosChanged : {
            var target=targetPos
            xPos.text="x="+target.x
            yPos.text="y="+target.y
            if (tracking===1)
            {
                frameImage.x=target.x*videoScaleX-frameImage.width
                frameImage.y=target.y*videoScaleY-frameImage.height
            }
        }
    }

    property string background: "#2d2d2d"
    property string primary: "#474747"
    property string accent: "#039be5"
    property string foreground: "#f7f7f7"
    property int oneHeight: 40
    property int devNumb: 0
    property int tracking: 0
    onTrackingChanged: imageFront.changeTrack(tracking)
    property bool needPos: false
    property real trackingAreaScal: 1
    property size videoSize: videoGrab.videoSize
    onVideoSizeChanged: {
        videoScaleX=image.width/videoSize.width
        videoScaleY=image.height/videoSize.height
    }
    property real videoScaleX: 1
    property real videoScaleY: 1

    FileDialog {
        id:fileDialog
        title: "bla bla bla"
        folder: shortcuts.home
        nameFilters: ["Video files (*.mp4 *.mpg *.ogg *.mov *.flv *.mkv *.avi)","All files (*)"]
        onAccepted: videoGrab.playFile(fileDialog.fileUrl)
    }


    Item{
        id: control
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: 250

        Rectangle{
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: 2
            color: background
            anchors.rightMargin: 0
        }

        Column{
            width: parent.width-anchors.leftMargin-anchors.rightMargin-2
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            spacing: 2

            ComboBox {
                id: videoDevices
                width: parent.width
                height: oneHeight
                model: videoGrab.devices
                onCurrentIndexChanged: devNumb=currentIndex
            }

            Row{
                id: row
                height: oneHeight
                width: parent.width
                spacing: 2

                Button{
                    text: qsTr("Refresh")
                    padding: 0
                    bottomPadding: 0
                    topPadding: 0
                    width: parent.width/2
                    onClicked: videoGrab.refresh()
                    height: oneHeight
                }
                Button{
                    text: qsTr("Open")
                    padding: 0
                    bottomPadding: 0
                    topPadding: 0
                    width: parent.width/2
                    height: oneHeight
                    onClicked: {
                        devNumb===0 ? fileDialog.open() :
                            videoGrab.openDevice(devNumb-1) //olikke because file first
                    }
                }
            }
            Column{
                anchors.left: parent.left
                anchors.right: parent.right
                height: implicitHeight
                anchors.margins: 0
                spacing: 2
                enabled:videoGrab.open
                Rectangle{
                    width: parent.width+10
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 2
                    color: background
                }
                Row{
                    height: oneHeight
                    width: parent.width
                    spacing: 2

                    Label{
                        text: qsTr("Brightness")
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width/3
                    }
                    Slider{
                        stepSize: 0.01
                        to: 1
                        from: 0
                        value: videoGrab.brightness
                        Material.accent: accent
                        width: parent.width*2/3-spacing
                        onMoved: videoGrab.brightness=value
                    }
                }
                Row{
                    height: oneHeight
                    width: parent.width
                    spacing: 2

                    Label{
                        text: qsTr("Contrast")
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width/3
                    }
                    Slider{
                        stepSize: 0.01
                        to: 1
                        from: 0
                        value: videoGrab.contrast
                        Material.accent: accent
                        width: parent.width*2/3-spacing
                        onMoved: videoGrab.contrast=value
                    }
                }
                Row{
                    height: oneHeight
                    width: parent.width
                    spacing: 2

                    Label{
                        text: qsTr("Saturation")
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width/3
                    }
                    Slider{
                        stepSize: 0.01
                        to: 1
                        from: 0
                        value: videoGrab.saturation
                        Material.accent: accent
                        width: parent.width*2/3-spacing
                        onMoved: videoGrab.saturation=value
                    }
                }
                Row{
                    height: oneHeight
                    width: parent.width
                    spacing: 2

                    Label{
                        text: qsTr("Hue")
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width/3
                    }
                    Slider{
                        stepSize: 0.01
                        to: 1
                        from: 0
                        value: videoGrab.hue
                        Material.accent: accent
                        width: parent.width*2/3-spacing
                        onMoved: videoGrab.hue=value
                    }
                }
                Rectangle{
                    width: parent.width+10
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 2
                    color: background
                }
            }

            Rectangle {
                width: 129
                height: 129
                color: background
                anchors.horizontalCenter: parent.horizontalCenter
                Image {
                    id: littleImage
                    property int littleCounter: 1
                    anchors.fill: parent
                    cache: false
                    smooth: true
                    autoTransform: true
                    fillMode: Image.PreserveAspectFit
                    source: "image://little/image"
                    function reload() {
                        littleCounter++
                        source = "image://little/image?id="+littleCounter
                    }
                }
            }
            Row{
                width: parent.width
                height: oneHeight
                spacing: 2
                Rectangle {
                    width: parent.width/2-parent.spacing/2
                    height: oneHeight
                    color: background
                    Label{
                        id: xPos
                        text: qsTr("x=0.00")
                        anchors.leftMargin: 10
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Rectangle {
                    width: parent.width/2-parent.spacing/2
                    height: oneHeight
                    color: background
                    Label{
                        id: yPos
                        text: qsTr("y=0.00")
                        anchors.leftMargin: 10
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                    }

                }

            }



            Rectangle{
                width: parent.width+10
                anchors.horizontalCenter: parent.horizontalCenter
                height: 2
                color: background
            }

            Row {
                height: oneHeight
                width: parent.width
                spacing: 2
                enabled:videoGrab.open && ! needPos

                Label{
                    text: qsTr("Фильтр")
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    clip: true
                    width: parent.width/3
                }
                Slider {
                    stepSize: 1
                    from: 1
                    value: 5
                    to: 10
                    width: parent.width*2/3-spacing
                    Material.accent: accent
                    onMoved: iProcessing.changeFilterParameter(value)
                }
            }
        }
    }

    Image {
        id: image
        property bool counter: false
        anchors.top: parent.top
        anchors.left: control.right
        anchors.topMargin: 10
        width: 1440
        height: 1152
        anchors.leftMargin: 10
        cache: false
        smooth: true
        autoTransform: false
        fillMode: Image.PreserveAspectFit
        source: "image://live/image"
        function reload() {
            counter = !counter
            source = "image://live/image?id="+counter
        }
        Image {
            id: crossImage
            fillMode: Image.PreserveAspectFit
            //olikke Problem
            //cant read property of imageProcessing, because he lives in different thread
            //result - cross leaves image border
            x: mouseArea.mouseX-width/2
            y: mouseArea.mouseY-width/2
            source: "qrc:/cross/cross.svg"
            visible: false
            sourceSize.width: width
            sourceSize.height: width
            //olikke - можно 32 не вбивать, а реальный размер взять?
            width: 32*trackingAreaScal
            height: width
        }
        ColorOverlay{
            anchors.fill: crossImage
            source:crossImage
            color:accent
            visible: tracking===2
        }
        Image {
            id: frameImage
            fillMode: Image.PreserveAspectFit
            source: "qrc:/cross/cross.svg"
            visible: false
            sourceSize.width: width
            sourceSize.height: width
            //olikke - можно 32 не вбивать, а реальный размер взять?
            width: 32*trackingAreaScal
            height: width
        }
        ColorOverlay{
            anchors.fill: frameImage
            source:frameImage
            color:"yellow"
            visible: tracking===1
        }

    }

    разобраться со скалированием

    MouseArea {
        function calcMousePos()
        {
            imageFront.mousePos(Qt.point(mouseX/videoScaleX,mouseY/videoScaleY))
        }

        id: mouseArea
        width: 1440
        height: 1152
        anchors.fill: image
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            //ручное сопровождение
            if (mouse.button===Qt.LeftButton)
            {
                tracking=2
                calcMousePos()
            }
            //автоматическое сопровождение
            if (mouse.button===Qt.RightButton)
            {
                tracking=1
                calcMousePos()
            }
        }
        onPositionChanged: {
            if (tracking===2) calcMousePos()
        }
        onWheel: {
             imageFront.changeTrackingArea(wheel.angleDelta.y>0)
        }
    }
}
