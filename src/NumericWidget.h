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

    int rowHeight;      // row height exclude spacing
    int cellTextCount;  // character count exclude spacing
    int cellWidth;      // character count include spacing
    int columnWidth;    // column width exclude spacing

    // Y axis
    int hHeaderTop;     // horizontal header bottom
    int hLineTop;       // horizontal line top
    int textTop;        // text field top

    // X axis
    int vHeaderTextCount;
    int vHeaderWidth;
    int vHeaderLeft;    // vertical header left
    int vLineLeft;      // vertical line left
    int textLeft;       // text field left

    int displayRowCount;
    int displayColumnCount;

    int vSpacing;
    int hSpacing;
    const int hSpacingCount = 2;
    const int lineWidth = 2;

    QString emptyBuffer;
    QVector<QString> textBuffer;    // 当前用于显示的文本内容
    QRect bufferRect;               // textBuffer中各单元格对应的行号和列号
};

#endif // NUMERICWIDGET_H
