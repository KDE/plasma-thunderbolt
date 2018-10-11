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

#ifndef FAKEMANAGER_H_
#define FAKEMANAGER_H_

#include <QObject>
#include <QDBusObjectPath>
#include <QList>
#include <QString>
#include <QJsonObject>

class FakeDevice;
class FakeManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.bolt1.Manager")

    Q_PROPERTY(unsigned int Version READ version CONSTANT)
    Q_PROPERTY(bool Probing READ isProbing CONSTANT)
    Q_PROPERTY(QString DefaultPolicy READ defaultPolicy CONSTANT)
    Q_PROPERTY(QString SecurityLevel READ securityLevel CONSTANT)
    Q_PROPERTY(QString AuthMode READ authMode WRITE setAuthMode)
public:
    explicit FakeManager(const QJsonObject &json, QObject *parent = nullptr);
    ~FakeManager() override;

    unsigned int version() const;
    bool isProbing() const;
    QString defaultPolicy() const;
    QString securityLevel() const;
    QString authMode() const;
    void setAuthMode(const QString &authMode);

public Q_SLOTS:
    QList<QDBusObjectPath> ListDevices() const;
    QDBusObjectPath DeviceByUid(const QString &uid) const;
    QDBusObjectPath EnrollDevice(const QString &uid, const QString &policy, const QString &flags);
    void ForgetDevice(const QString &uid);

Q_SIGNALS:
    void DeviceAdded(const QDBusObjectPath &device);
    void DeviceRemoved(const QDBusObjectPath &device);

private:
    bool mProbing = false;
    QString mDefaultPolicy = QStringLiteral("auto");
    QString mSecurityLevel = QStringLiteral("none");
    QString mAuthMode = QStringLiteral("enabled");

    QMap<QString /*uid*/, FakeDevice*> mDevices;
};


#endif
