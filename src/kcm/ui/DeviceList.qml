/*
 * SPDX-FileCopyrightText: 2018-2019 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts

import org.kde.bolt as Bolt
import org.kde.kirigami as Kirigami

import "utils.js" as Utils

ListView {
    id: view

    required property Bolt.DeviceModel deviceModel

    signal deviceClicked(Bolt.Device device)

    property int evalTrigger: 0

    Timer {
        interval: 2000
        running: view.count > 0
        repeat: true
        onTriggered: {
            view.evalTrigger++;
        }
    }

    model: deviceModel

    delegate: QQC2.ItemDelegate {
        id: delegate

        required property int index
        required property Bolt.Device device

        readonly property var deviceStatus: Utils.deviceStatus(device, true)

        width: ListView.view.width - ListView.view.leftMargin - ListView.view.rightMargin

        contentItem: RowLayout {
            spacing: Kirigami.Units.smallSpacing

            QQC2.BusyIndicator {
                visible: delegate.device.status === Bolt.Bolt.Status.Authorizing
                running: visible
                implicitWidth: Kirigami.Units.iconSizes.smallMedium
                implicitHeight: Kirigami.Units.iconSizes.smallMedium
            }

            QQC2.Label {
                Layout.fillWidth: true
                text: delegate.device.label
                elide: Text.ElideRight
                color: delegate.highlighted ? Kirigami.Theme.highlightedTextColor : Kirigami.Theme.textColor
            }

            QQC2.Label {
                text: view.evalTrigger, delegate.deviceStatus.text
                color: delegate.highlighted ? Kirigami.Theme.highlightedTextColor : Kirigami.Theme[delegate.deviceStatus.color]
            }
        }

        onClicked: {
            view.deviceClicked(device)
        }
    }
}
