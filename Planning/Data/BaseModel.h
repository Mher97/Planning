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
        //Overrides from abstract class
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &index) const override;
        virtual bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
        virtual bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
        virtual bool insertColumns(int position, int columns, const QModelIndex &parent) override;
        virtual bool removeColumns(int position, int columns, const QModelIndex &parent) override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

        QModelIndex createIndex(int row, int column, BaseItem* data) const;
        QModelIndex insertItem(BaseItem* item, int position, const QModelIndex &parentIndex);
        QModelIndex insertItem(BaseItem* item); //Overloaded function to insert item under root item as last child.
        ItemType::Type itemType(const QModelIndex& index) const;
        virtual	QModelIndexList childList(const QModelIndex& parent,
                               const ItemType::Type& childType = ItemType::Type::Default, bool recursive = false) const;
        QModelIndex itemIndex(BaseItem* item) const;
        QModelIndex ancestorItemByType(QModelIndex index, ItemType::Type type) const;
        void deleteItem(const QModelIndex& index);
        void setRootItem(BaseItem* newRoot);
        BaseItem* getRootItem()const;
        void clearModel();

        template <typename T>
        static T* itemByIndexAs(const QModelIndex& index)
        {
            if (index.isValid())
            {
                Q_ASSERT(nullptr != index.internalPointer());
                return reinterpret_cast<T*>(index.internalPointer());
            }
            return nullptr;
        }

    protected:
        BaseItem *m_rootItem;
        void initRootItem();
        BaseItem* getItem(const QModelIndex &index) const;
};


#endif
