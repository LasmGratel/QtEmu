/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
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

// Local
#include "mainwindow.h"
#include "machinewizard.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this -> setWindowTitle("QtEmu");
    this -> setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/icon/32x32/qtemu.png")));
    this -> setMinimumSize(700, 500);

    // Close the application when all windows are closed
    qApp -> setQuitOnLastWindowClosed(true);

    QSettings settings;

    m_configWindow = new ConfigWindow(this);
    m_helpwidget  = new HelpWidget(this);
    m_aboutwidget = new AboutWidget(this);

    // Prepare main layout
    m_osListWidget = new QListWidget(this);
    m_osListWidget -> setViewMode(QListView::ListMode);
    m_osListWidget -> setContextMenuPolicy(Qt::CustomContextMenu);
    m_osListWidget -> setIconSize(QSize(32, 32));
    m_osListWidget -> setMovement(QListView::Static);
    m_osListWidget -> setMaximumWidth(170);
    m_osListWidget -> setSpacing(7);

    m_osDetailsStackedWidget = new QStackedWidget(this);
    m_osDetailsStackedWidget -> setSizePolicy(QSizePolicy::Preferred,
                                             QSizePolicy::MinimumExpanding);

    m_containerLayout = new QHBoxLayout();
    m_containerLayout -> addWidget(m_osListWidget);
    m_containerLayout -> addWidget(m_osDetailsStackedWidget);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout -> addLayout(m_containerLayout, 20);
    m_mainLayout -> addSpacing(8);
    m_mainLayout -> addStretch(1);

    m_mainWidget = new QWidget(this);

    m_mainWidget -> setLayout(m_mainLayout);

    this -> setCentralWidget(m_mainWidget);

    // Create the menus
    createMenusActions();
    createMenus();
    createToolBars();
    loadMachines();

    this -> loadUI(m_osListWidget -> count());

    // Connect
    connect(m_osListWidget, &QListWidget::itemClicked,
            this, &MainWindow::changeMachine);

    connect(m_osListWidget, &QListWidget::customContextMenuRequested,
            this, &MainWindow::machinesMenu);
}

MainWindow::~MainWindow() {
    qDebug() << "MainWindow destroyed";
}

/*!
 * \brief Create the menus
 *
 * Create the menus for the top toolbar
 */
void MainWindow::createMenus() {

    // File
    m_fileMenu = new QMenu(tr("&File"), this);
    m_fileMenu -> addAction(m_preferencesAppAction);
    m_fileMenu -> addSeparator();
    m_fileMenu -> addAction(m_checkUpdateAppAction);
    m_fileMenu -> addSeparator();
    m_fileMenu -> addAction(m_exitAppAction);

    // Machine
    m_machineMenu = new QMenu(tr("&Machine"), this);
    m_machineMenu -> addAction(m_newMachineAction);
    m_machineMenu -> addAction(m_settingsMachineAction);
    m_machineMenu -> addAction(m_duplicateMachineAction);
    m_machineMenu -> addAction(m_removeMachineAction);
    //machineMenu -> addAction(groupMachine);

    // Help
    m_helpMenu = new QMenu(tr("&Help"), this);
    m_helpMenu -> addAction(m_helpQuickHelpAction);
    m_helpMenu -> addSeparator();
    m_helpMenu -> addAction(m_helpQtEmuWebsiteAction);
    m_helpMenu -> addAction(m_helpQtEmuBugTrackerAction);
    m_helpMenu -> addAction(m_helpQemuWebsiteAction);
    m_helpMenu -> addSeparator();
    m_helpMenu -> addAction(m_helpAboutAction);

    // Add the menus to the main menu bar
    this -> menuBar() -> addMenu(m_fileMenu);
    this -> menuBar() -> addMenu(m_machineMenu);
    this -> menuBar() -> addMenu(m_helpMenu);
}

/*!
 * \brief Create the actions for the menus
 *
 * Create the differents actions for the menus
 */
