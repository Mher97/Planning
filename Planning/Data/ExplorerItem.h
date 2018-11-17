#ifndef EXPLORERITEM_H
#define EXPLORERITEM_H

#include "BaseItem.h"

class MultipleNameDefinition : public std::exception
{
    public:
        template <typename T>
        MultipleNameDefinition(T&& errorText) : std::exception(), m_errorText(std::forward<T>(errorText))
        {

        }
        virtual const char* what() const noexcept override
        {
            auto stdStr = m_errorText.toStdString();
            char *result = new char[stdStr.length() + 1]();
            memcpy(result, stdStr.c_str(), stdStr.length());
            return result;
        }
    private:
        QString m_errorText;
};

class ExplorerItem : public BaseItem
{
    public:
        enum class ItemState : int
        {
            NEW = 0,
            FROMBASE = 1,
            DELETEDFROMBASE = 2,
        };
        ExplorerItem(BaseItem* parent = nullptr, const ItemState state = ItemState::NEW);
        ExplorerItem(const ExplorerItem& src);
        virtual ~ExplorerItem();
        //overrides from BaseItem
        virtual ExplorerItem* clone() const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual QVariant data(int column, int role) const override;
        virtual bool setData(int column, const QVariant &value, int role) override;
        virtual QIcon icon() const;
        bool nameIsAllowed(const QString& name) const;
        bool nameIsAllowedForChild(const QString& name, const ItemType::Type childType) const;
        QString uniqueNameForChild(const QString& name, const ItemType::Type childType) const;
        QString baseName(const QString& name) const;

        void setState(const ItemState state);
        ItemState getState()const;
        void setDirty(const bool edited);
        bool isDirty() const;
        void setId(const int id);
        int getId() const;
        void setParentId(const int parentId);
        int getParentId() const;
        void setDeleted(const int deleted);
        bool isDeleted()const;
    protected:
        int m_dbId;
        int m_dbParentId;
        ItemState m_state;               //read from base, or new, or deleted from base
        bool m_dirty;
        bool m_deleted;
};

#endif // EXPLORERITEM_H
