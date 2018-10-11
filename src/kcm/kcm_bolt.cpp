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

#include "kcm_bolt.h"

#include <KPluginFactory>
#include <KAboutData>
#include <KLocalizedString>

#include "device.h"
#include "devicemodel.h"

K_PLUGIN_FACTORY_WITH_JSON(KCMBoltFactory, "kcm_bolt.json", registerPlugin<KCMBolt>();)

KCMBolt::KCMBolt(QObject *parent, const QVariantList &args)
    : KQuickAddons::ConfigModule(parent, args)
{
    qmlRegisterType<Bolt::DeviceModel>("org.kde.bolt", 0, 1, "DeviceModel");
    qmlRegisterUncreatableType<Bolt::Device>("org.kde.bolt", 0, 1, "Device", QStringLiteral("Use DeviceModel"));


    auto about = new KAboutData(QStringLiteral("kcm_bolt"),
            i18n("Thunderbolt Device Management"),
            QStringLiteral("0.1"),
            i18n("System Settings module for managing Thunderbolt devices."),
            KAboutLicense::GPL);
    about->addAuthor(i18n("Daniel Vrátil"), {}, QStringLiteral("dvratil@kde.org"));
    setAboutData(about);
}

#include "kcm_bolt.moc"
