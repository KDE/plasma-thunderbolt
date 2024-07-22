/*
 * SPDX-FileCopyrightText: 2018-2019 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts

import org.kde.bolt as Bolt
import org.kde.kirigami as Kirigami

import "utils.js" as Utils

Kirigami.ScrollablePage {
    id: page

    property Bolt.Manager manager
    property Bolt.Device device

    property int evalTrigger: 0

    Timer {
        interval: 2000
        running: page.device !== null
        repeat: true
        onTriggered: page.evalTrigger++;
    }

    header: Kirigami.InlineMessage {
        id: errorMessage

        position: Kirigami.InlineMessage.Position.Header
        type: Kirigami.MessageType.Error
        showCloseButton: true

        function show(msg: string) {
            text = msg;
            visible = true;
        }
    }

    ColumnLayout {
        spacing: Kirigami.Units.smallSpacing * 5

        RowLayout {
            QQC2.Button {
                icon.name: "draw-arrow-back"
                visible: !pageRow.wideMode
                onClicked: pageRow.pop()
            }

            Kirigami.Heading {
                level: 2
                text: page.evalTrigger, (page.device?.name ?? "")
            }
        }

        Kirigami.FormLayout {
            QQC2.Label {
                text: page.evalTrigger, (page.device?.vendor ?? "")
                Kirigami.FormData.label: i18n("Vendor:")
            }
            QQC2.Label {
                text: page.evalTrigger, (page.device?.uid ?? "")
                Kirigami.FormData.label: i18n("UID:")
            }
            QQC2.Label {
                text: page.evalTrigger, page.device !== null ? Utils.deviceStatus(page.device, false).text : ""
                Kirigami.FormData.label: i18n("Status:")
            }
            QQC2.Label {
                visible: page.device !== null && page.device.status === Bolt.Bolt.Status.Authorized
                text: page.evalTrigger, page.device !== null ? Qt.formatDateTime(page.device.authorizeTime) : ""
                Kirigami.FormData.label: i18n("Authorized at:")
            }
            QQC2.Label {
                visible: page.device !== null && page.device.status === Bolt.Bolt.Status.Connected
                text: page.evalTrigger, page.device !== null ? Qt.formatDateTime(page.device.connectTime) : ""
                Kirigami.FormData.label: i18n("Connected at:")
            }
            QQC2.Label {
                visible: page.device !== null && page.device.status === Bolt.Bolt.Status.Disconnected
                text: page.evalTrigger, page.device !== null ? Qt.formatDateTime(page.device.storeTime) : ""
                Kirigami.FormData.label: i18n("Enrolled at:")
            }
            QQC2.Label {
                visible: page.device !== null && (page.device.status === Bolt.Bolt.Status.Authorized || page.device.status === Bolt.Bolt.Status.Disconnected)
                text: page.evalTrigger, page.device !== null && page.device.stored ? i18n("Yes") : i18n("No")
                Kirigami.FormData.label: i18n("Trusted:")
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter

            QQC2.Button {
                id: authorizeBtn
                text: page.device !== null && page.device.status === Bolt.Bolt.Status.Authorizing ? i18n("Authorizing...") : i18n("Authorize")
                enabled: page.device !== null && page.device.status !== Bolt.Bolt.Status.Authorizing
                visible: page.device !== null && (page.device.status === Bolt.Bolt.Status.Connected || page.device.status === Bolt.Bolt.Status.AuthError)
                onClicked: {
                    Bolt.QMLHelper.enrollDevice(
                        page.manager, page.device.uid, Bolt.Bolt.Policy.Default,
                        Bolt.Bolt.Auth.Boot | Bolt.Bolt.Auth.NoKey,
                        () => {
                            console.log("Thunderbolt device " + page.device.uid + " (" + page.device.name + ") enrolled successfully");
                        },
                        error => {
                            errorMessage.show(i18n("Failed to enroll device <b>%1</b>: %2", page.device.name, error));
                        }
                    );
                }
            }
            QQC2.Button {
                id: storeBtn
                text: i18n("Trust this Device")
                visible: page.device !== null && page.device.status === Bolt.Bolt.Status.Authorized && !page.device.stored
                onClicked: {
                    enabled = false;
                    Bolt.QMLHelper.enrollDevice(
                        page.manager, page.device.uid, Bolt.Bolt.Policy.Default,
                        Bolt.Bolt.Auth.Boot | Bolt.Bolt.Auth.NoKey,
                        () => {
                            enabled = true;
                            console.log("Thunderbolt Device " + page.device.uid + " (" + page.device.name + ") enrolled successfully");
                        },
                        error => {
                            enabled = true;
                            errorMessage.show(i18n("Failed to enroll device <b>%1</b>: %2", page.device.name, error));
                        }
                    );
                }
            }

            QQC2.Button {
                id: forgetBtn
                text: i18n("Revoke Trust")
                visible: page.device !== null && page.device.stored
                onClicked: {
                    enabled = false
                    Bolt.QMLHelper.forgetDevice(
                        page.manager, page.device.uid,
                        () => {
                            enabled = true;
                            console.log("Device " + page.device.uid + " successfully forgotten.");
                        },
                        error => {
                            enabled = true;
                            errorMessage.show(i18n("Error changing device trust: <b>%1</b>: %2", page.device.name, error));
                        }
                    );
                    // If the device is not connected it will cease to exist
                    // once forgotten, so we should pop this view
                    if (page.device.status === Bolt.Bolt.Status.Disconnected) {
                        pageRow.pop();
                    }
                }
            }
        }

        QQC2.Label {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true

            text: page.device !== null && !page.device.stored
                ? i18n("Hint: trusted device will be automatically authorized the next time it is connected to the computer.")
                : i18n("Hint: an untrusted device needs to be manually authorized each time it is connected to the computer.")
            visible: storeBtn.visible || forgetBtn.visible
            wrapMode: Text.WordWrap
            horizontalAlignment: Qt.AlignHCenter
        }
    }
}
