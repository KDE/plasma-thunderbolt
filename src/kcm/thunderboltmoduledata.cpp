/*
 * SPDX-FileCopyrightText: 2026 Alexander Wilms <f.alexander.wilms@outlook.com>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "thunderboltmoduledata.h"

#include "manager.h"

ThunderboltModuleData::ThunderboltModuleData(QObject *parent)
    : KCModuleData(parent)
    , m_manager(new Bolt::Manager(this))
{
    updateRelevance();
}

void ThunderboltModuleData::updateRelevance()
{
    bool relevant = m_manager->isAvailable();
    setRelevant(relevant);
}

#include "moc_thunderboltmoduledata.cpp"
