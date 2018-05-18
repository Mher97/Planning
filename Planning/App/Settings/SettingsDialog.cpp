#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "Settings/DbSettingsWidget.h"
#include "Settings/ISettings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_dbSettingsWidget(nullptr)
{
    ui->setupUi(this);
    initUi();
    initConnections();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::updateContent()
{
    for (int i = 0; i < ui->tabWidget->count(); ++i){
        auto settingsModule = dynamic_cast<ISettings*>(ui->tabWidget->widget(i));
        if (settingsModule != nullptr && settingsModule->isDirty()){
            settingsModule->updateContent();
        }
    }
}

void SettingsDialog::loadSettings()
{
    try{
        m_dbSettingsWidget->loadSettings();
    }catch(std::exception&){
        throw;
    }
}

const ISettings* SettingsDialog::getDbSettings()
{
    auto settingsInterFace = dynamic_cast<ISettings*>(m_dbSettingsWidget);
    return settingsInterFace;
}

void SettingsDialog::initUi()
{
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);
    m_dbSettingsWidget = new DbSettingsWidget();
    m_dbSettingsWidget->setAutoFillBackground(true);
    QPalette pal(QColor(230, 240, 249));
    m_dbSettingsWidget->setPalette(pal);
    ui->tabWidget->addTab(m_dbSettingsWidget, "Db Settings");
    setLayout(ui->verticalLayout);
}

void SettingsDialog::initConnections()
{
    connect(ui->saveButton, &QPushButton::clicked, this, &SettingsDialog::onSaveButtonClicked);
}

void SettingsDialog::onSaveButtonClicked()
{
    for (int i = 0; i < ui->tabWidget->count(); ++i){
        auto settingsModule = dynamic_cast<ISettings*>(ui->tabWidget->widget(i));
        if (settingsModule != nullptr && settingsModule->isDirty()){
            settingsModule->saveSettings();
        }
    }
}
