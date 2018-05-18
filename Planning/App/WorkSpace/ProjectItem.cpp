#include "ProjectItem.h"

ProjectItem::ProjectItem(const QVector<QVariant>& data, BaseItem *parent, const int state) :
    WorkSpaceItem(data, parent, state)
{
    if (m_state == WorkSpaceItem::ISNEW){
        m_itemData[WorkSpaceItem::NAME_COLUMN] = ItemType::ItemTypetoString(ItemType::Type::ProjectItem);
    }
    m_itemType = ItemType::Type::ProjectItem;
}

ProjectItem::ProjectItem(const ProjectItem &src) : WorkSpaceItem(src)
{

}

ProjectItem::~ProjectItem()
{

}

ProjectItem *ProjectItem::clone() const
{
    return new ProjectItem(*this);
}

Qt::ItemFlags ProjectItem::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QIcon ProjectItem::icon() const
{
    return QIcon(":/Resources/Project.png");
}
