#ifndef NUMERICTABLEPRIVATE_H
#define NUMERICTABLEPRIVATE_H

#include <QtCore/QObject>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollBar>
#include <NumericTable.h>
#include "NumericWidget.h"

class NumericTablePrivate : public QObject
{
    Q_OBJECT
public:
    explicit NumericTablePrivate(NumericTable* parent);

    virtual bool eventFilter(QObject* watched, QEvent* event) override;

    Q_DECLARE_PUBLIC(NumericTable)
    NumericTable* q_ptr;

    QGridLayout* layout;
    QScrollBar* horizontalScrollBar;
    QScrollBar* verticalScrollBar;
    NumericWidget* widget;
};

#endif // NUMERICTABLEPRIVATE_H
