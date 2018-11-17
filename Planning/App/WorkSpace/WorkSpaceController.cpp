#include "WorkSpaceController.h"
#include "../DbManager.h"
#include "../Data/BaseModel.h"
#include "../Data/ExplorerItem.h"
#include "../Data/ProjectItem.h"
#include "../Data/BranchItem.h"
#include "../Data/MonitorItem.h"
#include "../Data/ExplorerProxyModel.h"
#include "../Globals.h"

const QStringList WorkSpaceController::TableNames = QStringList()<< "BranchItems"
                                                                 <<"MonitorItems";

WorkSpaceController::WorkSpaceController() : m_lastInsertedItemId(0)
{

}

QString WorkSpaceController::getTableNameByType(const ItemType::Type type)
{
    switch(type){
        case ItemType::Type::BranchItem:
            return TableNames[0];
        case ItemType::Type::MonitorItem:
            return TableNames[1];
        default:
            return "";
    }
}

ItemType::Type WorkSpaceController::getNextLevelType(const ItemType::Type type)
{
    switch(type){
        case ItemType::Type::ProjectItem:
            return ItemType::Type::BranchItem;
        case ItemType::Type::BranchItem:
            return ItemType::Type::MonitorItem;
        default:
            return ItemType::Type::Default;
    }
}

WorkSpaceController& WorkSpaceController::getInstance()
{
    static WorkSpaceController instance;
    return instance;
}

void WorkSpaceController::loadModel(BaseModel *model)
{
    ProjectItem *projectItem = new ProjectItem();
    BranchItem *branchItem(nullptr);
    MonitorItem *monitorItem(nullptr);
    model->insertItem(projectItem);
    QList<QVector<QVariant> > result;
    int branchId = 0;
    int monitorId = 0;
    static const int BranchLength = 2;
    static const int MonitorLength = 3;
    static const int BranchNameColumn = 0;
    static const int BranchIdColumn = 1;
    static const int MonitorNameColumn = 2;
    static const int MonitorIdColumn = 3;
    QString queryText("Select * from BranchItems Left Join MonitorItems on BranchItems.id = MonitorItems.parentId "
                      "Order By BranchItems.id, MonitorItems.id;");
    try{

        result = DbManager::getInstance()->selectQuery(queryText);
    }
    catch(std::exception& ex){
        Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
    }
    foreach(const auto& record, result){
        auto currentBranchId = record.at(BranchIdColumn).toInt();
        if (currentBranchId != 0 && currentBranchId != branchId){
            branchItem = new BranchItem(/*record.mid(BranchNameColumn, BranchLength),*/ projectItem, ExplorerItem::ItemState::FROMBASE);
            model->insertItem(branchItem, projectItem->childCount(), model->itemIndex(projectItem));
            branchId = currentBranchId;
        }
        auto currentMonitorId = record.at(MonitorIdColumn).toInt();
        if (currentMonitorId !=0 && monitorId  != currentMonitorId){
            monitorItem = new MonitorItem(/*record.mid(MonitorNameColumn, MonitorLength),*/ branchItem, ExplorerItem::ItemState::FROMBASE);
            model->insertItem(monitorItem, branchItem->childCount(), model->itemIndex(branchItem));
            monitorId = currentMonitorId;
        }
    }
}

void WorkSpaceController::loadImportedData(MonitorItem *monitorItem)
{
    QString dataQuery("SELECT * FROM ImportedData where monitoringId = '" + QString::number(monitorItem->getId()) + "'");
    try{
        auto result = DbManager::getInstance()->selectQuery(dataQuery);
        if (!result.empty()){
            monitorItem->setCurrentData(result.at(0).at(MonitorItem::IMPORTED_DATA_COLUMN).toByteArray());
        }
    }catch(std::exception &ex){
        Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
    }
}

