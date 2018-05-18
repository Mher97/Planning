#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QAbstractItemModel>
#include "ItemType.h"

class BaseItem;

class BaseModel : public QAbstractItemModel
{
    Q_OBJECT
    public:
        explicit BaseModel(QObject *parent = nullptr);
        virtual ~BaseModel();
        // After creating an istance of BaseTreeModel call the initModel function
        virtual void initModel();
        //Ovewrides from abstract class
        QVariant data(const QModelIndex &index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex &index) const override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        bool insertColumns(int position, int columns, const QModelIndex &parent);

        Qt::ItemFlags flags(const QModelIndex &index) const override;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
        bool setHeaderData(int section, Qt::Orientation orientation,
                           const QVariant &value, int role = Qt::EditRole) override;
        QModelIndex createIndex(int row, int column, BaseItem* data) const;
        QModelIndex insertItem(BaseItem* item, int position, const QModelIndex &parentIndex);
        QModelIndex insertItem(BaseItem* item); //Overloaded function to insert item under root item as last child.
        bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
        bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
        BaseItem* getItem(const QModelIndex &index) const;
        ItemType::Type itemType(const QModelIndex& index) const;

        virtual	QModelIndexList childList(const QModelIndex& parent,
                               const ItemType::Type& childType = ItemType::Type::Default, bool recursive = false) const;
        QModelIndex itemIndex(BaseItem* item) const;

        QModelIndex ancestorItemByType(QModelIndex index, ItemType::Type type) const;

        void deleteItem(const QModelIndex& index);
        void setRootItem(BaseItem* newRoot);
        BaseItem* getRootItem()const;
        void clearModel();
    protected:
        BaseItem *m_rootItem;
        void initRootItem(const QVector<QVariant>& data);
    public slots:
        virtual void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif
