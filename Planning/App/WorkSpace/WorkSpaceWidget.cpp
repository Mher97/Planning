#include "WorkSpaceWidget.h"
#include "ui_WorkSpaceWidget.h"
#include "../Data/ExplorerModel.h"
#include "../Data/ExplorerProxyModel.h"
#include "WorkSpaceController.h"
#include "../Data/ProjectItem.h"
#include "../Data/BranchItem.h"
#include "../Data/MonitorItem.h"
#include "../DbManager.h"
#include "Waiting/BusyDialog.h"
#include "Waiting/BusyManager.h"
#include "../Globals.h"

const QList<ItemType::Type> WorkSpaceWidget::TYPES_ARRAY = QList<ItemType::Type>() << ItemType::Type::ProjectItem
                                                                       << ItemType::Type::BranchItem
                                                                       << ItemType::Type::MonitorItem;


WorkSpaceWidget::WorkSpaceWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::WorkSpaceWidget), m_model(nullptr),
    m_proxyModel(nullptr),m_currentMonitoringIndex(QModelIndex())
{
    ui->setupUi(this);
    initUi();
    initModel();
    initConnections();
}

WorkSpaceWidget::~WorkSpaceWidget()
{
    delete ui;
    for(size_t i =0; i < m_deletedFromBase.size(); ++i)
        m_deletedFromBase[i].clear();
}

void WorkSpaceWidget::saveImportedFile(const QString &fileName)
{
    QFile *file = new QFile(fileName);
    if (!file->open(QFile::ReadOnly | QFile::Text)){
        Globals::showMessage(QMessageBox::Warning, "App", "File did not open");
    }else{
        QModelIndex index = m_proxyModel->mapToSource(ui->treeView->currentIndex());
        //index = m_model->ancestorItemByType(index, ItemType::Type::MonitorItem);
        //MonitorItem *item = static_cast<MonitorItem*>(m_model->getItem(index));
        MonitorItem* item = BaseModel::itemByIndexAs<MonitorItem>(index);
        if (item == nullptr){
            return;
        }
        QByteArray byteArray = file->readAll();
        byteArray = byteArray.toBase64();
        byteArray = qCompress(byteArray);
        item->setCurrentData(byteArray);
        if (item->getState() == ExplorerItem::ItemState::FROMBASE){
            item->setDataChanged(true);
        }
        emit monitoringDataChanged(item);
    }
}

void WorkSpaceWidget::initUi()
{
    setLayout(ui->verticalLayout);
}

void WorkSpaceWidget::initModel()
{
    m_model = new ExplorerModel(this);
    m_model->initModel();
    m_proxyModel = new ExplorerProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    ui->treeView->setModel(m_proxyModel);
}

void WorkSpaceWidget::initConnections()
{
    connect(ui->tbNew, &QToolButton::clicked, this, &WorkSpaceWidget::onTbNewClicked);
    connect(ui->tbDelete, &QToolButton::clicked, this, &WorkSpaceWidget::onTbDeleteClicked);
    connect(ui->tbLoad, &QToolButton::clicked, this, &WorkSpaceWidget::onTbLoadClicked);
    connect(ui->tbSave, &QToolButton::clicked, this, &WorkSpaceWidget::onTbSaveClicked);
    connect(m_model, &ExplorerModel::dataChanged, this, &WorkSpaceWidget::onDataChanged);
    connect(m_model, &ExplorerModel::detectedMultipleNameDefinition, this, &WorkSpaceWidget::onDetectedMultipleNameDefinition);
    connect(ui->treeView, &QTreeView::clicked, this, &WorkSpaceWidget::currentItemChanged);
}

void WorkSpaceWidget::onTbNewClicked()
{
    ExplorerItem *parent(nullptr);
    QModelIndex index;
    index = m_proxyModel->mapToSource(ui->treeView->currentIndex());
    if (index.row() != -1 && index.column() != -1)
    {
        //parent = static_cast<WorkSpaceItem*>(m_model->getItem(index));
        parent = BaseModel::itemByIndexAs<ExplorerItem>(index);
        ItemType::Type parentType = parent->itemType();
        switch(parentType)
        {
            case ItemType::Type::ProjectItem:{
                BranchItem *branchItem = new BranchItem();
                branchItem->setName(parent->uniqueNameForChild(branchItem->getName(), branchItem->itemType()));
                QModelIndex branchIndex = m_model->insertItem(branchItem, parent->childCount(), index);
                ui->treeView->expand(m_proxyModel->mapFromSource(branchIndex));
                break;
            }
            case ItemType::Type::BranchItem:{
                ui->treeView->expand(m_proxyModel->mapFromSource(index));
                MonitorItem *monitorItem = new MonitorItem(parent);
                monitorItem->setName(parent->uniqueNameForChild(monitorItem->getName(), monitorItem->itemType()));
                monitorItem->setParentId(parent->getId());
                QModelIndex monitorIndex = m_model->insertItem(monitorItem, parent->childCount(), index);
                ui->treeView->expand(m_proxyModel->mapFromSource(monitorIndex));
                break;
            }
            default:
                break;
        }
    }else{
        ProjectItem *projectItem = new ProjectItem();
        QModelIndex projectIndex = m_model->insertItem(projectItem, m_model->getRootItem()->childCount(), index);
        ui->treeView->expand(m_proxyModel->mapFromSource(projectIndex));
        ui->tbNew->setEnabled(false);
    }
}

