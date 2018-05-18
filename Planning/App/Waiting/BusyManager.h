#ifndef BUSYMANAGER_H
#define BUSYMANAGER_H

#include <QObject>

class BusyManager: public QObject
{
    Q_OBJECT
    public:
        static BusyManager& getInstance()
        {
            static BusyManager instance;
            return instance;
        }
    private:
        BusyManager(QObject *parent = nullptr) : QObject(parent){}
        virtual ~BusyManager() = default;
    signals:
        void mustBeStartBusy(const QString& text);
        void mustBeFinishBusy();
};

#endif // BUSYMANAGER_H
