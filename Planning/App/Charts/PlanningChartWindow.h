#ifndef PLANNINGCHARTWINDOW_H
#define PLANNINGCHARTWINDOW_H

#include "BaseChartWindow.h"

class PlanningChartWindow : public BaseChartWindow
{
    public:
        explicit PlanningChartWindow(QWidget *parent = nullptr);
        void keepBrush() override;
};

#endif // PLANNINGCHARTWINDOW_H
