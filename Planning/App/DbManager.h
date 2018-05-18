#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql>
#include <exception>
#include <QMap>
#include <QVariant>
#include <mutex>

class DbException : public std::exception
{
    public:
    template <typename Str>
        DbException(Str&& message) : std::exception(), m_message(std::forward<Str>(message)){}
        virtual const char* what() const noexcept override
        {
            return QStringToConstChar(m_message);
        }
    private:
        QString m_message;
        static const char* QStringToConstChar(const QString& inputString)
        {
            char *result = new char[inputString.length() + 1]();
            memcpy(result, inputString.toStdString().c_str(), inputString.length());
            return result;
        }
};

class DbManager
{
    public:
        static DbManager *getInstance();
        DbManager(const DbManager&) = delete;
        DbManager(DbManager&&) = delete;
        DbManager& operator = (const DbManager&) = delete;
        DbManager& operator = (DbManager&&) = delete;
        void openDatabase(const QString &connectionName, const QString &hostName, const QString &dbName,
                          const QString &userName, const QString &password, const int portName);
        QVariant getLastInsertId()const;
        void executeQuery(QSqlQuery &query);
        void executeQuery(const QString& queryText);
        void prepareQuery(const QString &text, const QMap<QString, QVariant> &values);
        void executeLastPreparedQuery();
        void prepareQueryBatch(const QString& queryText, const QList<QVariantList>& values);
        void executeLastPreparedQueryBatch();
        QList<QVector<QVariant>> selectQuery(QSqlQuery &query);
        QList<QVector<QVariant>> selectQuery(const QString &queryText);
    private:
        static DbManager *m_instance;
        QSqlDatabase m_database;
        QVariant m_lastInsertId;
        QSqlQuery m_lastPreparedQuery;
        static std::mutex ThreadSafer;
    protected:
        DbManager();
        virtual ~DbManager() = default;
};

#endif // DBMANAGER_H
