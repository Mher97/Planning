#include "MappingWidget.h"
#include "ui_MappingWidget.h"
#include "../Data/BaseModel.h"
#include "DataReading/CheckBoxItemDelegate.h"
#include "../Data/MappingProxyModel.h"
#include "../Data/MappingModel.h"
#include "DataReading/DataHandler.h"
#include <QFileDialog>
#include <QDesktopServices>
#include "../Globals.h"

MappingWidget::MappingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MappingWidget), m_mappingModel(nullptr), m_proxyModel(nullptr),
    m_checkBoxItemDelegate(nullptr), m_dataHandler(new DataHandler())
{
    ui->setupUi(this);
    initModel();
    initUi();
    initConnections();
}

MappingWidget::~MappingWidget()
{
    delete ui;
    delete m_checkBoxItemDelegate;
    m_checkBoxItemDelegate = nullptr;
    delete m_proxyModel;
    m_proxyModel = nullptr;
    delete m_mappingModel;
    m_mappingModel = nullptr;
    delete m_dataHandler;
    m_dataHandler = nullptr;
}

void MappingWidget::reset()
{
    m_mappingModel->clearModel();
    ui->importButton->setEnabled(false);
    ui->exportButton->setEnabled(false);
}

void MappingWidget::initModel()
{
    m_mappingModel = new MappingModel();
    m_proxyModel = new MappingProxyModel();
    m_checkBoxItemDelegate = new CheckBoxItemDelegate();
    m_proxyModel->setSourceModel(m_mappingModel);
    ui->treeView->setModel(m_proxyModel);
    for (int column = 1; column < m_mappingModel->columnCount(); ++column){
        ui->treeView->setItemDelegateForColumn(column, m_checkBoxItemDelegate);
        ui->treeView->setColumnWidth(column, 200);
    }
    ui->treeView->setColumnWidth(0,300);
    ui->treeView->header()->setStretchLastSection(true);
}

void MappingWidget::initUi()
{
    setLayout(ui->verticalLayout);
}

void MappingWidget::initConnections()
{
    connect(m_mappingModel, &MappingModel::importAllowed, [&](const auto flag){ui->importButton->setEnabled(flag);});
    connect(ui->importButton, &QPushButton::clicked, this, &MappingWidget::onImportButtonClicked);
    connect(ui->exportButton, &QToolButton::clicked, this, &MappingWidget::onExportButtonClicked);
}

bool MappingWidget::mappedColumnIsValid(const int columnNumber)
{
    return columnNumber >= 0;
}

void MappingWidget::handleData(const QByteArray& data, const QMap<MonitorItem::COLUMN_DATA_TYPE, int> monitorMapping)
{
    static const int MinNeedHeadersCount = 3;
    m_mappingModel->clearModel();
    ui->exportButton->setEnabled(true);
    ui->importButton->setEnabled(false);
    m_dataHandler->setData(data);
    QStringList headers = m_dataHandler->getHeaders();
    auto nameColumn = monitorMapping[MonitorItem::COLUMN_DATA_TYPE::NAME_COLUMN];
    auto durationColumn = monitorMapping[MonitorItem::COLUMN_DATA_TYPE::DURATION_COLUMN];
    auto dependencyColumn =monitorMapping[MonitorItem::COLUMN_DATA_TYPE::DEPENDENCY_COLUMN];
    if (headers.size() < MinNeedHeadersCount)
        Globals::showMessage(QMessageBox::Warning, "Data Reading", "Column count less than needed");
    for(int i = 0; i < headers.size(); ++i){
        MappingItem * mappingItem = new MappingItem();
        if (headers[i].isEmpty()){
            Globals::showMessage(QMessageBox::Warning, "Data Reading", "Empty header in column " + QString::number(i));
        }
        mappingItem->setRawColName(headers[i]);
        mappingItem->setColumnNumber(i);
        m_mappingModel->insertItem(mappingItem);
        if (i == nameColumn){
            m_mappingModel->setData(m_mappingModel->index(i, static_cast<int>(MappingItem::DATA_TYPES::DATA_TASK_CODE)),
                                    Qt::Checked, Qt::CheckStateRole);
        }else if (i == durationColumn){
            m_mappingModel->setData(m_mappingModel->index(i, static_cast<int>(MappingItem::DATA_TYPES::DATA_DURATION)),
                                    Qt::Checked, Qt::CheckStateRole);
        }else if (i == dependencyColumn){
            m_mappingModel->setData(m_mappingModel->index(i, static_cast<int>(MappingItem::DATA_TYPES::DATA_DEPENDENCE)),
                                    Qt::Checked, Qt::CheckStateRole);
        }
    }
}

QHash<QString, QPair<int, QStringList>> MappingWidget::readData()
{
    m_mapping.clear();
    for (int i = 0 ; i < m_mappingModel->rowCount(); ++i){
        QModelIndex index = m_mappingModel->index(i, 0);
        //MappingItem* mappingItem = static_cast<MappingItem*>(m_mappingModel->getItem(index));
        MappingItem* mappingItem = BaseModel::itemByIndexAs<MappingItem>(index);
        if (mappingItem != nullptr){
            for (int j = 1; j < m_mappingModel->columnCount(); ++j){
                if (mappingItem->data(j, Qt::CheckStateRole).toBool()){
                    m_mapping[MappingItem::DATA_TYPES(j)] = mappingItem->getColumnNumber();
                    break;
                }
            }
        }
    }
    try{
        return m_dataHandler->readMappedData(m_mapping);
    }catch(std::exception&){
        throw;
    }
}

QMap<MappingItem::DATA_TYPES, int> MappingWidget::getMapping() const
{
    return m_mapping;
}

void MappingWidget::onExportButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export data as .csv", "", "(*.csv);;All Files (*)");
    if (fileName.isEmpty()){
        return;
    }
    else{
        QFile file(fileName + ".csv");
        if (!file.open(QIODevice::WriteOnly)){
            Globals::showMessage(QMessageBox::Warning, "Unable to open file", file.errorString());
            return;
        }
        file.write(m_dataHandler->getData());
        QDesktopServices::openUrl(QUrl::fromLocalFile(file.fileName()));
    }
}
