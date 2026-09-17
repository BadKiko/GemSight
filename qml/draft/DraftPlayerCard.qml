import QtQuick
import QtQuick.Layouts

import Qcm.Material as MD

import "../components"
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
    property url avatarUrl
    property url heroPortraitUrl

    MD.MProp.elevation: MD.MProp.elevation.level1
    type: MD.Enum.CardOutlined

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        HeroPortrait {
            Layout.fillWidth: true
            Layout.preferredHeight: 64
            source: root.heroPortraitUrl
            fallbackText: root.displayHero.length ? root.displayHero.charAt(0) : "?"
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: MD.Token.spacing.small

            RowLayout {
                Layout.fillWidth: true
                Layout.topMargin: MD.Token.spacing.small
                Layout.leftMargin: MD.Token.spacing.medium
                Layout.rightMargin: MD.Token.spacing.medium
                spacing: MD.Token.spacing.small

                PlayerAvatar {
                    displayName: root.displayPlayer
                    source: root.avatarUrl
                    showRing: root.profileOpen
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2

                    MD.Label {
                        Layout.fillWidth: true
                        text: root.displayPlayer.length ? root.displayPlayer : Messages.waitingPick
                        typescale: MD.Token.typescale.title_small
                        elide: Text.ElideRight
                        maximumLineCount: 1
                    }

                    MD.Label {
                        Layout.fillWidth: true
                        text: root.displayHero.length ? root.displayHero : "—"
                        typescale: MD.Token.typescale.body_small
                        color: MD.MProp.color.on_surface_variant
                        elide: Text.ElideRight
                    }
                }

                MD.AssistChip {
                    visible: root.roleLabel.length > 0
                    text: root.roleLabel
                    MD.MProp.backgroundColor: MD.MProp.color.secondary_container
                }
            }

            WinrateBar {
                Layout.fillWidth: true
                Layout.leftMargin: MD.Token.spacing.medium
                Layout.rightMargin: MD.Token.spacing.medium
                visible: root.profileOpen && root.displayWinRate >= 0
                winRate: root.displayWinRate
            }

            MD.Label {
                Layout.fillWidth: true
                Layout.leftMargin: MD.Token.spacing.medium
                Layout.rightMargin: MD.Token.spacing.medium
                Layout.bottomMargin: MD.Token.spacing.small
                text: root.displayStatus
                typescale: MD.Token.typescale.body_small
                color: MD.MProp.color.on_surface_variant
                wrapMode: Text.WordWrap
                maximumLineCount: 2
            }
        }
    }
}
