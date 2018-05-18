#include "PlanningChartWidget.h"
#include "PlanningChart.h"
#include "PlanningSeries.h"
#include "PlanningChartWindow.h"
#include <QLineSeries>
#include "ui_BaseChartWidget.h"
#include "QtCharts/QCategoryAxis"
#include "../Engine/Shceduler.h"

PlanningChartWidget::PlanningChartWidget(QWidget* parent) : BaseChartWidget(parent)
{
    BaseChartWidget::m_chartWindow = new PlanningChartWindow(this);
    ui->chartView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->chartView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

PlanningChartWidget::~PlanningChartWidget()
{

}

QtCharts::QChart* PlanningChartWidget::constructChart()
{
    auto planningChart = new PlanningChart();
    QVector<int> horizontalValues;
    int verticalMaxValue = 0;
    int horizontalMaxValue = 0;
    int phaseNum = 1;
    int minDuration = INT_MAX;
    int maxDuration = 0;
    int verticalMaxLevel = 0;
    planningChart->setTheme(m_currentChartTheme);
    m_maxGeometricSeries.fill(0, m_maxGeometricSeries.size());
    foreach(const auto& phase, m_data){
        foreach(const auto& info, phase){
            auto lowerSeries = new QtCharts::QLineSeries;
            auto upperSeries = new QtCharts::QLineSeries;
            auto isFound = false;
            auto level = 0;
            for(int startLevel = 0; startLevel < m_maxGeometricSeries.size(); ++startLevel){
                if (info->StartTime - 1 >= m_maxGeometricSeries[startLevel]){
                    level = startLevel;
                    isFound = true;
                    break;
                }
            }
            if (!isFound){
                m_maxGeometricSeries.push_back(0);
                level = m_maxGeometricSeries.size() - 1;
            }
            lowerSeries->append(QPointF(info->StartTime - 1, level * 2));
            lowerSeries->append(QPointF(info->FinishTime, level * 2));
            upperSeries->append(QPointF(info->StartTime - 1, level * 2 + 1));
            upperSeries->append(QPointF(info->FinishTime, level * 2 + 1));
            m_maxGeometricSeries[level] = info->FinishTime;
            horizontalValues.push_back(info->StartTime - 1);
            verticalMaxValue = qMax(verticalMaxValue, level * 2 + 1);
            horizontalMaxValue = qMax(horizontalMaxValue, info->FinishTime);
            minDuration = qMin(minDuration, info->FinishTime - info->StartTime + 1);
            maxDuration = qMax(maxDuration, info->FinishTime - info->StartTime + 1);
            verticalMaxLevel = qMax(verticalMaxLevel, level);
            auto planningSeries = new PlanningSeries(info, phaseNum, lowerSeries, upperSeries);
            connect(planningSeries, &QtCharts::QAreaSeries::hovered, this, &PlanningChartWidget::onSeriesHover);
            planningChart->addSeries(planningSeries);
        }
        ++phaseNum;
    }
    planningChart->createDefaultAxes();
    planningChart->setAxisX(constructHorizontalAxis(horizontalValues, horizontalMaxValue),
                            planningChart->series().isEmpty() ? nullptr : planningChart->series().first());
    auto extraAxis = constructHorizontalExtraAxis(horizontalMaxValue);
    planningChart->addAxis(extraAxis, Qt::AlignTop);
    if (!planningChart->series().isEmpty()){
        planningChart->series().first()->attachAxis(extraAxis);
    }
    planningChart->setAxisY(constructVerticalAxis(verticalMaxValue));
    planningChart->legend()->hide();
    planningChart->setAnimationOptions(QtCharts::QChart::AllAnimations);
    planningChart->setMinimumWidth(qMax(minimumWidth(), horizontalMaxValue/ minDuration * 10));
    planningChart->setMinimumHeight(qMax(minimumHeight(),verticalMaxLevel * 10));
    return planningChart;
}

QtCharts::QAbstractAxis* PlanningChartWidget::constructHorizontalAxis(QVector<int> &values, const int maxValue)
{
    int previousValue = 1;
    std::sort(values.begin(), values.end());
    auto axisX = new QtCharts::QCategoryAxis();
    axisX->setRange(0, maxValue);
    foreach(const auto& value, values){
        if (value == previousValue){
            continue;
        }
        axisX->append(QString::number(value), value);
        previousValue = value;
    }
    axisX->append(QString::number(maxValue), maxValue);
    axisX->setLabelsPosition(QtCharts::QCategoryAxis::AxisLabelsPositionOnValue);
    return axisX;
}

QtCharts::QAbstractAxis *PlanningChartWidget::constructHorizontalExtraAxis(const int maxValue)
{
    static const int TickCount = 4;
    auto extraAxis= new QtCharts::QCategoryAxis();
    if (maxValue < TickCount){
        extraAxis->setTickCount(maxValue + 1);
        for (int i = 0; i < extraAxis->tickCount(); ++i)
            extraAxis->append(QString::number(i), i);
    }else{
        QList<int> values;
        int middleValue = (maxValue)/2;
        values.push_back(0);
        values.push_back(middleValue/2);
        values.push_back(middleValue);
        values.push_back(middleValue + (maxValue - middleValue)/ 2);
        values.push_back(maxValue);
        foreach(const auto& value, values){
            extraAxis->append(QString::number(value), value);
        }
    }
    extraAxis->setRange(0, maxValue);
    extraAxis->setLabelsPosition(QtCharts::QCategoryAxis::AxisLabelsPositionOnValue);
    return extraAxis;
}

QtCharts::QAbstractAxis* PlanningChartWidget::constructVerticalAxis(const int maxValue)
{
    auto axisY = new QtCharts::QValueAxis();
    axisY->setLabelsVisible(false);
    axisY->setRange(0, maxValue);
    axisY->setTitleText("Tasks");
    return axisY;
}

void PlanningChartWidget::chartThemeChanged(int index)
{
    BaseChartWidget::chartThemeChanged(index);
    foreach(auto series, ui->chartView->chart()->series()){
        auto planningSeries = static_cast<PlanningSeries*>(series);
        if (planningSeries!= nullptr){
            planningSeries->updateBrush();
        }
    }
    BaseChartWidget::m_chartWindow->keepBrush();
}

void PlanningChartWidget::onSeriesHover(const QPointF &point, bool state)
{
    Q_UNUSED(point);
    auto sender = static_cast<PlanningSeries*>(QObject::sender());
    if (sender == nullptr)
        return;
    if (state){
        auto seriesInfo = sender->getTaskInfo();
        sender->chart()->setToolTip(QString("Name : %1\nStart Time : %2\nFinish Time : %3").
                                    arg(QString::fromStdString(seriesInfo->TaskId)).
                                    arg(QString::number(seriesInfo->StartTime)).
                                    arg(QString::number(seriesInfo->FinishTime)));
    }else{
        sender->chart()->setToolTip("");
    }
}


