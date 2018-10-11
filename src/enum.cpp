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

#include "enum.h"

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
    } else if (str == QLatin1Literal("autherror")) {
        return Bolt::Status::AuthError;
    } else {
        Q_ASSERT(false);
        return Bolt::Status::Unknown;
    }
}

Bolt::AuthFlags Bolt::authFlagsFromString(const QString &str)
{
    if (str == QLatin1Literal("none")) {
        return Bolt::Auth::None;
    }
    // TODO
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
        Q_ASSERT(false);
        return Bolt::Policy::Unknown;
    }
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
        Q_ASSERT(false);
        return Bolt::AuthMode::Disabled;
    }
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
        Q_ASSERT(false);
        return Bolt::Security::Unknown;
    }
}
