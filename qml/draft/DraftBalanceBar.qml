import QtQuick
import QtQuick.Layouts

import GemSight.Core 1.0
import Qcm.Material as MD

import "../i18n"

MD.Card {
    id: root

    type: MD.Enum.CardFilled
    MD.MProp.elevation: MD.MProp.elevation.level0
    MD.MProp.backgroundColor: MD.MProp.color.surface_container_high

    implicitHeight: content.implicitHeight + MD.Token.spacing.medium * 2

    ColumnLayout {
        id: content
        anchors.fill: parent
        anchors.margins: MD.Token.spacing.medium
        spacing: MD.Token.spacing.small

        RowLayout {
            Layout.fillWidth: true
            spacing: MD.Token.spacing.medium

            MD.Label {
                text: Messages.balanceTitle
                typescale: MD.Token.typescale.title_small
            }

            Item { Layout.fillWidth: true }

            MD.Label {
                text: Math.round(Core.draft.evaluator.liveWinProbability * 1000) / 10 + "%"
                typescale: MD.Token.typescale.headline_small
                color: Core.draft.evaluator.liveWinProbability >= 0.5
                       ? MD.MProp.color.primary
                       : MD.MProp.color.error
            }
        }

        MD.LinearIndicator {
            Layout.fillWidth: true
            indeterminate: false
            from: 0
            to: 1
            value: Core.draft.evaluator.liveWinProbability
        }

        MD.Label {
            Layout.fillWidth: true
            text: Core.draft.evaluator.balanceHint
            typescale: MD.Token.typescale.body_small
            color: MD.MProp.color.on_surface_variant
            wrapMode: Text.WordWrap
        }
    }
}
