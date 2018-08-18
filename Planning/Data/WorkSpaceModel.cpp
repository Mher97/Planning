#include "WorkSpaceModel.h"
#include "BaseItem.h"
#include "QDebug"

WorkSpaceModel::WorkSpaceModel(QObject *parent) : BaseModel(parent)
{

}

WorkSpaceModel::~WorkSpaceModel()
{

}

bool WorkSpaceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = false;
    BaseItem* item = getItem(index);
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
