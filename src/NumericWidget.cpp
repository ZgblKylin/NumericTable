#include "NumericWidget.h"
#include "NumericTablePrivate.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <QtGui/QtEvents>
#include <QtGui/QPainter>

NumericWidget::NumericWidget(NumericTable* parent)
    : QWidget(parent),
      q(parent)
{
    setVerticalHeaderWidth(5);
    setColumnWidth(5);
}

int NumericWidget::getVerticalHeaderWidth() const
{
    return vHeaderTextCount;
}

void NumericWidget::setVerticalHeaderWidth(int characters)
{
    if (vHeaderTextCount != characters)
    {
        vHeaderTextCount = characters;
        vHeaderWidth = fontMetrics().averageCharWidth() * characters;
        dataChanged();
        emit verticalHeaderWidthChanged(vHeaderTextCount);
    }
}

void NumericWidget::resetVerticalHeaderWidth()
{
    if (model)
    {
        QString text = QString::number(model->rowCount());
        setVerticalHeaderWidth(text.count());
    }
}

int NumericWidget::getColumnWidth() const
{
    return cellTextCount;
}

void NumericWidget::setColumnWidth(int characters)
{
    if (cellTextCount != characters)
    {
        cellTextCount = characters;
        cellWidth = cellTextCount + hSpacingCount;
        columnWidth = fontMetrics().averageCharWidth() * characters;
        dataChanged();
        emit columnWidthChanged(cellTextCount);
    }
}

void NumericWidget::resetColumnWidth()
{
    if (model)
    {
        QString text = QString::number(model->columnCount());
        setColumnWidth(text.count());
    }
}

QAbstractTableModel* NumericWidget::getModel() const
{
    return model;
}

void NumericWidget::setModel(QAbstractTableModel* m)
{
    if (!m || (model == m)) {
        return;
    }

    if (model == m)
    {
        disconnect(model, 0, this, 0);
        model->setParent(nullptr);
    }
    disconnect(m, 0, this, 0);

    model = m;
    model->setParent(this);
    connect(model, &QAbstractItemModel::columnsInserted,
            this, &NumericWidget::dataChanged);
    connect(model, &QAbstractItemModel::columnsMoved,
            this, &NumericWidget::dataChanged);
    connect(model, &QAbstractItemModel::columnsRemoved,
            this, &NumericWidget::dataChanged);
    connect(model, &QAbstractItemModel::rowsInserted,
            this, &NumericWidget::dataChanged);
    connect(model, &QAbstractItemModel::rowsMoved,
            this, &NumericWidget::dataChanged);
    connect(model, &QAbstractItemModel::rowsRemoved,
            this, &NumericWidget::dataChanged);
    connect(model, &QAbstractItemModel::dataChanged,
            this, &NumericWidget::dataChanged);
    connect(model, &QAbstractItemModel::headerDataChanged,
            this, &NumericWidget::dataChanged);
    connect(model, &QAbstractItemModel::layoutChanged,
            this, &NumericWidget::dataChanged);
    connect(model, &QAbstractItemModel::modelReset,
            this, &NumericWidget::dataChanged);

    dataChanged();
}

void NumericWidget::dataChanged()
{
    QFont font = q->font();
    font.setFamily(QStringLiteral("Consolas"));
    setFont(font);

    if (!model) {
        return;
    }
    q->horizontalScrollBar()->setRange(0, model->columnCount());
    q->verticalScrollBar()->setRange(0, model->rowCount());
    if ((model->rowCount() == 0) || (model->columnCount() == 0)) {
        return;
    }

    setVerticalHeaderWidth(vHeaderTextCount);
    setColumnWidth(cellTextCount);

    QFontMetrics metrics = fontMetrics();

    hSpacing = metrics.averageCharWidth() * hSpacingCount;
    vSpacing = metrics.leading() + metrics.descent();

    rowHeight = metrics.ascent();
    hHeaderTop = vSpacing;
    hLineTop = hHeaderTop + rowHeight + vSpacing;
    textTop = hLineTop + lineWidth + vSpacing;

    vHeaderLeft = vSpacing;
    vLineLeft = vHeaderLeft + vHeaderWidth + hSpacing;
    textLeft = vLineLeft + lineWidth + hSpacing;

    displayRowCount = std::floor((height() - textTop) / qreal(rowHeight + vSpacing));
    QScrollBar* verticalScrollBar = q->verticalScrollBar();
    int vMax = std::max(0, model->rowCount() - displayRowCount);
    verticalScrollBar->setMaximum(vMax);
    verticalScrollBar->setPageStep(displayRowCount);

    displayColumnCount = std::floor((width() - textLeft) / qreal(columnWidth + hSpacing));
    QScrollBar* horizontalScrollBar = q->horizontalScrollBar();
    int hMax = std::max(0, model->columnCount() - displayColumnCount);
    horizontalScrollBar->setMaximum(hMax);
    horizontalScrollBar->setPageStep(displayColumnCount);

    if ((displayRowCount <= 0) || (displayColumnCount <= 0)) {
        return;
    }

    bufferFillText();

    update();
}

