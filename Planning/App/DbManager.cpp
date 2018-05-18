#include "DbManager.h"
#include <QApplication>
#include <QDebug>

std::mutex DbManager::ThreadSafer;

DbManager* DbManager::m_instance = new DbManager();

DbManager::DbManager() : m_lastInsertId()
{

}

DbManager *DbManager::getInstance()
{
    std::lock_guard<std::mutex> lock(ThreadSafer);
    if (!m_instance)
        m_instance = new DbManager();
    return m_instance;
}

void DbManager::openDatabase(const QString &connectionName, const QString &hostName,
                                   const QString &dbName, const QString &userName,
                                   const QString &password, const int portName)
{
    qDebug()<<connectionName<<" "<<hostName<<" "<<dbName<<" "<<userName<<" "<<password<<" "<<portName;
    m_database = QSqlDatabase::addDatabase(connectionName);
    m_database.setHostName(hostName);
    m_database.setDatabaseName(dbName);
    m_database.setUserName(userName);
    m_database.setPassword(password);
    m_database.setPort(portName);
    if (m_database.isOpen())
        return;
    if (!m_database.open()){
        throw DbException(m_database.lastError().text());
    }
}

QVariant DbManager::getLastInsertId() const
{
    return m_lastInsertId;
}

void DbManager::executeQuery(QSqlQuery &query)
{
    if (!query.isActive() && !query.exec())
    {
        query.finish();
        throw DbException(query.lastError().text());
    }
    m_lastInsertId = query.lastInsertId();
    query.finish();
}

void DbManager::executeQuery(const QString &queryText)
{
    QSqlDatabase::database().transaction();
    QSqlQuery query(queryText);
    try{
        executeQuery(query);
        QSqlDatabase::database().commit();
    }
    catch(std::exception &){
        throw;
    }
}

void DbManager::prepareQuery(const QString &text, const QMap<QString, QVariant > &values)
{
    QSqlQuery query;
    if (query.prepare(text))
    {
        for (QMap<QString, QVariant>::const_iterator it = values.begin(); it!=values.end(); ++it){
            query.bindValue(":" + it.key(), it.value());
        }
    }else{
        throw DbException(query.lastError().text());
    }
    m_lastPreparedQuery = query;
}

void DbManager::executeLastPreparedQuery()
{
    try{
        executeQuery(m_lastPreparedQuery);
    }catch(std::exception&){
        throw;
    }
}

void DbManager::prepareQueryBatch(const QString& queryText, const QList<QVariantList> &values)
{
    QSqlQuery query;
    if (!query.prepare(queryText))
        throw DbException(query.lastError().text());
    foreach(auto variantList, values){
        query.addBindValue(variantList);
    }
    m_lastPreparedQuery = query;
}

void DbManager::executeLastPreparedQueryBatch()
{
    if (m_lastPreparedQuery.execBatch()){
        m_lastInsertId = m_lastPreparedQuery.lastInsertId();
    }else{
        qDebug()<<m_lastPreparedQuery.lastError();
        throw DbException(m_lastPreparedQuery.lastError().text());
    }
}

QList<QVector<QVariant> > DbManager::selectQuery(QSqlQuery &query)
{
    if (query.exec())
    {
        QList<QVector<QVariant>> result;
        while (query.next())
        {
            QSqlRecord record = query.record();
            if (!record.isEmpty()){
                result.push_back(QVector<QVariant>());
                for(int i = 0; i < record.count(); i++){
                    result.back().push_back(record.value(i));
                }
            }
        }
        return result;
    }
    throw DbException(query.lastError().text());
}

QList<QVector<QVariant> > DbManager::selectQuery(const QString &queryText)
{
    QSqlQuery query(queryText);
    try{
        return selectQuery(query);
    }
    catch(std::exception &){
        throw;
    }
}





