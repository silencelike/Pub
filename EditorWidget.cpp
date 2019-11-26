#include "EditorWidget.h"

#include <QAction>

namespace mobile {

EditorWidget::EditorWidget(QWidget *parent)
	: QWidget(parent)
{
	QLabel *nameLabel = new QLabel(tr("Name:"));
	QLabel *mccLabel = new QLabel(tr("MCC:"));
	QLabel *mncLabel = new QLabel(tr("MNC:"));

	const QIcon icon(":/icons/buttonDelete.png");
	QAction *action = new QAction("X");
	m_nameLine = new QLineEdit;
	m_nameLine->addAction(action, QLineEdit::TrailingPosition);
	m_nameLine->setStyleSheet("background: rgb(255, 255, 255)");

	m_mccLine = new QLineEdit;
	m_mccLine->setStyleSheet("background: rgb(240, 240, 240)");
	m_mccLine->setReadOnly(true);

	m_mncLine = new QLineEdit;
	m_mncLine->setStyleSheet("background: rgb(240, 240, 240)");
	m_mncLine->setReadOnly(true);

	m_countryIconLabel = new QLabel(this);
	m_operatorIconLabel = new QLabel(this);

	QHBoxLayout *mccLayout = new QHBoxLayout;
	mccLayout->addWidget(m_mccLine);
	mccLayout->addWidget(m_countryIconLabel);

	m_saveButton = new QPushButton(tr("&Save"));
	m_cancelButton = new QPushButton(tr("&Cancel"));

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_saveButton);
	buttonsLayout->addWidget(m_cancelButton);

	QLabel *title = new QLabel(tr("Operator editor"));
	QHBoxLayout *topLayout = new QHBoxLayout;
	title->setStyleSheet("font-weight: bold; font-family: Arial; font-size: 10pt");
	topLayout->addWidget(m_operatorIconLabel, Qt::AlignLeft);
	topLayout->addWidget(title, Qt::AlignRight);

	QHBoxLayout *placeHolderLayout = new QHBoxLayout;
	placeHolderLayout->setMargin(3);

	QGridLayout *bottomLayout = new QGridLayout;
	bottomLayout->addWidget(nameLabel, 0, 0);
	bottomLayout->addWidget(m_nameLine, 0, 1);
	bottomLayout->addWidget(mccLabel, 1, 0);
	bottomLayout->addLayout(mccLayout, 1, 1);
	bottomLayout->addWidget(mncLabel, 2, 0);
	bottomLayout->addWidget(m_mncLine, 2, 1);
	bottomLayout->addLayout(buttonsLayout, 3, 1);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(placeHolderLayout);
	mainLayout->addLayout(bottomLayout);
	mainLayout->setAlignment(topLayout, Qt::AlignHCenter);
	setLayout(mainLayout);

	setMinimumWidth(280);
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
	setWindowTitle(tr("Operator editor"));

	connect(m_saveButton, &QPushButton::clicked, this, [this]() {
		if (m_editMode == EditMode::Update) {
			if (m_nameLine->text().isEmpty()) {
				m_nameLine->setStyleSheet("border: 2px solid red;");
				return;
			}
			emit updateRowRequest();
		} else
		if (m_editMode == EditMode::Insert) {
			if (m_nameLine->text().isEmpty()
				|| m_mccLine->text().isEmpty()
				|| m_mncLine->text().isEmpty()) {
				return;
			}
			emit insertRowRequest();
		}
		close();
	});
	connect(m_cancelButton, &QPushButton::clicked, this, &EditorWidget::close);
	connect(m_nameLine, &QLineEdit::textEdited, [this](const QString &str) {
		if (m_editMode == EditMode::Insert) {
			emit nameEdited(str);
		}
	});
}

void EditorWidget::setNameLine(const QString &value)
{
	m_nameLine->setText(value);
}

void EditorWidget::setMccLine(const QString &value)
{
	m_mccLine->setText(value);
}

void EditorWidget::setMncLine(const QString &value)
{
	 m_mncLine->setText(value);
}

void EditorWidget::setCountryIcon(const QPixmap &icon)
{
	m_countryIconLabel->setPixmap(icon);
}

void EditorWidget::setOperatorIcon(const QPixmap &icon)
{
	m_operatorIconLabel->setPixmap(icon);
}

QString EditorWidget::nameLine() const
{
	return m_nameLine->text();
}

QString EditorWidget::mccLine() const
{
	return m_mccLine->text();
}

QString EditorWidget::mncLine() const
{
	return m_mncLine->text();
}

const QPixmap *EditorWidget::countryIcon() const
{
	return m_countryIconLabel->pixmap();
}

const QPixmap *EditorWidget::operatorIcon() const
{
	return m_operatorIconLabel->pixmap();
}

EditMode EditorWidget::editMode() const
{
	return m_editMode;
}

void EditorWidget::setEditMode(EditMode mode)
{
	if (m_editMode == mode) {
		return;
	}
	m_editMode = mode;
	emit editModeChanged(m_editMode);
}

void EditorWidget::clear()
{
	m_nameLine->setText("");
	m_mccLine->setText("0");
	m_mncLine->setText("0");

	m_countryIconLabel->setPixmap(QPixmap());
	m_operatorIconLabel->setPixmap(QPixmap());
	m_countryIconLabel->setText("?");
	m_operatorIconLabel->setText("?");
	m_countryIconLabel->setStyleSheet("font-size: 17pt; color: blue;");
	m_operatorIconLabel->setStyleSheet("font-size: 17pt; color: blue;");
}

void EditorWidget::setSaveButtonEnabled(bool value)
{
	m_saveButton->setEnabled(value);
}

}
