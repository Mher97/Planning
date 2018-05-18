#ifndef WORKLOADCHARTWIDGET_H
#define WORKLOADCHARTWIDGET_H

#include "BaseChartWidget.h"

class TaskInfo;

class WorkLoadChartWidget : public BaseChartWidget
{
    public:
        explicit WorkLoadChartWidget(QWidget *parent = nullptr);
        virtual ~WorkLoadChartWidget();
        template <typename TaskList>
        void updateChart(TaskList&& data)
        {
            m_data = std::forward<TaskList>(data);
            changeChart();
        }
    private:
        virtual QtCharts::QChart* constructChart() override;
        virtual QtCharts::QAbstractAxis* constructHorizontalAxis(const int maxValue);
        virtual QtCharts::QAbstractAxis* constructVerticalAxis(const int maxValue);
        QList<QList<const TaskInfo*>> m_data;
        QVector<int> m_workLoad;
        QPair<int,int> getSeriesInfo(const QPointF &point)const;
    private slots:
        void onSeriesHover(const QPointF &point, bool state);
};

#endif // WORKLOADCHARTWIDGET_H
