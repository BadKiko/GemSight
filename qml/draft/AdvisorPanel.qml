import QtQuick
import QtQuick.Layouts

import Qcm.Material as MD

import "../i18n"

MD.Card {
    MD.MProp.elevation: MD.MProp.elevation.level2

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        MD.Text {
            text: Messages.advisorTitle
            typescale: MD.Token.typescale.title_medium
        }

        MD.Text {
            Layout.fillWidth: true
            text: qsTr("Стартовая сборка и модификаторы линии появятся в M4.")
            wrapMode: Text.WordWrap
            typescale: MD.Token.typescale.body_medium
            color: MD.MProp.color.on_surface_variant
        }

        Item { Layout.fillHeight: true }
    }
}
