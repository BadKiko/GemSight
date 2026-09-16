import QtQuick
import QtQuick.Layouts

import GemSight.Core 1.0
import Qcm.Material as MD

import "../i18n"

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        MD.Text {
            text: Messages.enemiesTitle
            typescale: MD.Token.typescale.title_medium
        }

        ListView {
            id: enemyList
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: ListView.Horizontal
            spacing: 8
            model: Core.draft.enemies
            delegate: EnemyColumn {
                required property int teamSlot
                required property string heroName
                required property string playerName
                required property string statusText
                required property double winRate
                required property bool profileUnlocked

                width: (enemyList.width - 4 * enemyList.spacing) / 5
                height: enemyList.height
                slotIndex: teamSlot
                displayHero: heroName
                displayPlayer: playerName
                displayStatus: statusText
                displayWinRate: winRate
                profileOpen: profileUnlocked
            }
        }
    }
}
