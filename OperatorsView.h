#pragma once
#include "EditorWidget.h"
#include "storage.h"

#include <QObject>
#include <QTreeView>

namespace mobile {

class OperatorsView : public QTreeView
{
    Q_OBJECT
 public:
    explicit OperatorsView(const std::shared_ptr<Storage> &storage);
    ~OperatorsView();

private slots:
    void updateRow();
    void insertRow();

private:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void openEditor(EditMode mode, const QModelIndex &index = QModelIndex());
    void closeEvent(QCloseEvent *event) override;

    EditorWidget *m_editor = nullptr;
    std::shared_ptr<Storage> m_storage;

    QPushButton *m_addButton;

    QModelIndex m_indexForInsert = QModelIndex();
};

}
