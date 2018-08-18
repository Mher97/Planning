#ifndef WORKSPACEITEM_H
#define WORKSPACEITEM_H

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

class WorkSpaceItem : public BaseItem
{
    public:
        static const int ISNEW = 1;
        static const int FROMBASE = 2;
        static const int DELETEDFROMBASE = 3;
        static const int NAME_COLUMN = 0;
        static const int ID_COLUMN = 1;
        static const int PARENT_ID_COLUMN = 2;
        WorkSpaceItem(const QVector<QVariant>& data, BaseItem* parent = nullptr, const int state = WorkSpaceItem::ISNEW);
        WorkSpaceItem(const WorkSpaceItem& src);
        virtual ~WorkSpaceItem();
        //overrides from BaseItem
        virtual WorkSpaceItem* clone() const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual QVariant data(int column, int role) const override;
        virtual bool setData(int column, const QVariant &value, int role) override;
        void setState(const int state);
        int getState()const;
        void setEdited(const bool edited);
        bool getEdited() const;
        void setId(const int id);
        int getId() const;
        void setParentId(const int id);
        int getParentId() const;
        void setDeleted(const int deleted);
        bool isDeleted()const;
    protected:
        int m_state;               //read from base, or new, or deleted from base
        bool m_dirty;
        bool m_deleted;
};

#endif // WORKSPACEITEM_H
