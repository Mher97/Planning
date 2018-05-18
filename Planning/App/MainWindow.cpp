#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include "WorkSpace/WorkSpaceWidget.h"
#include "DbManager.h"
#include "Settings/SettingsDialog.h"
#include "DataReading/MappingWidget.h"
#include "../Engine/Shceduler.h"
#include "ChartsWidget.h"
#include "PlanningTree/PlanningWidget.h"
#include "WorkSpace/MonitorItem.h"
#include "Settings/ISettings.h"
#include "Settings/DbSettingsWidget.h"
#include "HelpDialog.h"
#include "Waiting/BusyDialog.h"
#include "Waiting/BusyThread.h"
#include "Waiting/BusyManager.h"
#include "Globals.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_workSpaceWidget(nullptr),
    m_mappingWidget(nullptr), m_settingsDialog(nullptr), m_centralWidget(nullptr),
    m_chartsWidget(new ChartsWidget), m_planningWidget(new PlanningWidget), m_shceduler(new Shceduler),
    m_currentMonitorItem(nullptr)
{
    ui->setupUi(this);
    initSettings();
    initDatabase();
    initUi();
    initWorkSpace();
    initMappingWidget();
    initProgessThread();
    initConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSettings()
{
    m_settingsDialog = new SettingsDialog(this);
    try{
        m_settingsDialog->loadSettings();
    }catch(std::exception& ex){
        Globals::showMessage(QMessageBox::Critical, "App", ex.what());
    }
}

void MainWindow::initDatabase()
{
    try{
        auto dbSettings = const_cast<ISettings*>(m_settingsDialog->getDbSettings());
        if (dbSettings == nullptr){
            Globals::showMessage(QMessageBox::Critical, "App", "Problem with Db Settings");
            return;
        }
        DbManager::getInstance()->openDatabase("QMYSQL",
                          dbSettings->getSettings()->value(DbSettingsWidget::settingsNamesToString(DbSettingsWidget::DB_SETTINGS::HOST_NAME)).toString(),
                          dbSettings->getSettings()->value(DbSettingsWidget::settingsNamesToString(DbSettingsWidget::DB_SETTINGS::DB_NAME)).toString(),
                          dbSettings->getSettings()->value(DbSettingsWidget::settingsNamesToString(DbSettingsWidget::DB_SETTINGS::USER_NAME)).toString(),
                          dbSettings->getSettings()->value(DbSettingsWidget::settingsNamesToString(DbSettingsWidget::DB_SETTINGS::PASSWORD)).toString(),
                          dbSettings->getSettings()->value(DbSettingsWidget::settingsNamesToString(DbSettingsWidget::DB_SETTINGS::PORT_NUMBER)).toInt());;
    }
    catch(std::exception &ex){
        qDebug()<<ex.what();
        Globals::showMessage(QMessageBox::Critical, "App", ex.what());
    }
}

void MainWindow::initWorkSpace()
{
    m_workSpaceWidget = new WorkSpaceWidget();
    ui->workSpaceDockWidget->setWidget(m_workSpaceWidget);
}

void MainWindow::initMappingWidget()
{
    m_mappingWidget = new MappingWidget();
    m_centralWidget->addTab(m_mappingWidget, "Data Mapping");
}

void MainWindow::initUi()
{
    ui->mainToolBar->addWidget(ui->tbImport);
    ui->mainToolBar->addWidget(ui->tbSettings);
    ui->mainToolBar->addWidget(ui->tbPlanning);
    ui->mainToolBar->addWidget(ui->tbCharts);
    ui->mainToolBar->addWidget(ui->tbHelp);
    m_centralWidget = new QTabWidget(this);
    setCentralWidget(m_centralWidget);
    m_centralWidget->show();
}

void MainWindow::initProgessThread()
{
    m_progressThread = new BusyThread();
}

void MainWindow::initConnections()
{
    connect(ui->tbImport, &QToolButton::clicked, this, &MainWindow::onTbImportClicked);
    connect(ui->tbSettings, &QToolButton::clicked, this, &MainWindow::onTbSettingsClicked);
    connect(ui->tbCharts, &QToolButton::clicked, this, &MainWindow::onTbChartsClicked);
    connect(ui->tbPlanning, &QToolButton::clicked, this, &MainWindow::onTbPlanningClicked);
    connect(ui->tbHelp, &QToolButton::clicked, this, &MainWindow::onTbHelpClicked);
    connect(m_workSpaceWidget, &WorkSpaceWidget::workSpaceCurrentItemChanged, this, &MainWindow::onWorkSpaceCurrentItemChanged);
    connect(m_workSpaceWidget, &WorkSpaceWidget::monitoringDataChanged, this, &MainWindow::onMonitoringDataChanged);
    connect(m_mappingWidget, &MappingWidget::onImportButtonClicked, this, &MainWindow::dataImport);
    connect(&BusyManager::getInstance(), &BusyManager::mustBeStartBusy, this, &MainWindow::startBusy);
    connect(&BusyManager::getInstance(), &BusyManager::mustBeFinishBusy, this, &MainWindow::finishBusy);
    connect(m_progressThread, &BusyThread::progressMustBeUpdate, this, &MainWindow::updateBusy, Qt::DirectConnection);
}

void MainWindow::onTbImportClicked()
{
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("Data Files (*.csv)");
    if (fileDialog.exec())
    {
        QStringList list = fileDialog.selectedFiles();
        if (list.size() == 1){
            m_workSpaceWidget->saveImportedFile(list.front());
        }
    }
}

void MainWindow::onTbSettingsClicked()
{
    m_settingsDialog->updateContent();
    m_settingsDialog->exec();
}

void MainWindow::onTbChartsClicked()
{
    int index = m_centralWidget->indexOf(m_chartsWidget);
    if (index == -1)
        index = m_centralWidget->addTab(m_chartsWidget, "Charts");
    m_centralWidget->setCurrentIndex(index);
}

void MainWindow::onTbPlanningClicked()
{
    int index = m_centralWidget->indexOf(m_planningWidget);
    if (index == -1)
        index = m_centralWidget->addTab(m_planningWidget, "Planning");
    m_centralWidget->setCurrentIndex(index);
}

void MainWindow::onTbHelpClicked()
{
    HelpDialog *helpDialog = new HelpDialog(this);
    helpDialog->setAttribute(Qt::WA_DeleteOnClose);
    helpDialog->setWindowFlag(Qt::Window);
    helpDialog->exec();
}

void MainWindow::onWorkSpaceCurrentItemChanged(bool isMonitoring, bool isMonitoringChanged)
{
    ui->tbImport->setEnabled(isMonitoring);
    if (!isMonitoring){
        m_currentMonitorItem = nullptr;
        ui->tbPlanning->setEnabled(false);
        ui->tbCharts->setEnabled(false);
    }
    if (isMonitoringChanged){
        m_centralWidget->removeTab(m_centralWidget->indexOf(m_planningWidget));
        m_centralWidget->removeTab(m_centralWidget->indexOf(m_chartsWidget));
        m_chartsWidget->reset();
        ui->tbPlanning->setEnabled(false);
        ui->tbCharts->setEnabled(false);
    }else{
        ui->tbPlanning->setEnabled(ui->tbPlanning->isEnabled());
        ui->tbCharts->setEnabled(ui->tbCharts->isEnabled());
    }
    if (m_currentMonitorItem == nullptr || !m_currentMonitorItem->hasData()){
        m_mappingWidget->reset();
    }
}

void MainWindow::onMonitoringDataChanged(MonitorItem * monitorItem)
{
    startBusy("Please Wait...");
    m_currentMonitorItem = monitorItem;
    if (monitorItem ->hasData()){
        m_mappingWidget->handleData(monitorItem->getCurrentData(), monitorItem->getDataMapping());
    }
    finishBusy();
}

void MainWindow::dataImport()
{
    startBusy("Calculatation...");
    m_centralWidget->removeTab(m_centralWidget->indexOf(m_planningWidget));
    m_centralWidget->removeTab(m_centralWidget->indexOf(m_chartsWidget));
    QApplication::processEvents();
    for (int i = 1; i <= 2e9; ++i);
    bool isException = false;
    QHash<QString, QPair<int, QStringList>> data;
    try{
        data = m_mappingWidget->readData();
        /*foreach(const auto& key, data.keys()){
            qDebug()<<key<<" "<<data.value(key).first;
            foreach(auto v, data.value(key).second)
                qDebug()<<v<<" ";
            qDebug()<<"\n";
        }*/
    }catch(std::exception& ex){
        Globals::showMessage(QMessageBox::Critical, "Validation", ex.what());
        isException = true;
    }
    if (!isException){
        ui->tbPlanning->setEnabled(true);
        ui->tbCharts->setEnabled(true);
        QList<QPair<MonitorItem::COLUMN_DATA_TYPE,int>> mappingList;
        mappingList.push_back(qMakePair(MonitorItem::COLUMN_DATA_TYPE::NAME_COLUMN,
                                        m_mappingWidget->getMapping().value(MappingItem::DATA_TYPES::DATA_TASK_CODE)));
        mappingList.push_back(qMakePair(MonitorItem::COLUMN_DATA_TYPE::DURATION_COLUMN,
                                        m_mappingWidget->getMapping().value(MappingItem::DATA_TYPES::DATA_DURATION)));
        mappingList.push_back(qMakePair(MonitorItem::COLUMN_DATA_TYPE::DEPENDENCY_COLUMN,
                                        m_mappingWidget->getMapping().value(MappingItem::DATA_TYPES::DATA_DEPENDENCE)));
        m_currentMonitorItem->changeMapping(mappingList, false);
        std::unordered_map<std::string, std::pair<int, std::vector<std::string>>> stdData;
        foreach(const auto& key, data.keys()){
            std::vector<std::string> stringList;
            foreach(const auto& value, data.value(key).second){
                stringList.push_back(value.toStdString());
            }
            stdData[key.toStdString()] = std::make_pair(data.value(key).first, stringList);
        }
        try{
            auto result = m_shceduler->scheduleData(stdData);
            handleResult(result);
        }catch(std::exception &ex){
            isException = true;
            Globals::showMessage(QMessageBox::Critical, "Engine", ex.what());
        }
    }
    finishBusy();
    if (!isException){
        Globals::showMessage(QMessageBox::Information, "Engine", "Calculation completed!");
    }
}

void MainWindow::handleResult(const std::vector<std::vector<const TaskInfo*>>& result)
{
    QList<QList<const TaskInfo*>> passedData;
    foreach(const auto& list, result){
        QList<const TaskInfo*> passedList;
        foreach(const auto& item, list){
            passedList.push_back(item);
        }
        passedData.push_back(passedList);
    }
    m_planningWidget->updatePlaningData(passedData);
    m_chartsWidget->updateCharts(passedData);
}

void MainWindow::startBusy(const QString &text)
{
    BusyDialog *busyDialog = new BusyDialog(text, this);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    busyDialog->setWindowModality(Qt::WindowModal);
    busyDialog->setWindowFlag(Qt::Widget);
    busyDialog->setAttribute(Qt::WA_DeleteOnClose);
    busyDialog->move(rect().center() - busyDialog->rect().center());
    busyDialog->show();
    m_progressThread->start(QThread::HighestPriority);
    QApplication::processEvents();
}

void MainWindow::finishBusy()
{
    auto busyDialog = findChild<BusyDialog*>();
    if (busyDialog != nullptr){
        busyDialog->close();
    }
    QApplication::restoreOverrideCursor();
    m_progressThread->quit();
}

void MainWindow::updateBusy()
{
    auto busyDialog = findChild<BusyDialog*>();
    if (busyDialog != nullptr){
        busyDialog->updateBusyIndicators();
        busyDialog->update();
        QApplication::processEvents();
    }
}










