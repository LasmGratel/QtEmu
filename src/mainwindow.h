/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
 * Copyright (C) 2017 Sergio Carlavilla <carlavilla @ mailbox.org>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>
#include <QSettings>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDesktopServices>
#include <QDebug>

// Local
#include "helpwidget.h"
#include "aboutwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

        void createMenus();
        void createMenusActions();

    public slots:
        void visitQtEmuWebsite();
        void visitQtEmuBugTracker();
        void visitQemuWebsite();

    protected:

    private:
        // Start menus
        QMenu *fileMenu;
        QMenu *machineMenu;
        QMenu *helpMenu;

        QAction *exitApp;
        QAction *checkUpdateApp;
        QAction *preferencesApp;

        QAction *newMachine;
        QAction *addMachine;
        QAction *settingsMachine;
        QAction *duplicateMachine;
        QAction *removeMachine;
        QAction *groupMachine;

        QAction *helpQuickHelp;
        QAction *helpQtEmuWebsite;
        QAction *helpQtEmuBugTracker;
        QAction *helpQemuWebsite;
        QAction *helpAbout;
        // End menus

        // Widgets
        HelpWidget *helpwidget;
        AboutWidget *aboutwidget;
};


#endif // MAINWINDOW_H
