#ifndef PLANNINGCHART_H
#define PLANNINGCHART_H

#include <QtCharts/QChart>
#include "Charts/IChart.h"

class PlanningChart : public IChart
{
    public:
        explicit PlanningChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());
    protected:
        virtual void wheelEvent(QGraphicsSceneWheelEvent *event) override;
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override ;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // PLANNINGCHART_H
