#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMutex>

namespace Ui {
class MainWindow;
}

class WorkSpaceWidget;
class SettingsDialog;
class MappingWidget;
class Shceduler;
class TaskInfo;
class ChartsWidget;
class PlanningWidget;
class MonitorItem;
class BusyThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    private:
        Ui::MainWindow *ui;
        WorkSpaceWidget *m_workSpaceWidget;
        MappingWidget *m_mappingWidget;
        SettingsDialog *m_settingsDialog;
        QTabWidget *m_centralWidget;
        ChartsWidget *m_chartsWidget;
        PlanningWidget *m_planningWidget;
        Shceduler *m_shceduler;
        MonitorItem *m_currentMonitorItem;
        BusyThread *m_progressThread;
        void initSettings();
        void initDatabase();
        void initWorkSpace();
        void initMappingWidget();
        void initUi();
        void initProgessThread();
        void initConnections();
        void handleResult(const std::vector<std::vector<const TaskInfo*>>& result);
    private slots:
        void onTbImportClicked();
        void onTbSettingsClicked();
        void onTbChartsClicked();
        void onTbPlanningClicked();
        void onTbHelpClicked();
        void onWorkSpaceCurrentItemChanged(bool isMonitoring, bool isMonitoringChanged);
        void onMonitoringDataChanged(MonitorItem * monitorItem);
        void dataImport();
        void startBusy(const QString& text);
        void finishBusy();
        void updateBusy();
};

#endif // MAINWINDOW_H
