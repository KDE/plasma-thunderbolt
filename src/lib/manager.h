/*
 * Copyright (c) 2018  Daniel Vrátil <dvratil@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>

#include <functional>
#include <memory>

#include "enum.h"
#include "kbolt_export.h"

class QDBusObjectPath;
class OrgFreedesktopBolt1ManagerInterface;
namespace Bolt
{

class Device;
class KBOLT_EXPORT Manager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isAvailable READ isAvailable CONSTANT)
    Q_PROPERTY(uint version READ version CONSTANT STORED false)
    Q_PROPERTY(bool isProbing READ isProbing CONSTANT STORED false)
    Q_PROPERTY(Bolt::Policy defaultPolicy READ defaultPolicy CONSTANT STORED false)
    Q_PROPERTY(Bolt::Security securityLevel READ securityLevel CONSTANT STORED false)
    Q_PROPERTY(Bolt::AuthMode authMode READ authMode WRITE setAuthMode STORED false)

public:
    explicit Manager(QObject *parent = nullptr);
    ~Manager() override;

    bool isAvailable() const;

    uint version() const;
    bool isProbing() const;
    Policy defaultPolicy() const;
    Security securityLevel() const;
    AuthMode authMode() const;
    void setAuthMode(AuthMode mode);

public Q_SLOTS:
    QSharedPointer<Bolt::Device> device(const QString &uid) const;
    QSharedPointer<Bolt::Device> device(const QDBusObjectPath &path) const;
    QList<QSharedPointer<Bolt::Device>> devices() const;

    void enrollDevice(const QString &uid, Bolt::Policy policy, Bolt::AuthFlags flags);
    void forgetDevice(const QString &uid);

Q_SIGNALS:
    void deviceAdded(const QSharedPointer<Bolt::Device> &device);
    void deviceRemoved(const QSharedPointer<Bolt::Device> &device);

private:
    QSharedPointer<Device> device(std::function<bool(const QSharedPointer<Device> &)> &&match) const;
    QScopedPointer<OrgFreedesktopBolt1ManagerInterface> mInterface;

    uint mVersion = 0;
    Policy mPolicy = Policy::Unknown;
    Security mSecurity = Security::Unknown;
    AuthMode mAuthMode = AuthMode::Disabled;
    bool mIsProbing = false;

    QList<QSharedPointer<Device>> mDevices;
};

} // namespace

#endif