void WorkSpaceController::loadDataMapping(MonitorItem *monitorItem)
{
    QString dataQuery("SELECT * FROM ImportedDataMapping where monitoringId = '" + QString::number(monitorItem->getId()) + "'");
    try{
        auto result = DbManager::getInstance()->selectQuery(dataQuery);
        QList<QPair<MonitorItem::COLUMN_DATA_TYPE, int>> mappingList;
        if (!result.empty()){
            mappingList.push_back(qMakePair(MonitorItem::COLUMN_DATA_TYPE::NAME_COLUMN,
                                            result.at(0).at(static_cast<int>(MonitorItem::COLUMN_DATA_TYPE::NAME_COLUMN) + 1).toInt()));
            mappingList.push_back(qMakePair(MonitorItem::COLUMN_DATA_TYPE::DURATION_COLUMN,
                                            result.at(0).at(static_cast<int>(MonitorItem::COLUMN_DATA_TYPE::DURATION_COLUMN) + 1).toInt()));
            mappingList.push_back(qMakePair(MonitorItem::COLUMN_DATA_TYPE::DEPENDENCY_COLUMN,
                                            result.at(0).at(static_cast<int>(MonitorItem::COLUMN_DATA_TYPE::DEPENDENCY_COLUMN) + 1).toInt()));
        }
        if (!mappingList.isEmpty())
            monitorItem->changeMapping(mappingList);
    }catch(std::exception &ex){
        Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
    }
}


void WorkSpaceController::deleteItems(std::array<QList<int>, 2> &deletedItems)
{
    for (size_t j = 0; j < deletedItems.size(); ++j)
    {
        QString deleteQueryText("DELETE FROM ");
        QString importedDataQuery("Delete FROM ImportedData WHERE ");
        QString importedDataMappingQuery("Delete FROM ImportedDataMapping WHERE ");
        deleteQueryText+=(TableNames[j] + " WHERE ");
        if (!deletedItems[j].isEmpty())
        {
            for (int i = 0; i < deletedItems[j].size(); ++i)
            {
                if (i != deletedItems[j].size() - 1 && deletedItems[j][i] > 0){
                    deleteQueryText += (" id = '" + QString::number(deletedItems[j][i]) + "' OR");
                    if (j == 1){
                        importedDataQuery += (" monitoringId = '" + QString::number(deletedItems[j][i]) + "' OR");
                        importedDataMappingQuery += (" monitoringId = '" + QString::number(deletedItems[j][i]) + "' OR");
                    }
                }
                else{
                    deleteQueryText += (" id = '" + QString::number(deletedItems[j][i]) + "'");
                    if (j == 1){
                        importedDataQuery += (" monitoringId = '" + QString::number(deletedItems[j][i]) + "'");
                        importedDataMappingQuery += (" monitoringId = '" + QString::number(deletedItems[j][i]) + "'");
                    }
                }
            }
            qDebug()<<deleteQueryText;
            try{
                DbManager::getInstance()->executeQuery(deleteQueryText);
                if (j == 1){
                    qDebug()<<importedDataQuery;
                    qDebug()<<importedDataMappingQuery;
                    DbManager::getInstance()->executeQuery(importedDataQuery);
                    DbManager::getInstance()->executeQuery(importedDataMappingQuery);
                }
            }
            catch(std::exception &e){
                Globals::showMessage(QMessageBox::Critical, "WorkSpace", e.what());
            }
        }
        deletedItems[j].clear();
    }
}

void WorkSpaceController::saveModel(BaseModel *model, std::array<QList<int>, 2> &deletedItems)
{
    deleteItems(deletedItems);
    //auto rootItem = static_cast<WorkSpaceItem*>(model->getItem(model->index(0, 0)));
    auto rootItem = BaseModel::itemByIndexAs<BaseItem>(model->index(0,0));
    if (rootItem == nullptr)
        return;
    auto branchItems = rootItem->childList();
    saveLevel(branchItems, getNextLevelType(ItemType::Type::ProjectItem));
}

