#ifndef WORKSPACECONTROLLER_H
#define WORKSPACECONTROLLER_H

#include <QList>
#include <array>
#include <ItemType.h>

class BaseModel;
class BaseItem;
class WorkSpaceItem;
class MonitorItem;

class WorkSpaceController
{
    public:
        static WorkSpaceController& getInstance();
        WorkSpaceController(const WorkSpaceController &) = delete;
        WorkSpaceController(WorkSpaceController &&) = delete;
        WorkSpaceController& operator = (const WorkSpaceController &) = delete;
        WorkSpaceController& operator = (WorkSpaceController&&) = delete;
        void saveModel(BaseModel *model, std::array<QList<int>, 2> &deletedItems);
        void loadModel(BaseModel *model);
        void loadImportedData(MonitorItem *monitorItem);
        void loadDataMapping(MonitorItem* monitorItem);
    private:
        WorkSpaceController();
        static const QStringList TableNames;
        static QString getTableNameByType(const ItemType::Type type);
        static ItemType::Type getNextLevelType(const ItemType::Type type);
        void deleteItems(std::array<QList<int>, 2> &deletedItems);
        void saveLevel(QList<BaseItem*> Items, const ItemType::Type type);
        void updateItem(WorkSpaceItem *item);
        void saveModelHelper(WorkSpaceItem *item, int deep);
        void saveImportedData(MonitorItem* monitorItem);
        void updateImportedData(MonitorItem* monitorItem);
        void saveDataMapping(MonitorItem* monitorItem);
        void updateDataMapping(MonitorItem* monitorItem);
        int m_lastInsertedItemId;
};

#endif // DATACONTROLLER_H
