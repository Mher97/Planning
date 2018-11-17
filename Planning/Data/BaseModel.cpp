#include "BaseModel.h"
#include <QStringList>
#include <QFile>
#include "BaseItem.h"

BaseModel::BaseModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootItem(nullptr)
{
    initModel();
}

BaseModel::~BaseModel()
{

}

void BaseModel::initModel()
{
    initRootItem();
}

bool BaseModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = false;
    BaseItem* item = getItem(index);
    result = item->setData(index.column(), value,role );
    if (result){
        emit dataChanged(index, index);
    }
    return result;
}

QVariant BaseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }
    BaseItem *item = getItem(index);
    return item->data(index.column(),role);
}

bool BaseModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal){
        return false;
    }
    if( m_rootItem == nullptr ){
        return false;
    }
    bool result = m_rootItem->setData(section,value,role);
    if (result){
        emit headerDataChanged(orientation, section, section);
    }
    return result;
}

QVariant BaseModel::headerData(int section, Qt::Orientation orientation,int role) const
{
    if(m_rootItem == nullptr){
        return QVariant();
    }
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_rootItem->data(section,role);
    }
    return QVariant();
}

QModelIndex BaseModel::index(int row, int column, const QModelIndex &parent) const
{
        if (parent.isValid() && parent.column() != 0){
            return QModelIndex();
        }
        BaseItem *parentItem = getItem(parent);
        if(parentItem == nullptr){
            return QModelIndex();
        }
        if(row < 0 || row > parentItem->childCount()){
            return QModelIndex();
        }
        BaseItem *childItem = parentItem->child(row);
        return childItem != nullptr ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex BaseModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QModelIndex();
    }
    BaseItem *childItem = getItem(index);
    BaseItem *parentItem = childItem->parent();
    if (parentItem == nullptr){
        return QModelIndex();
    }
    if (parentItem == m_rootItem){
        return QModelIndex();
    }
    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool BaseModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    BaseItem *parentItem = getItem(parent);
    bool success = false;
    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows);
    endInsertRows();
    return success;
}

bool BaseModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    BaseItem *parentItem = getItem(parent);
    bool success = true;
    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    return success;
}

bool BaseModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    Q_ASSERT(m_rootItem != nullptr);
    bool success;
    beginInsertColumns(parent, position, position + columns - 1);
    success = m_rootItem->insertColumns(position, columns);
    endInsertColumns();
    return success;
}

bool BaseModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    Q_ASSERT(m_rootItem != nullptr);
    bool success;
    beginRemoveColumns(parent, position, position + columns - 1);
    success = m_rootItem->insertColumns(position, columns);
    endRemoveColumns();
    return success;
}

int BaseModel::rowCount(const QModelIndex &parent) const
{
    BaseItem *parentItem = getItem(parent);
    return parentItem != nullptr ? parentItem->childCount() : 0;
}

int BaseModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_rootItem->columnCount();
}

Qt::ItemFlags BaseModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
        return Qt::NoItemFlags;
    }
    BaseItem *item = getItem(index);
    return item->flags(index);
}

QModelIndex BaseModel::createIndex(int row, int column, BaseItem* data) const
{
    return QAbstractItemModel::createIndex(row, column, data);
}

QModelIndex BaseModel::insertItem(BaseItem* item)
{
    QModelIndex parentIndex = QModelIndex();
    BaseItem* parentItem = getItem(parentIndex);
    int position = parentItem->childCount();
    beginInsertRows( parentIndex, position, position );
    parentItem->insertChild(position, item);
    endInsertRows();
    return index(position, 0, parentIndex);
}

QModelIndex BaseModel::insertItem(BaseItem* item, int position, const QModelIndex &parentIndex)
{
    BaseItem* parentItem = getItem(parentIndex);
    beginInsertRows(parentIndex, position, position);
    parentItem->insertChild(position, item);
    endInsertRows();
    return index(position, 0, parentIndex);
}

void BaseModel::initRootItem()
{
    m_rootItem = new BaseItem();
}

ItemType::Type BaseModel::itemType(const QModelIndex& index) const
{
    BaseItem* item = getItem(index);
    return item->itemType();
}

QModelIndexList BaseModel::childList(const QModelIndex& parent, const ItemType::Type& childType, bool recursive) const
{
    QModelIndexList childList;
    for (int i = 0; i < rowCount(parent); ++i)
    {
        QModelIndex child = index(i, 0, parent);
        if (itemType(child) == childType || childType == ItemType::Type::Default){
            childList.append(child);
        }
        if (recursive){
            childList.append(this->childList(child, childType, recursive));
        }
    }
    return childList;
}

QModelIndex BaseModel::itemIndex(BaseItem* item) const
{
    Q_ASSERT(item != 0);
    QModelIndex result;
    int row = item->childNumber();
    return row != -1 ? result = createIndex(row, 0, item) : result;
}

QModelIndex BaseModel::ancestorItemByType(QModelIndex index, ItemType::Type type) const
{
    QModelIndex parentIndex = QModelIndex();
    if(index.isValid())
    {
        if(itemType(index) == type){
            parentIndex = index;
        }
        else
        {
            while(itemType(index) != ItemType::Type::RootItem)
            {
                index = parent(index);
                if(itemType(index) == type){
                    parentIndex = index;
                    break;
                }
            }
        }
    }
    return parentIndex;
}

void BaseModel::deleteItem(const QModelIndex& index)
{
    if (!index.isValid()){
        return;
    }
    removeRows(index.row(), 1, index.parent());
}

void BaseModel::setRootItem(BaseItem* newRoot)
{
    beginResetModel();
    m_rootItem = newRoot;
    endResetModel();
}

BaseItem* BaseModel::getRootItem()const
{
    return m_rootItem;
}

void BaseModel::clearModel()
{
    int RowCount = rowCount();
    for (int row = 0; row < RowCount; ++row){
        QModelIndex index = this->index(0,0);
        deleteItem(index);
    }
}

BaseItem* BaseModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        Q_ASSERT(nullptr != index.internalPointer());
        BaseItem *item = static_cast<BaseItem*>(index.internalPointer());
        if (item){
            return item;
        }
    }
    return m_rootItem;
}
