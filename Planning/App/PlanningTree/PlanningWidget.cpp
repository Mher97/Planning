#include "PlanningWidget.h"
#include "ui_PlanningWidget.h"
#include "../Data/PlanningModel.h"
#include "../Data/PlanningProxyModel.h"
#include "../Data/PlanningItem.h"
#include "FilterModel.h"
#include "FilterItem.h"
#include "../Engine/Shceduler.h"
#include <QListView>
#include "../Globals.h"

PlanningWidget::PlanningWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanningWidget), m_planningModel(nullptr), m_planningProxyModel(nullptr), m_filterModel(nullptr)
{
    ui->setupUi(this);
    initUi();
    initModelView();
    initConnections();
}

PlanningWidget::~PlanningWidget()
{
    delete ui;
}

void PlanningWidget::updatePlanningData(const QList<QList<const TaskInfo*>>& data)
{
    ui->filterComboBox->clear();
    ui->treeView->setUpdatesEnabled(false);
    m_planningModel->constructTree(data, Globals::getColorForPhase);
    ui->treeView->expandAll();
    ui->treeView->setUpdatesEnabled(true);
    fillFilterComboBox(data);
}

void PlanningWidget::initUi()
{
    m_filterModel = new FilterModel();
    ui->filterComboBox->setModel(m_filterModel);
    ui->filterComboBox->setView(new QListView(this));
    setLayout(ui->verticalLayout);
}

void PlanningWidget::initModelView()
{
    m_planningModel = new PlanningModel();
    m_planningProxyModel = new PlanningProxyModel();
    m_planningProxyModel->setSourceModel(m_planningModel);
    ui->treeView->setModel(m_planningProxyModel);
    ui->treeView->setColumnWidth(PlanningItem::NAME_COLUMN, 300);
    ui->treeView->setColumnWidth(PlanningItem::START_COLUMN, 300);
    ui->treeView->setColumnWidth(PlanningItem::FINISH_COLUMN, 300);
    ui->treeView->header()->setSortIndicator(-1, Qt::AscendingOrder);
}

void PlanningWidget::initConnections()
{
    connect(ui->filterComboBox, &FilterComboBox::popupHidden, this, &PlanningWidget::onPopupHidden);
}

void PlanningWidget::fillFilterComboBox(const QList<QList<const TaskInfo*>> &data)
{
    m_filterModel->appendRow(new QStandardItem("Filter Data"));
    if (data.isEmpty())
        return;
    for (int i = 0; i <= data.size(); ++i){
        QString itemText = "Phase("+ QString::number(i) + ")";
        if (i == 0){
            itemText = "All";
        }
        auto item = new FilterItem(itemText);
        item->setCheckable(true);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Checked, Qt::CheckStateRole);
        m_filterModel->appendRow(item);
        connect(item, &FilterItem::manualDataChanged, this, &PlanningWidget::onManualDataChanged);
    }
}

void PlanningWidget::onManualDataChanged(FilterItem *item)
{
    if (item == nullptr){
        return;
    }
    if (item->row() == 1){
        for (int i = item->row() + 1; i <= m_filterModel->rowCount(); ++i){
            auto childItem = static_cast<FilterItem*>(m_filterModel->item(i, 0));
            if (childItem ==nullptr)
                continue;
            childItem->setData(item->data(Qt::CheckStateRole), Qt::CheckStateRole, FilterItem::DATA_CHANGE_TYPE::AUTO);
        }
    }else{
        auto parentItem = static_cast<FilterItem*>(m_filterModel->item(1, 0));
        int checkedCount = 0;
        if (parentItem == nullptr)
            return;
        if (item->data(Qt::CheckStateRole) == Qt::Unchecked){
            parentItem->setData(Qt::Unchecked, Qt::CheckStateRole, FilterItem::DATA_CHANGE_TYPE::AUTO);
        }else{
            for (int i =2; i <= m_filterModel->rowCount(); ++i){
                auto childItem = static_cast<FilterItem*>(m_filterModel->item(i, 0));
                if (childItem ==nullptr)
                    continue;
                if (childItem->data(Qt::CheckStateRole) == Qt::Checked){
                    ++checkedCount;
                }
            }
            if (checkedCount == m_filterModel->rowCount() - 2){
                parentItem->setData(Qt::Checked, Qt::CheckStateRole, FilterItem::DATA_CHANGE_TYPE::AUTO);
            }
        }
    }
}

void PlanningWidget::onPopupHidden()
{
    QVector<bool> filterVector;
    for (int i =1; i < m_filterModel->rowCount(); ++i){
        filterVector.push_back(m_filterModel->item(i, 0)->data(Qt::CheckStateRole).toBool());
    }
    m_planningProxyModel->setFilterVector(filterVector);
    ui->treeView->expandAll();
}




