#include "BaseItem.h"
#include <QStringList>
//#include "Globals.h"

BaseItem::BaseItem(const QVector<QVariant>& data, BaseItem *parent): QObject(parent),
    m_itemData(data),
    m_parentItem(parent),
    m_itemType(ItemType::Type::RootItem)
{

}

BaseItem::BaseItem(BaseItem *parent): QObject(parent),
    m_parentItem(parent),
    m_itemType(ItemType::Type::RootItem)
{
    m_itemData = QVector<QVariant>(25);
}

BaseItem::BaseItem(const BaseItem& src): QObject()
{
    m_itemData = src.m_itemData;
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

int BaseItem::childNumber() const
{
    if (m_parentItem){
        return m_parentItem->m_childItems.indexOf(const_cast<BaseItem*>(this));
    }
    return 0;
}

int BaseItem::columnCount() const
{
    return m_itemData.count();
}


QVariant BaseItem::data(int column, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole){
        return m_itemData.value(column);
    }else if (role == Qt::DecorationRole){
        return icon();
    }
    return QVariant();
}

bool BaseItem::setData(int column,const QVariant &value, int role)
{
    Q_UNUSED(role);
    if (column < 0 || column >= m_itemData.size())
    {
        return false;
    }
    m_itemData[column] = value;
    return true;
}

bool BaseItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > m_childItems.size()){
        return false;
    }
    for (int row = 0; row < count; ++row)
    {
        QVector<QVariant> data(columns);
        BaseItem *item = new BaseItem(data, this);
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
    if (position < 0 || position > m_itemData.size()){
        return false;
    }
    for (int column = 0; column < columns; ++column)
    {
        m_itemData.insert(position, QVariant());
    }
    foreach (BaseItem *child, m_childItems)
    {
        child->insertColumns(position, columns);
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
    if (m_itemData.size()){
        m_itemData[0] = name;
    }
}

QString BaseItem::getName() const
{
    if (m_itemData.size()){
        return m_itemData[0].toString();
    }
    return QString();
}

bool BaseItem::nameIsAllowed(const QString& name) const
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

bool BaseItem::nameIsAllowedForChild(const QString& name, const ItemType::Type childType) const
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

QString BaseItem::uniqueNameForChild(const QString& name, const ItemType::Type childType) const
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

QString BaseItem::baseName(const QString& name) const
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

void BaseItem::setItemData(const QVector<QVariant>& itemDataVector)
{
    m_itemData.clear();
    m_itemData = itemDataVector;
}
