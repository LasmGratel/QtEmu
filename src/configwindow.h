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

#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

// Qt
#include <QWidget>
#include <QSettings>
#include <QIcon>
#include <QFileDialog>
#include <QDir>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>

#include <QDebug>

class ConfigWindow : public QWidget {
    Q_OBJECT

    public:
        explicit ConfigWindow(QWidget *parent = 0);
        ~ConfigWindow();

    void createGeneralPage();
    void createInputPage();
    void createUpdatePage();
    void createLanguagePage();
    void createStartPage();
    void createProxyPage();

    signals:

    public slots:

    protected:

    private:
       QListWidget *optionsListWidget;
       QListWidget *languagesListView;

       QStackedWidget *categoriesStackedWidget;

       QLabel *defaultMachineFolder;
       QLabel *languageLabel;
       QLabel *languageDescription;
       QLabel *languageSelected;
       QLabel *languageAuthors;

       QVBoxLayout *mainLayout;
       QVBoxLayout *generalPageLayout;
       QVBoxLayout *inputPageLayout;
       QVBoxLayout *updatePageLayout;
       QVBoxLayout *languagePageLayout;
       QVBoxLayout *startPageLayout;
       QVBoxLayout *proxyPageLayout;

       QHBoxLayout *topLayout;
       QHBoxLayout *buttonsLayout;
       QHBoxLayout *languageHLayout;

       QWidget *generalPageWidget;
       QWidget *inputPageWidget;
       QWidget *updatePageWidget;
       QWidget *languagePageWidget;
       QWidget *startPageWidget;
       QWidget *proxyPageWidget;

       QPushButton *saveButton;
       QPushButton *closeButton;

       QAction *closeAction;

};

#endif // CONFIGWINDOW_H