void WorkSpaceController::saveLevel(QList<BaseItem*> Items, const ItemType::Type type)
{
    if (type == ItemType::Type::Default)
        return;
    bool isException = false;
    QString queryText;
    int insertNum = 0;
    if (type == ItemType::Type::BranchItem){
        queryText = QString("Insert into %1  (name) Values(?)").arg(getTableNameByType(type));
    }else{
        queryText = QString("Insert into %1  (name, parentId) Values(?,?)").arg(getTableNameByType(type));
    }
    QList<QVariantList> bindList;
    QVariantList insertNameList;
    QVariantList insertParentIdList;
    QList<BaseItem*> childItems;
    foreach (auto item, Items){
        auto workSpaceItem = static_cast<ExplorerItem*>(item);
        if (workSpaceItem == nullptr)
            continue;
        if (workSpaceItem->getState() == ExplorerItem::ItemState::NEW){
            insertNameList.push_back(workSpaceItem->getName());
            insertParentIdList.push_back(workSpaceItem->getParentId());
        }else{
            updateItem(workSpaceItem);
        }
    }
    bindList.push_back(insertNameList);
    insertNum = insertNameList.size();
    if (type != ItemType::Type::BranchItem)
        bindList.push_back(insertParentIdList);
    try{
        qDebug()<<queryText;
        DbManager::getInstance()->prepareQueryBatch(queryText, bindList);
        DbManager::getInstance()->executeLastPreparedQueryBatch();
    }catch(std::exception& ex){
        isException = true;
        qDebug()<<queryText;
        Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
    }
    if (isException){
        return;
    }
    auto insertId = DbManager::getInstance()->getLastInsertId().toInt() - insertNum + 1;
    foreach(auto item, Items){
        auto workSpaceItem = static_cast<ExplorerItem*>(item);
        if (workSpaceItem == nullptr)
            continue;
        if (workSpaceItem->getState() == ExplorerItem::ItemState::NEW){
            workSpaceItem->setState(ExplorerItem::ItemState::FROMBASE);
            workSpaceItem->setId(insertId);
            if (workSpaceItem->itemType() == ItemType::Type::MonitorItem)
                saveImportedData(static_cast<MonitorItem*>(workSpaceItem));
            ++insertId;
        }
        childItems += workSpaceItem->childList();
        foreach (auto child, workSpaceItem->childList()){
            auto childItem = static_cast<ExplorerItem*>(child);
            if (childItem == nullptr)
                continue;
            if (childItem->getState() == ExplorerItem::ItemState::NEW){
                childItem->setParentId(workSpaceItem->getId());
            }
        }
    }
    bindList.clear();
    insertNameList.clear();
    insertParentIdList.clear();
    saveLevel(childItems, getNextLevelType(type));
}

void WorkSpaceController::updateItem(ExplorerItem *item)
{
    QString queryText;
    QMap<QString, QVariant> values;
    bool mustBeExecute = false;
    if (item->isDirty()){
        mustBeExecute = true;
        queryText = QString("Update %1 Set name = :name where id = '%2'").
                arg(getTableNameByType(item->itemType())).arg(item->getId());
        values.insert("name", item->getName());
    }
    if (mustBeExecute){
        try{
            qDebug()<<queryText;
            DbManager::getInstance()->prepareQuery(queryText, values);
            DbManager::getInstance()->executeLastPreparedQuery();
        }catch(std::exception& ex){
            Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
        }
    }
    if (item->itemType() == ItemType::Type::MonitorItem){
        auto monitorItem = static_cast<MonitorItem*>(item);
        updateImportedData(monitorItem);
        updateDataMapping(monitorItem);
    }
}

void WorkSpaceController::saveImportedData(MonitorItem *monitorItem)
{
    if (monitorItem == nullptr)
        return;
    if (monitorItem->hasData()){
        bool isException = false;
        QString preparedQuery("INSERT INTO ImportedData (data, monitoringId)"
                             "VALUES(:data, :monitoringId)");
        QMap<QString, QVariant> values;
        values.insert("data", monitorItem->getCurrentData());
        values.insert("monitoringId", monitorItem->getId());
        try{
            qDebug()<<preparedQuery;
            DbManager::getInstance()->prepareQuery(preparedQuery, values);
            DbManager::getInstance()->executeLastPreparedQuery();
        }catch(std::exception &ex){
            qDebug()<<"From save Imported Data  Query";
            isException = true;
            Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
        }
        if (!isException){
            saveDataMapping(monitorItem);
        }
    }
}

