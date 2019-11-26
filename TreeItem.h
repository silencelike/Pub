#pragma once

#include <QMap>
#include <memory>
#include <vector>

class QVariant;

namespace mobile {

class TreeItem
{
public:
	friend class TreeModel;

	TreeItem(TreeItem *parent);
	virtual ~TreeItem();

	int rowCount() const;
	int columnCount() const;
	QVariant data(int column, int role) const;
	bool setData(const QVariant &value, int column, int role);

	void insertRows(int first, int count);
	void removeRows(int first, int count);
	void insertColumns(int first, int count);

	bool canInsertRows(int first, int count) const;
	bool canRemoveRows(int first, int count) const;
	bool canInsertColumns(int first, int count) const;

	int indexOf(TreeItem *item) const;
	TreeItem *at(int row) const;

private:
	void insertNewChild(int row);
	bool isValidColumn(int value) const;

private:
	TreeItem *m_parent = nullptr;
	std::vector<std::unique_ptr<TreeItem>> m_children;
	std::vector<QMap<int, QVariant>> m_data;
};

}
