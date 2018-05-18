#ifndef CHARTSWIDGET_H
#define CHARTSWIDGET_H

#include <QWidget>

namespace Ui {
class ChartsWidget;
}

class TaskInfo;

class ChartsWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit ChartsWidget(QWidget *parent = nullptr);
        ~ChartsWidget();
        void updateCharts(const QList<QList<const TaskInfo*>>& data = {});
        void updateCharts(QList<QList<const TaskInfo*>>&& data = {});
        void reset();
    private:
        Ui::ChartsWidget *ui;
        void initUi();
};

#endif // RESULTWIDGET_H
