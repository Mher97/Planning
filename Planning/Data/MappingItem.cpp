#include "MappingItem.h"

QString MappingItem::dataTypesToString(MappingItem::DATA_TYPES type)
{
    switch (type)
    {
        case MappingItem::DATA_TYPES::COLUMN_RAW_NAME:
            return "Column Name";
        case MappingItem::DATA_TYPES::DATA_TASK_CODE:
            return "Task Id";
        case MappingItem::DATA_TYPES::DATA_DURATION:
            return "Task Duration";
        case MappingItem::DATA_TYPES::DATA_DEPENDENCE:
            return "Depended Tasks";
        default:
            return {};
    }
}

MappingItem::MappingItem(BaseItem *parent) :
    BaseItem(parent),
    m_mappingVector(QVector<QVariant>(3)),
    m_rawColName(),
    m_columnNumber(-1)
{
    m_itemType = ItemType::Type::MappingItem;
}

MappingItem::~MappingItem()
{

}

void MappingItem::setRawColName(const QString& colName)
{
    m_rawColName = colName;
}

QString MappingItem::getRawColName() const
{
    return m_rawColName;
}

void MappingItem::setColumnNumber(const int number)
{
    m_columnNumber = number;
}

int MappingItem::getColumnNumber() const
{
    return m_columnNumber;
}

MappingItem *MappingItem::clone() const
{
    return new MappingItem(*this);
}

int MappingItem::columnCount() const
{
    return m_mappingVector.size() + 1;
}

QVariant MappingItem::data(int column, int role) const
{
    if (column < 0 || column >= columnCount())
           return QVariant();
    if (role == Qt::CheckStateRole){
        if(column > static_cast<int>(MappingItem::DATA_TYPES::COLUMN_RAW_NAME)){
            return ((m_mappingVector[column - 1] == Qt::Checked) ? Qt::Checked : Qt::Unchecked );
        }else{
            return QVariant();
        }
    }
    if (role == Qt::DisplayRole) {
        if (column == static_cast<int>(MappingItem::DATA_TYPES::COLUMN_RAW_NAME)) {
            return QVariant(getRawColName());
        }
    }
    return QVariant();
}

bool MappingItem::setData(int column, const QVariant &value, int role)
{
    if (column < 0 || column >= columnCount())
           return false;
    if (column == 0){
        setName(value.toString());
    }
    if ((role == Qt::CheckStateRole)&& (column > static_cast<int>(MappingItem::DATA_TYPES::COLUMN_RAW_NAME))) {
        m_mappingVector[column - 1] = value;
        return true;
    }
    return false;
}

Qt::ItemFlags MappingItem::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return flags;
}

void MappingItem::clearSelections()
{
    int start = static_cast<int>(MappingItem::DATA_TYPES::DATA_TASK_CODE);
    int end = static_cast<int>(MappingItem::DATA_TYPES::DATA_DEPENDENCE);
    for (int i = start; i <= end; ++i) {
        m_mappingVector[i] = Qt::Unchecked;
    }
}
