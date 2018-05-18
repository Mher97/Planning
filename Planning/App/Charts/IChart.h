#ifndef ICHART_H
#define ICHART_H

#include <QtCharts/QChart>

class IChart : public QtCharts::QChart
{
    public:
        explicit IChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());
        virtual ~IChart();
    private:
        bool m_pressed;
        QPointF m_pressedPoint;
    protected:
        virtual void wheelEvent(QGraphicsSceneWheelEvent *event) override = 0;
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override = 0;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override = 0;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override = 0;
};

#endif // ICHART_H
