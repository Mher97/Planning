#ifndef PLANNINGCHARTWIDGET_H
#define PLANNINGCHARTWIDGET_H

#include "Charts/BaseChartWidget.h"
#include <QtCharts/QAbstractAxis>

class TaskInfo;

class PlanningChartWidget : public BaseChartWidget
{
    public:
        explicit PlanningChartWidget(QWidget *parent = nullptr);
        virtual ~PlanningChartWidget();
        template <typename TaskList>
        void updateChart(TaskList&& data)
        {
            m_data = std::forward<TaskList>(data);
            changeChart();
        }
    private:
        virtual QtCharts::QChart* constructChart() override;
        virtual QtCharts::QAbstractAxis* constructHorizontalAxis(QVector<int>& bottomValues, const int maxValue);
        virtual QtCharts::QAbstractAxis* constructHorizontalExtraAxis(const int maxValue);
        virtual QtCharts::QAbstractAxis* constructVerticalAxis(const int maxValue);
        QList<QList<const TaskInfo*>> m_data;
        QVector<int> m_maxGeometricSeries;
        QRectF m_defaultRect;
    private slots:
        virtual void chartThemeChanged(int index)override;
        void onSeriesHover(const QPointF &point, bool state);
};

#endif // PLANNINGCHARTWIDGET_H