void WorkSpaceController::updateImportedData(MonitorItem *monitorItem)
{
    if (monitorItem == nullptr)
        return;
    if (monitorItem->hasData() && monitorItem->isDataChanged()){
        bool isException = false;
        QString existQuery("SELECT * FROM ImportedData where monitoringId = '" + QString::number(monitorItem->getId()) + "'");
        QList<QVector<QVariant>> exist;
        try{
            exist = DbManager::getInstance()->selectQuery(existQuery);
        }
        catch(std::exception &ex){
            qDebug()<<"From update Imported Data Query";
            Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
        }
        if (!isException && exist.isEmpty()){
            saveImportedData(monitorItem);
            return;
        }
        QString preparedQuery("UPDATE ImportedData SET data = :data where monitoringId = '"
                              + QString::number(monitorItem->getId()) + "'");
        QMap<QString, QVariant> values;
        values.insert("data", monitorItem->getCurrentData());
        try{
            qDebug()<<preparedQuery;
            DbManager::getInstance()->prepareQuery(preparedQuery, values);
            DbManager::getInstance()->executeLastPreparedQuery();
        }
        catch(std::exception &ex){
            Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
        }
    }
}

void WorkSpaceController::saveDataMapping(MonitorItem *monitorItem)
{
    if (monitorItem->hasMapping()){
        QString preparedQuery("Insert into ImportedDataMapping (nameColumn, durationColumn, dependencyColumn, monitoringId)"
                            "VALUES(:nameColumn, :durationColumn, :dependencyColumn, :monitoringId)");
        QMap<QString, QVariant> values;
        qDebug()<<monitorItem->getDataMapping().value(MonitorItem::COLUMN_DATA_TYPE::NAME_COLUMN);
        qDebug()<<monitorItem->getDataMapping().value(MonitorItem::COLUMN_DATA_TYPE::DURATION_COLUMN);
        qDebug()<<monitorItem->getDataMapping().value(MonitorItem::COLUMN_DATA_TYPE::DEPENDENCY_COLUMN);
        qDebug()<<monitorItem->getId();
        values.insert("nameColumn", monitorItem->getDataMapping().value(MonitorItem::COLUMN_DATA_TYPE::NAME_COLUMN));
        values.insert("durationColumn", monitorItem->getDataMapping().value(MonitorItem::COLUMN_DATA_TYPE::DURATION_COLUMN));
        values.insert("dependencyColumn", monitorItem->getDataMapping().value(MonitorItem::COLUMN_DATA_TYPE::DEPENDENCY_COLUMN));
        values.insert("monitoringId", monitorItem->getId());
        try{
            qDebug()<<preparedQuery;
            DbManager::getInstance()->prepareQuery(preparedQuery, values);
            DbManager::getInstance()->executeLastPreparedQuery();
        }catch(std::exception &ex){
            qDebug()<<"From save Data  Mapping Query";
            Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
        }
    }
}

void WorkSpaceController::updateDataMapping(MonitorItem *monitorItem)
{
    if (monitorItem->hasMapping() && monitorItem->isMappingChanged()){
        bool isException = false;
        QString existQuery("SELECT * FROM ImportedDataMapping where monitoringId = '" + QString::number(monitorItem->getId()) + "'");
        QList<QVector<QVariant>> exist;
        try{
            exist = DbManager::getInstance()->selectQuery(existQuery);
        }
        catch(std::exception &ex){
            qDebug()<<"From update Data Mapping Query";
            Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
        }
        if (!isException && exist.isEmpty()){
            saveDataMapping(monitorItem);
            return;
        }
        QString preparedQuery("UPDATE ImportedDataMapping SET nameColumn = :nameColumn, durationColumn = :durationColumn,"
                            "dependencyColumn = :dependencyColumn where monitoringId = '"
                            + QString::number(monitorItem->getId()) + "'");
        QMap<QString, QVariant> values;
        values.insert("nameColumn", monitorItem->getDataMapping().value(MonitorItem::COLUMN_DATA_TYPE::NAME_COLUMN));
        values.insert("durationColumn", monitorItem->getDataMapping().value(MonitorItem::COLUMN_DATA_TYPE::DURATION_COLUMN));
        values.insert("dependencyColumn", monitorItem->getDataMapping().value(MonitorItem::COLUMN_DATA_TYPE::DEPENDENCY_COLUMN));
        try{
            qDebug()<<preparedQuery;
            DbManager::getInstance()->prepareQuery(preparedQuery, values);
            DbManager::getInstance()->executeLastPreparedQuery();
        }
        catch(std::exception &ex){
            Globals::showMessage(QMessageBox::Critical, "WorkSpace", ex.what());
        }
    }
}




