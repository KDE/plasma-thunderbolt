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
import org.kde.kcmutils
import org.kde.bolt 0.1 as Bolt
import "utils.js" as Utils

Kirigami.Page {
    ConfigModule.buttons: ConfigModule.NoAdditionalButton
    id: root

    title: kcm.name
    implicitWidth: Kirigami.Units.gridUnit * 20
    implicitHeight: pageRow.contentHeight > 0 ? Math.min(pageRow.contentHeight, Kirigami.Units.gridUnit * 20)
                                              : Kirigami.Units.gridUnit * 20

    Bolt.Manager {
        id: boltManager
    }

    Kirigami.PageRow {
        id: pageRow
        clip: true
        anchors.fill: parent

        Component.onCompleted: {
            if (boltManager.isAvailable) {
                if (boltManager.securityLevel == Bolt.Bolt.Security.DPOnly
                        || boltManager.securityLevel == Bolt.Bolt.Security.USBOnly) {
                    pageRow.push(noBoltPage, { text: i18n("Thunderbolt support has been disabled in BIOS") })
                } else {
                    pageRow.push(deviceList, { manager: boltManager })
                }
            } else {
                pageRow.push(noBoltPage, { text: i18n("Thunderbolt subsystem is not available") })
            }
        }
    }

    Component {
        id: noBoltPage
        Kirigami.Page {
            property alias text: label.text
            Kirigami.PlaceholderMessage {
                id: label

                anchors.centerIn: parent
                width: parent.width - (Kirigami.Units.largeSpacing *4)
            }
        }
    }

    Component {
        id: deviceList
        DeviceList {
            property alias manager: model.manager
            deviceModel: Bolt.DeviceModel {
                id: model
                showHosts: false
            }

            onItemClicked: function(device) {
                pageRow.push(deviceView, { manager: manager, device: device })
            }
        }
    }

    Component {
        id: deviceView
        DeviceView {
        }
    }
}
