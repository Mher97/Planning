#include "IChart.h"
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>

IChart::IChart(QGraphicsItem *parent, Qt::WindowFlags wFlags) : QtCharts::QChart(parent, wFlags)
{

}

IChart::~IChart()
{

}

void IChart::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->accept();
    if (event->delta() > 0)
        zoomIn();
    else zoomOut();
}

void IChart::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        m_pressed = true;
        m_pressedPoint = event->pos();
    }
}

void IChart::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    m_pressed = false;
}

void IChart::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_pressed)
    {
        scroll(-event->pos().x() + m_pressedPoint.x(), event->pos().y() - m_pressedPoint.y());
        m_pressedPoint = event->pos();
    }
}
