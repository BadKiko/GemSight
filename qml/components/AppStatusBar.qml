import QtQuick
import QtQuick.Layouts

import GemSight.Core 1.0
import Qcm.Material as MD

import "../i18n"

Rectangle {
    id: root

    implicitHeight: 36
    color: MD.Token.color.surface_container_high
    radius: MD.Token.shape.corner.medium

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: MD.Token.spacing.medium
        anchors.rightMargin: MD.Token.spacing.medium
        spacing: MD.Token.spacing.large

        MD.Label {
            text: Messages.gsiLabel + ": "
                  + (Core.draft.gsiListening ? qsTr("слушает :27042") : qsTr("ожидание"))
            typescale: MD.Token.typescale.label_large
            color: MD.MProp.color.on_surface_variant
        }

        MD.Label {
            text: qsTr("Фаза:") + " " + Core.matchPhase
            typescale: MD.Token.typescale.label_large
            color: MD.MProp.color.on_surface_variant
        }

        MD.Label {
            Layout.fillWidth: true
            text: Core.draft.statusLine
            typescale: MD.Token.typescale.body_small
            color: MD.MProp.color.on_surface_variant
            elide: Text.ElideRight
        }
    }
}
