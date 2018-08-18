#include "FilterItem.h"

void FilterItem::setData(const QVariant &value, int role, const FilterItem::DATA_CHANGE_TYPE type)
{
    QStandardItem::setData(value, role);
    if (type == DATA_CHANGE_TYPE::MANUAL){
        emit manualDataChanged(this);
    }
}
