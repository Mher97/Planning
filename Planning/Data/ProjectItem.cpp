#include "ProjectItem.h"

ProjectItem::ProjectItem(BaseItem *parent, const ExplorerItem::ItemState state) :
    ExplorerItem(parent, state)
{
    if (m_state == ItemState::NEW){
        m_name = ItemType::ItemTypetoString(ItemType::Type::ProjectItem);
    }
    m_itemType = ItemType::Type::ProjectItem;
}

ProjectItem::ProjectItem(const ProjectItem &src) : ExplorerItem(src)
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
