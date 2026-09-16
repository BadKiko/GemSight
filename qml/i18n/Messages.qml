pragma Singleton

import QtQuick

QtObject {
    readonly property string appTitle: qsTr("GemSight")
    readonly property string draftTitle: qsTr("Драфт")
    readonly property string alliesTitle: qsTr("Союзники")
    readonly property string enemiesTitle: qsTr("Враги")
    readonly property string advisorTitle: qsTr("Советник")
    readonly property string demoTurbo: qsTr("Демо: Turbo")
    readonly property string gsiLabel: qsTr("GSI")
    readonly property string policyLabel: qsTr("Политика разведки")
    readonly property string waitingPick: qsTr("Ожидание пика")
}
