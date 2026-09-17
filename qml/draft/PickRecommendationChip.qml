import QtQuick
import QtQuick.Layouts

import Qcm.Material as MD

Item {
    id: root

    required property string heroName
    required property int tier
    required property string message
    required property double matchupAdvantage
    required property int gamesTotal

    implicitWidth: column.implicitWidth
    implicitHeight: column.implicitHeight

    readonly property color chipBg: {
        switch (root.tier) {
        case 0: return MD.Token.color.primary_container
        case 1: return MD.Token.color.secondary_container
        case 3: return MD.Token.color.error_container
        case 4: return MD.Token.color.tertiary_container
        default: return MD.Token.color.surface_container_high
        }
    }

    readonly property color chipFg: {
        switch (root.tier) {
        case 0: return MD.Token.color.on_primary_container
        case 1: return MD.Token.color.on_secondary_container
        case 3: return MD.Token.color.on_error_container
        case 4: return MD.Token.color.on_tertiary_container
        default: return MD.Token.color.on_surface
        }
    }

    ColumnLayout {
        id: column
        width: parent.width
        spacing: MD.Token.spacing.extra_small

        MD.AssistChip {
            Layout.fillWidth: true
            text: root.heroName
            MD.MProp.backgroundColor: root.chipBg
            MD.MProp.textColor: root.chipFg
        }

        MD.Label {
            Layout.fillWidth: true
            text: root.message
            typescale: MD.Token.typescale.body_small
            color: MD.MProp.color.on_surface_variant
            wrapMode: Text.WordWrap
            maximumLineCount: 3
        }

        MD.Label {
            Layout.fillWidth: true
            text: {
                const pct = (Math.round(root.matchupAdvantage * 10) / 10)
                const sign = pct > 0 ? "+" : ""
                return sign + pct + "% · " + root.gamesTotal + " " + qsTr("игр")
            }
            typescale: MD.Token.typescale.label_small
            color: MD.MProp.color.outline
        }
    }
}
