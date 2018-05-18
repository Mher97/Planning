#include "ChartsWidget.h"
#include "ui_ChartsWidget.h"

ChartsWidget::ChartsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartsWidget)
{
    ui->setupUi(this);
    initUi();
}

ChartsWidget::~ChartsWidget()
{
    delete ui;
}

void ChartsWidget::updateCharts(const QList<QList<const TaskInfo*>> &data)
{
    ui->planningChartWidget->updateChart(data);
    ui->workLoadChartWidget->updateChart(data);
}

void ChartsWidget::updateCharts(QList<QList<const TaskInfo*>> &&data)
{
    ui->planningChartWidget->updateChart(data);
    ui->workLoadChartWidget->updateChart(data);
}

void ChartsWidget::reset()
{
    ui->planningChartWidget->closeChartWindow();
    ui->workLoadChartWidget->closeChartWindow();
}

void ChartsWidget::initUi()
{
    setLayout(ui->gridLayout);
    ui->planningChartWidget->setTitle("Planning Chart  ");
    ui->workLoadChartWidget->setTitle("Work Load Chart");
}



