/*
 * SPDX-FileCopyrightText: 2018-2019 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef KCMBOLT_H
#define KCMBOLT_H

#include <KQuickAddons/ConfigModule>

class KCMBolt : public KQuickAddons::ConfigModule
{
    Q_OBJECT

public:
    KCMBolt(QObject *parent, const KPluginMetaData &metaData, const QVariantList &args);
    ~KCMBolt() override = default;
};

#endif
