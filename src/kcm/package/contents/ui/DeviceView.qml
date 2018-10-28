/*
 * Copyright (c) 2018  Daniel Vrátil <dvratil@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.3

import org.kde.kirigami 2.4 as Kirigami
import org.kde.kquickcontrolsaddons 2.0 as KQCAddons
import org.kde.bolt 0.1 as Bolt
import "utils.js" as Utils

Kirigami.Page {
    id: page

    property Bolt.Manager manager: null
    property Bolt.Device device: null

    ColumnLayout {
        spacing: Kirigami.Units.smallSpacing * 5

        RowLayout {
            ToolButton {
                icon.name: "draw-arrow-back"
                visible: !pageRow.wideMode
                onClicked: pageRow.pop()
            }

            Kirigami.Heading {
                level: 2
                text: device ? device.name : ""
            }
        }

        Kirigami.FormLayout {
            Label {
                text: device ? device.vendor : ""
                Kirigami.FormData.label: i18n("Vendor:")
            }
            Label {
                text: device ? device.uid : ""
                Kirigami.FormData.label: i18n("UID:")
            }
            Label {
                text: device ? Utils.deviceStatus(device) : ""
                Kirigami.FormData.label: i18n("Status:")
            }
            Label {
                visible: device && device.status == Bolt.Bolt.Status.Authorized
                text: device ? Qt.formatDateTime(device.authorizeTime) : ""
                Kirigami.FormData.label: i18n("Authorized at:")
            }
            Label {
                visible: device && device.status == Bolt.Bolt.Status.Connected
                text: device ? Qt.formatDateTime(device.connectTime) : ""
                Kirigami.FormData.label: i18n("Connected at:")
            }
            Label {
                visible: device && device.status == Bolt.Bolt.Status.Disconnected
                text: device ? Qt.formatDateTime(device.storeTime) : ""
                Kirigami.FormData.label: i18n("Enrolled at:")
            }
        }

        RowLayout {
            Button {
                id: authorizeBtn
                text: i18n("Authorize")
                visible: device && device.status == Bolt.Bolt.Status.Connected
                onClicked: {
                    if (device.stored) {
                        device.authorize(Bolt.Bolt.Auth.None);
                    } else {
                        manager.enrollDevice(device.uid, Bolt.Bolt.Policy.Default, Bolt.Bolt.Auth.None);
                    }
                }
            }
            Button {
                id: forgetBtn
                text: i18n("Forget Device")
                visible: device && device.stored
                onClicked: {
                    manager.forgetDevice(device.uid)
                }
            }
        }
    }
}
