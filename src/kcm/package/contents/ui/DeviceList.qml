import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.3

import org.kde.kirigami 2.4 as Kirigami
import org.kde.kquickcontrolsaddons 2.0 as KQCAddons
import org.kde.kcm 1.2 as KCM
import org.kde.bolt 0.1 as Bolt
import "utils.js" as Utils

Kirigami.Page {
    id: page

    property Bolt.DeviceModel deviceModel: null

    signal clicked(Bolt.Device device)

    flickable: view

    ColumnLayout {
        anchors.fill: parent
        spacing: Kirigami.Units.largeSpacing

        RowLayout {
            CheckBox {
                id: enableBox
                text: i18n("Allow Thunderbolt devices")

                onCheckedChanged: function() {
                    deviceModel.manager.authMode = enableBox.checked
                        ? Bolt.Bolt.AuthMode.Enabled
                        : Bolt.Bolt.AuthMode.Disabled;
                }

                Component.onCompleted: checked = (deviceModel.manager.authMode == Bolt.Bolt.AuthMode.Enabled)
            }
        }


        ListView {
            id: view
            model: deviceModel

            Layout.fillWidth: true
            Layout.fillHeight: true

            delegate: Kirigami.BasicListItem {
                width: view.width
                icon: {
                    if (model.device.status == Bolt.Bolt.Status.Authorized) {
                        return "emblem-checked";
                    } else if (model.device.status == Bolt.Bolt.Status.Connected) {
                        return "emblem-remove";
                    } else {
                       return "emblem-unavailable";
                   }
                }
                label: model.device.label

                Label {
                    id: statusLabel

                    Layout.alignment: Qt.AlignRight

                    text: Utils.deviceStatus(model.device)
                }

                onClicked: {
                    page.clicked(model.device)
                }
            }
        }
    }
}
