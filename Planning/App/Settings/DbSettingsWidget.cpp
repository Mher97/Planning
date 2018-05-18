#include "DbSettingsWidget.h"
#include "ui_DbSettingsWidget.h"
#include "QMessageBox"

QString DbSettingsWidget::settingsNamesToString(const DbSettingsWidget::DB_SETTINGS settingsName)
{
    switch(settingsName)
    {
        case DB_SETTINGS::CONNECTION_NAME :
            return "ConnectionName";
        case DB_SETTINGS::HOST_NAME :
            return "HostName";
        case DB_SETTINGS::DB_NAME :
            return "DbName";
        case DB_SETTINGS::USER_NAME :
            return "UserName";
        case DB_SETTINGS::PASSWORD :
            return "Password";
        case DB_SETTINGS::PORT_NUMBER :
            return "PortNumber";
        default:
            return {};
    }
}

DbSettingsWidget::DbSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ISettings(),
    ui(new Ui::DbSettingsWidget)

{
    ui->setupUi(this);
    initUi();
    initConnections();
}

DbSettingsWidget::~DbSettingsWidget()
{
    delete ui;
}

void DbSettingsWidget::loadSettings()
{
    QString filePath = QApplication::applicationDirPath() + "/config.ini";
    m_settings = new QSettings(filePath, QSettings::IniFormat);
    if (m_settings->allKeys().size() != SETTINGS_COUNT){
        throw SettingsException();
    }
    updateContent();
}

void DbSettingsWidget::saveSettings()
{
    bool changed = false;
    if (ui->hostLineEdit->text().isEmpty()){
        QMessageBox::information(this, "App", "HostName is Empty");
        return;
    }
    if (ui->dbLineEdit->text().isEmpty()){
        QMessageBox::information(this, "App", "DbName is Empty");
        return;
    }
    if (ui->userLineEdit->text().isEmpty()){
        QMessageBox::information(this, "App", "UserName is Empty");
        return;
    }
    if (ui->passwordLineEdit->text().isEmpty()){
        QMessageBox::information(this, "App", "Password is Empty");
        return;
    }
    if (ui->portLineEdit->text().isEmpty()){
        QMessageBox::information(this, "App", "PortNumber is Empty");
        return;
    }
    if (ui->hostLineEdit->text()!= m_settings->value(settingsNamesToString(DB_SETTINGS::HOST_NAME)).toString()){
        m_settings->setValue(settingsNamesToString(DB_SETTINGS::HOST_NAME), ui->hostLineEdit->text());
        changed = true;
    }
    if (ui->dbLineEdit->text() != m_settings->value(settingsNamesToString(DB_SETTINGS::DB_NAME)).toString()){
        m_settings->setValue(settingsNamesToString(DB_SETTINGS::DB_NAME), ui->dbLineEdit->text());
        changed = true;
    }
    if (ui->userLineEdit->text() != m_settings->value(settingsNamesToString(DB_SETTINGS::USER_NAME)).toString()){
        m_settings->setValue(settingsNamesToString(DB_SETTINGS::USER_NAME), ui->userLineEdit->text());
        changed = true;
    }
    if (ui->passwordLineEdit->text() != m_settings->value(settingsNamesToString(DB_SETTINGS::PASSWORD)).toString()){
        m_settings->setValue(settingsNamesToString(DB_SETTINGS::PASSWORD), ui->passwordLineEdit->text());
        changed = true;
    }
    if (ui->portLineEdit->text() != m_settings->value(settingsNamesToString(DB_SETTINGS::PORT_NUMBER)).toString()){
        m_settings->setValue(settingsNamesToString(DB_SETTINGS::PORT_NUMBER), ui->portLineEdit->text() );
        changed = true;
    }
    if (changed){
        QMessageBox::information(nullptr, "Database", "New Db Settings saved.\nPlease restart application");
    }
}

void DbSettingsWidget::updateContent()
{
    ui->hostLineEdit->setText(m_settings->value(settingsNamesToString(DB_SETTINGS::HOST_NAME)).toString());
    ui->dbLineEdit->setText(m_settings->value(settingsNamesToString(DB_SETTINGS::DB_NAME)).toString());
    ui->userLineEdit->setText(m_settings->value(settingsNamesToString(DB_SETTINGS::USER_NAME)).toString());
    ui->passwordLineEdit->setText(m_settings->value(settingsNamesToString(DB_SETTINGS::PASSWORD)).toString());
    ui->portLineEdit->setText(m_settings->value(settingsNamesToString(DB_SETTINGS::PORT_NUMBER)).toString());
}

void DbSettingsWidget::initUi()
{
    setLayout(ui->gridLayout);
}

void DbSettingsWidget::initConnections()
{
    connect(ui->hostLineEdit, &QLineEdit::textChanged, [this](){m_dirty = true;});
    connect(ui->hostLineEdit, &QLineEdit::textChanged, [this](){m_dirty = true;});
    connect(ui->hostLineEdit, &QLineEdit::textChanged, [this](){m_dirty = true;});
    connect(ui->hostLineEdit, &QLineEdit::textChanged, [this](){m_dirty = true;});
    connect(ui->hostLineEdit, &QLineEdit::textChanged, [this](){m_dirty = true;});
}
