/*
 * SPDX-FileCopyrightText: 2018-2019 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2024 ivan tkachenko <me@ratijas.tk>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts

import org.kde.bolt as Bolt
import org.kde.kirigami as Kirigami
import org.kde.kirigami.delegates as KD
import org.kde.kirigami.platform as Platform

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

        text: device.label

        contentItem: RowLayout {
            spacing: Kirigami.Units.smallSpacing

            QQC2.BusyIndicator {
                running: delegate.device.status === Bolt.Bolt.Status.Authorizing
                implicitWidth: Kirigami.Units.iconSizes.smallMedium
                implicitHeight: Kirigami.Units.iconSizes.smallMedium
            }

            KD.TitleSubtitle {
                Layout.fillWidth: true
                title: delegate.text
                subtitle: view.evalTrigger, delegate.deviceStatus.text
                font: delegate.font
                selected: delegate.highlighted || delegate.down
                subtitleColor: selected
                    ? Platform.Theme.highlightedTextColor
                    : Platform.ColorUtils.linearInterpolation(Kirigami.Theme[delegate.deviceStatus.color], Platform.Theme.backgroundColor, 0.3)
                wrapMode: Text.Wrap
            }
        }

        onClicked: {
            view.deviceClicked(device)
        }
    }
}
