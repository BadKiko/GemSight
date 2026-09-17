pragma Singleton

import QtQuick

QtObject {
    readonly property string appTitle: qsTr("GemSight")
    readonly property string draftTitle: qsTr("Драфт")
    readonly property string alliesTitle: qsTr("Союзники")
    readonly property string enemiesTitle: qsTr("Враги")
    readonly property string advisorTitle: qsTr("Советник пиков")
    readonly property string advisorHint: qsTr("Персональные рекомендации с учётом сигнатур и матчапа (демо M2).")
    readonly property string advisorEmpty: qsTr("Запустите «Демо: Turbo» или дождитесь обновления драфта.")
    readonly property string balanceTitle: qsTr("Баланс драфта")
    readonly property string threatTitle: qsTr("Угрозы вперёд")
    readonly property string threatPlaceholder: qsTr("Топ контрпиков появится после M3–M4 (lookahead).")
    readonly property string demoTurbo: qsTr("Демо: Turbo")
    readonly property string gsiLabel: qsTr("GSI")
    readonly property string gsiHelp: qsTr("Скопируйте cfg в game/dota/cfg/gamestate_integration/ и добавьте -gamestateintegration в Steam.")
    readonly property string policyLabel: qsTr("Политика разведки")
    readonly property string waitingPick: qsTr("Ожидание пика")
}
