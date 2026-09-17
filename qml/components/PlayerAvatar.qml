import QtQuick

import Qcm.Material as MD

Item {
    id: root

    property url source
    property string displayName: ""
    property bool showRing: false

    implicitWidth: size
    implicitHeight: size

    readonly property int size: MD.Token.spacing.extra_large + MD.Token.spacing.small
    readonly property int ringPad: 3
    readonly property int ringWidth: 2

    Rectangle {
        anchors.fill: parent
        visible: root.showRing
        radius: width / 2
        color: "transparent"
        border.width: root.ringWidth
        border.color: MD.Token.color.primary
    }

    Rectangle {
        id: face
        anchors.centerIn: parent
        width: root.size
        height: root.size
        radius: width / 2
        color: MD.Token.color.surface_container_highest
        clip: true

        Image {
            id: avatarImage
            anchors.fill: parent
            source: root.source
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
            cache: true
            visible: status === Image.Ready
        }

        MD.Label {
            anchors.centerIn: parent
            visible: !root.source.toString().length || avatarImage.status !== Image.Ready
            text: root.displayName.length ? root.displayName.charAt(0).toUpperCase() : "?"
            typescale: MD.Token.typescale.title_small
            color: MD.Token.color.on_surface
        }
    }
}
