#ifndef QBINARYDATAVIEWVIEWPORT_H
#define QBINARYDATAVIEWVIEWPORT_H

#include <QWidget>
#include <QTimer>
#include <QScrollArea>
#include "qbinarydatasource.h"

class QBinaryDataViewViewport : public QWidget
{
    Q_OBJECT
public:

    enum {CURSOR_TIMEOUT = 500 };

    enum {
        RowIndex = Qt::UserRole + 1,
        ColumnIndex = Qt::UserRole + 2,
        Offset = Qt::UserRole + 3,
        RawOffset = Qt::UserRole + 4,
        RawData = Qt::UserRole + 5,
        ViewportRect = Qt::UserRole + 6,
        ItemVisible = Qt::UserRole + 7
    };

    explicit QBinaryDataViewViewport(QWidget *parent = 0);
    virtual ~QBinaryDataViewViewport(void);

    void setDataSource(QBinaryDataSource *newDataSource);
    QBinaryDataSource *dataSource() const;

    void setGroupSize(quint8 newGroupSize);
    int groupSize() const;

    int topRow() const;
    int leftColumn() const;
    int linesPerPage() const;

    void setupScrollArea(QAbstractScrollArea *scrollArea);

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

protected:
    QList<QMap<int,QVariant> > getDataToRender(int rowsPerScreen);

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

private:
    QBinaryDataSource *dataSource_;
    QAbstractScrollArea *scrollArea_;
    quint8 groupSize_; /* bytes per group */
    int linesPerPage_;
    int columnsPerPage_;
    bool setupScrollBarsNeeded_;
    int topRow_;
    int totalRowCount_;
    int leftColumn_;
    int totalColumnCount_;

    int cx_;
    int cy_;
    int xmargin_;
    int ymargin_;

    bool addressBarVisible_;
    bool presentationBarVisible_;

    /* cashe for the data  */
    class CashedData {
        int linesPerPage_;
        int topRow_;
        int totalRowCount_;
        QList<QMap<int,QVariant> > data_;
    public:
        CashedData();
        CashedData( int linesPerPage,
                    int topRow,
                    int totalRowCount,
                    QList<QMap<int,QVariant> > data
                    );
        CashedData( const CashedData &src );
        QList<QMap<int,QVariant> >& data();
        bool isCasheActual(
                int linesPerPage,
                int topRow,
                int totalRowCount
                );
        void reset();
    };
    CashedData dataCache_;

    QTimer *cursorPaintTimer_;
    QList<QMap<int,QVariant> > dataOnTheScreen_;
    QModelIndex currentCursorPosition_;
    bool cursorVisibility_;
    QMap<int,QVariant> currentCursorItemData_;
    QByteArray currentCursorInputBuffer_;
};

#endif // QBINARYDATAVIEWVIEWPORT_H
