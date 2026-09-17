import QtQuick
import QtQuick.Layouts
import QtQuick.Window

import GemSight.Core 1.0
import Qcm.Material as MD

import "../theme"
import "../i18n"
import "../components"
import "../draft"
import "../settings"

MD.ApplicationWindow {
    id: root

    visible: true
    width: 1320
    height: 860
    minimumWidth: 1040
    minimumHeight: 680
    title: Messages.appTitle
    color: MD.Token.color.surface_container
    flags: customTitleBar ? (Qt.Window | Qt.FramelessWindowHint) : Qt.Window

    readonly property bool customTitleBar: Qt.platform.os === "windows"

    MD.MProp.textColor: MD.MProp.color.on_surface
    MD.MProp.backgroundColor: MD.MProp.color.surface_container

    property int windowClass: MD.Token.window_class.select_type(width)
    MD.MProp.size.windowClass: windowClass

    property int pageIndex: 0

    readonly property var navModel: [
        { name: Messages.draftTitle, icon: MD.Token.icon.sports_esports }
    ]

    Timer {
        interval: 200
        running: true
        repeat: false
        onTriggered: root.windowClass = MD.Token.window_class.select_type(root.width)
    }

    Component.onCompleted: {
        Appearance.apply()
        Core.draft.startGsi()
    }

    SettingsSheet {
        id: settingsSheet
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        AppTitleBar {
            visible: root.customTitleBar
            Layout.fillWidth: true
            window: root
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            AppRail {
                id: navRail
                Layout.fillHeight: true
                model: root.navModel
                currentIndex: root.pageIndex
                onActivated: function (index) { root.pageIndex = index }
                onSettingsRequested: settingsSheet.openSettings()
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.topMargin: MD.Token.spacing.small
                Layout.rightMargin: MD.Token.spacing.small
                Layout.bottomMargin: MD.Token.spacing.small
                spacing: MD.Token.spacing.small

                MD.Pane {
                    id: mainPane
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    padding: 0
                    radius: MD.Token.shape.corner.extra_large
                    corners: MD.Util.corners(radius)
                    backgroundColor: MD.Token.color.surface

                    Item {
                        id: mainPaneClip
                        anchors.fill: parent
                        clip: true

                        layer.enabled: true
                        layer.effect: MD.RoundClip {
                            corners: mainPane.corners
                            size: Qt.vector2d(mainPaneClip.width, mainPaneClip.height)
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: MD.Token.spacing.large
                            spacing: MD.Token.spacing.medium

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: MD.Token.spacing.medium

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: MD.Token.spacing.extra_small

                                    MD.Label {
                                        text: Messages.draftTitle
                                        typescale: MD.Token.typescale.headline_small
                                    }

                                    MD.Label {
                                        Layout.fillWidth: true
                                        text: Core.draft.statusLine
                                        typescale: MD.Token.typescale.body_medium
                                        color: MD.MProp.color.on_surface_variant
                                        elide: Text.ElideRight
                                    }
                                }

                                MD.AssistChip {
                                    text: Core.session.scoutPolicy
                                    MD.MProp.backgroundColor: MD.MProp.color.secondary_container
                                }

                                MD.Button {
                                    text: Messages.demoTurbo
                                    mdState.type: MD.Enum.BtFilled
                                    onClicked: Core.draft.simulateTurboDraft()
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                spacing: MD.Token.spacing.medium

                                DraftScreen {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.preferredWidth: 0
                                }

                                AdvisorPanel {
                                    Layout.preferredWidth: 300
                                    Layout.fillHeight: true
                                }
                            }
                        }
                    }
                }

                AppStatusBar {
                    Layout.fillWidth: true
                }
            }
        }
    }

    WindowResizeEdges {
        visible: root.customTitleBar
        anchors.fill: parent
        window: root
    }
}
