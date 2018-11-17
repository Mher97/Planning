#include "PlanningItem.h"

PlanningItem::PlanningItem(const QString &taskName,
                           const int startTime,
                           const int finishTime,
                           const int phaseNumber,
                           QColor phaseColor,
                           BaseItem *parent):
    BaseItem(parent),
    m_startTime(startTime),
    m_finishTime(finishTime),
    m_phaseNumber(phaseNumber),
    m_phaseColor(phaseColor)
{
    m_name = taskName;
    m_itemType = ItemType::Type::PlanningItem;
}

PlanningItem::~PlanningItem()
{

}

PlanningItem *PlanningItem::clone() const
{
    return new PlanningItem(*this);
}

int PlanningItem::columnCount() const
{
    return 3;
}

bool PlanningItem::setData(int column, const QVariant &value, int role)
{
    Q_UNUSED(role);
    switch(column){
        case NAME_COLUMN:{
           setName(value.toString());
           return true;
        }
        case START_COLUMN:{
            setStartTime(value.toInt());
            return true;
        }
        case FINISH_COLUMN:{
            setFinishTime(value.toInt());
            return true;
        }
        default:
            return false;
    }
    return false;
}

QVariant PlanningItem::data(int column, int role) const
{
    if (role == Qt::DecorationRole && column == 0){
        return getPhaseColor();
    }else if (role == Qt::DisplayRole){
        switch(column){
            case NAME_COLUMN:
                return getName();
            case START_COLUMN:
                return m_startTime;
            case FINISH_COLUMN:
                return m_finishTime;
            default:
                return QVariant();
        }
    }
    else{
        return QVariant();
    }
}

Qt::ItemFlags PlanningItem::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void PlanningItem::setPhaseNumber(const int phaseNumber)
{
    m_phaseNumber = phaseNumber;
}

int PlanningItem::getPhaseNumber() const
{
    return m_phaseNumber;
}

void PlanningItem::setPhaseColor(const QColor &phaseColor)
{
    m_phaseColor = phaseColor;
}

QColor PlanningItem::getPhaseColor() const
{
    return m_phaseColor;
}

int PlanningItem::getStartTime() const
{
    return m_startTime;
}

int PlanningItem::getFinishTime() const
{
    return m_finishTime;
}

void PlanningItem::setFinishTime(const int finishTime)
{
    m_finishTime = finishTime;
}

void PlanningItem::setStartTime(const int startTime)
{
    m_startTime = startTime;
}


