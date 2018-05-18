#include "PlanningChart.h"

PlanningChart::PlanningChart(QGraphicsItem *parent, Qt::WindowFlags wFlags) : IChart(parent, wFlags)
{

}

void PlanningChart::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    IChart::wheelEvent(event);
}

void PlanningChart::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    IChart::mousePressEvent(event);
}

void PlanningChart::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    IChart::mouseReleaseEvent(event);
}

void PlanningChart::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    IChart::mouseMoveEvent(event);
}


