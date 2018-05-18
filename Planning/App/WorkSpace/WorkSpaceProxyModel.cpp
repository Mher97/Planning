#include "WorkSpaceProxyModel.h"

WorkSpaceProxyModel::WorkSpaceProxyModel(QObject *parent) : BaseProxyModel(parent)
{

}

WorkSpaceProxyModel::~WorkSpaceProxyModel()
{

}

bool WorkSpaceProxyModel::filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    return (source_col < 1);
}

