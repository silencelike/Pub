#include "OperatorsView.h"
#include "OperatorsDelegate.h"
#include "OperatorsModel.h"

#include <QModelIndex>
#include <QMouseEvent>

namespace mobile {

OperatorsView::OperatorsView(const std::shared_ptr<Storage> &storage)
    : m_storage(storage)
{
    auto width = 600;
    auto height = 700;
    auto addButtonWidth = 35;
    auto addButtonheight = 35;

    setStyleSheet("background-color: rgb(245, 245, 245)");
    resize(width, height);

    m_addButton = new QPushButton("+", this);
    m_addButton->setStyleSheet("font-size: 17pt");
    m_addButton->resize(addButtonWidth, addButtonheight);
    m_addButton->move(width - m_addButton->width() * 2, height - m_addButton->height() * 1.5);

    connect(m_addButton, &QPushButton::clicked, this, [this]() {
        openEditor(EditMode::Insert);
    });

    connect(this, &QAbstractItemView::clicked, this, [this]() {
        if (!currentIndex().parent().isValid()) {
            if (!isExpanded(currentIndex())) {
                expand(currentIndex());
            } else {
                collapse(currentIndex());
            }
            return;
        }
    });

}

OperatorsView::~OperatorsView()
{
    if (m_editor != nullptr) {
        delete m_editor;
    }
}

void OperatorsView::updateRow()
{
    itemDelegate()->setModelData(m_editor, model(), currentIndex());

    const auto currIndex = currentIndex();
    const auto model = currIndex.model();

    auto mcc = model->data(currIndex, RoleNames::Mcc).toString();
    auto mnc = model->data(currIndex, RoleNames::Mnc).toString();
    auto name = model->data(currIndex, RoleNames::OperatorName).toString();

    m_storage->updateRecord(mcc, mnc, name);
}

void OperatorsView::insertRow()
{
    if (m_indexForInsert.isValid()) {
        auto mcc = m_editor->mccLine();
        auto mnc = m_editor->mncLine();
        auto name = m_editor->nameLine();
        auto icon = m_editor->operatorIcon();
        auto countryIcon = m_editor->countryIcon();
        OperatorsModel *model = static_cast<OperatorsModel*>(this->model());
        model->addOperator(mcc, mnc, name, *icon, *countryIcon, m_indexForInsert);
        m_indexForInsert = {};
        m_storage->insertRecord(mcc, mnc, name);
    }
}

void OperatorsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    auto currIndex = currentIndex();

    if (event->buttons() != Qt::MouseButton::LeftButton) {
       return;
    }
    if (!currIndex.parent().isValid()) {
        if (!isExpanded(currIndex)) {
            expand(currIndex);
        } else {
            collapse(currIndex);
        }
        return;
    }
    if (itemDelegate(currIndex) == nullptr) {
        return;
    }
    openEditor(EditMode::Update, currIndex);
}

void OperatorsView::openEditor(EditMode mode, const QModelIndex &index)
{
    if (m_editor == nullptr) {
        m_editor = static_cast<EditorWidget*>(itemDelegate()->createEditor(nullptr, QStyleOptionViewItem(), index));
        connect(m_editor, &EditorWidget::updateRowRequest, this, &OperatorsView::updateRow);
        connect(m_editor, &EditorWidget::insertRowRequest, this, &OperatorsView::insertRow);
        connect(m_editor, &EditorWidget::nameEdited, this, [this](const QString &str) {
            OperatorsModel *model = static_cast<OperatorsModel*>(this->model());

            auto indexList = model->match(model->index(0, 0), RoleNames::OperatorName, str, -1, Qt::MatchRecursive);
            for (auto index: indexList) {
                if (index.parent().isValid()) {
                    m_indexForInsert = index.parent();
                    itemDelegate(index)->setEditorData(m_editor, index);
                    m_editor->setSaveButtonEnabled(true);
                    break;
                }
            }
        });
    }

    m_editor->setEditMode(mode);
    if (mode == EditMode::Update) {
        itemDelegate(index)->setEditorData(m_editor, index);
        m_editor->setSaveButtonEnabled(true);
    } else
    if (mode == EditMode::Insert) {
        m_editor->clear();
        m_editor->setSaveButtonEnabled(false);
    }

    if (m_editor->isVisible() && !m_editor->isActiveWindow()) {
        m_editor->activateWindow();
        return;
    }
    m_editor->show();
}

void OperatorsView::closeEvent(QCloseEvent *event)
{
    if (m_editor != nullptr) {
        m_editor->close();
    }
    QWidget::closeEvent(event);
}

}
