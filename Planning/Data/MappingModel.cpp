#include "MappingModel.h"
#include "MappingItem.h"

MappingModel::MappingModel(QObject *parent) : BaseModel(parent)
{

}

MappingModel::~MappingModel()
{

}

bool MappingModel::isImportAllowed()
{
    bool result = false;
    int mappedColumns = 0;
    for (int row = 0; row < rowCount(); ++row){
        QModelIndex index = this->index(row, 0);
        MappingItem *mappingItem = static_cast<MappingItem*>(getItem(index));
        int end = static_cast<int>(MappingItem::DATA_TYPES::DATA_DEPENDENCE);
        for (int col = 1; col <= end; ++col){
            mappingItem->data(col, Qt::CheckStateRole);
            mappedColumns += mappingItem->data(col, Qt::CheckStateRole).toBool();
        }
    }
    if (mappedColumns == static_cast<int>(MappingItem::DATA_TYPES::DATA_DEPENDENCE) -
            static_cast<int>(MappingItem::DATA_TYPES::COLUMN_RAW_NAME))
    {
        result = true;
    }
    return result;
}

bool MappingModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = false;
    BaseItem* item = getItem(index);
    MappingItem* mappingItem = static_cast<MappingItem*>(item);
    result = mappingItem->setData(index.column(), value, role );
    if(mappingItem != nullptr) {
        emit layoutAboutToBeChanged();
        removeRowColumnChecks(index.row(),index.column());
        emit layoutChanged();
        emit importAllowed(isImportAllowed());
    }
    if (result) {
        emit dataChanged(index, index);
    }
    return result;
}

void MappingModel::removeRowColumnChecks(const int row, const int column)
{
    for (int i = 0; i< rowCount(); ++i){
        QModelIndex index = this->index(i, 0);
        MappingItem *mappingItem = static_cast<MappingItem*>(getItem(index));
        if (mappingItem != nullptr){
            if (i == row){
                for (int j = 1; j < mappingItem->columnCount(); ++j){
                    if (j != column){
                        mappingItem->setData(j ,Qt::Unchecked, Qt::CheckStateRole);
                    }
                }
            }
            else{
                mappingItem->setData(column, Qt::Unchecked, Qt::CheckStateRole);
            }
        }
    }
}
