#ifndef UNITSMODEL_H
#define UNITSMODEL_H

#include <QAbstractListModel>
#include "../lib/unitsmanager.h"

class UnitsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit UnitsModel(QObject *parent = nullptr);

    UnitsManager::UnitObjPtr getUnit(const QModelIndex& index);
    void modelReset();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    UnitsManager* _manager = nullptr;
};

#endif // UNITSMODEL_H
