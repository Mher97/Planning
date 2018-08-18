#ifndef WORKSPACEPROXYMODEL_H
#define WORKSPACEPROXYMODEL_H

#include "BaseProxyModel.h"

class WorkSpaceProxyModel : public BaseProxyModel
{
    Q_OBJECT
    public:
        explicit WorkSpaceProxyModel(QObject *parent = nullptr);
        virtual ~WorkSpaceProxyModel();
    protected:
        virtual bool filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const override;
};

#endif // WORKSPACEPROXYMODEL_H
