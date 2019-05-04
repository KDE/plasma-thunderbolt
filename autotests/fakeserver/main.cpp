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

#include <QCoreApplication>
#include <QDBusConnection>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QJsonDocument>
#include <QFile>
#include <QTimer>

#include <iostream>

#include "fakeserver.h"

int main(int argc, char **argv)
{
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication::setApplicationName(QStringLiteral("fakeserver"));

    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    QCommandLineOption cfgOption(QStringLiteral("cfg"), QStringLiteral("Config file"), QStringLiteral("FILE"));
    parser.addOption(cfgOption);
    parser.addHelpOption();
    parser.process(app);
    if (!parser.isSet(cfgOption)) {
        std::cout << "Missing option --cfg" << std::endl;
        parser.showHelp();
        return 0;
    }

    try {
        FakeServer server(parser.value(cfgOption));
        return app.exec();
    } catch (const FakeServerException &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
