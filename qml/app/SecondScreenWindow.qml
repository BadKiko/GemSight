import QtQuick
import QtQuick.Layouts

import GemSight.Core 1.0
import Qcm.Material as MD

import "../theme"
import "../i18n"
import "../draft"

MD.ApplicationWindow {
    Component.onCompleted: Appearance.apply()
    id: root

    visible: true
    width: 1280
    height: 800
    minimumWidth: 960
    minimumHeight: 600
    title: Messages.appTitle

    color: MD.Token.color.surface_container
    MD.MProp.textColor: MD.MProp.color.on_surface
    MD.MProp.backgroundColor: MD.MProp.color.surface_container

    property int windowClass: MD.Token.window_class.select_type(width)
    MD.MProp.size.windowClass: windowClass

    Timer {
        interval: 200
        running: true
        repeat: false
        onTriggered: root.windowClass = MD.Token.window_class.select_type(root.width)
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            MD.Text {
                text: Messages.draftTitle
                typescale: MD.Token.typescale.headline_medium
            }

            MD.AssistChip {
                text: Core.session.scoutPolicy
                MD.MProp.backgroundColor: MD.MProp.color.secondary_container
            }

            Item { Layout.fillWidth: true }

            MD.Button {
                text: Messages.demoTurbo
                onClicked: Core.draft.simulateTurboDraft()
            }
        }

        MD.Text {
            Layout.fillWidth: true
            text: Core.draft.statusLine
            typescale: MD.Token.typescale.body_medium
            color: MD.MProp.color.on_surface_variant
            wrapMode: Text.WordWrap
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 16

            DraftScreen {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 0
            }

            AdvisorPanel {
                Layout.preferredWidth: 280
                Layout.fillHeight: true
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            MD.Text {
                text: Messages.gsiLabel + ": " + (Core.draft.gsiListening ? qsTr("слушает :27042") : qsTr("остановлен"))
                typescale: MD.Token.typescale.label_large
            }

            MD.Text {
                text: qsTr("Фаза:") + " " + Core.matchPhase
                typescale: MD.Token.typescale.label_large
                Layout.leftMargin: 24
            }
        }
    }
}