void NumericWidget::wheelEvent(QWheelEvent* event)
{
    qApp->sendEvent(q->horizontalScrollBar(), static_cast<QEvent*>(event));
    qApp->sendEvent(q->verticalScrollBar(), event);
}

void NumericWidget::resizeEvent(QResizeEvent*)
{
    dataChanged();
}

void NumericWidget::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
    case QEvent::FontChange:
        dataChanged();
        break;
    default:
        break;
    }
}

void NumericWidget::paintEvent(QPaintEvent*)
{
    if (!model) {
        return;
    }
    if ((displayRowCount <= 0) || (displayColumnCount <= 0)) {
        return;
    }

    QPainter painter(this);
    painter.fillRect(rect(), palette().color(QPalette::Base));
    drawLines(painter);
    drawVerticalHeader(painter);
    drawColumns(painter);
}

void NumericWidget::drawLines(QPainter& painter)
{
    painter.save();
    painter.setPen(QPen(painter.pen().color(),
                        lineWidth,
                        Qt::SolidLine));
    painter.drawLine(QPoint(0, hLineTop),
                     QPoint(width(), hLineTop));
    painter.drawLine(QPoint(vLineLeft, 0),
                     QPoint(vLineLeft, height()));
    painter.restore();
}

void NumericWidget::drawVerticalHeader(QPainter& painter)
{
    QString text;
    for (int row = bufferRect.top(); row <= bufferRect.bottom(); ++row)
    {
        text += model->headerData(row, Qt::Vertical, Qt::DisplayRole).toString();
        text += "\n";
    }

    QRect rect = QRect(QPoint(vHeaderLeft, textTop),
                       QSize(vHeaderWidth, height() - textTop));
    painter.drawText(rect,
                     Qt::AlignTop | Qt::AlignRight | Qt::TextDontClip,
                     text);
}

void NumericWidget::drawColumns(QPainter& painter)
{
    for (int column = bufferRect.left(); column <= bufferRect.right(); ++column)
    {
        int i = column - bufferRect.left();
        QRect rect = QRect(QPoint(textLeft + i * (columnWidth + hSpacing),
                                  hHeaderTop),
                           QSize(columnWidth,
                                 rowHeight));
        QString text = QStringLiteral("%1")
                       .arg(model->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString(),
                            cellTextCount,
                            QLatin1Char(' '));
        painter.drawText(rect,
                         Qt::AlignCenter | Qt::TextDontClip,
                         text);
    }

    QRect rect = QRect(QPoint(textLeft, textTop),
                       QSize(width() - textLeft, rowHeight + vSpacing));
    for (int i = 0; i < bufferRect.height(); ++i)
    {
        rect.moveTop(textTop + i * (rowHeight + vSpacing));
        painter.drawText(rect,
                         Qt::AlignTop | Qt::AlignLeft | Qt::TextDontClip,
                         textBuffer.at(i));
    }
}

void NumericWidget::bufferFillText()
{
    int rowBegin = q->verticalScrollBar()->value();
    int columnBegin = q->horizontalScrollBar()->value();
    int rowEnd = std::min(rowBegin + displayRowCount, model->rowCount());
    int columnEnd = std::min(columnBegin + displayColumnCount,
                             model->columnCount());

    int columnCharacterCount = displayColumnCount * cellWidth + 1;
    emptyBuffer.resize(columnCharacterCount);
    emptyBuffer.fill(QLatin1Char(' '));
    emptyBuffer[columnCharacterCount - 1] = '\n';

    QRect rect = QRect(QPoint(columnBegin, rowBegin),
                       QPoint(columnEnd - 1, rowEnd - 1));
    bufferAddWidth(rect.width() - bufferRect.width());
    bufferAddHeight(rect.height() - bufferRect.height());
    bufferMoveLeft(rect.left() - bufferRect.left());
    bufferMoveTop(rect.top() - bufferRect.top());
    bufferRect = rect;
}

