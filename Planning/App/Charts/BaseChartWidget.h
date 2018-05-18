#ifndef BASECHARTWIDGET_H
#define BASECHARTWIDGET_H

#include <QWidget>
#include <QtCharts/QChart>

namespace Ui {
class BaseChartWidget;
}

class BaseChartWindow;

class BaseChartWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit BaseChartWidget(QWidget *parent = nullptr);
        virtual ~BaseChartWidget();
        void setTitle(const QString& title);
        QString getTitle()const;
        void closeChartWindow();
    private:
        void initUi();
        void initConnections();
    protected:
        Ui::BaseChartWidget *ui;
        BaseChartWindow *m_chartWindow;
        QtCharts::QChart::ChartTheme m_currentChartTheme;
        virtual void changeChart();
        virtual void updateChartView();
        virtual QtCharts::QChart* constructChart() = 0;
    protected slots:
        virtual void maximize();
        virtual void chartThemeChanged(int index);
};

#endif // BASECHARTWIDGET_H
