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

#include "kcm_bolt.h"

#include <KPluginFactory>
#include <KAboutData>
#include <KLocalizedString>

#include "device.h"
#include "devicemodel.h"
#include "manager.h"
#include "enum.h"

#include <QDebug>

K_PLUGIN_FACTORY_WITH_JSON(KCMBoltFactory, "kcm_bolt.json", registerPlugin<KCMBolt>();)

class QMLHelper : public QObject
{
    Q_OBJECT
public:
    explicit QMLHelper(QObject *parent = nullptr)
        : QObject(parent)
    {}

public Q_SLOTS:
    void authorizeDevice(Bolt::Device *device, Bolt::AuthFlags authFlags,
                         QJSValue successCb = {}, QJSValue errorCb = {})
    {
        device->authorize(authFlags, invoke(successCb), invoke<QString>(errorCb));
    }

    void enrollDevice(Bolt::Manager *manager, const QString &uid, Bolt::Policy policy,
                      Bolt::AuthFlags authFlags, QJSValue successCb = {}, QJSValue errorCb = {})
    {
        manager->enrollDevice(uid, policy, authFlags, invoke(successCb), invoke<QString>(errorCb));
    }

    void forgetDevice(Bolt::Manager *manager, const QString &uid, QJSValue successCb, QJSValue errorCb)
    {
        manager->forgetDevice(uid, invoke(successCb), invoke<QString>(errorCb));
    }

private:
    template<typename ... Args>
    std::function<void(Args ...)> invoke(QJSValue cb_)
    {
        return [cb = std::move(cb_)](Args && ... args) mutable {
            Q_ASSERT(cb.isCallable());
            cb.call({std::forward<Args>(args) ...});
        };
    }
};

KCMBolt::KCMBolt(QObject *parent, const QVariantList &args)
    : KQuickAddons::ConfigModule(parent, args)
{
    qmlRegisterType<Bolt::DeviceModel>("org.kde.bolt", 0, 1, "DeviceModel");
    qmlRegisterType<Bolt::Manager>("org.kde.bolt", 0, 1, "Manager");
    qmlRegisterUncreatableType<Bolt::Device>("org.kde.bolt", 0, 1, "Device",
            QStringLiteral("Use DeviceModel"));
    qmlRegisterUncreatableMetaObject(Bolt::staticMetaObject, "org.kde.bolt", 0, 1, "Bolt",
            QStringLiteral("For enums and flags only"));
    qmlRegisterSingletonType<QMLHelper>("org.kde.bolt", 0, 1, "QMLHelper",
            [](auto, auto) -> QObject* { return new QMLHelper(); });

    auto about = new KAboutData(QStringLiteral("kcm_bolt"),
            i18n("Thunderbolt Device Management"),
            QStringLiteral("0.1"),
            i18n("System Settings module for managing Thunderbolt devices."),
            KAboutLicense::GPL);
    about->addAuthor(i18n("Daniel Vrátil"), {}, QStringLiteral("dvratil@kde.org"));
    setAboutData(about);
}

#include "kcm_bolt.moc"
