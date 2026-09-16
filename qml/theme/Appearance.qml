pragma Singleton

import QtQuick
import Qcm.Material as MD

QtObject {
    function apply() {
        MD.Token.color.useSysColorSM = false
        MD.Token.color.useSysAccentColor = false
        MD.Token.themeMode = MD.Enum.Dark
        MD.Token.color.accentColor = "#7C9CFF"
        Qt.styleHints.colorScheme = Qt.Dark
    }

    Component.onCompleted: apply()
}
