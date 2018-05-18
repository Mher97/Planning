#ifndef PLANNINGMODEL_H
#define PLANNINGMODEL_H

#include "../BaseModel.h"

class TaskInfo;

class PlanningModel : public BaseModel
{
    Q_OBJECT
    public:
        explicit PlanningModel(QObject *parent = nullptr);
        virtual ~PlanningModel();
        void constructTree(const QList<QList<const TaskInfo*>>& data);
};

#endif // PLANNINGMODEL_H
