#include "ExplorerItem.h"

ExplorerItem::ExplorerItem(BaseItem *parent, const ExplorerItem::ItemState state) :
    BaseItem(parent),
    m_dbId(0),
    m_dbParentId(0),
    m_state(state),
    m_dirty(false),
    m_deleted(false)
{
    m_itemType = ItemType::Type::ExplorerItem;
}

ExplorerItem::ExplorerItem(const ExplorerItem &src) :
    BaseItem(src),
    m_state(src.m_state),
    m_dirty(src.m_dirty),
    m_deleted(src.m_deleted)
{

}

ExplorerItem::~ExplorerItem()
{

}

ExplorerItem *ExplorerItem::clone() const
{
    return new ExplorerItem(*this);
}

QVariant ExplorerItem::data(int column, int role) const
{
    if (column == 0){
        if (role == Qt::DisplayRole || role == Qt::EditRole){
            return getName();
        }else if (role == Qt::DecorationRole){
            return icon();
        }
    }
    return QVariant();
}

bool ExplorerItem::setData(int column, const QVariant &value, int role)
{
    if(column == 0)
    {
        QString name = value.toString().trimmed();
        if(name.isEmpty()){
            return false;
        }
        if(nameIsAllowed(name)){
            setName(name);
            return true;
        }else{
            throw MultipleNameDefinition(QString("Item with name '%1' already exists.").arg(name));
        }
    }else{
        return BaseItem::setData(column, value, role);
    }
}

Qt::ItemFlags ExplorerItem::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QIcon ExplorerItem::icon() const
{
    return QIcon();
}

bool ExplorerItem::nameIsAllowed(const QString& name) const
{
    if (!m_parentItem){
        return true;
    }
    for (int i = 0; i < m_parentItem->childCount(); ++i)
    {
        BaseItem* child = m_parentItem->child(i);
        if (child != this && child->itemType() == this->itemType() && child->getName() == name)
        {
            return false;
        }
    }
    return true;
}

bool ExplorerItem::nameIsAllowedForChild(const QString& name, const ItemType::Type childType) const
{
    // Check if name exists for one of the children.
    for (int i = 0; i < this->childCount(); ++i)
    {
        BaseItem* childItem = this->child(i);
        if (childItem->itemType() == childType && childItem->getName() == name){
            return false;
        }
    }
    return true;
}

QString ExplorerItem::uniqueNameForChild(const QString& name, const ItemType::Type childType) const
{
    Q_ASSERT(!name.isEmpty());
    QString base = baseName(name);
    QString newName = base;
    int id = 1;
    while (!nameIsAllowedForChild(newName, childType))
    {
        ++id;
        newName = base + "(" + QString::number(id) + ")";
    }
    return newName;
}

QString ExplorerItem::baseName(const QString& name) const
{
    Q_ASSERT(!name.isEmpty());
    QString strippedBaseName = name;
    QRegExp rx("^(.*)\\(\\d+\\)$");
    if (rx.indexIn(strippedBaseName) != -1){
        strippedBaseName = rx.cap(1);
    }
    // Strip leading and trailing whitespaces
    strippedBaseName = strippedBaseName.trimmed();
    return strippedBaseName;
}


void ExplorerItem::setState(const ItemState state)
{
    m_state = state;
}

ExplorerItem::ItemState ExplorerItem::getState() const
{
    return m_state;
}

void ExplorerItem::setDirty(const bool value)
{
    m_dirty = value;
}

bool ExplorerItem::isDirty() const
{
    return m_dirty;
}

int ExplorerItem::getId() const
{
    return m_dbId;
}

void ExplorerItem::setId(const int id)
{
    m_dbId = id;
}

void ExplorerItem::setParentId(const int parentId)
{
    m_dbParentId = parentId;
}

int ExplorerItem::getParentId() const
{
    return m_dbParentId;
}

void ExplorerItem::setDeleted(const int deleted)
{
    m_deleted = deleted;
}

bool ExplorerItem::isDeleted() const
{
    return m_deleted;
}


