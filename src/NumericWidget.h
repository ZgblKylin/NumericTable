#ifndef NUMERICWIDGET_H
#define NUMERICWIDGET_H

#include <QtCore/QAbstractTableModel>
#include <QtWidgets/QWidget>
#include <NumericTable>

class NumericTablePrivate;
class NumericWidget : public QWidget
{
    Q_OBJECT
public:
    NumericWidget(NumericTable* parent = nullptr);

    int getVerticalHeaderWidth() const;
    void setVerticalHeaderWidth(int characters);
    void resetVerticalHeaderWidth();
    Q_SIGNAL void verticalHeaderWidthChanged(int characters);

    int getColumnWidth() const;
    void setColumnWidth(int characters);
    void resetColumnWidth();
    Q_SIGNAL void columnWidthChanged(int characters);

    QAbstractTableModel* getModel() const;
    void setModel(QAbstractTableModel* model);

    void dataChanged();

protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void changeEvent(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    void drawLines(QPainter& painter);
    void drawVerticalHeader(QPainter& painter);
    void drawColumns(QPainter& painter);

    void bufferFillText();
    void bufferMoveLeft(int cellCount);
    void bufferMoveTop(int cellCount);
    void bufferAddWidth(int cellCount);
    void bufferAddHeight(int cellCount);
    void bufferFillCells(const QRect& cells);

    NumericTable* q;
    QAbstractTableModel* model = nullptr;

    int vHeaderTextCount;   // max count of vertical header text characters
    int vHeaderWidth;       // vertical header width in pixels

    int cellTextCount;      // max count of cell text characters
    int cellWidth;          // character count include spacing
    int columnWidth;        // column width in pixels, exclude spacing

    // Y axis
    int hHeaderTop;     // horizontal header bottom position in pixels
    int hLineTop;       // horizontal line top position in pixels
    int textTop;        // text field top position in pixels
    int rowHeight;      // row height exclude spacing

    // X axis
    int vHeaderLeft;        // vertical header left position in pixels
    int vLineLeft;          // vertical line left position in pixels
    int textLeft;           // text field left position in pixels

    int displayRowCount;    // row count displayed based on font and widget rect
    int displayColumnCount; // col count displayed based on font and widget rect

    int vSpacing;                   // horizontal spacing in pixels
    int hSpacing;                   // vertical spacing in pixels
    const int hSpacingCount = 2;    // horizontal spacing character count(' ')
    const int lineWidth = 2;        // horizontal and vertical line width

    QString emptyBuffer;
    QVector<QString> textBuffer;    // current text data to be displayed
    QRect bufferRect;               // index row and col for cells in textBuffer
};

#endif // NUMERICWIDGET_H
