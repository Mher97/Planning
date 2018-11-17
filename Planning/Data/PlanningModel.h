#ifndef PLANNINGMODEL_H
#define PLANNINGMODEL_H

#include "BaseModel.h"
#include <functional>

class TaskInfo;

class PlanningModel : public BaseModel
{
    Q_OBJECT
    public:
        explicit PlanningModel(QObject *parent = nullptr);
        virtual ~PlanningModel();
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        void constructTree(const QList<QList<const TaskInfo*>>& data, const std::function<QColor(const int)>& colorIndexer);
    private:
        static const int COLUMN_COUNT = 3;
};

#endif // PLANNINGMODEL_H
