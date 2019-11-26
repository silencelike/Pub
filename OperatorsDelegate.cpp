#include "OperatorsDelegate.h"
#include "EditorWidget.h"

namespace mobile {

OperatorsDelegate::OperatorsDelegate(QObject *parent)
{
}

QWidget *OperatorsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QWidget *editor = new EditorWidget(parent);
	return editor;
}

void OperatorsDelegate::setEditorData(QWidget *editorWidget, const QModelIndex &index) const
{
	EditorWidget *editor = static_cast<EditorWidget*>(editorWidget);

	editor->setMccLine(index.model()->data(index, RoleNames::Mcc).toString());
	editor->setMncLine(index.model()->data(index, RoleNames::Mnc).toString());
	editor->setNameLine(index.model()->data(index, RoleNames::OperatorName).toString());
	editor->setOperatorIcon(qvariant_cast<QPixmap>(index.model()->data(index, RoleNames::OperatorIcon)));
	editor->setCountryIcon(qvariant_cast<QPixmap>(index.model()->data(index, RoleNames::CountryIcon)));
}

void OperatorsDelegate::setModelData(QWidget *editorWidget, QAbstractItemModel *model, const QModelIndex &index) const
{
	EditorWidget *editor = static_cast<EditorWidget*>(editorWidget);
	model->setData(index, editor->nameLine(), RoleNames::OperatorName);
}

void OperatorsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

void OperatorsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
	auto isParent = !index.parent().isValid();
	auto nameRole = isParent ? RoleNames::CountryName : RoleNames::OperatorName;
	auto imageRole = isParent ? RoleNames::CountryIcon : RoleNames::OperatorIcon;

	const auto mcc = index.model()->data(index, RoleNames::Mcc).toString();
	const auto mnc = index.model()->data(index, RoleNames::Mnc).toString();
	const auto name = index.model()->data(index, nameRole).toString();
	const auto image = qvariant_cast<QPixmap>(index.model()->data(index, imageRole));

	const auto text = QString("%1 (%2, %3)").arg(name).arg(mcc).arg(mnc);
	auto textIndent = 5;
	auto textRectWidth = opt.rect.width() - image.width() * 3;

	painter->save();
	painter->drawPixmap(opt.rect.x(), opt.rect.y(), image);
	QRect textRect(opt.rect.x() + image.width() + textIndent, opt.rect.y(), textRectWidth, opt.rect.height());
	painter->drawText(textRect, isParent ? name : text);
	painter->restore();

	QStyledItemDelegate::paint(painter, opt, index);
}

QSize OperatorsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QSize(option.rect.width(), 20);
}

}
