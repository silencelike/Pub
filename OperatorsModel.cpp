#include "OperatorsModel.h"

#include <QPixmap>

namespace mobile {

OperatorsModel::OperatorsModel(const std::shared_ptr<Storage> &storage, QObject *parent)
	: TreeModel(parent)
	, m_storage(storage)
{
	fillOperators();
}

QModelIndex OperatorsModel::addCountry(const QString &mcc, const QString &name, const QPixmap &icon)
{
	if (columnCount() < 1) {
		insertColumn(0);
	}

	auto row = rowCount();
	insertRow(row);
	auto idx = index(row, 0);

	setData(idx, mcc, static_cast<int>(RoleNames::Mcc));
	setData(idx, name, static_cast<int>(RoleNames::CountryName));
	setData(idx, icon, static_cast<int>(RoleNames::CountryIcon));
	return idx;
}

void OperatorsModel::addOperator(const QString &mcc, const QString &mnc, const QString &name,
								 const QPixmap &icon, const QPixmap &countryIcon, const QModelIndex &parentIndex)
{
	int row = rowCount(parentIndex);
	insertRow(row, parentIndex);
	auto idx = index(row, 0, parentIndex);

	setData(idx, mcc, static_cast<int>(RoleNames::Mcc));
	setData(idx, mnc, static_cast<int>(RoleNames::Mnc));
	setData(idx, name, static_cast<int>(RoleNames::OperatorName));
	setData(idx, icon, static_cast<int>(RoleNames::OperatorIcon));
	setData(idx, countryIcon, static_cast<int>(RoleNames::CountryIcon));
}

void OperatorsModel::fillOperators()
{
	resetModel();

	m_storage->execQuery("Select ctr.code as code, ctr.name as country, \
						  opr.mcc as mcc, opr.mnc as mnc, opr.name as operator \
						  From countries as ctr \
						  Inner Join operators as opr \
						  On ctr.mcc = opr.mcc  \
					   /* Group By operator \ */ \
						  Order By country");

	QString lastCountry;
	QModelIndex countryIndex;

	for (auto que = m_storage->query(); que.next(); ) {
		auto mcc = que.value("mcc").toString();
		auto mnc = que.value("mnc").toString();
		auto oper = que.value("operator").toString();
		auto country = que.value("country").toString();
		QPixmap countryIcon(":/icons/" + que.value("code").toString() + ".png");
		QPixmap operatorIcon(":/icons/" + mcc + "_" + mnc + ".png");

		if (country != lastCountry) {
			countryIndex = addCountry(mcc, country, countryIcon);
			lastCountry = country;
		}
		addOperator(mcc, mnc, oper, operatorIcon, countryIcon, countryIndex);
	}
}

}
