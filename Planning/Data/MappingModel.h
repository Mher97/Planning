#ifndef MAPPINGMODEL_H
#define MAPPINGMODEL_H

#include "BaseModel.h"

class MappingModel : public BaseModel
{
    Q_OBJECT
    public:
        explicit MappingModel(QObject *parent = nullptr);
        virtual ~MappingModel();
        virtual bool isImportAllowed();
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    private:
        static const int COLUMN_COUNT = 4;
        void removeRowColumnChecks(const int row, const int column);
    signals:
        void importAllowed(bool value);
};

#endif // MAPPINGMODEL_H
