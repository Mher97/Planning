#ifndef WORKLOADSERIES_H
#define WORKLOADSERIES_H

#include "QtCharts/QLineSeries"

class WorkLoadSeries : public QtCharts::QLineSeries
{
    Q_OBJECT
    public:
        WorkLoadSeries(QObject* parent = nullptr);
};

#endif // WORKLOADSERIES_H
