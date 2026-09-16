import QtQuick
import QtQuick.Layouts

import Qcm.Material as MD

import "../i18n"

MD.Card {
    id: root

    property int slotIndex: 0
    property string displayHero: ""
    property string displayPlayer: ""
    property string displayStatus: ""
    property double displayWinRate: -1
    property bool profileOpen: false

    MD.MProp.elevation: MD.MProp.elevation.level1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        MD.Text {
            text: qsTr("Слот %1").arg(root.slotIndex + 1)
            typescale: MD.Token.typescale.label_large
            color: MD.MProp.color.on_surface_variant
        }

        MD.Text {
            Layout.fillWidth: true
            text: root.displayPlayer.length ? root.displayPlayer : Messages.waitingPick
            typescale: MD.Token.typescale.title_medium
            wrapMode: Text.WordWrap
        }

        MD.Text {
            Layout.fillWidth: true
            text: root.displayHero.length ? root.displayHero : "—"
            typescale: MD.Token.typescale.body_small
            color: MD.MProp.color.on_surface_variant
        }

        WinrateBar {
            Layout.fillWidth: true
            visible: root.profileOpen && root.displayWinRate >= 0
            winRate: root.displayWinRate
        }

        MD.Text {
            Layout.fillWidth: true
            text: root.displayStatus
            typescale: MD.Token.typescale.body_small
            wrapMode: Text.WordWrap
        }
    }
}
