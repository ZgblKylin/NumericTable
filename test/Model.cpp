#include "Model.h"
#include <QtGui/QFontMetrics>

Model::Model(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int Model::rowCount(const QModelIndex&) const
{
    return 10000000;
}

int Model::columnCount(const QModelIndex&) const
{
    return 10000;
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return (quint64(index.row()) * quint64(columnCount()) + index.column()) % 256;
    case Qt::TextAlignmentRole:
        return int(Qt::Alignment(Qt::AlignRight | Qt::AlignHCenter));
    default:
        break;
    }

    return QVariant();
}
