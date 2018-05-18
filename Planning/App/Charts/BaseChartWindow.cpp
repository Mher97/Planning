#include "BaseChartWindow.h"
#include "ui_BaseChartWindow.h"

BaseChartWindow::BaseChartWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseChartWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->scrollArea->setWidget(ui->chartView);
    setLayout(ui->mainLayout);
}

BaseChartWindow::~BaseChartWindow()
{
    delete ui;
}

void BaseChartWindow::setChart(QtCharts::QChart *chart)
{
    if (chart != nullptr){
        chart->update(ui->chartView->chart()->rect());
        ui->chartView->setChart(chart);
        ui->chartView->setMinimumWidth(chart->minimumWidth());
    }
}

void BaseChartWindow::keepBrush()
{

}

QtCharts::QChart *BaseChartWindow::getChart() const
{
    return ui->chartView->chart();
}
