#ifndef PLANNINGWIDGET_H
#define PLANNINGWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class PlanningWidget;
}

class PlanningModel;
class PlanningProxyModel;
class FilterItem;
class FilterModel;
class TaskInfo;

class PlanningWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit PlanningWidget(QWidget *parent = nullptr);
        ~PlanningWidget();
        void updatePlanningData(const QList<QList<const TaskInfo*>>& data);
    private:
        Ui::PlanningWidget *ui;
        PlanningModel *m_planningModel;
        PlanningProxyModel *m_planningProxyModel;
        FilterModel *m_filterModel;
        void initUi();
        void initModelView();
        void initConnections();
        void fillFilterComboBox(const QList<QList<const TaskInfo*>>& data);
    private slots:
        void onManualDataChanged(FilterItem *item);
        void onPopupHidden();
};

#endif // PLANNINGWIDGET_H
