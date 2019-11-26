#pragma once

#include <QAbstractItemModel>
#include <memory>

namespace mobile {

class TreeItem;

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	explicit TreeModel(QObject *parent = nullptr);
	~TreeModel() override;

	QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = {}) const override;
	int columnCount(const QModelIndex &parent = {}) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	bool insertRows(int row, int count, const QModelIndex &parent = {}) override;
	bool insertColumns(int column, int count, const QModelIndex &parent = {}) override;
	bool removeRows(int row, int count, const QModelIndex &parent = {}) override;

	void resetModel();

private:
	const TreeItem *at(const QModelIndex &index) const;
	TreeItem *at(const QModelIndex &index);
	QModelIndex indexOf(TreeItem *item) const;

private:
	std::unique_ptr<TreeItem> m_root;
};

}
