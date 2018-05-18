#ifndef BASECHARTWINDOW_H
#define BASECHARTWINDOW_H

#include <QWidget>
#include <QtCharts/QChart>

namespace Ui {
class BaseChartWindow;
}

class BaseChartWindow : public QWidget
{
    Q_OBJECT
    public:
        explicit BaseChartWindow(QWidget *parent = nullptr);
        virtual ~BaseChartWindow();
        virtual void setChart(QtCharts::QChart *chart);
        virtual void keepBrush();
        QtCharts::QChart* getChart()const;
    private:
        Ui::BaseChartWindow *ui;
};

#endif // BASECHARTWINDOW_H
