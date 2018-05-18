#ifndef WORKLOADCHART_H
#define WORKLOADCHART_H

#include "Charts/IChart.h"

class WorkLoadChart : public IChart
{
    public:
        explicit WorkLoadChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());
    protected:
        virtual void wheelEvent(QGraphicsSceneWheelEvent *event) override;
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // WORKLOADCHART_H
