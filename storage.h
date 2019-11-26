#pragma once
#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace mobile {

class Storage : public QObject
{
	Q_OBJECT
public:
	explicit Storage(const QString &path, const QString &dbDriver);
	~Storage();

	bool open();
	void close();

	void updateRecord(const QString &mcc, const QString &mnc, const QString &name);
	void insertRecord(const QString &mcc, const QString &mnc, const QString &name);

	void execQuery(const QString &query);
	QSqlQuery query() const;

private:
	QSqlDatabase m_db;
	QString m_path;
	QSqlQuery m_query;
};

}
