import QtQuick
import QtQuick.Layouts

import Qcm.Material as MD

RowLayout {
    id: root

    property double winRate: 0.5

    MD.Label {
        text: qsTr("WR")
        typescale: MD.Token.typescale.label_small
        Layout.preferredWidth: 28
    }

    MD.LinearIndicator {
        Layout.fillWidth: true
        indeterminate: false
        from: 0
        to: 1
        value: Math.max(0, Math.min(1, root.winRate))
    }

    MD.Label {
        text: Math.round(root.winRate * 1000) / 10 + "%"
        typescale: MD.Token.typescale.label_small
        Layout.preferredWidth: 48
        horizontalAlignment: Text.AlignRight
    }
}
