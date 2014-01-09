#ifndef QBINARYDATAVIEWVIEWPORT_H
#define QBINARYDATAVIEWVIEWPORT_H

#include <QWidget>
#include <QVariant>
#include <QTimer>
#include <QPointer>
#include <QScrollArea>

#include "qbinarydatasource.h"

enum ViewportItemDataType {
    RowIndex = Qt::UserRole + 1,
    ColumnIndex = Qt::UserRole + 2,
    Offset = Qt::UserRole + 3,
    RawOffset = Qt::UserRole + 4,
    RawData = Qt::UserRole + 5,
    ViewportRect = Qt::UserRole + 6,
    ItemVisible = Qt::UserRole + 7
};

typedef QMap<int, QVariant> ViewportItemData;

class QBinaryDataViewViewport : public QWidget
{
    Q_OBJECT
public:

    enum {CURSOR_TIMEOUT = 500 };
    enum {NOT_PRINTABLE_ITEM = '.' };

    explicit QBinaryDataViewViewport(QWidget *parent = 0);
    virtual ~QBinaryDataViewViewport(void);

    void setDataSource(QBinaryDataSource *newDataSource);
    QBinaryDataSource *dataSource() const;

    void setGroupSize(quint8 newGroupSize);
    int groupSize() const;

    int topRow() const;
    int leftColumn() const;
    int linesPerPage() const;

    void initScrollArea(QAbstractScrollArea *scrollArea);

    bool isSupportedKeyEvent(QKeyEvent * event);

    void paintEvent(QPaintEvent * event);
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void resizeEvent(QResizeEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void keyPressEvent(QKeyEvent * event);

    QModelIndex cursorPosition(void) const;

signals:
    void cursorPositionChanged(const QModelIndex &prev, const QModelIndex &current);

public slots:
    void setCursorPosition(const QModelIndex &position);

    void setTopRow(int topRow);
    void setLeftColumn(int leftColumn);
    void cursorBlinkingTrigger();

    void moveCursorToIndex(const QModelIndex& index);

protected:
    QList<ViewportItemData> getDataToRender(int rowsPerScreen);

    void paintItem(QPainter &painter, const QRect &itemRect, const QMap<int,QVariant> &data);
    void paintViewport(QPainter &painter, QStringList &addresses, QStringList &presentations);
    void paintAddressBar(QPainter &painter, const QStringList &addresses);
    void paintPresenationBar(QPainter &painter, const QStringList &presenations);

    bool updateGeometry(QPainter &painter);
    QRegion calculateAddressBarRegion(QPainter &painter);
    QRegion calculatePresentationBarRegion(QPainter &painter);
    QRegion calculateViewportRegion(QPainter &painter);

    void startCursorBlinking();
    void stopCursorBlinking();
    void paintCursor(QPainter &painter);

    void updateCursorPositionByMouseEvent(QMouseEvent * event);
    void setupScrollBars();

    bool isVisibleIndex(const QModelIndex& index) const;
    void scrollToIndex(const QModelIndex& index);

private:

    /* cashe for the data  */
    class CashedData {
        int linesPerPage_;
        int topRow_;
        int totalRowCount_;
        QList<ViewportItemData> data_;
    public:
        CashedData();
        CashedData( int linesPerPage,
                    int topRow,
                    int totalRowCount,
                    QList<ViewportItemData> data
                    );
        CashedData( const CashedData &src );
        QList<ViewportItemData >& data();
        bool isCasheActual(
                int linesPerPage,
                int topRow,
                int totalRowCount
                );
        void reset();
    };

    QBinaryDataSource*      dataSource_;
    QAbstractScrollArea*    scrollArea_;
    quint8                  groupSize_;

    int                     linesPerPage_;
    int                     columnsPerPage_;
    bool                    setupScrollBarsNeeded_;

    int                     topRow_;
    int                     totalRowCount_;
    int                     leftColumn_;
    int                     totalColumnCount_;

    int                     cx_;
    int                     cy_;
    int                     xmargin_;
    int                     ymargin_;

    bool                    addressBarVisible_;
    bool                    presentationBarVisible_;

    CashedData              dataCache_;

    QList<ViewportItemData> dataOnTheScreen_;
    QPointer<QTimer>        cursorPaintTimer_;
    bool                    cursorVisibility_;

    QModelIndex             currentCursorPosition_;
    ViewportItemData        currentCursorItemData_;
    QByteArray              currentCursorInputBuffer_;
};

#endif // QBINARYDATAVIEWVIEWPORT_H
