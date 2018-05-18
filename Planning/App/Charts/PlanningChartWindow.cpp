#include "PlanningChartWindow.h"
#include "ui_BaseChartWindow.h"
#include "PlanningSeries.h"

PlanningChartWindow::PlanningChartWindow(QWidget *parent) : BaseChartWindow(parent)
{

}

void PlanningChartWindow::keepBrush()
{
    foreach(auto series, getChart()->series()){
        auto planningSeries = static_cast<PlanningSeries*>(series);
        planningSeries->updateBrush();
    }
}
