#include "BusyDialog.h"
#include "ui_BusyDialog.h"

BusyDialog::BusyDialog(const QString &text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BusyDialog),
    m_currentIndicatorIndex(-1),
    m_currentIndicator(nullptr)
{
    ui->setupUi(this);
    ui->label->setText(text);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(size());
    setLayout(ui->gridLayout);
}

BusyDialog::~BusyDialog()
{
    delete ui;
}

void BusyDialog::updateBusyIndicators()
{
    if (m_currentIndicator != nullptr){
        m_currentIndicator->deactivate();
    }
    m_currentIndicatorIndex = (m_currentIndicatorIndex + 1) % INDICATORS_COUNT;
    switch(m_currentIndicatorIndex){
        case 0:
            ui->busyIndicator0->activate();
            m_currentIndicator = ui->busyIndicator0;
            break;
        case 1:
            ui->busyIndicator1->activate();
            m_currentIndicator = ui->busyIndicator1;
            break;
        case 2:
            ui->busyIndicator2->activate();
            m_currentIndicator = ui->busyIndicator2;
            break;
        case 3:
            ui->busyIndicator3->activate();
            m_currentIndicator = ui->busyIndicator3;
            break;
        case 4:
            ui->busyIndicator4->activate();
            m_currentIndicator = ui->busyIndicator4;
            break;
        case 5:
            ui->busyIndicator5->activate();
            m_currentIndicator = ui->busyIndicator5;
            break;
        default:
            break;
    }
}
