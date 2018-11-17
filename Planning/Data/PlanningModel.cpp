#include "PlanningModel.h"
#include "PlanningItem.h"
#include "../Engine/Shceduler.h"

PlanningModel::PlanningModel(QObject *parent) : BaseModel(parent)
{

}

PlanningModel::~PlanningModel()
{

}

int PlanningModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

void PlanningModel::constructTree(const QList<QList<const TaskInfo*>>& data, const std::function<QColor(const int)>& colorIndexer)
{
    clearModel();
    if (data.isEmpty())
        return;
    int phaseNum = 1;
    auto rootItem = new PlanningItem(ItemType::ItemTypetoString(ItemType::Type::PlanningItem));
    insertItem(rootItem);
    foreach(const auto& phase, data){
        auto phaseColor = colorIndexer(phaseNum);
        auto planningAllItem = new PlanningItem("Phase(" + QString::number(phaseNum) + ")", 0, 0, phaseNum, phaseColor);
        insertItem(planningAllItem, phaseNum - 1, itemIndex(rootItem));
        int position = 0;
        foreach(const auto& taskInfo, phase){
            auto planningItem = new PlanningItem(QString::fromStdString(taskInfo->TaskId),
                                                 taskInfo->StartTime,
                                                 taskInfo->FinishTime,
                                                 phaseNum,
                                                 phaseColor);
            insertItem(planningItem, position, itemIndex(planningAllItem));
            ++position;
        }
        ++phaseNum;
    }
}
