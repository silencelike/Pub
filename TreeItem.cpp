#include "TreeItem.h"

#include <QVariant>

using namespace mobile;

TreeItem::TreeItem(TreeItem *parent)
	: m_parent(parent)
{
}

TreeItem::~TreeItem()
{
}

int TreeItem::rowCount() const
{
	return static_cast<int>(m_children.size());
}

int TreeItem::columnCount() const
{
	return static_cast<int>(m_data.size());
}

void TreeItem::insertRows(int first, int count)
{
	if (!canInsertRows(first, count)) {
		return;
	}
	for (auto row = first, size = first + count; row < size; ++row) {
		insertNewChild(row);
	}
}

void TreeItem::removeRows(int first, int count)
{
	if (!canRemoveRows(first, count)) {
		return;
	}
	auto begin = m_children.begin() + first;
	auto end = begin + count;
	m_children.erase(begin, end);
}

void TreeItem::insertColumns(int first, int count)
{
	if (!canInsertColumns(first, count)) {
		return;
	}
	for (auto &child : m_children) {
		child->insertColumns(first, count);
	}
	while (count-- > 0) {
		if (m_data.empty())
			m_data.push_back({});
		else
			m_data.insert(m_data.begin() + first, {});
	}
}

bool TreeItem::canInsertColumns(int first, int count) const
{
	return (count > 0) && first >= 0 && first <= columnCount();
}

bool TreeItem::canInsertRows(int first, int count) const
{
	return (count > 0) && first >= 0 && first <= rowCount();
}

bool TreeItem::canRemoveRows(int first, int count) const
{
	return (count > 0) && first >= 0 && (first + count - 1 < rowCount());
}

TreeItem *TreeItem::at(int row) const
{
	if (row >= static_cast<int>(m_children.size()) || row < 0) {
		return nullptr;
	}
	return m_children.at(static_cast<size_t>(row)).get();
}

QVariant TreeItem::data(int column, int role) const
{
	if (!isValidColumn(column)) {
		return {};
	}
	return m_data[static_cast<size_t>(column)].value(role);
}

bool TreeItem::setData(const QVariant &value, int column, int role)
{
	if (!isValidColumn(column)) {
		return false;
	}
	m_data[static_cast<size_t>(column)][role] = value;
	return true;
}

int TreeItem::indexOf(TreeItem *item) const
{
	auto it = std::find_if(m_children.begin(), m_children.end(), [item](const auto &x) {
		return x.get() == item;
	});

	if (it == m_children.end()) {
		return -1;
	}
	return static_cast<int>(std::distance(m_children.begin(), it));
}

void TreeItem::insertNewChild(int row)
{
	auto child = std::make_unique<TreeItem>(this);
	child->insertColumns(0, columnCount());
	m_children.insert(m_children.begin() + row, std::move(child));
}

bool TreeItem::isValidColumn(int value) const
{
	return value >= 0 && value < columnCount();
}
