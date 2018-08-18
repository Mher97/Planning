#include "FilterModel.h"
#include "FilterItem.h"

bool FilterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    auto filterItem = static_cast<FilterItem*>(this->item(index.row(), index.column()));
    if (filterItem != nullptr){
        filterItem->setData(value, role);
        return true;
    }
    return false;
}
