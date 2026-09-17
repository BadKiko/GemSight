pragma Singleton

import QtQuick
import QtCore
import Qcm.Material as MD

Item {
    id: root

    readonly property int defaultThemeMode: MD.Enum.Dark
    readonly property int defaultPaletteType: MD.Enum.PaletteMonochrome
    readonly property string defaultAccentColor: "#7C9CFF"

    Settings {
        id: store
        category: "appearance"
        property int themeMode: root.defaultThemeMode
        property int paletteType: root.defaultPaletteType
        property string accentColor: root.defaultAccentColor
    }

    function apply() {
        MD.Token.color.useSysColorSM = false
        MD.Token.color.useSysAccentColor = false
        MD.Token.themeMode = store.themeMode
        MD.Token.color.paletteType = store.paletteType
        MD.Token.color.accentColor = store.accentColor
        Qt.styleHints.colorScheme = store.themeMode === MD.Enum.Dark ? Qt.Dark : Qt.Light
    }

    Component.onCompleted: apply()
}
