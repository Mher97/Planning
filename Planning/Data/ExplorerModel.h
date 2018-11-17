#ifndef EXPLORERMODEL_H
#define EXPLORERMODEL_H

#include "BaseModel.h"

class ExplorerModel : public BaseModel
{
    Q_OBJECT
    public:
        explicit ExplorerModel(QObject *parent = nullptr);
        virtual ~ExplorerModel();
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    signals:
        void detectedMultipleNameDefinition(const QString& message);
};

#endif // WORKSPACEMODEL_H
