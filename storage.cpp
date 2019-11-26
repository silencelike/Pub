#include "Storage.h"

#include <QLoggingCategory>
#include <QSqlError>

namespace L {
Q_LOGGING_CATEGORY(database, "mobile.database", QtWarningMsg);
}

namespace mobile {

Storage::~Storage()
{
    close();
}


Storage::Storage(const QString &path, const QString &dbDriver)
    : m_path(path)
{
    m_db = QSqlDatabase::addDatabase(dbDriver);
    m_db.setDatabaseName(m_path);
}

bool Storage::open()
{
    if (!m_db.open()) {
        qCWarning(L::database) << "Could not database open";
        return false;
    }
    return true;
}

void Storage::close()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void Storage::updateRecord(const QString &mcc, const QString &mnc, const QString &name)
{
    auto queryStr = QString("update operators \
                             set name = \"%1\" \
                             where mcc = %2 and mnc = %3").arg(name).arg(mcc).arg(mnc);
    execQuery(queryStr);
}

void Storage::insertRecord(const QString &mcc, const QString &mnc, const QString &name)
{
    auto queryStr = QString("insert into operators (mcc, mnc, name) \
                             values (%1, %2, \"%3\")").arg(mcc).arg(mnc).arg(name);
    execQuery(queryStr);
}

void Storage::execQuery(const QString &que)
{
    m_query.setForwardOnly(true);
    m_query = m_db.exec(que);
    if (m_query.lastError().isValid()) {
        qCWarning(L::database) << m_query.lastError();
        m_query.clear();
    }
}

QSqlQuery Storage::query() const
{
    return m_query;
}

}
