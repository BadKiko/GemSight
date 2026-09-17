import QtQuick
import QtQuick.Layouts

import GemSight.Core 1.0
import Qcm.Material as MD

import "../i18n"

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        spacing: MD.Token.spacing.medium

        DraftBalanceBar {
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: MD.Token.spacing.medium

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 0
                spacing: MD.Token.spacing.small

                MD.Label {
                    text: Messages.enemiesTitle
                    typescale: MD.Token.typescale.label_large
                    color: MD.MProp.color.on_surface_variant
                }

                ListView {
                    id: enemyList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    orientation: ListView.Horizontal
                    spacing: MD.Token.spacing.small
                    model: Core.draft.enemies
                    delegate: DraftPlayerCard {
                        required property int teamSlot
                        required property string heroName
                        required property string playerName
                        required property string statusText
                        required property double winRate
                        required property bool profileUnlocked
                        required property string roleLabel
                        required property double roleConfidence

                        width: Math.max(160, (enemyList.width - 4 * enemyList.spacing) / 5)
                        height: enemyList.height
                        slotIndex: teamSlot
                        displayHero: heroName
                        displayPlayer: playerName
                        displayStatus: statusText
                        displayWinRate: winRate
                        profileOpen: profileUnlocked
                        roleLabel: roleLabel
                        roleConfidence: roleConfidence
                    }
                }
            }

            LookaheadThreatCard {
                Layout.preferredWidth: 220
                Layout.fillHeight: true
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: MD.Token.spacing.small

            MD.Label {
                text: Messages.alliesTitle
                typescale: MD.Token.typescale.label_large
                color: MD.MProp.color.on_surface_variant
            }

            ListView {
                id: allyList
                Layout.fillWidth: true
                Layout.preferredHeight: 148
                orientation: ListView.Horizontal
                spacing: MD.Token.spacing.small
                model: Core.draft.allies
                delegate: DraftPlayerCard {
                    required property int teamSlot
                    required property string heroName
                    required property string playerName
                    required property string statusText
                    required property double winRate
                    required property bool profileUnlocked
                    required property string roleLabel
                    required property double roleConfidence

                    width: Math.max(140, (allyList.width - 4 * allyList.spacing) / 5)
                    height: allyList.height
                    slotIndex: teamSlot
                    displayHero: heroName
                    displayPlayer: playerName
                    displayStatus: statusText
                    displayWinRate: winRate
                    profileOpen: profileUnlocked
                    roleLabel: roleLabel
                    roleConfidence: roleConfidence
                }
            }
        }
    }
}
