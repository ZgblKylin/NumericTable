#ifndef NUMERICTABLE_H
#define NUMERICTABLE_H

#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollBar>
#include <QtCore/QAbstractTableModel>

#ifndef NUMERICTABLE_LIB
#define NUMERICTABLE_SHARED_EXPORT Q_DECL_IMPORT
#else
#define NUMERICTABLE_SHARED_EXPORT Q_DECL_EXPORT
#endif

class NumericTablePrivate;
class NUMERICTABLE_SHARED_EXPORT NumericTable : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int verticalHeaderWidth READ verticalHeaderWidth WRITE setVerticalHeaderWidth NOTIFY verticalHeaderWidthChanged RESET resetVerticalHeaderWidth)
    Q_PROPERTY(int columnWidth READ columnWidth WRITE setColumnWidth NOTIFY columnWidthChanged RESET resetColumnWidth)

public:
    explicit NumericTable(QWidget* parent = nullptr);
    virtual ~NumericTable();

    QScrollBar* horizontalScrollBar() const;
    QScrollBar* verticalScrollBar() const;

    int verticalHeaderWidth() const;
    void setVerticalHeaderWidth(int characters);
    void resetVerticalHeaderWidth();
    Q_SIGNAL void verticalHeaderWidthChanged(int characters);

    int columnWidth() const;
    void setColumnWidth(int characters);
    void resetColumnWidth();
    Q_SIGNAL void columnWidthChanged(int characters);

    QAbstractTableModel* model() const;
    void setModel(QAbstractTableModel* model, bool autoReset = true);

private:
    Q_DECLARE_PRIVATE(NumericTable)
    NumericTablePrivate* d_ptr;
};

#endif // NUMERICTABLE_H
