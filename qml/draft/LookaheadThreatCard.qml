import QtQuick
import QtQuick.Layouts

import GemSight.Core 1.0
import Qcm.Material as MD

import "../components"
import "../i18n"

MD.Card {
    id: root

    type: MD.Enum.CardOutlined
    MD.MProp.elevation: MD.MProp.elevation.level0

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: MD.Token.spacing.medium
        spacing: MD.Token.spacing.small

        MD.Label {
            text: Messages.threatTitle
            typescale: MD.Token.typescale.title_small
        }

        ListView {
            id: threatList
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: MD.Token.spacing.small
            clip: true
            model: Core.draft.evaluator.lookaheadThreats

            delegate: RowLayout {
                required property int heroId
                required property string heroName
                required property string heroPortraitUrl
                required property string reason
                required property double threatScore

                width: threatList.width
                spacing: MD.Token.spacing.small

                HeroPortrait {
                    Layout.preferredWidth: 72
                    Layout.preferredHeight: 40
                    source: heroPortraitUrl
                    fallbackText: heroName.charAt(0)
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2

                    MD.Label {
                        Layout.fillWidth: true
                        text: heroName
                        typescale: MD.Token.typescale.label_large
                        elide: Text.ElideRight
                    }

                    MD.Label {
                        Layout.fillWidth: true
                        text: reason
                        typescale: MD.Token.typescale.body_small
                        color: MD.MProp.color.on_surface_variant
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                    }
                }

                MD.Label {
                    text: Math.round(threatScore * 100) + "%"
                    typescale: MD.Token.typescale.label_large
                    color: MD.MProp.color.error
                }
            }
        }

        MD.Label {
            Layout.fillWidth: true
            visible: threatList.count === 0
            text: Messages.threatPlaceholder
            typescale: MD.Token.typescale.body_small
            color: MD.MProp.color.on_surface_variant
            wrapMode: Text.WordWrap
        }
    }
}
