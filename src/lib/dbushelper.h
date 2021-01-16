/*
 * Copyright (c) 2018 - 2019 Daniel Vrátil <dvratil@kde.org>
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

#ifndef DBUSHELPER_H_
#define DBUSHELPER_H_

#include <QDBusAbstractInterface>
#include <QDBusConnection>

namespace KBolt
{
class Device;
}

namespace DBusHelper
{
QDBusConnection connection();
QString serviceName();

using CallErrorCallback = std::function<void(const QString &)>;
using CallOkCallback = std::function<void()>;
void handleCall(QDBusPendingCall call, CallOkCallback &&okCb, CallErrorCallback &&errCb, QObject *parent);

template<typename... V>
void call(QDBusAbstractInterface *iface, const QString &method, const V &... args, CallOkCallback &&okCb, CallErrorCallback &&errCb, QObject *parent = nullptr)
{
    handleCall(iface->asyncCall(method, args...), std::move(okCb), std::move(errCb), parent);
}

} // namespace

#endif
