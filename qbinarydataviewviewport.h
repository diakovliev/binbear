#ifndef QBINARYDATAVIEWVIEWPORT_H
#define QBINARYDATAVIEWVIEWPORT_H

#include <QWidget>
#include <QVariant>
#include <QTimer>
#include <QPointer>
#include <QScrollArea>

#include "qabstractbinarydatasource.h"

enum ViewportItemDataType {
    RowIndex = Qt::UserRole + 1,
    ColumnIndex = Qt::UserRole + 2,
    Offset = Qt::UserRole + 3,
    RawOffset = Qt::UserRole + 4,
    RawData = Qt::UserRole + 5,
    ViewportRect = Qt::UserRole + 6,
    ItemVisible = Qt::UserRole + 7
};

enum Cursor_Mode {
    Normal,
    Selection
};

typedef QMap<int, QVariant> ViewportItemData;

class QBinaryDataViewViewport : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool addressBarVisible READ addressBarVisible WRITE setAddressBarVisible USER true)
    Q_PROPERTY(bool presentationBarVisible READ presentationBarVisible WRITE setPresentationBarVisible USER true)
    Q_PROPERTY(bool isEditorMode READ isEditorMode WRITE setEditorMode USER true)
    Q_PROPERTY(unsigned int groupSize READ groupSize WRITE setGroupSize USER true)

public:

    /* Some constants */
    enum {
        CURSOR_TIMEOUT = 500,
        NOT_PRINTABLE_ITEM = '.'
    };

    struct DataSelection {
        QModelIndex pos1;
        QModelIndex pos2;

        bool isValid()
        {
            return pos1.isValid() && pos2.isValid();
        }
    };

    explicit QBinaryDataViewViewport(QWidget *parent = 0);
    virtual ~QBinaryDataViewViewport(void);

    // properties
    bool addressBarVisible() const;
    bool presentationBarVisible() const;
    bool isEditorMode() const;

    void setDataSource(QAbstractBinaryDataSource *newDataSource);
    QAbstractBinaryDataSource *dataSource() const;

    void setGroupSize(unsigned int newGroupSize);
    unsigned int groupSize() const;

    int topRow() const;
    int leftColumn() const;
    int linesPerPage() const;

    DataSelection selection() const;

    void initScrollArea(QAbstractScrollArea *scrollArea);

    void paintEvent(QPaintEvent * event);
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void resizeEvent(QResizeEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

    bool isSupportedKeyPressEvent(QKeyEvent * event) const;
    bool isSupportedKeyReleaseEvent(QKeyEvent * event) const;

    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

signals:
    void Cursor_positionChanged(const QModelIndex &prev, const QModelIndex &current);
    void Cursor_selectionChanged(const QModelIndex &begin, const QModelIndex &end);
    void Cursor_selectionDone(const QModelIndex &begin, const QModelIndex &end);
    void Cursor_selectionCanceled();
    void queryForContextMenu(QPoint pos);

public slots:    
    void setTopRow(int topRow);
    void setLeftColumn(int leftColumn);

    void Cursor_blinkingTrigger();
    void Cursor_setPosition(const QModelIndex &position);
    void Cursor_moveToIndex(const QModelIndex& index);

    // properties
    void setAddressBarVisible(bool value);
    void setPresentationBarVisible(bool value);
    void setEditorMode(bool value);

protected:
    QList<ViewportItemData> getDataToRender(int rowsPerScreen);

    void paintItem(QPainter &painter, const QRect &itemRect, const QMap<int,QVariant> &data);
    void paintViewport(QPainter &painter, QStringList &addresses, QStringList &presentations);
    void paintAddressBar(QPainter &painter, const QStringList &addresses);
    void paintPresenationBar(QPainter &painter, const QStringList &presenations);

    void setupScrollBars();

    bool updateGeometry(QPainter &painter);
    QRegion calculateAddressBarRegion(QPainter &painter);
    QRegion calculatePresentationBarRegion(QPainter &painter);
    QRegion calculateViewportRegion(QPainter &painter);

    bool isVisibleIndex(const QModelIndex& index) const;
    void scrollToIndex(const QModelIndex& index);

    // Cursor
    void Cursor_initTimer();
    void Cursor_startBlinking();
    void Cursor_stopBlinking();
    void Cursor_paint(QPainter &painter);
    void Cursor_updatePositionByMouseEvent(QMouseEvent * event);
    void Cursor_resetItemVisibility();
    void Cursor_setItemVisibility(const ViewportItemData &itemData);
    QModelIndex Cursor_addToInputBuffer(char inputData);
    QModelIndex Cursor_clearInputBuffer();
    QModelIndex Cursor_position(void) const;
    void Cursor_keyPressEvent(QKeyEvent *event);
    void Cursor_keyReleaseEvent(QKeyEvent *event);
    void Cursor_startSelection();
    void Cursor_finishSelection();
    void Cursor_resetSelection();

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

    QAbstractBinaryDataSource           *dataSource_;
    QAbstractScrollArea                 *scrollArea_;

    unsigned int            groupSize_;

    int                     linesPerPage_;
    int                     columnsOnView_;
    bool                    setupScrollBarsNeeded_;

    int                     topRow_;
    int                     totalRowCount_;
    int                     leftColumn_;
    int                     totalColumnCount_;

    int                     cx_;
    int                     cy_;
    int                     xmargin_;
    int                     ymargin_;

    bool                    addressBarVisible_; // property
    bool                    presentationBarVisible_; // property

    CashedData              dataCache_;

    QList<ViewportItemData> dataOnTheScreen_;

    bool                    isEditorMode_;  // property
    QPointer<QTimer>        cursorPaintTimer_;
    bool                    cursorVisibility_;

    QModelIndex             savedCursorPosition_;
    QModelIndex             currentCursorPosition_;
    ViewportItemData        currentCursorItemData_;
    QByteArray              currentCursorInputBuffer_;
    Cursor_Mode             currentCursorMode_;
    DataSelection           currentSelection_;

};

#endif // QBINARYDATAVIEWVIEWPORT_H
