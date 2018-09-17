#ifndef MODEL_H
#define MODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QSize>

class Model : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit Model(QObject* parent = nullptr);

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void fontChanged(QFont font);

private:
    QSize itemSize;
};

#endif // MODEL_H
