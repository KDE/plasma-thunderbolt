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

#include "enum.h"
#include "libkbolt_debug.h"

#include <QStringList>
#include <QVector>
#include <QStringRef>

Bolt::Status Bolt::statusFromString(const QString &str)
{
    if (str == QLatin1Literal("unknown")) {
        return Bolt::Status::Unknown;
    } else if (str == QLatin1Literal("disconnected")) {
        return Bolt::Status::Disconnected;
    } else if (str == QLatin1Literal("connecting")) {
        return Bolt::Status::Connecting;
    } else if (str == QLatin1Literal("connected")) {
        return Bolt::Status::Connected;
    } else if (str == QLatin1Literal("authorizing")) {
        return Bolt::Status::Authorizing;
    } else if (str == QLatin1Literal("authorized")) {
        return Bolt::Status::Authorized;
    } else if (str == QLatin1Literal("auth-error")) {
        return Bolt::Status::AuthError;
    } else {
        qCCritical(log_libkbolt, "Unknown Status enum value '%s'", qUtf8Printable(str));
        Q_ASSERT(false);
        return Bolt::Status::Unknown;
    }
}

Bolt::AuthFlags Bolt::authFlagsFromString(const QString &str)
{
    const auto splitRef = str.splitRef(QLatin1Literal("|"));
    Bolt::AuthFlags outFlags = Bolt::Auth::None;
    for (const auto &flag : splitRef) {
        const auto f = flag.trimmed();
        if (f == QLatin1Literal("none")) {
            outFlags |= Bolt::Auth::None;
        } else if (f == QLatin1Literal("nopcie")) {
            outFlags |= Bolt::Auth::NoPCIE;
        } else if (f == QLatin1Literal("secure")) {
            outFlags |= Bolt::Auth::Secure;
        } else if (f == QLatin1Literal("nokey")) {
            outFlags |= Bolt::Auth::NoKey;
        } else if (f == QLatin1Literal("boot")) {
            outFlags |= Bolt::Auth::Boot;
        } else {
            qCCritical(log_libkbolt, "Unknown AuthFlags enum value '%s'", qUtf8Printable(str));
            Q_ASSERT(false);
            return Bolt::Auth::None;
        }
    }
    return outFlags;
}

QString Bolt::authFlagsToString(AuthFlags flags)
{
    QStringList str;
    if (flags == AuthFlags(Bolt::Auth::None)) {
        return QLatin1Literal("none");
    }
    if (flags & Bolt::Auth::NoPCIE) {
        str.push_back(QLatin1Literal("nopcie"));
    }
    if (flags & Bolt::Auth::Secure) {
        str.push_back(QLatin1Literal("secure"));
    }
    if (flags & Bolt::Auth::NoKey) {
        str.push_back(QLatin1Literal("nokey"));
    }
    if (flags & Bolt::Auth::Boot) {
        str.push_back(QLatin1Literal("boot"));
    }

    return str.join(QLatin1Literal(" | "));
}


Bolt::KeyState Bolt::keyStateFromString(const QString &str)
{
    if (str == QLatin1Literal("unknown")) {
        return Bolt::KeyState::Unknown;
    } else if (str == QLatin1Literal("missing")) {
        return Bolt::KeyState::Missing;
    } else if (str == QLatin1Literal("have")) {
        return Bolt::KeyState::Have;
    } else if (str == QLatin1Literal("new")) {
        return Bolt::KeyState::New;
    } else {
        qCCritical(log_libkbolt, "Unknown KeyState enum value '%s'", qUtf8Printable(str));
        Q_ASSERT(false);
        return Bolt::KeyState::Unknown;
    }
}

Bolt::Policy Bolt::policyFromString(const QString &str)
{
    if (str == QLatin1Literal("unknown")) {
        return Bolt::Policy::Unknown;
    } else if (str == QLatin1Literal("default")) {
        return Bolt::Policy::Default;
    } else if (str == QLatin1Literal("manual")) {
        return Bolt::Policy::Manual;
    } else if (str == QLatin1Literal("auto")) {
        return Bolt::Policy::Auto;
    } else {
        qCCritical(log_libkbolt, "Unknown Policy enum value '%s'", qUtf8Printable(str));
        Q_ASSERT(false);
        return Bolt::Policy::Unknown;
    }
}

QString Bolt::policyToString(Bolt::Policy policy)
{
    switch (policy) {
    case Policy::Unknown:
        return QLatin1Literal("unknown");
    case Policy::Auto:
        return QLatin1Literal("auto");
    case Policy::Default:
        return QLatin1Literal("default");
    case Policy::Manual:
        return QLatin1Literal("manual");
    }

    Q_UNREACHABLE();
    return {};
}

Bolt::Type Bolt::typeFromString(const QString &str)
{
    if (str == QLatin1Literal("unknown")) {
        return Bolt::Type::Unknown;
    } else if (str == QLatin1Literal("host")) {
        return Bolt::Type::Host;
    } else if (str == QLatin1Literal("peripheral")) {
        return Bolt::Type::Peripheral;
    } else {
        qCCritical(log_libkbolt, "Unknown Type enum value '%s'", qUtf8Printable(str));
        Q_ASSERT(false);
        return Bolt::Type::Unknown;
    }
}

Bolt::AuthMode Bolt::authModeFromString(const QString &str)
{
    if (str == QLatin1Literal("disabled")) {
        return Bolt::AuthMode::Disabled;
    } else if (str == QLatin1Literal("enabled")) {
        return Bolt::AuthMode::Enabled;
    } else {
        qCCritical(log_libkbolt, "Unknown AuthMode enum value '%s'", qUtf8Printable(str));
        Q_ASSERT(false);
        return Bolt::AuthMode::Disabled;
    }
}

QString Bolt::authModeToString(Bolt::AuthMode authMode)
{
    switch (authMode) {
    case Bolt::AuthMode::Enabled:
        return QStringLiteral("enabled");
    case Bolt::AuthMode::Disabled:
        return QStringLiteral("disabled");
    }

    Q_UNREACHABLE();
    return {};
}

Bolt::Security Bolt::securityFromString(const QString &str)
{
    if (str == QLatin1Literal("unknown")) {
        return Bolt::Security::Unknown;
    } else if (str == QLatin1Literal("none")) {
        return Bolt::Security::None;
    } else if (str == QLatin1Literal("dponly")) {
        return Bolt::Security::DPOnly;
    } else if (str == QLatin1Literal("user")) {
        return Bolt::Security::User;
    } else if (str == QLatin1Literal("secure")) {
        return Bolt::Security::Secure;
    } else if (str == QLatin1Literal("usbonly")) {
        return Bolt::Security::USBOnly;
    } else {
        qCCritical(log_libkbolt, "Unknown Security enum value '%s'", qUtf8Printable(str));
        Q_ASSERT(false);
        return Bolt::Security::Unknown;
    }
}

