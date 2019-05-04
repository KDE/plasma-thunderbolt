/*
 * Copyright (c) 2018 - 2019  Daniel Vrátil <dvratil@kde.org>
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

#ifndef BOLT_ENUM_H_
#define BOLT_ENUM_H_

#include "kbolt_export.h"

#include <QMetaType>
#include <QMetaObject>
#include <QString>

namespace Bolt
{
// NOTE: Keep this split over two lines otherwise MOC may fail to see
// the Q_NAMESPACE macro if KBOLT_EXPORT is not expanded correctly.
KBOLT_EXPORT
Q_NAMESPACE

enum class Status {
    Unknown = -1,
    Disconnected,
    Connecting,
    Connected,
    Authorizing,
    AuthError,
    Authorized
};

Q_ENUM_NS(Status)

Status statusFromString(const QString &str);

enum class Auth {
    None = 0,
    NoPCIE = 1 << 0,
    Secure = 1 << 1,
    NoKey  = 1 << 2,
    Boot   = 1 << 3
};
Q_ENUM_NS(Auth)
Q_DECLARE_FLAGS(AuthFlags, Auth)

AuthFlags authFlagsFromString(const QString &str);
QString authFlagsToString(AuthFlags flags);

enum class KeyState {
    Unknown = -1,
    Missing,
    Have,
    New
};
Q_ENUM_NS(KeyState)

KeyState keyStateFromString(const QString &str);

enum class Policy {
    Unknown = -1,
    Default,
    Manual,
    Auto
};
Q_ENUM_NS(Policy)

Policy policyFromString(const QString &str);
QString policyToString(Policy policy);

enum class Type {
    Unknown = -1,
    Host,
    Peripheral
};
Q_ENUM_NS(Type)

Type typeFromString(const QString &str);


enum class AuthMode {
    Disabled = 0,
    Enabled
};
Q_ENUM_NS(AuthMode)

AuthMode authModeFromString(const QString &str);
QString authModeToString(AuthMode authMode);

enum class Security {
    Unknown = -1,
    None,
    DPOnly,
    User = '1',
    Secure = '2',
    USBOnly = 4
};
Q_ENUM_NS(Security)

Security securityFromString(const QString &str);

} // namespace

Q_DECLARE_METATYPE(Bolt::Status)
Q_DECLARE_METATYPE(Bolt::AuthFlags)
Q_DECLARE_METATYPE(Bolt::KeyState)
Q_DECLARE_METATYPE(Bolt::Policy)
Q_DECLARE_METATYPE(Bolt::Type)
Q_DECLARE_METATYPE(Bolt::AuthMode)
Q_DECLARE_METATYPE(Bolt::Security)
Q_DECLARE_OPERATORS_FOR_FLAGS(Bolt::AuthFlags)


#endif
