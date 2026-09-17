import QtQuick
import QtQuick.Layouts

import GemSight.Core 1.0
import Qcm.Material as MD

import "../i18n"

MD.Pane {
    id: root

    padding: MD.Token.spacing.medium
    backgroundColor: MD.Token.color.surface_container_low
    radius: MD.Token.shape.corner.large

    ColumnLayout {
        anchors.fill: parent
        spacing: MD.Token.spacing.medium

        MD.Label {
            text: Messages.advisorTitle
            typescale: MD.Token.typescale.title_small
        }

        MD.Label {
            Layout.fillWidth: true
            text: Messages.advisorHint
            typescale: MD.Token.typescale.body_small
            color: MD.MProp.color.on_surface_variant
            wrapMode: Text.WordWrap
        }

        ListView {
            id: list
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: MD.Token.spacing.medium
            clip: true
            model: Core.advisor.pickRecommendations

            delegate: PickRecommendationChip {
                width: list.width
            }
        }

        MD.Label {
            Layout.fillWidth: true
            visible: list.count === 0
            text: Messages.advisorEmpty
            typescale: MD.Token.typescale.body_medium
            color: MD.MProp.color.on_surface_variant
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
