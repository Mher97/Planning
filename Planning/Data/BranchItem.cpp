#include "BranchItem.h"

BranchItem::BranchItem(QVector<QVariant> data, BaseItem *parent, const int state)
    : WorkSpaceItem(data,parent, state)
{
    m_itemType = ItemType::Type::BranchItem;
    if (m_state == WorkSpaceItem::ISNEW){
        m_itemData[WorkSpaceItem::NAME_COLUMN] = ItemType::ItemTypetoString(ItemType::Type::BranchItem);
    }
}

BranchItem::BranchItem(const BranchItem &src) : WorkSpaceItem(src)
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
