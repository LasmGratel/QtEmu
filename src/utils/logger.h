/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef LOGGER_H
#define LOGGER_H

// Qt
#include <QDateTime>
#include <QSettings>
#include <QFile>
#include <QTextStream>

#include <QDebug>

// Local

class Logger {

    public:
        explicit Logger();
        ~Logger();

        void logMachineCreation(const QString &message);
        void logMachineAction(const QString &message);

        void logQtemuAction(const QString &message);
        void logQtemuError(const QString &message);

    signals:

    public slots:

    protected:

    private:
        QFile *logsFile;

};

#endif // LOGGER_H
