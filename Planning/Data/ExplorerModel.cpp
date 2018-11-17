#include "ExplorerModel.h"
#include "ExplorerItem.h"
#include "QDebug"

ExplorerModel::ExplorerModel(QObject *parent) : BaseModel(parent)
{

}

ExplorerModel::~ExplorerModel()
{

}

bool ExplorerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = false;
    ExplorerItem* item = BaseModel::itemByIndexAs<ExplorerItem>(index);
    try{
        result = item->setData(index.column(), value, role);
    }catch(std::exception& ex){
        emit detectedMultipleNameDefinition(QString(ex.what()));
    }
    if (result){
        emit dataChanged(index, index);
    }
    return result;
}
