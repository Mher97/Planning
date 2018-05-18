#include "BaseChartWidget.h"
#include "ui_BaseChartWidget.h"
#include "Charts/BaseChartWindow.h"

BaseChartWidget::BaseChartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseChartWidget), m_chartWindow(nullptr),
    m_currentChartTheme(QtCharts::QChart::ChartThemeLight)
{
    ui->setupUi(this);
    initUi();
    initConnections();
}

BaseChartWidget::~BaseChartWidget()
{
    delete ui;
    ui = nullptr;
    delete m_chartWindow;
    m_chartWindow = nullptr;
}

void BaseChartWidget::setTitle(const QString &title)
{
    ui->titleLabel->setText(title);
    m_chartWindow->setWindowTitle(title);
}

QString BaseChartWidget::getTitle() const
{
    return ui->titleLabel->text();
}

void BaseChartWidget::closeChartWindow()
{
    m_chartWindow->close();
}

void BaseChartWidget::initUi()
{
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->themeComboBox->addItem("Light", QtCharts::QChart::ChartThemeLight);
    ui->themeComboBox->addItem("Blue Cerulean", QtCharts::QChart::ChartThemeBlueCerulean);
    ui->themeComboBox->addItem("Dark", QtCharts::QChart::ChartThemeDark);
    ui->themeComboBox->addItem("Brown Sand", QtCharts::QChart::ChartThemeBrownSand);
    ui->themeComboBox->addItem("Blue NCS", QtCharts::QChart::ChartThemeBlueNcs);
    ui->themeComboBox->addItem("High Contrast", QtCharts::QChart::ChartThemeHighContrast);
    ui->themeComboBox->addItem("Blue Icy", QtCharts::QChart::ChartThemeBlueIcy);
    ui->themeComboBox->addItem("Qt", QtCharts::QChart::ChartThemeQt);
    ui->scrollArea->setWidget(ui->chartView);
    setLayout(ui->mainLayout);
}

void BaseChartWidget::initConnections()
{
    connect(ui->tbMaximize, &QToolButton::clicked, this, &BaseChartWidget::maximize);
    connect(ui->themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BaseChartWidget::chartThemeChanged);
}

void BaseChartWidget::changeChart()
{
    auto previousChart = ui->chartView->chart();
    delete previousChart;
    previousChart = nullptr;
    QtCharts::QChart* currentChart = constructChart();
    currentChart->setTheme(m_currentChartTheme);
    updateChartView();
    currentChart->update(ui->chartView->chart()->rect());
    ui->chartView->setChart(currentChart);
    if (!m_chartWindow->isHidden()){
        auto windowChart = constructChart();
        m_chartWindow->setChart(windowChart);
    }
}

void BaseChartWidget::updateChartView()
{
    ui->chartView = new QtCharts::QChartView();
    ui->scrollArea->setWidget(ui->chartView);
}

void BaseChartWidget::maximize()
{
    if (m_chartWindow != nullptr){
        auto windowChart = constructChart();
        m_chartWindow->setChart(windowChart);
        m_chartWindow->activateWindow();
        m_chartWindow->show();
    }
}

void BaseChartWidget::chartThemeChanged(int index)
{
    ui->chartView->chart()->setTheme(m_currentChartTheme = QtCharts::QChart::ChartTheme(index));
    if (!m_chartWindow->isHidden()){
        m_chartWindow->getChart()->setTheme(m_currentChartTheme);
    }
}
