#include "ExplorerProxyModel.h"

ExplorerProxyModel::ExplorerProxyModel(QObject *parent) : BaseProxyModel(parent)
{

}

ExplorerProxyModel::~ExplorerProxyModel()
{

}

bool ExplorerProxyModel::filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    return (source_col < 1);
}
