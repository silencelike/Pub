#pragma once
#include "OperatorsHelper.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace mobile {

class EditorWidget : public QWidget
{
    Q_OBJECT
public:
    EditorWidget(QWidget *parent = nullptr);

    void setNameLine(const QString &value);
    void setMccLine(const QString &value);
    void setMncLine(const QString &value);

    void setCountryIcon(const QPixmap &icon);
    void setOperatorIcon(const QPixmap &icon);

    QString nameLine() const;
    QString mccLine() const;
    QString mncLine() const;

    const QPixmap *countryIcon() const;
    const QPixmap *operatorIcon() const;

    EditMode editMode() const;
    void setEditMode(EditMode mode);

    void clear();
    void setSaveButtonEnabled(bool value);

signals:
    void updateRowRequest();
    void insertRowRequest();
    void cancelClicked();
    void editModeChanged(EditMode mode);
    void nameEdited(const QString &str);

private:
    QLabel *m_nameLabel;
    QLabel *m_mccLabel;
    QLabel *m_mncLabel;

    QLineEdit *m_nameLine;
    QLineEdit *m_mccLine;
    QLineEdit *m_mncLine;

    QLabel *m_countryIconLabel;
    QLabel *m_operatorIconLabel;

    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;

    EditMode m_editMode = EditMode::Undefined;
};

}
