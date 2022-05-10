#include "unitsmodel.h"
#include "../lib/unitobject.h"

UnitsModel::UnitsModel(QObject *parent)
    : QAbstractListModel(parent)
    , _manager(UnitsManager::Instance())
{
}

UnitsManager::UnitObjPtr UnitsModel::getUnit(const QModelIndex &index)
{
    return _manager->getUnit(index.row());
}

void UnitsModel::modelReset()
{
    beginResetModel();
    endResetModel();
}

int UnitsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (_manager == nullptr)
    {
        return 0;
    }

    return _manager->count();
}

QVariant UnitsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    {
        auto unit = _manager->getUnit(index.row());
        return unit->unitTitle() + " | " + unit->unitName();
    }

    return QVariant();
}
