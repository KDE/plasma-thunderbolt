/*
    Copyright (c) 2018 Daniel Vrátil <dvratil@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#ifndef BOLT_ENUM_H_
#define BOLT_ENUM_H_

#include <QMetaType>
#include <QString>

namespace Bolt
{

enum class Status {
    Unknown = -1,
    Disconnected,
    Connecting,
    Connected,
    Authorizing,
    AuthError,
    Authorized
};

Status statusFromString(const QString &str);

enum class Auth {
    None = 0,
    NoPCIE = 1 << 0,
    Secure = 1 << 1,
    NoKey  = 1 << 2,
    Boot   = 1 << 3
};
Q_DECLARE_FLAGS(AuthFlags, Auth)

AuthFlags authFlagsFromString(const QString &str);

enum class KeyState {
    Unknown = -1,
    Missing,
    Have,
    New
};

KeyState keyStateFromString(const QString &str);

enum class Policy {
    Unknown = -1,
    Default,
    Manual,
    Auto
};

Policy policyFromString(const QString &str);

enum class Type {
    Unknown = -1,
    Host,
    Peripheral
};

Type typeFromString(const QString &str);


enum class AuthMode {
    Disabled = 0,
    Enabled
};

AuthMode authModeFromString(const QString &str);

enum class Security {
    Unknown = -1,
    None,
    DPOnly,
    User = '1',
    Secure = '2',
    USBOnly = 4
};

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
