#include "BaseProxyModel.h"
#include "BaseModel.h"
#include "BaseItem.h"

BaseProxyModel::BaseProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

BaseProxyModel::~BaseProxyModel()
{

}

bool BaseProxyModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) {
        return false;
    }
    auto model = sourceModel();
    if (model == nullptr) {
        return false;
    }
    return model->setData(mapToSource(index), value, role);
}

QVariant BaseProxyModel::data(const QModelIndex &index, int role) const
{
    QModelIndex souIndex = mapToSource(index);
    if (!souIndex.isValid()){
        return QVariant();
    }
    BaseModel *dataModel = dynamic_cast<BaseModel*>(sourceModel());
    if(dataModel == nullptr){
        return QVariant();
    }
    BaseItem *item = dataModel->getItem(souIndex);
    if(souIndex.column() == 0 && role == Qt::CheckStateRole){
        return QVariant();
    }
    return item->data(souIndex.column(),role);
}

QVariant BaseProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < m_headerData.size()  && (orientation == Qt::Horizontal && role == Qt::DisplayRole)) {
        return m_headerData.at(section);
    }
    return QVariant();
}

bool BaseProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_row);
    Q_UNUSED(source_parent);
    return true;
}

bool BaseProxyModel::filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_col);
    Q_UNUSED(source_parent)
    return true;
}
