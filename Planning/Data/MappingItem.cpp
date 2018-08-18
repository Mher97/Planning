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

MappingItem::MappingItem(BaseItem *parent) : BaseItem(parent), m_columnNumber(-1)
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


QVariant MappingItem::data(int column, int role) const
{
    if (column < 0 || column >= m_itemData.size())
           return QVariant();
    if (role == Qt::CheckStateRole){
        if(column > static_cast<int>(MappingItem::DATA_TYPES::COLUMN_RAW_NAME)){
            return ((m_itemData[column] == Qt::Checked) ? Qt::Checked : Qt::Unchecked );
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
    if (column < 0 || column >= m_itemData.size())
           return false;
    if ((role == Qt::CheckStateRole)&& (column > static_cast<int>(MappingItem::DATA_TYPES::COLUMN_RAW_NAME))) {
        m_itemData[column] = value;
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
    int start = static_cast<int>(MappingItem::DATA_TYPES::COLUMN_RAW_NAME);
    int end = static_cast<int>(MappingItem::DATA_TYPES::DATA_DEPENDENCE);
    for (int i = start; i <= end; ++i) {
        m_itemData[i] = Qt::Unchecked;
    }
}
