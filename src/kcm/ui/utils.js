/*
 * SPDX-FileCopyrightText: 2018-2019 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2024 ivan tkachenko <me@ratijas.tk>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

.import org.kde.kirigami as Kirigami
.import org.kde.bolt as Bolt

function deviceStatus(device /*Bolt.Device*/, withStored /*bool*/) {
    const { authFlags, status, stored } = device;
    let text = "";
    let color = "textColor";

    switch (status) {
    case Bolt.Bolt.Status.Disconnected:
        text = i18n("Disconnected");
        break;
    case Bolt.Bolt.Status.Connecting:
        text = i18n("Connecting");
        break;
    case Bolt.Bolt.Status.Connected:
        text = i18n("Connected");
        color = "neutralTextColor";
        break;
    case Bolt.Bolt.Status.AuthError:
        text = i18n("Authorization Error");
        break;
    case Bolt.Bolt.Status.Authorizing:
        text = i18n("Authorizing");
        break;
    case Bolt.Bolt.Status.Authorized:
        color = "positiveTextColor";
        if (authFlags & Bolt.Bolt.Auth.NoPCIE) {
            text = i18n("Reduced Functionality");
        } else {
            text = i18n("Connected & Authorized");
        }
        break;
    }
    if (withStored && stored) {
        if (text !== "") {
            text += ", ";
        }
        text += i18n("Trusted");
    }

    return { text, color };
}
