#include "PlanningModel.h"
#include "PlanningItem.h"
#include "../Engine/Shceduler.h"

PlanningModel::PlanningModel(QObject *parent) : BaseModel(parent)
{

}

PlanningModel::~PlanningModel()
{

}

void PlanningModel::constructTree(const QList<QList<const TaskInfo*>>& data)
{
    clearModel();
    if (data.isEmpty())
        return;
    int phaseNum = 1;
    auto rootItem = new PlanningItem();
    rootItem->setName(ItemType::ItemTypetoString(ItemType::Type::PlanningItem));
    insertItem(rootItem);
    foreach(const auto& phase, data){
        auto phaseItem = new PlanningItem(phaseNum);
        phaseItem->setName("Phase(" + QString::number(phaseNum) + ")");
        insertItem(phaseItem, phaseNum - 1, itemIndex(rootItem));
        int position = 0;
        foreach(const auto& taskInfo, phase){
            auto taskItem= new PlanningItem(phaseNum);
            taskItem->setName(QString::fromStdString(taskInfo->TaskId));
            taskItem->setData(PlanningItem::START_COLUMN, QString("\t") + QString::number(taskInfo->StartTime), Qt::EditRole);
            taskItem->setData(PlanningItem::FINISH_COLUMN, QString("\t") +QString::number(taskInfo->FinishTime), Qt::EditRole);
            insertItem(taskItem, position, itemIndex(phaseItem));
            ++position;
        }
        ++phaseNum;
    }
}
