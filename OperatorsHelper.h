#pragma once

#include <QMetaObject>
#include <QObject>

namespace mobile {

enum RoleNames
{
    Mcc = Qt::UserRole + 1,
    Mnc,
    CountryName,
    CountryIcon,
    OperatorName,
    OperatorIcon,
    AddButton
};

namespace EditModeNs {
Q_NAMESPACE

enum class EditMode : uint
{
    Update,
    Insert,

    Undefined
};
Q_ENUM_NS(EditMode)
}
using EditMode = EditModeNs::EditMode;

}
