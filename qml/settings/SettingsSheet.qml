import QtQuick
import QtQuick.Layouts

import GemSight.Core 1.0
import Qcm.Material as MD

import "../i18n"

MD.BottomSheet {
    id: root

    sheetType: MD.Enum.BottomSheetModal
    dismissOnDragDown: false

    function openSettings() {
        root.open()
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: MD.Token.spacing.large
        spacing: MD.Token.spacing.medium

        MD.Label {
            text: qsTr("Настройки")
            typescale: MD.Token.typescale.headline_small
        }

        MD.Card {
            Layout.fillWidth: true
            type: MD.Enum.CardOutlined

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: MD.Token.spacing.medium
                spacing: MD.Token.spacing.small

                MD.Label {
                    text: Messages.gsiLabel
                    typescale: MD.Token.typescale.title_small
                }

                MD.Label {
                    Layout.fillWidth: true
                    text: Messages.gsiHelp
                    typescale: MD.Token.typescale.body_small
                    color: MD.MProp.color.on_surface_variant
                    wrapMode: Text.WordWrap
                }

                MD.Button {
                    text: qsTr("Запустить GSI-сервер")
                    onClicked: {
                        Core.draft.startGsi()
                        root.close()
                    }
                }
            }
        }

        MD.Card {
            Layout.fillWidth: true
            type: MD.Enum.CardOutlined

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: MD.Token.spacing.medium
                spacing: MD.Token.spacing.small

                MD.Label {
                    text: qsTr("STRATZ API")
                    typescale: MD.Token.typescale.title_small
                }

                MD.Label {
                    Layout.fillWidth: true
                    text: qsTr("Токен для batch-запросов (M3). Хранится локально.")
                    typescale: MD.Token.typescale.body_small
                    color: MD.MProp.color.on_surface_variant
                    wrapMode: Text.WordWrap
                }

                MD.TextField {
                    Layout.fillWidth: true
                    text: Core.settings.stratzApiToken
                    placeholderText: qsTr("Вставьте токен STRATZ")
                    onTextChanged: Core.settings.stratzApiToken = text
                }
            }
        }

        MD.Button {
            Layout.alignment: Qt.AlignRight
            text: qsTr("Закрыть")
            mdState.type: MD.Enum.BtText
            onClicked: root.close()
        }
    }
}
