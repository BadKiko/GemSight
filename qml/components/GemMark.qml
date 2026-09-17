import QtQuick

import Qcm.Material as MD

Item {
    id: root

    property color fillColor: MD.Token.color.on_primary_container

    implicitWidth: 28
    implicitHeight: 28

    MD.Label {
        anchors.centerIn: parent
        text: "GS"
        typescale: MD.Token.typescale.title_small
        color: root.fillColor
    }
}
