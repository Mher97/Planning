#include "WorkLoadChart.h"

WorkLoadChart::WorkLoadChart(QGraphicsItem *parent, Qt::WindowFlags wFlags) : IChart(parent, wFlags)
{

}

void WorkLoadChart::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    IChart::wheelEvent(event);
}

void WorkLoadChart::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    IChart::mousePressEvent(event);
}

void WorkLoadChart::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    IChart::mouseReleaseEvent(event);
}

void WorkLoadChart::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    IChart::mouseMoveEvent(event);
}
