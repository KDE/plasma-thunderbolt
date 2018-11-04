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

#include "dbushelper.h"

#include <QDBusConnection>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>
#include <QDBusError>

namespace {

bool isFakeEnv()
{
    return qEnvironmentVariableIsSet("KBOLT_FAKE");
}

} // namespace

QDBusConnection DBusHelper::connection()
{
    if (isFakeEnv()) {
        return QDBusConnection::sessionBus();
    } else {
        return QDBusConnection::systemBus();
    }
}

QString DBusHelper::serviceName()
{
    if (isFakeEnv()) {
        return QStringLiteral("org.kde.fakebolt");
    } else {
        return QStringLiteral("org.freedesktop.bolt");
    }
}

void DBusHelper::handleCall(QDBusPendingCall call, CallOkCallback &&okCb,
                            CallErrorCallback &&errCb, QObject *parent)
{
    auto watcher = new QDBusPendingCallWatcher(call);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished,
                     parent, [okCb = std::move(okCb), errCb = std::move(errCb)]
                         (QDBusPendingCallWatcher *watcher) {
        watcher->deleteLater();
        const QDBusPendingReply<void> reply(*watcher);
        if (reply.isError()) {
            if (errCb) {
                errCb(reply.error().message());
            }
        } else if (okCb) {
            okCb();
        }
    });
}
