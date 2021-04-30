/*
 * SPDX-FileCopyrightText: 2018-2019 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "kcm_bolt.h"

#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>

#include "device.h"
#include "devicemodel.h"
#include "enum.h"
#include "manager.h"

#include <QDebug>

K_PLUGIN_FACTORY_WITH_JSON(KCMBoltFactory, "kcm_bolt.json", registerPlugin<KCMBolt>();)

class QMLHelper : public QObject
{
    Q_OBJECT
public:
    explicit QMLHelper(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

public Q_SLOTS:
    void authorizeDevice(Bolt::Device *device, Bolt::AuthFlags authFlags, QJSValue successCb = {}, QJSValue errorCb = {})
    {
        device->authorize(authFlags, invoke(successCb), invoke<QString>(errorCb));
    }

    void
    enrollDevice(Bolt::Manager *manager, const QString &uid, Bolt::Policy policy, Bolt::AuthFlags authFlags, QJSValue successCb = {}, QJSValue errorCb = {})
    {
        manager->enrollDevice(uid, policy, authFlags, invoke(successCb), invoke<QString>(errorCb));
    }

    void forgetDevice(Bolt::Manager *manager, const QString &uid, QJSValue successCb, QJSValue errorCb)
    {
        manager->forgetDevice(uid, invoke(successCb), invoke<QString>(errorCb));
    }

private:
    template<typename... Args> std::function<void(Args...)> invoke(QJSValue cb_)
    {
        return [cb = std::move(cb_)](Args &&... args) mutable {
            Q_ASSERT(cb.isCallable());
            cb.call({std::forward<Args>(args)...});
        };
    }
};

KCMBolt::KCMBolt(QObject *parent, const QVariantList &args)
    : KQuickAddons::ConfigModule(parent, args)
{
    qmlRegisterType<Bolt::DeviceModel>("org.kde.bolt", 0, 1, "DeviceModel");
    qmlRegisterType<Bolt::Manager>("org.kde.bolt", 0, 1, "Manager");
    qmlRegisterUncreatableType<Bolt::Device>("org.kde.bolt", 0, 1, "Device", QStringLiteral("Use DeviceModel"));
    qmlRegisterUncreatableMetaObject(Bolt::staticMetaObject, "org.kde.bolt", 0, 1, "Bolt", QStringLiteral("For enums and flags only"));
    qmlRegisterSingletonType<QMLHelper>("org.kde.bolt", 0, 1, "QMLHelper", [](auto, auto) -> QObject * {
        return new QMLHelper();
    });

    auto about = std::make_unique<KAboutData>(QStringLiteral("kcm_bolt"),
                                              i18n("Thunderbolt Device Management"),
                                              QStringLiteral("0.1"),
                                              i18n("System Settings module for managing Thunderbolt devices."),
                                              KAboutLicense::GPL);
    about->addAuthor(i18n("Daniel Vrátil"), {}, QStringLiteral("dvratil@kde.org"));
    setAboutData(about.release());
}

#include "kcm_bolt.moc"
