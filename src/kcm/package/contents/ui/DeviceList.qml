/*
 * SPDX-FileCopyrightText: 2018-2019 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.3

import org.kde.kirigami 2.12 as Kirigami
import org.kde.kquickcontrolsaddons 2.0 as KQCAddons
import org.kde.kcm 1.2 as KCM
import org.kde.bolt 0.1 as Bolt
import "utils.js" as Utils

KCM.ScrollViewKCM {
    id: page

    property Bolt.DeviceModel deviceModel: null

    signal itemClicked(Bolt.Device device)

    header: RowLayout {
        CheckBox {
            id: enableBox
            text: i18n("Enable Thunderbolt devices")

            checked: deviceModel.manager.authMode == Bolt.Bolt.AuthMode.Enabled

            onToggled: {
                deviceModel.manager.authMode = enableBox.checked
                    ? Bolt.Bolt.AuthMode.Enabled
                    : Bolt.Bolt.AuthMode.Disabled
            }
        }
    }

    view: ListView {
        id: view
        model: deviceModel
        enabled: enableBox.checked

        property int _evalTrigger: 0

        Timer {
            interval: 2000
            running: view.visible
            repeat: true
            onTriggered: view._evalTrigger++;
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Kirigami.Units.largeSpacing

            visible: view.count === 0

            text: i18n("No Thunderbolt devices connected")
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

                    property var _deviceStatus: Utils.deviceStatus(model.device, true)

                    text: view._evalTrigger, _deviceStatus.text
                    color: _deviceStatus.color
                }
            }

            onClicked: {
                page.itemClicked(model.device)
            }
        }
    }
}
