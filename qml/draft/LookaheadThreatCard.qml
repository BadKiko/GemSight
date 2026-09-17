import QtQuick
import QtQuick.Layouts

import Qcm.Material as MD

import "../i18n"

MD.Card {
    id: root

    type: MD.Enum.CardOutlined
    MD.MProp.elevation: MD.MProp.elevation.level0

    implicitHeight: content.implicitHeight + MD.Token.spacing.medium * 2

    ColumnLayout {
        id: content
        anchors.fill: parent
        anchors.margins: MD.Token.spacing.medium
        spacing: MD.Token.spacing.small

        MD.Label {
            text: Messages.threatTitle
            typescale: MD.Token.typescale.title_small
        }

        MD.Label {
            Layout.fillWidth: true
            text: Messages.threatPlaceholder
            typescale: MD.Token.typescale.body_medium
            color: MD.MProp.color.on_surface_variant
            wrapMode: Text.WordWrap
        }
    }
}
