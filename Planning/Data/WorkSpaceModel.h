#ifndef WORKSPACEMODEL_H
#define WORKSPACEMODEL_H

#include "BaseModel.h"

class WorkSpaceModel : public BaseModel
{
    Q_OBJECT
    public:
        explicit WorkSpaceModel(QObject *parent = nullptr);
        virtual ~WorkSpaceModel();
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    signals:
        void detectedMultipleNameDefinition(const QString& message);
};

#endif // WORKSPACEMODEL_H
