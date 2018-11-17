#include "BranchItem.h"

BranchItem::BranchItem(BaseItem *parent, const ExplorerItem::ItemState state)
    : ExplorerItem(parent, state)
{
    m_itemType = ItemType::Type::BranchItem;
    if (m_state == ExplorerItem::ItemState::NEW){
        m_name = ItemType::ItemTypetoString(ItemType::Type::BranchItem);
    }
}

BranchItem::BranchItem(const BranchItem &src) : ExplorerItem(src)
{

}

BranchItem::~BranchItem()
{

}

BranchItem* BranchItem::clone() const
{
    return new BranchItem(*this);
}

QIcon BranchItem::icon() const
{
    return QIcon(":/Resources/branch.png");
}

Qt::ItemFlags BranchItem::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
