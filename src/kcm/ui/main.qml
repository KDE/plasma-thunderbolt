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
import org.kde.kcmutils as KCMUtils
import org.kde.kirigami as Kirigami

KCMUtils.ScrollViewKCM {
    id: root

    KCMUtils.ConfigModule.buttons: KCMUtils.ConfigModule.NoAdditionalButton

    sidebarMode: true

    implicitWidth: Kirigami.Units.gridUnit * 20
    implicitHeight: Kirigami.Units.gridUnit * 20

    actions: Kirigami.Action {
        id: toggleAuthModeAction

        text: i18n("Enabled")
        checkable: true
        checked: boltManager.authMode === Bolt.Bolt.AuthMode.Enabled
        displayComponent: QQC2.Switch {
            action: toggleAuthModeAction
        }

        onToggled: source => {
            boltManager.authMode = checked
                ? Bolt.Bolt.AuthMode.Enabled
                : Bolt.Bolt.AuthMode.Disabled
        }
    }

    Bolt.DeviceModel {
        id: deviceModel

        manager: Bolt.Manager {
            id: boltManager
        }

        showHosts: false
    }

    readonly property var placeholderMessage: {
        const icon = "preferences-desktop-thunderbolt";
        if (boltManager.isAvailable) {
            if ([Bolt.Bolt.Security.DPOnly, Bolt.Bolt.Security.USBOnly].includes(boltManager.securityLevel)) {
                return {
                    visible: true,
                    icon,
                    text: i18n("Thunderbolt support has been disabled in BIOS"),
                    explanation: i18n("Follow your system manufacturer's guide to enable Thunderbolt support"),
                };
            } else if (view.count === 0) {
                return {
                    visible: true,
                    icon,
                    text: i18n("No Thunderbolt devices connected"),
                    explanation: i18n("Plug in a Thunderbolt device"),
                };
            } else {
                return {
                    visible: false,
                    icon: "",
                    text: "",
                    explanation: "",
                }
            }
        } else {
            return {
                visible: true,
                icon: "preferences-desktop-thunderbolt",
                text: i18n("Thunderbolt subsystem is disabled or unavailable"),
                explanation: i18n("If the device supports Thunderbolt, try plugging in a Thunderbolt device"),
            }
        }
    }

    Kirigami.PlaceholderMessage {
        anchors.centerIn: parent
        width: parent.width - (Kirigami.Units.largeSpacing * 4)

        visible: root.placeholderMessage.visible
        icon.name: root.placeholderMessage.icon
        text: root.placeholderMessage.text
        explanation: root.placeholderMessage.explanation
    }

    function openDeviceView(device: Bolt.Device): void {
        KCMUtils.ConfigModule.pop();
        KCMUtils.ConfigModule.push("DeviceView.qml", { manager: boltManager, device });
    }

    view: DeviceList {
        id: view

        deviceModel: deviceModel
        enabled: boltManager.authMode === Bolt.Bolt.AuthMode.Enabled

        onDeviceClicked: device => root.openDeviceView(device)
    }
}
