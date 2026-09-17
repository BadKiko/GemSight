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
    property string roleLabel: ""
    property double roleConfidence: 0

    MD.MProp.elevation: MD.MProp.elevation.level1
    type: MD.Enum.CardOutlined

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: MD.Token.spacing.medium
        spacing: MD.Token.spacing.small

        RowLayout {
            Layout.fillWidth: true
            spacing: MD.Token.spacing.small

            MD.Label {
                text: qsTr("Слот %1").arg(root.slotIndex + 1)
                typescale: MD.Token.typescale.label_large
                color: MD.MProp.color.on_surface_variant
            }

            Item { Layout.fillWidth: true }

            MD.AssistChip {
                visible: root.roleLabel.length > 0
                text: root.roleLabel
                MD.MProp.backgroundColor: MD.MProp.color.secondary_container
            }
        }

        MD.Label {
            Layout.fillWidth: true
            text: root.displayPlayer.length ? root.displayPlayer : Messages.waitingPick
            typescale: MD.Token.typescale.title_small
            wrapMode: Text.WordWrap
            maximumLineCount: 2
        }

        MD.Label {
            Layout.fillWidth: true
            text: root.displayHero.length ? root.displayHero : "—"
            typescale: MD.Token.typescale.body_medium
            color: MD.MProp.color.on_surface_variant
        }

        WinrateBar {
            Layout.fillWidth: true
            visible: root.profileOpen && root.displayWinRate >= 0
            winRate: root.displayWinRate
        }

        MD.Label {
            Layout.fillWidth: true
            text: root.displayStatus
            typescale: MD.Token.typescale.body_small
            color: MD.MProp.color.on_surface_variant
            wrapMode: Text.WordWrap
            maximumLineCount: 3
        }

        MD.Label {
            Layout.fillWidth: true
            visible: root.roleLabel.length > 0 && root.roleConfidence > 0
            text: qsTr("Роль (демо): ~%1%").arg(Math.round(root.roleConfidence * 100))
            typescale: MD.Token.typescale.label_small
            color: MD.MProp.color.outline
        }
    }
}
