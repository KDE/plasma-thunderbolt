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
                        : Bolt.Bolt.AuthMode.Disabled
                }

                Component.onCompleted: checked = (deviceModel.manager.authMode == Bolt.Bolt.AuthMode.Enabled)
            }
        }


        ListView {
            id: view
            model: deviceModel

            enabled: enableBox.checked

            Layout.fillWidth: true
            Layout.fillHeight: true

            property int _evalTrigger: 0

            Timer {
                interval: 2000
                running: view.visible
                repeat: true
                onTriggered: view._evalTrigger++;
            }

            delegate: Kirigami.AbstractListItem {
                id: item
                width: view.width

                RowLayout {
                    id: layout
                    spacing: Kirigami.Units.smallSpacing * 2
                    property bool indicateActiveFocus: item.pressed || Kirigami.Settings.tabletMode || item.activeFocus || (item.ListView.view ? item.ListView.view.activeFocus : false)

                    BusyIndicator {
                        id: busyIndicator
                        visible: model.device.status == Bolt.Bolt.Status.Authorizing
                        running: visible
                        Layout.minimumHeight: Kirigami.Units.iconSizes.smallMedium
                        Layout.maximumHeight: Layout.minimumHeight
                        Layout.minimumWidth: height
                    }

                    Label {
                        id: label
                        text: model.device.label
                        Layout.fillWidth: true
                        Layout.topMargin: Kirigami.Units.smallSpacing
                        Layout.bottomMargin: Kirigami.Units.smallSpacing
                        color: parent.indicateActiveFocus && (item.highlighted || item.checked || item.pressed) ? item.activeTextColor : item.textColor
                        elide: Text.ElideRight
                        font: item.font
                    }

                    Label {
                        id: statusLabel

                        Layout.alignment: Qt.AlignRight

                        text: view._evalTrigger, Utils.deviceStatus(model.device, true) 
                    }
                }

                onClicked: {
                    page.clicked(model.device)
                }
            }
        }
    }
}
