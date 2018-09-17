#include <NumericTable.h>
#include "NumericTablePrivate.h"
#include <QtCore/QEvent>
#include <QtGui/QFontDatabase>

NumericTablePrivate::NumericTablePrivate(NumericTable* parent)
    : QObject(parent),
      layout(new QGridLayout(parent)),
      horizontalScrollBar(new QScrollBar(Qt::Horizontal, parent)),
      verticalScrollBar(new QScrollBar(Qt::Vertical, parent)),
      widget(new NumericWidget(parent))
{
    QFontDatabase::addApplicationFont(QStringLiteral(":/Fonts/consola.ttf"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/Fonts/consolab.ttf"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/Fonts/consolai.ttf"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/Fonts/consolaz.ttf"));

    parent->resize(400, 300);

    layout->addWidget(widget, 0, 0);
    layout->addWidget(horizontalScrollBar, 1, 0);
    layout->addWidget(verticalScrollBar, 0, 1);

    horizontalScrollBar->setSingleStep(1);
    connect(horizontalScrollBar, &QScrollBar::valueChanged,
            widget, &NumericWidget::dataChanged);

    verticalScrollBar->setSingleStep(1);
    connect(verticalScrollBar, &QScrollBar::valueChanged,
            widget, &NumericWidget::dataChanged);

    connect(widget, &NumericWidget::verticalHeaderWidthChanged,
            parent, &NumericTable::verticalHeaderWidthChanged);
    connect(widget, &NumericWidget::columnWidthChanged,
            parent, &NumericTable::columnWidthChanged);

    parent->installEventFilter(this);
}

bool NumericTablePrivate::eventFilter(QObject* watched, QEvent* event)
{
    bool ret = QObject::eventFilter(watched, event);
    switch (event->type())
    {
    case QEvent::Resize:
    case QEvent::LanguageChange:
    case QEvent::FontChange:
        widget->dataChanged();
        break;
    default:
        break;
    }
    return ret;
}

NumericTable::NumericTable(QWidget* parent)
    : QFrame(parent),
      d_ptr(new NumericTablePrivate(this))
{
}

NumericTable::~NumericTable()
{
    delete d_ptr;
}

QScrollBar* NumericTable::horizontalScrollBar() const
{
    Q_D(const NumericTable);
    return d->horizontalScrollBar;
}

QScrollBar* NumericTable::verticalScrollBar() const
{
    Q_D(const NumericTable);
    return d->verticalScrollBar;
}

int NumericTable::verticalHeaderWidth() const
{
    Q_D(const NumericTable);
    return d->widget->getVerticalHeaderWidth();
}

void NumericTable::setVerticalHeaderWidth(int characters)
{
    Q_D(NumericTable);
    d->widget->setVerticalHeaderWidth(characters);
}

void NumericTable::resetVerticalHeaderWidth()
{
    Q_D(NumericTable);
    d->widget->resetVerticalHeaderWidth();
}

int NumericTable::columnWidth() const
{
    Q_D(const NumericTable);
    return d->widget->getColumnWidth();
}

void NumericTable::setColumnWidth(int characters)
{
    Q_D(NumericTable);
    d->widget->setColumnWidth(characters);
}

void NumericTable::resetColumnWidth()
{
    Q_D(NumericTable);
    d->widget->resetColumnWidth();
}

QAbstractTableModel* NumericTable::model() const
{
    Q_D(const NumericTable);
    return d->widget->getModel();
}

void NumericTable::setModel(QAbstractTableModel* model, bool autoReset)
{
    Q_D(NumericTable);
    d->widget->setModel(model);
    if (autoReset)
    {
        resetVerticalHeaderWidth();
        resetColumnWidth();
    }
}
