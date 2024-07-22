/*
 * SPDX-FileCopyrightText: 2018-2019 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

import QtQuick
import QtQuick.Layouts

import org.kde.bolt as Bolt
import org.kde.kcmutils as KCMUtils
import org.kde.kirigami as Kirigami

Kirigami.Page {
    id: root

    KCMUtils.ConfigModule.buttons: KCMUtils.ConfigModule.NoAdditionalButton

    title: kcm.name

    implicitWidth: Kirigami.Units.gridUnit * 20
    implicitHeight: pageRow.contentHeight > 0
        ? Math.min(pageRow.contentHeight, Kirigami.Units.gridUnit * 20)
        : Kirigami.Units.gridUnit * 20

    Bolt.DeviceModel {
        id: deviceModel

        manager: Bolt.Manager {
            id: boltManager

            function isDPOnlyOrUSBOnly(): bool {
                return [Bolt.Bolt.Security.DPOnly, Bolt.Bolt.Security.USBOnly].includes(securityLevel);
            }
        }

        showHosts: false
    }

    Kirigami.PlaceholderMessage {
        visible: boltManager.isAvailable && boltManager.isDPOnlyOrUSBOnly()
        anchors.centerIn: parent
        width: parent.width - (Kirigami.Units.largeSpacing * 4)
        icon.name: "preferences-desktop-thunderbolt"
        text: i18n("Thunderbolt support has been disabled in BIOS")
        explanation: i18n("Follow your system manufacturer's guide to enable Thunderbolt support")
    }

    Kirigami.PlaceholderMessage {
        visible: !boltManager.isAvailable
        anchors.centerIn: parent
        width: parent.width - (Kirigami.Units.largeSpacing * 4)
        icon.name: "preferences-desktop-thunderbolt"
        text: i18n("Thunderbolt subsystem is disabled or unavailable")
        explanation: i18n("If the device supports Thunderbolt, try plugging in a Thunderbolt device")
    }

    Kirigami.PageRow {
        id: pageRow
        clip: true
        anchors.fill: parent

        Component.onCompleted: {
            if (boltManager.isAvailable && !boltManager.isDPOnlyOrUSBOnly()) {
                pageRow.push(deviceList, { deviceModel })
            }
        }
    }

    Component {
        id: deviceList
        DeviceList {
            onItemClicked: device => {
                pageRow.push(deviceView, { device })
            }
        }
    }

    Component {
        id: deviceView
        DeviceView {
            manager: boltManager
        }
    }
}
