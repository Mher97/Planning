#ifndef BUSYTHREAD_H
#define BUSYTHREAD_H

#include <QThread>
#include <QTimer>

class BusyThread : public QThread
{
    Q_OBJECT
    public:
        explicit BusyThread(QObject *parent = nullptr);
        virtual ~BusyThread();
        void run()override;
    private:
        static const int INTERVAL = 100;
        QTimer *m_timer;
        void init();
    signals:
        void progressMustBeUpdate();
};

#endif // BUSYTHREAD_H
