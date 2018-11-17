#ifndef EXPLORERPROXYMODEL_H
#define EXPLORERPROXYMODEL_H

#include "BaseProxyModel.h"

class ExplorerProxyModel : public BaseProxyModel
{
    Q_OBJECT
    public:
        explicit ExplorerProxyModel(QObject *parent = nullptr);
        virtual ~ExplorerProxyModel();
    protected:
        virtual bool filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const override;
};

#endif // EXPLORERPROXYMODEL_H
