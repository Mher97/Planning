#include "BaseItem.h"
#include <QStringList>

BaseItem::BaseItem(BaseItem *parent): QObject(parent),
    m_parentItem(parent),
    m_columnCount(1),
    m_name(),
    m_itemType(ItemType::Type::RootItem)
{

}

BaseItem::BaseItem(const BaseItem& src): QObject()
{
    m_name = src.m_name;
    m_itemType = src.m_itemType;
    for (int i = 0; i < src.childCount(); ++i)
    {
        BaseItem* newItem = src.child(i)->clone();
        insertChild(i, newItem);
    }
}

BaseItem::~BaseItem()
{
    qDeleteAll(m_childItems);
    m_childItems.clear();
}

BaseItem* BaseItem::clone() const
{
    return new BaseItem(*this);
}

BaseItem *BaseItem::parent() const
{
    return m_parentItem;
}

BaseItem* BaseItem::child(int number) const
{
    return m_childItems.value(number);
}

int BaseItem::childCount() const
{
    return m_childItems.count();
}

int BaseItem::columnCount() const
{
    return m_columnCount;
}

int BaseItem::childNumber() const
{
    if (m_parentItem){
        return m_parentItem->m_childItems.indexOf(const_cast<BaseItem*>(this));
    }
    return 0;
}

QVariant BaseItem::data(int column, int role) const
{
    if (column == 0){
        if (role == Qt::DisplayRole || role == Qt::EditRole){
            return getName();
        }
    }
    return QVariant();
}

bool BaseItem::setData(int column,const QVariant &value, int role)
{
    Q_UNUSED(role);
    if (column == 0){
        setName(value.toString());
        return true;
    }
    return false;
}

bool BaseItem::insertChildren(int position, int count)
{
    if (position < 0 || position > m_childItems.size()){
        return false;
    }
    for (int row = 0; row < count; ++row)
    {
        BaseItem *item = new BaseItem(this);
        m_childItems.insert(position, item);
    }
    return true;
}

void BaseItem::setParentItem(BaseItem * item)
{
    setParent(item);
    m_parentItem = item;
}

bool BaseItem::insertChild(int position, BaseItem * child)
{
    if (position < 0 || position > m_childItems.size()){
        return false;
    }
    m_childItems.insert(position, child);
    child->setParentItem(this);
    return true;
}

QList<BaseItem*> BaseItem::childList(const ItemType::Type childType, bool byDepth) const
{
    QList<BaseItem*> listToReturn;
    for (int i = 0; i < childCount(); ++i)
    {
        BaseItem* childItem = child(i);
        if (childItem->itemType() == childType || childType == ItemType::Type::Default){
            listToReturn.append(childItem);
        }
        if (byDepth){
            listToReturn.append(childItem->childList(childType, byDepth));
        }
    }
    return listToReturn;
}

bool BaseItem::removeChildren(int position, int count)
{
    if ((position < 0) || ((position + count) > m_childItems.size())){
        return false;
    }
    for (int row = 0; row < count; ++row)
    {
        BaseItem * item = m_childItems.takeAt(position);
        delete item;
        item = nullptr;
    }
    return true;
}

bool BaseItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > columnCount()){
        return false;
    }
    m_columnCount += columns;
    foreach (BaseItem *child, m_childItems)
    {
        child->insertColumns(position, columns);
    }
    return true;
}

bool BaseItem::removeColumns(int position, int columns)
{
    if (position < 0 || position > columnCount()){
        return false;
    }
    m_columnCount -= columns;
    foreach (BaseItem *child, m_childItems)
    {
        child->removeColumns(position, columns);
    }
    return true;
}

Qt::ItemFlags BaseItem::flags(const QModelIndex &index) const
{
    return index.isValid() ? Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable :  Qt::NoItemFlags;
}

ItemType::Type BaseItem::itemType() const
{
    return m_itemType;
}

QString BaseItem::tagName() const
{
   return ItemType::ItemTypetoString(m_itemType);
}

QIcon BaseItem::icon() const
{
    return QIcon();
}

void BaseItem::setName(const QString &name)
{
    m_name = name;
}

QString BaseItem::getName() const
{
    return m_name;
}