void MainWindow::createMenusActions() {

    // Actions for File menu
    m_preferencesAppAction = new QAction(QIcon::fromTheme("configure",
                                                          QIcon(":/icon/32x32/qtemu.png")),
                                         tr("Preferences"),
                                         this);
    //preferencesApp -> setShortcut(Qt::Key_F1);
    connect(m_preferencesAppAction, &QAction::triggered,
            m_configWindow, &QWidget::show);

    m_checkUpdateAppAction = new QAction(QIcon::fromTheme("update-none",
                                                          QIcon(":/icon/32x32/qtemu.png")),
                                         tr("Check for updates"),
                                         this);

    m_exitAppAction = new QAction(QIcon::fromTheme("application-exit",
                                                   QIcon(":/icon/32x32/qtemu.png")),
                                  tr("Exit"),
                                  this);

    // Actions for Machine menu

    m_newMachineAction = new QAction(QIcon::fromTheme("project-development-new-template",
                                                      QIcon(":/icon/32x32/qtemu.png")),
                                     tr("New Machine"),
                                     this);
    connect(m_newMachineAction, &QAction::triggered,
            this, &MainWindow::createNewMachine);

    m_settingsMachineAction = new QAction(QIcon::fromTheme("settings-configure",
                                                           QIcon(":/icon/32x32/qtemu.png")),
                                          tr("Machine Settings"),
                                          this);

    m_duplicateMachineAction = new QAction(QIcon::fromTheme("edit-duplicate",
                                                            QIcon(":/icon/32x32/qtemu.png")),
                                           tr("Duplicate Machine"),
                                           this);

    m_removeMachineAction = new QAction(QIcon::fromTheme("project-development-close",
                                                         QIcon(":/icon/32x32/qtemu.png")),
                                        tr("Remove Machine"),
                                        this);
    connect(m_removeMachineAction, &QAction::triggered,
            this, &MainWindow::deleteMachine);

    /*groupMachine = new QAction(QIcon::fromTheme("view-group",
                                                QIcon(":/icon/32x32/qtemu.png")),
                               tr("Group Machines"),
                               this);
    */

    // Actions for Help menu
    m_helpQuickHelpAction = new QAction(QIcon::fromTheme("help-contents",
                                                         QIcon(":/icon/32x32/qtemu.png")),
                                        tr("QtEmu &Quick Help"),
                                        this);
    m_helpQuickHelpAction -> setShortcut(Qt::Key_F1);
    connect(m_helpQuickHelpAction, &QAction::triggered,
            m_helpwidget, &QWidget::show);

    m_helpQtEmuWebsiteAction = new QAction(QIcon::fromTheme("internet-web-browser",
                                                            QIcon(":/icon/32x32/qtemu.png")),
                                           tr("QtEmu &Website"),
                                           this);
    connect(m_helpQtEmuWebsiteAction, &QAction::triggered,
            this, &MainWindow::visitQtEmuWebsite);

    m_helpQtEmuBugTrackerAction = new QAction(QIcon::fromTheme("tools-report-bug",
                                                               QIcon(":/icon/32x32/qtemu.png")),
                                              tr("QtEmu Bug Tracker"),
                                              this);
    connect(m_helpQtEmuBugTrackerAction, &QAction::triggered,
            this, &MainWindow::visitQtEmuBugTracker);

    m_helpQemuWebsiteAction = new QAction(QIcon::fromTheme("internet-web-browser",
                                                           QIcon(":/icon/32x32/qtemu.png")),
                                          tr("Qemu &Website"),
                                          this);
    connect(m_helpQemuWebsiteAction, &QAction::triggered,
            this, &MainWindow::visitQemuWebsite);

    m_helpAboutAction = new QAction(QIcon::fromTheme("qtemu",
                                                     QIcon(":/icon/32x32/qtemu.png")),
                                    tr("&About QtEmu"),
                                    this);
    connect(m_helpAboutAction, &QAction::triggered,
            m_aboutwidget, &QWidget::show);

    // Actions for Machine toolbar
    m_startMachineAction = new QAction(this);
    m_startMachineAction -> setIcon(QIcon::fromTheme("kt-start",
                                                     QIcon(":/icon/32x32/qtemu.png")));
    m_startMachineAction -> setToolTip(tr("Start machine"));
    connect(m_startMachineAction, &QAction::triggered,
            this, &MainWindow::runMachine);

    m_stopMachineAction = new QAction(this);
    m_stopMachineAction -> setIcon(QIcon::fromTheme("kt-stop",
                                                    QIcon(":/icon/32x32/qtemu.png")));
    m_stopMachineAction -> setToolTip(tr("Stop machine"));

    m_resetMachineAction = new QAction(this);
    m_resetMachineAction -> setIcon(QIcon::fromTheme("chronometer-reset",
                                                     QIcon(":/icon/32x32/qtemu.png")));
    m_resetMachineAction -> setToolTip(tr("Reset machine"));

    m_pauseMachineAction = new QAction(this);
    m_pauseMachineAction -> setIcon(QIcon::fromTheme("kt-pause",
                                                     QIcon(":/icon/32x32/qtemu.png")));
    m_pauseMachineAction -> setToolTip(tr("Pause machine"));

    m_saveMachineAction = new QAction(this);
    m_saveMachineAction -> setIcon(QIcon::fromTheme("system-save-session",
                                                    QIcon(":/icon/32x32/qtemu.png")));
    m_saveMachineAction -> setToolTip(tr("Save machine"));

}

