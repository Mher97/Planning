#include "WorkSpaceItem.h"

WorkSpaceItem::WorkSpaceItem(const QVector<QVariant>& data, BaseItem *parent, const int state) : BaseItem(data, parent),
                                                m_state(state),m_dirty(false), m_deleted(false)
{
    m_itemType = ItemType::Type::WorkSpaceItem;
}

WorkSpaceItem::WorkSpaceItem(const WorkSpaceItem &src) : BaseItem(src), m_state(src.m_state),
                                                         m_dirty(src.m_dirty), m_deleted(src.m_deleted)
{

}

WorkSpaceItem::~WorkSpaceItem()
{

}

WorkSpaceItem *WorkSpaceItem::clone() const
{
    return new WorkSpaceItem(*this);
}

Qt::ItemFlags WorkSpaceItem::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void WorkSpaceItem::setState(const int state)
{
    m_state = state;
}

int WorkSpaceItem::getState() const
{
    return m_state;
}

void WorkSpaceItem::setEdited(const bool value)
{
    m_dirty = value;
}

bool WorkSpaceItem::getEdited() const
{
    return m_dirty;
}

int WorkSpaceItem::getId() const
{
    return m_itemData[WorkSpaceItem::ID_COLUMN].toInt();
}

void WorkSpaceItem::setId(const int id)
{
    m_itemData[WorkSpaceItem::ID_COLUMN] = id;
}

void WorkSpaceItem::setParentId(const int id)
{
    m_itemData[WorkSpaceItem::PARENT_ID_COLUMN] = id;
}

int WorkSpaceItem::getParentId() const
{
    return m_itemData[WorkSpaceItem::PARENT_ID_COLUMN].toInt();
}

void WorkSpaceItem::setDeleted(const int deleted)
{
    m_deleted = deleted;
}

bool WorkSpaceItem::isDeleted() const
{
    return m_deleted;
}