void WorkSpaceWidget::onTbDeleteClicked()
{
    QModelIndex index = m_proxyModel->mapToSource(ui->treeView->currentIndex());
    //WorkSpaceItem *item = static_cast<WorkSpaceItem*>(m_model->getItem(index));
    ExplorerItem *item = BaseModel::itemByIndexAs<ExplorerItem>(index);
    if (item == nullptr)
        return;
    ItemType::Type itemType = item->itemType();
    if (itemType == ItemType::Type::MonitorItem
            || itemType == ItemType::Type::BranchItem
            || itemType == ItemType::Type::ProjectItem)
    {
        deleteHelper(item);
        m_model->deleteItem(index);
    }
    ui->treeView->setCurrentIndex(QModelIndex());
    currentItemChanged();
}

void WorkSpaceWidget::deleteHelper(ExplorerItem *item)
{
    item->setDeleted(true);
    if(item->getState() == ExplorerItem::ItemState::FROMBASE)
    {
        item->setState(ExplorerItem::ItemState::DELETEDFROMBASE);
        ItemType::Type itemType = item->itemType();
        for (int i = 0; i < TYPES_COUNT; ++i)
        {
            if (itemType == TYPES_ARRAY[i + 1]){
                m_deletedFromBase[i].push_back(item->getId());
                break;
            }
        }
    }
    for (int i = 0; i < item->childCount(); ++i)
    {
        ExplorerItem *child = static_cast<ExplorerItem*>(item->child(i));
        if (child != nullptr && !child->isDeleted()){
            deleteHelper(child);
        }
    }
}

void WorkSpaceWidget::onTbLoadClicked()
{
    BusyManager::getInstance().mustBeStartBusy("Loading Data...");
    for (int i = 1; i <=2e9; ++i){}
    setUpdatesEnabled(false);
    int end = m_model->rowCount();
    for (int i = 0; i < end; ++i){
        QModelIndex index = m_model->index(0, 0);
        m_model->deleteItem(index);
    }
    for (int i = 0; i < TYPES_COUNT; ++i){
        m_deletedFromBase[i].clear();
    }
    WorkSpaceController::getInstance().loadModel(m_model);
    ui->treeView->expandAll();
    currentItemChanged();
    ui->tbSave->setEnabled(true);
    setUpdatesEnabled(true);
    BusyManager::getInstance().mustBeFinishBusy();
}

void WorkSpaceWidget::onTbSaveClicked()
{
    BusyManager::getInstance().mustBeStartBusy("Saving Data...");
    for (int i = 1; i <=2e9; ++i);
    WorkSpaceController::getInstance().saveModel(m_model, m_deletedFromBase);
    BusyManager::getInstance().mustBeFinishBusy();
}

void WorkSpaceWidget::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(roles)
    if (topLeft == bottomRight){
        auto explorerItem = BaseModel::itemByIndexAs<ExplorerItem>(topLeft);
        if (explorerItem != nullptr){
            explorerItem->setDirty(true);
        }
    }
}

void WorkSpaceWidget::onDetectedMultipleNameDefinition(const QString &message)
{
    Globals::showMessage(QMessageBox::Critical, "WorkSpace", message);
}

void WorkSpaceWidget::currentItemChanged()
{
    bool isMonitoring = false;
    bool isMonitoringChanged = true;
    QModelIndex index = m_proxyModel->mapToSource(ui->treeView->currentIndex());
    ItemType::Type itemType;
    if (index.isValid())
    {
        //itemType = m_model->getItem(index)->itemType();
        itemType = BaseModel::itemByIndexAs<ExplorerItem>(index)->itemType();
        switch(itemType){
            case ItemType::Type::ProjectItem:
            case ItemType::Type::BranchItem:
                m_currentMonitoringIndex = QModelIndex();
                ui->tbNew->setEnabled(true);
                ui->tbDelete->setEnabled(true);
                break;
            case ItemType::Type::MonitorItem:
            {
                isMonitoring = true;
                getMonitoringData(index);
                ui->tbNew->setEnabled(false);
                ui->tbDelete->setEnabled(false);
                break;
            }
            default:
                break;
        }
        if (itemType == ItemType::Type::MonitorItem)
            ui->tbDelete->setEnabled(true);
    }
    else{
        ui->tbNew->setEnabled(false);
        ui->tbDelete->setEnabled(false);
    }
    MonitorItem* currentMonitor(nullptr);
    QModelIndex monitorIndex = index;
    if(index.isValid() && (m_model->itemType(index) != ItemType::Type::MonitorItem)){
        monitorIndex = index;
    }
    if(monitorIndex.isValid()){
        isMonitoring = true;
        if (monitorIndex == m_currentMonitoringIndex){
            isMonitoringChanged = false;
        }
        m_currentMonitoringIndex = monitorIndex;
        //currentMonitor = dynamic_cast<MonitorItem*>(m_model->getItem(monitorIndex));
        currentMonitor = BaseModel::itemByIndexAs<MonitorItem>(monitorIndex);
        if(currentMonitor == nullptr){
            return;
        }
    }
    emit workSpaceCurrentItemChanged(isMonitoring, isMonitoringChanged);
}

void WorkSpaceWidget::getMonitoringData(const QModelIndex &monitoringIndex)
{
    //MonitorItem *monitorItem = static_cast<MonitorItem*>(m_model->getItem(monitoringIndex));
    MonitorItem* monitorItem = BaseModel::itemByIndexAs<MonitorItem>(monitoringIndex);
    if (monitorItem == nullptr || monitoringIndex == m_currentMonitoringIndex){
        return;
    }
    if (monitorItem->isFirstSelection()){
        WorkSpaceController::getInstance().loadImportedData(monitorItem);
    }
    if (monitorItem->hasData())
    {
        if (monitorItem->isFirstSelection()){
            WorkSpaceController::getInstance().loadDataMapping(monitorItem);
        }
    }
    else{
        Globals::showMessage(QMessageBox::Warning, "WorkSpace", "Data haven't been imported");
    }
    monitorItem->setFirstSelection(false);
    emit monitoringDataChanged(monitorItem);
}


