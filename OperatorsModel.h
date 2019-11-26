#pragma once
#include "TreeModel.h"
#include "Storage.h"

#include <memory>
#include "OperatorsHelper.h"

namespace mobile {

class OperatorsModel : public TreeModel
{
public:
    explicit OperatorsModel(const std::shared_ptr<Storage> &storage, QObject *parent = nullptr);

    QModelIndex addCountry(const QString &mcc, const QString &name, const QPixmap &icon);

    void addOperator(const QString &mcc, const QString &mnc, const QString &name,
                     const QPixmap &icon, const QPixmap &countryIcon, const QModelIndex &parentIndex);
    void fillOperators();

private:
    std::shared_ptr<Storage> m_storage;
};

}
