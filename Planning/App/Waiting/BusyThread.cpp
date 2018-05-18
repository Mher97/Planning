#include "BusyThread.h"

BusyThread::BusyThread(QObject *parent) : QThread(parent)
{
    init();
}

BusyThread::~BusyThread()
{
    delete m_timer;
    m_timer = nullptr;
}

void BusyThread::run()
{
    m_timer->start();
    exec();
}

void BusyThread::init()
{
    m_timer = new QTimer();
    m_timer->setInterval(INTERVAL);
    m_timer->setSingleShot(false);
    m_timer->moveToThread(this);
    connect(m_timer, &QTimer::timeout, [this](){emit progressMustBeUpdate();});
}
