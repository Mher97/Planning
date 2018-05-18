#include "PlanningItem.h"
#include "../Globals.h"

PlanningItem::PlanningItem(const int phase, const QVector<QVariant> &data, BaseItem *parent)
    : BaseItem(data, parent), m_phaseNumber(phase)
{
    m_itemType = ItemType::Type::PlanningItem;
}

PlanningItem::PlanningItem(const int phase, BaseItem *parent) : BaseItem(parent), m_phaseNumber(phase)
{
    m_itemType = ItemType::Type::PlanningItem;
}

PlanningItem::~PlanningItem()
{

}

PlanningItem *PlanningItem::clone() const
{
    return new PlanningItem(*this);
}

QVariant PlanningItem::data(int column, int role) const
{
    if (role == Qt::DecorationRole && column == 0){
        if (!m_phaseNumber){
            return QColor("#000000");
        }
        return Globals::getColorForPhase(m_phaseNumber);
    }else{
        return BaseItem::data(column, role);
    }
}

Qt::ItemFlags PlanningItem::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int PlanningItem::getPhaseNumber() const
{
    return m_phaseNumber;
}
