#ifndef MAPPINGPROXYMODEL_H
#define MAPPINGPROXYMODEL_H

#include "BaseProxyModel.h"

class MappingProxyModel : public BaseProxyModel
{
    Q_OBJECT
    public:
        static const int COLUMN_COUNT = 4;
        explicit MappingProxyModel(QObject *parent = nullptr);
        virtual ~MappingProxyModel();
    protected:
        virtual bool filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const override;
};

#endif // MAPPINGPROXYMODEL_H
