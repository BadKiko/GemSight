import QtQuick
import QtQuick.Layouts

import Qcm.Material as MD

Item {
    id: root

    property url source
    property string fallbackText: "?"

    implicitWidth: 96
    implicitHeight: 54

    MD.ElevationRectangle {
        anchors.fill: parent
        radius: MD.Token.shape.corner.medium
        color: MD.Token.color.surface_container_highest
        elevation: MD.Token.elevation.level1
        clip: true

        Image {
            id: heroImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            source: root.source
            asynchronous: true
            cache: true
            visible: status === Image.Ready
        }

        MD.Label {
            anchors.centerIn: parent
            visible: heroImage.status !== Image.Ready
            text: root.fallbackText
            typescale: MD.Token.typescale.label_large
            color: MD.Token.color.on_surface_variant
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: parent.height * 0.45
            gradient: Gradient {
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 1.0; color: "#99000000" }
            }
        }
    }
}