void NumericWidget::bufferMoveLeft(int cellCount)
{
    bufferRect.moveLeft(bufferRect.left() + cellCount);

    int distance = std::abs(cellCount) * cellWidth;
    if (std::abs(cellCount) < displayColumnCount)
    {
        if (cellCount > 0)
        {
            for(QString& buffer : textBuffer)
            {
                std::copy(buffer.begin() + distance,
                          buffer.end(),
                          buffer.begin());
                std::fill(buffer.begin() + distance,
                          buffer.end(),
                          QChar(' '));
                buffer[buffer.count() - 1] = QChar('\n');
            }
            QRect intersected = bufferRect;
            intersected.setLeft(bufferRect.left() + cellCount);
            bufferFillCells(intersected);
        }
        else
        {
            for(QString& buffer : textBuffer)
            {
                std::copy(buffer.begin(),
                          buffer.end() - distance,
                          buffer.begin() + distance);
                std::fill(buffer.begin(),
                          buffer.end() - distance,
                          QChar(' '));
                buffer[buffer.count() - 1] = QChar('\n');
            }
            QRect intersected = bufferRect;
            intersected.setRight(bufferRect.right() - std::abs(cellCount));
            bufferFillCells(intersected);
        }
    }
    else
    {
        std::fill(textBuffer.begin(),
                   textBuffer.end(),
                   emptyBuffer);
        bufferFillCells(bufferRect);
    }
}

void NumericWidget::bufferMoveTop(int cellCount)
{
    bufferRect.moveTop(bufferRect.top() + cellCount);

    if (std::abs(cellCount) < displayRowCount)
    {
        int distance = std::abs(cellCount);
        if (cellCount > 0)
        {
            std::copy(textBuffer.begin() + distance,
                      textBuffer.end(),
                      textBuffer.begin());
            std::fill(textBuffer.begin() + distance,
                      textBuffer.end(),
                      emptyBuffer);
            QRect intersected = bufferRect;
            intersected.setTop(bufferRect.top() + cellCount);
            bufferFillCells(intersected);
        }
        else
        {
            std::copy(textBuffer.begin(),
                      textBuffer.end() - distance,
                      textBuffer.begin() + distance);
            std::fill(textBuffer.begin(),
                      textBuffer.end() - distance,
                      emptyBuffer);
            QRect intersected = bufferRect;
            intersected.setBottom(bufferRect.bottom() - std::abs(cellCount));
            bufferFillCells(intersected);
        }
    }
    else
    {
        std::fill(textBuffer.begin(),
                   textBuffer.end(),
                   emptyBuffer);
        bufferFillCells(bufferRect);
    }
}

void NumericWidget::bufferAddWidth(int cellCount)
{
    bufferRect.setWidth(bufferRect.width() + cellCount);
    int distance = std::abs(cellCount) * cellWidth;
    if (cellCount > 0)
    {
        for(QString& buffer : textBuffer)
        {
            buffer.resize(buffer.count() + distance);
            std::fill(buffer.end() - distance,
                      buffer.end(),
                      QChar(' '));
            buffer[buffer.count() - 1] = QChar('\n');
        }
        QRect intersected = bufferRect;
        intersected.setLeft(bufferRect.left() +
                            (bufferRect.width() - cellCount));
        bufferFillCells(intersected);
    }
    else
    {
        for(QString& buffer : textBuffer)
        {
            buffer = buffer.left(buffer.count() - distance);
            buffer[buffer.count() - 1] = QChar('\n');
        }
    }
}

void NumericWidget::bufferAddHeight(int cellCount)
{
    bufferRect.setHeight(bufferRect.height() + cellCount);
    int distance = std::abs(cellCount);
    if (cellCount > 0)
    {
        textBuffer.resize(textBuffer.count() + distance);
        std::fill(textBuffer.end() - distance,
                  textBuffer.end(),
                  emptyBuffer);
        QRect intersected = bufferRect;
        intersected.setTop(bufferRect.top() +
                           (bufferRect.height() - cellCount));
        bufferFillCells(intersected);
    }
    else
    {
        textBuffer.resize(textBuffer.count() - distance);
    }
}

void NumericWidget::bufferFillCells(const QRect& cells)
{
    for (int row = cells.top(); row <= cells.bottom(); ++row)
    {
        int i = row - bufferRect.top();
        QString& buffer = textBuffer[i];
        for (int col = cells.left(); col <= cells.right(); ++col)
        {
            int j = col - bufferRect.left();
            QString text = model->index(row, col).data(Qt::DisplayRole).toString();
            int index = j * cellWidth;
            index += cellTextCount;
            index -= text.count();
            std::copy(text.begin(), text.end(), buffer.begin() + index);
        }
    }
}
