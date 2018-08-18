#include "PlanningProxyModel.h"
#include "BaseModel.h"
#include "PlanningItem.h"

PlanningProxyModel::PlanningProxyModel(QObject *parent) : BaseProxyModel(parent)
{
    m_headerData<<"Task Id"<<"Start Time"<<"Finish Time";
}

PlanningProxyModel::~PlanningProxyModel()
{

}

void PlanningProxyModel::setFilterVector(const QVector<bool> &filterVector)
{
    if (m_filterVector != filterVector){
        m_filterVector = filterVector;
        invalidate();
    }
}

bool PlanningProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    BaseModel *dataModel = dynamic_cast<BaseModel*>(sourceModel());
    if(dataModel == nullptr){
        return false;
    }
    QModelIndex index = dataModel->index(source_row, 0, source_parent);
    PlanningItem *planningItem = static_cast<PlanningItem*>(dataModel->getItem(index));
    if (planningItem != nullptr){
        if (planningItem->getPhaseNumber() && m_filterVector.size() > planningItem->getPhaseNumber()){
            if (m_filterVector.at(planningItem->getPhaseNumber())){
                return true;
            }
            else return false;
        }
        return true;
    }
    return true;
}

bool PlanningProxyModel::filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    return (source_col < COLUMN_COUNT);
}
