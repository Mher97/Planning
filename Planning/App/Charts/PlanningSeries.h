#ifndef PLANNINGSERIES_H
#define PLANNINGSERIES_H

#include <QtCharts/QAreaSeries>

class TaskInfo;

class PlanningSeries : public QtCharts::QAreaSeries
{
    public:
        template <typename ...Args>
        PlanningSeries(const TaskInfo* taskInfo, const int phaseNumber, Args&& ... args) :
            QtCharts::QAreaSeries(std::forward<Args>(args)...), m_taskInfo(const_cast<TaskInfo*>(taskInfo)), m_phaseNumber(phaseNumber)
        {
            init();
        }
        void updateBrush();
        TaskInfo* getTaskInfo()const;
    private:
        TaskInfo* m_taskInfo;
        int m_phaseNumber;
        void init();
};

#endif // PLANNINGSERIES_H
