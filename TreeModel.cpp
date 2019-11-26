#include "TreeModel.h"
#include "TreeItem.h"

#include <QMap>

using namespace mobile;

TreeModel::TreeModel(QObject *parent)
	: QAbstractItemModel(parent)
	, m_root(std::make_unique<TreeItem>(nullptr))
{
}

TreeModel::~TreeModel()
{
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	auto parentItem = at(parent);
	if (row >= parentItem->rowCount() || column >= parentItem->columnCount() || row < 0 || column < 0) {
		return {};
	}
	return createIndex(row, column, at(parent)->at(row));
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	return indexOf(at(index)->m_parent);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	// only first column has children
	if (parent.isValid() && parent.column() != 0) {
		return 0;
	}
	return at(parent)->rowCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	return at(parent)->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return {};
	}
	return at(index)->data(index.column(), role);
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (data(index, role) == value) {
		return false;
	}
	if (!at(index)->setData(value, index.column(), role)) {
		return false;
	}
	emit dataChanged(index, index, {role});
	return true;
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
	auto item = at(parent);
	if (!item->canInsertRows(row, count)) {
		return false;
	}
	beginInsertRows(parent, row, row + count - 1);
	item->insertRows(row, count);
	endInsertRows();

	return true;
}

bool TreeModel::insertColumns(int column, int count, const QModelIndex &parent)
{
	auto item = at(parent);
	if (!item->canInsertColumns(column, count)) {
		return false;
	}
	beginInsertColumns(parent, column, column + count - 1);
	item->insertColumns(column, count);
	endInsertColumns();

	return true;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
	auto item = at(parent);
	if (!item->canRemoveRows(row, count)) {
		return false;
	}
	beginRemoveRows(parent, row, row + count - 1);
	item->removeRows(row, count);
	endRemoveRows();

	return true;
}

void TreeModel::resetModel()
{
	beginResetModel();
	m_root = std::make_unique<TreeItem>(nullptr);
	endResetModel();
}

TreeItem *TreeModel::at(const QModelIndex &index)
{
	return const_cast<TreeItem *>(static_cast<const TreeModel *>(this)->at(index));
}

const TreeItem *TreeModel::at(const QModelIndex &index) const
{
	auto item = reinterpret_cast<const TreeItem *>(index.internalPointer());
	if (!item) {
		item = m_root.get();
	}
	return item;
}

QModelIndex TreeModel::indexOf(TreeItem *item) const
{
	if (!item || !item->m_parent) {
		return {};
	}
	return createIndex(item->m_parent->indexOf(item), 0, item);
}