/**
 * @brief Create the toolbar
 *
 * Create the main toolbar of the app
 */
void MainWindow::createToolBars() {
    this -> m_mainToolBar = addToolBar(tr("Toolbar"));

    m_mainToolBar -> setToolButtonStyle(Qt::ToolButtonFollowStyle);

    m_mainToolBar -> addAction(this -> m_newMachineAction);
    m_mainToolBar -> addAction(this -> m_settingsMachineAction);
    m_mainToolBar -> addSeparator();
    m_mainToolBar -> addAction(this -> m_startMachineAction);
    m_mainToolBar -> addAction(this -> m_stopMachineAction);
    m_mainToolBar -> addAction(this -> m_resetMachineAction);
    m_mainToolBar -> addAction(this -> m_pauseMachineAction);
    m_mainToolBar -> addAction(this -> m_saveMachineAction);

    m_mainToolBar -> setMovable(false);

}

/*!
 * \brief Load created machines
 *
 * Load all the machines stored in the qtemu.json file on config data folder
 */
void MainWindow::loadMachines() {

    QSettings settings;
    settings.beginGroup("DataFolder");

    QString dataDirectoryPath = settings.value("QtEmuData",
                                               QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/")).toString();
    settings.endGroup();

    // Open the file with the machines
    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");

    QFile machinesFile(qtemuConfig);
    machinesFile.open(QIODevice::ReadWrite); // TODO: Check if open the file fails

    // Read all data included in the file
    QByteArray machinesData = machinesFile.readAll();
    QJsonDocument machinesDocument(QJsonDocument::fromJson(machinesData));
    QJsonArray machines = machinesDocument["machines"].toArray();

    for (int i = 0; i < machines.size(); ++i) {
        QJsonObject machineJSON = machines[i].toObject();

        QListWidgetItem *machine = new QListWidgetItem(machineJSON["name"].toString(), this -> m_osListWidget);
        machine -> setData(QMetaType::QUuid, machineJSON["uuid"].toString());
        // TODO: Check if the json it's incomplete and the image not exits
        machine -> setIcon(QIcon(":/images/os/64x64/" +
                                 SystemUtils::getOsIcon(machineJSON["icon"].toString())));
    }


}

/*!
 * \brief Open QtEmu Website
 *
 * Open the QtEmu official webpage into your browser
 */
void MainWindow::visitQtEmuWebsite() {
    QDesktopServices::openUrl(QUrl("https://www.qtemu.org"));
}

/*!
 * \brief Open QtEmu Bug Tracker
 *
 * Open the QtEmu Bug Tracker into your browser
 */
void MainWindow::visitQtEmuBugTracker() {
    QDesktopServices::openUrl(QUrl("https://gitlab.com/carlavilla/Qtemu/issues"));
}

/*!
 * \brief Open Qemu Website
 *
 * Open the Qemu official webpage into your browser
 */
void MainWindow::visitQemuWebsite() {
    QDesktopServices::openUrl(QUrl("https://www.qemu.org/"));
}

/*!
 * \brief Open the create machine wizard
 *
 * Open the machine wizard to create a new machine
 */
void MainWindow::createNewMachine() {

    m_machine = new Machine(this);

    m_machine -> addAudio("ac97", "Intel AC97(82801AA)");
    m_machine -> setSocketCount(0);
    m_machine -> setCoresSocket(0);
    m_machine -> setThreadsCore(0);
    m_machine -> setMaxHotCPU(0);

    MachineWizard newMachineWizard(m_machine, this -> m_osListWidget, this);

    newMachineWizard.show();
    newMachineWizard.exec();

    this -> loadUI(this -> m_osListWidget -> count());

}

/*!
 * \brief Start the selected machine
 *
 * Read the machine config file,
 * make the command and start the selected machine
 */
void MainWindow::runMachine() {
    QUuid machineUuid = this -> m_osListWidget -> currentItem() -> data(QMetaType::QUuid).toUuid();

    QStringList args = MachineUtils::generateMachineCommand(machineUuid);

    QProcess *machineProcess = new QProcess();

    QString program;

    #ifdef Q_OS_LINUX
    program = "qemu-system-x86_64";
    #endif

    machineProcess -> start(program, args);

    // TODO: Control the output of the machineProcess

    // Connections


}

/*!
  * \brief Delete the selected machine
  *
  * Delete the selected machine and its associated files
  */
void MainWindow::deleteMachine() {
    QUuid machineUuid = this -> m_osListWidget -> currentItem() -> data(QMetaType::QUuid).toUuid();

    bool isMachineDeleted = MachineUtils::deleteMachine(machineUuid);

    if (isMachineDeleted) {
        this -> m_osListWidget -> takeItem(this -> m_osListWidget -> currentRow());
        this -> loadUI(this -> m_osListWidget -> count());
    }

}

/*!
 * \brief Enable/Disable buttons
 *
 * Enable/Disable the buttons in the menubar and in the main ui
 * If the osListWidget item doesn't have elements, elements
 * related to the VM actions are disabled. If the osListWidget have
 * at least one element, elements are enabled
 */
void MainWindow::loadUI(const int itemCount) {

    this -> m_stopMachineAction  -> setEnabled(false);
    this -> m_resetMachineAction -> setEnabled(false);
    this -> m_pauseMachineAction -> setEnabled(false);
    this -> m_saveMachineAction  -> setEnabled(false);

    if (itemCount == 0) {
        this -> m_settingsMachineAction  -> setEnabled(false);
        this -> m_duplicateMachineAction -> setEnabled(false);
        this -> m_removeMachineAction    -> setEnabled(false);

        this -> m_startMachineAction     -> setEnabled(false);
    } else {
        this -> m_osListWidget -> setCurrentRow(0);

        this -> m_settingsMachineAction  -> setEnabled(true);
        this -> m_duplicateMachineAction -> setEnabled(true);
        this -> m_removeMachineAction    -> setEnabled(true);

        this -> m_startMachineAction     -> setEnabled(true);
    }

}

/**
 * @brief Enable or disable the machine action items
 * @param machineItem, machine item of the list
 *
 * Enable/Disable the machine action items depending the
 * state of the machine
 */
void MainWindow::changeMachine(QListWidgetItem *machineItem) {
    qDebug() << "Change machine" << machineItem -> data(QMetaType::QUuid).toUuid();
}

void MainWindow::machinesMenu(const QPoint &pos) {
    qDebug() << "Machine menu, pos: " << pos;
}
