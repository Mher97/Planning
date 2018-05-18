#include "WorkLoadChartWidget.h"
#include "Charts/WorkLoadChart.h"
#include "Charts/WorkLoadSeries.h"
#include "BaseChartWindow.h"
#include "ui_BaseChartWidget.h"
#include "../Engine/Shceduler.h"
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QAreaSeries>

WorkLoadChartWidget::WorkLoadChartWidget(QWidget *parent) : BaseChartWidget(parent)
{
    BaseChartWidget::m_chartWindow = new BaseChartWindow(this);
}

WorkLoadChartWidget::~WorkLoadChartWidget()
{

}

QtCharts::QChart *WorkLoadChartWidget::constructChart()
{
    m_workLoad.clear();
    auto workLoadChart = new WorkLoadChart();
    auto workLoadSeries = new QtCharts::QLineSeries();
    int lastDay = 0;
    int maxVerticalValue = 0;
    workLoadChart->setTheme(m_currentChartTheme);
    foreach(const auto& list, m_data){
        foreach(const auto& value, list){
           lastDay = qMax(lastDay, value->FinishTime);
        }
    }
    if (lastDay){
        m_workLoad = QVector<int>(lastDay + 2, 0);
        foreach(const auto& list, m_data){
            foreach(const auto& value, list){
                ++m_workLoad[value->StartTime];
                --m_workLoad[value->FinishTime + 1];
            }
        }
        for (int i = 1; i < m_workLoad.size(); ++i){
            m_workLoad[i] += m_workLoad[i - 1];
            maxVerticalValue = qMax(maxVerticalValue, m_workLoad[i]);
        }
        for(int i = 0; i < m_workLoad.size(); ++i){
            workLoadSeries->append(QPointF(i, m_workLoad[i]));
        }
    }
    workLoadChart->addSeries(workLoadSeries);
    workLoadChart->createDefaultAxes();
    workLoadChart->setAxisX(constructHorizontalAxis(lastDay),
                           workLoadChart->series().isEmpty() ? nullptr : workLoadChart->series().first());
    workLoadChart->setAxisY(constructVerticalAxis(maxVerticalValue),
                            workLoadChart->series().isEmpty() ? nullptr : workLoadChart->series().first());
    workLoadChart->legend()->hide();
    workLoadChart->setAnimationOptions(QtCharts::QChart::AllAnimations);
    connect(workLoadSeries, &QtCharts::QLineSeries::hovered, this, &WorkLoadChartWidget::onSeriesHover);
    return workLoadChart;
}

QtCharts::QAbstractAxis *WorkLoadChartWidget::constructHorizontalAxis(const int maxValue)
{
    auto axisX = new QtCharts::QCategoryAxis();
    int middleValue;
    if (!maxValue)
        return axisX;
    QList<int> values;
    axisX->setRange(0, maxValue + 1);
    middleValue = (maxValue + 1)/2;
    values.push_back(0);
    values.push_back(middleValue/2);
    values.push_back(middleValue);
    values.push_back(middleValue + (maxValue - middleValue)/ 2);
    values.push_back(maxValue + 1);
    axisX->setLabelsPosition(QtCharts::QCategoryAxis::AxisLabelsPositionOnValue);
    foreach(const  auto value, values){
        axisX->append(QString::number(value), value);
    }
    return axisX;
}

QtCharts::QAbstractAxis *WorkLoadChartWidget::constructVerticalAxis(const int maxValue)
{
    static const int TickCount = 4;
    int rangeMax = 0;
    int stepLength = 0;
    int step = 1;
    auto axisY = new QtCharts::QCategoryAxis();
    if (!maxValue)
        return axisY;
    if (maxValue < TickCount){
        axisY->setTickCount(maxValue + 2);
        rangeMax = maxValue + 1;
    }else{
        int part = maxValue/TickCount;
        int remainder = maxValue%TickCount;
        rangeMax = (part += (remainder ? 1 : 0)) * TickCount;
    }
    stepLength = rangeMax/TickCount;
    if (!stepLength)
        ++stepLength;
    forever{
        auto value = step* stepLength;
        if (value > rangeMax)
            break;
        axisY->append(QString::number(value), value);
        ++step;
    }
    if (rangeMax == maxValue)
        ++rangeMax;
    axisY->setRange(0, rangeMax);
    axisY->setLabelsPosition(QtCharts::QCategoryAxis::AxisLabelsPositionOnValue);
    return axisY;
}

void WorkLoadChartWidget::onSeriesHover(const QPointF &point, bool state)
{
    auto sender = static_cast<QtCharts::QLineSeries*>(QObject::sender());
    if (sender == nullptr)
        return;
    if (state){
        auto info = getSeriesInfo(point);
        sender->chart()->setToolTip(QString("Time : %1 \nTasks : %2").
                                    arg(QString::number(info.first)).arg(QString::number(info.second)));
    }else{
        sender->chart()->setToolTip("");
    }
}

QPair<int, int> WorkLoadChartWidget::getSeriesInfo(const QPointF &point)const
{
    double xValue = point.x();
    int intXValue = static_cast<int>(point.x());
    if (xValue != intXValue){
        ++intXValue;
    }
    return qMakePair(intXValue, m_workLoad.at(intXValue));
}




