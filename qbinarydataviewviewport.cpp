#include <QPaintEvent>
#include <QPainter>
#include <QStyle>
#include <QPalette>
#include <QScrollBar>
#include <QPoint>
#include <QDebug>
#include <QTimer>
#include <QStyleOptionFocusRect>
#include <QColor>

#include "qbinarydataviewviewport.h"

#define DEBUG_GEOMETRY 0
//#define TRACE

#ifdef TRACE
#define TRACE_IN  qDebug( "++%s", __FUNCTION__)
#define TRACE_OUT qDebug("--%s", __FUNCTION__)
#else
#define TRACE_IN
#define TRACE_OUT
#endif

class ScopedPainter {
    QPainter *painter_;

public:
    ScopedPainter(QPainter *painter)
        : painter_(painter)
    {
        TRACE_IN;
        painter_->save();
        TRACE_OUT;
    }

    ~ScopedPainter()
    {
        TRACE_IN;
        painter_->restore();
        TRACE_OUT;
    }
};

QBinaryDataViewViewport::QBinaryDataViewViewport(QWidget *parent)
    : QWidget(parent)
    , dataSource_(0)
    , scrollArea_(0)
    , groupSize_(4)
    , linesPerPage_(1)
    , columnsOnView_(1)
    , setupScrollBarsNeeded_(false)
    , topRow_(0)
    , totalRowCount_(0)
    , leftColumn_(0)
    , totalColumnCount_(0)
    , cx_(32)
    , cy_(32)
    , xmargin_(8)
    , ymargin_(8)
    , addressBarVisible_(true) // property
    , presentationBarVisible_(true) // property
    , dataCache_()
    , dataOnTheScreen_()
    , isEditorMode_(true) // property
    , cursorPaintTimer_(NULL)
    , cursorVisibility_(true)
    , savedCursorPosition_()
    , currentCursorPosition_()
    , currentCursorItemData_()
    , currentCursorInputBuffer_()
    , currentCursorMode_(Normal)
    , currentSelection_()
{
    TRACE_IN;

    Cursor_initTimer();

    TRACE_OUT;
}

QBinaryDataViewViewport::~QBinaryDataViewViewport(void)
{
    TRACE_IN;

    TRACE_OUT;
}

void QBinaryDataViewViewport::setDataSource(QAbstractBinaryDataSource *newDataSource)
{
    TRACE_IN;

    if (newDataSource != dataSource_)
    {
        dataSource_ = newDataSource;
        topRow_ = 0;
        leftColumn_ = 0;
        totalRowCount_ = 0;
        totalColumnCount_ = 0;
        dataCache_.reset();
        cursorVisibility_ = true;
        currentCursorItemData_.clear();
        currentCursorInputBuffer_.clear();
        savedCursorPosition_ = QModelIndex();
        if (dataSource_)
        {
            currentCursorPosition_ = dataSource_->index(0, 0);
        }
        else
        {
            currentCursorPosition_ = QModelIndex();
        }
        currentSelection_.pos1 = QModelIndex();
        currentSelection_.pos2 = QModelIndex();

        update();

        emit Cursor_positionChanged(QModelIndex(), currentCursorPosition_);
    }

    TRACE_OUT;
}

QAbstractBinaryDataSource *QBinaryDataViewViewport::dataSource() const
{
    TRACE_IN;
    TRACE_OUT;
    return dataSource_;
}

void QBinaryDataViewViewport::setGroupSize(quint8 newGroupSize)
{
    TRACE_IN;
    if (newGroupSize != groupSize_) {        
        groupSize_ = newGroupSize;
        update();
    }
    TRACE_OUT;
}

int QBinaryDataViewViewport::groupSize() const
{
    TRACE_IN;
    TRACE_OUT;
    return groupSize_;
}

void QBinaryDataViewViewport::setTopRow(int topRow)
{
    TRACE_IN;

    if (topRow != topRow_)
    {
        topRow_ = topRow;

        setupScrollBarsNeeded_ = true;

        update();
    }

    TRACE_OUT;
}

int QBinaryDataViewViewport::topRow() const
{
    TRACE_IN;
    TRACE_OUT;
    return topRow_;
}

void QBinaryDataViewViewport::setLeftColumn(int leftColumn)
{
    TRACE_IN;

    if (leftColumn_ != leftColumn)
    {
        leftColumn_ = leftColumn;

        setupScrollBarsNeeded_ = true;

        update();
    }

    TRACE_OUT;
}

int QBinaryDataViewViewport::leftColumn() const
{
    TRACE_IN;
    TRACE_OUT;
    return leftColumn_;
}

void QBinaryDataViewViewport::scrollToIndex(const QModelIndex& index)
{
    TRACE_IN;

    if (!dataSource_) return;

    while (topRow_ > index.row())
        --topRow_;
    while (topRow_ + linesPerPage_ < index.row() + 1)
        ++topRow_;

    while (leftColumn_ > index.column())
        --leftColumn_;
    while (leftColumn_ + columnsOnView_ < index.column() + 1)
        ++leftColumn_;

    setupScrollBarsNeeded_ = true;

    update();

    TRACE_OUT;
}

void QBinaryDataViewViewport::resizeEvent(QResizeEvent * event)
{
    TRACE_IN;

    Q_UNUSED(event);

    scrollToIndex(currentCursorPosition_);

    TRACE_OUT;
}

void QBinaryDataViewViewport::setupScrollBars()
{
    TRACE_IN;

    if (!dataSource_) return;

    if (totalRowCount_ > 0 && linesPerPage_ < totalRowCount_)
    {
        scrollArea_->verticalScrollBar()->setRange(0, totalRowCount_-linesPerPage_);
        scrollArea_->verticalScrollBar()->setPageStep(linesPerPage_-1);
        scrollArea_->verticalScrollBar()->setSingleStep(1);
        if (scrollArea_->verticalScrollBar()->value() != topRow_)
        {
            scrollArea_->verticalScrollBar()->setValue(topRow_);
        }
    }
    else
    {
        scrollArea_->verticalScrollBar()->setRange(0,0);
    }

    if (totalColumnCount_ > 0 && columnsOnView_ < totalColumnCount_ )
    {
        scrollArea_->horizontalScrollBar()->setRange(0, totalColumnCount_-columnsOnView_);
        scrollArea_->horizontalScrollBar()->setPageStep(columnsOnView_-1);
        scrollArea_->horizontalScrollBar()->setSingleStep(1);
        if (scrollArea_->horizontalScrollBar()->value() != leftColumn_)
        {
            scrollArea_->horizontalScrollBar()->setValue(leftColumn_);
        }
    }
    else
    {
        scrollArea_->horizontalScrollBar()->setRange(0,0);
    }

    TRACE_OUT;
}

void QBinaryDataViewViewport::initScrollArea(QAbstractScrollArea *scrollArea)
{
    TRACE_IN;

    scrollArea_ = scrollArea;

    if (dataSource_)
    {
        scrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        connect(scrollArea_->verticalScrollBar(), SIGNAL(valueChanged(int)),
                this, SLOT(setTopRow(int)));
        connect(scrollArea_->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                this, SLOT(setLeftColumn(int)));
    }
    else
    {
        disconnect(scrollArea_->verticalScrollBar(), SIGNAL(valueChanged(int)),
                   this, SLOT(setTopRow(int)));
        disconnect(scrollArea_->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                   this, SLOT(setLeftColumn(int)));

        scrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    setupScrollBarsNeeded_ = true;

    TRACE_OUT;
}

int QBinaryDataViewViewport::linesPerPage() const
{
    TRACE_IN;
    TRACE_OUT;
    return linesPerPage_;
}

bool QBinaryDataViewViewport::CashedData::isCasheActual(
        int linesPerPage,
        int topRow,
        int totalRowCount
        ) {
    return  !data_.isEmpty() &&
            linesPerPage_ >= linesPerPage &&
            topRow_ != topRow &&
            totalRowCount_ != totalRowCount;
}

void QBinaryDataViewViewport::CashedData::reset() {
     linesPerPage_ = 0;
     topRow_ = 0;
     totalRowCount_ = 0;
     data_.clear();
}

QList<ViewportItemData>& QBinaryDataViewViewport::CashedData::data() {
    return data_;
}

QBinaryDataViewViewport::CashedData::CashedData( int linesPerPage,
            int topRow,
            int totalRowCount,
            QList<ViewportItemData> data
            )
            : linesPerPage_(linesPerPage)
            , topRow_(topRow)
            , totalRowCount_(totalRowCount)
            , data_(data) {}

QBinaryDataViewViewport::CashedData::CashedData( const CashedData &src )
    : linesPerPage_(src.linesPerPage_)
    , topRow_(src.topRow_)
    , totalRowCount_(src.totalRowCount_)
    , data_(src.data_) {}

QBinaryDataViewViewport::CashedData::CashedData()
    : linesPerPage_(0)
    , topRow_(0)
    , totalRowCount_(0)
    , data_() {}

QList<ViewportItemData> QBinaryDataViewViewport::getDataToRender(int rowsPerScreen)
{
    QList<ViewportItemData> dataToRender;
    if (!dataSource_)
        return dataToRender;

    Q_ASSERT_X(dataSource_, __FUNCTION__, "Data source is NULL");

    totalRowCount_      = dataSource_->rowCount(QModelIndex());
    totalColumnCount_   = dataSource_->columnCount(QModelIndex());

    if (!dataCache_.isCasheActual(rowsPerScreen,topRow_,totalRowCount_)) {

        /* retrieve data from the model */
        int idx,startIdx;
        if (topRow_ < totalRowCount_)
        {
            idx         = topRow_;
            startIdx    = topRow_;
        } else
        {
            return dataToRender;
        }

        Q_ASSERT(idx >= 0);

        while (idx < startIdx + rowsPerScreen && idx < totalRowCount_)
        {
            for (int col = 0; col < totalColumnCount_; ++col)
            {
                QModelIndex index   = dataSource_->index(idx,col);
                QVariant data       = dataSource_->data(index,Qt::UserRole);
                if (!data.isNull())
                {
                    ViewportItemData itemData = dataSource_->itemData(index);

                    QMap<QString,QVariant> userData = data.toMap();

                    itemData[RawData]       = userData["char"];
                    itemData[RawOffset]     = userData["pos"];
                    itemData[RowIndex]      = idx;
                    itemData[ColumnIndex]   = col;

                    dataToRender.append(itemData);
                }
            }
            ++idx;
        }

        dataCache_ = CashedData(rowsPerScreen,topRow_,totalRowCount_,dataToRender);

    } else {

        /* get data from the cashe */
        dataToRender = dataCache_.data();

    }

    return dataToRender;
}

/*

  View scheme

  |         |           |              |^
  |         |           |              |*
  | Address | HEX data  | Presentation |*
  | bar     | (octets)  | bar          |*
  |         |           |              |*
  |         |           |              |V
  <************************************>

  */

void QBinaryDataViewViewport::Cursor_paint(QPainter &painter)
{
    TRACE_IN;

    // {{ CURSOR paint
    if (!currentCursorInputBuffer_.isEmpty())
    {
        QString presentation = QString::fromAscii(currentCursorInputBuffer_);
        if (presentation.length() == 1)
            presentation.append("_");
        currentCursorItemData_[Qt::DisplayRole] = presentation;
    }

    QRect itemRect = currentCursorItemData_[ViewportRect].toRect();

#if DEBUG_GEOMETRY >= 1
    painter.drawRect(itemRect);
#endif/**/

    int alignment  = currentCursorItemData_[Qt::TextAlignmentRole].toUInt();
    QString out    = currentCursorItemData_[Qt::DisplayRole].toString();
    QVariant bg    = currentCursorItemData_[Qt::BackgroundRole];

    QRect adjusted = itemRect.adjusted(-((xmargin_/2)+1),0,(xmargin_/2)+1,0);

#if DEBUG_GEOMETRY >= 1
    painter.drawRect(adjusted);
#endif/**/

    QPalette::ColorGroup colorGroup = QPalette::Active;
    if (dataSource_ && (dataSource_->flags(currentCursorPosition_) & Qt::ItemIsEditable))
    {
    }
    else
    {
        colorGroup = QPalette::Disabled;
    }

    if (!bg.isNull())
    {
        painter.fillRect(adjusted, bg.value<QColor>());
    }
    else
    {
        painter.fillRect(adjusted, style()->standardPalette().color(colorGroup, QPalette::Base));
    }

    painter.setCompositionMode(QPainter::CompositionMode_HardLight);

    painter.setPen(style()->standardPalette().color(colorGroup, QPalette::HighlightedText));
    painter.fillRect(itemRect, style()->standardPalette().brush(colorGroup, QPalette::Highlight));

    painter.drawText(itemRect,
                     alignment,
                     out);

    // }} CURSOR

    TRACE_OUT;
}

void QBinaryDataViewViewport::paintItem(QPainter &painter, const QRect &itemRect, const ViewportItemData &itemData)
{
    TRACE_IN;

#if DEBUG_GEOMETRY >= 1
    painter.drawRect(itemRect);
#endif/**/

    QString out     = itemData[Qt::DisplayRole].toString();
    int alignment   = itemData[Qt::TextAlignmentRole].toUInt();
    QVariant bg     = itemData[Qt::BackgroundRole];

    QRect adjusted = itemRect.adjusted(-((xmargin_/2)+1),0,(xmargin_/2)+1,0);

#if DEBUG_GEOMETRY >= 1
    painter.drawRect(adjusted);
#endif/**/

    if (!bg.isNull())
    {
        painter.fillRect(adjusted, bg.value<QColor>());
    }
    else
    {
        painter.fillRect(adjusted, style()->standardPalette().color(QPalette::Base));
    }

    QModelIndex index = dataSource_->index(itemData[RowIndex].toInt(), itemData[ColumnIndex].toInt());
    if (index.isValid() && index != currentCursorPosition_)
    {
        QBrush highlight        = style()->standardPalette().brush(QPalette::Highlight);

        if (currentCursorMode_ == Selection)
        {
            if (dataSource_->indexInRange(index, savedCursorPosition_, currentCursorPosition_))
            {
                QColor highlightedText  = style()->standardPalette().color(QPalette::HighlightedText);

                painter.fillRect(adjusted, highlight);
                painter.setPen(highlightedText);
            }
        }
        else
        {
            if (dataSource_->indexInRange(index, currentSelection_.pos1, currentSelection_.pos2))
            {
                QColor highlightedText  = style()->standardPalette().color(QPalette::HighlightedText);

                painter.fillRect(adjusted, highlight);
                painter.setPen(highlightedText);
            }
        }
    }

    painter.drawText(itemRect,
                     alignment,
                     out);

    TRACE_OUT;
}

bool QBinaryDataViewViewport::updateGeometry(QPainter &painter)
{
    TRACE_IN;

    if (!rect().isValid() || size().width() * size().height() <= 0) {
        TRACE_OUT;
        return false;
    }

    /* detect geometry (one byte geometry) */
    QRect detectRect = painter.fontMetrics().boundingRect("WW");
    cx_ = detectRect.width();
    cy_ = detectRect.height();
    ymargin_ = painter.fontMetrics().lineSpacing();
    xmargin_ = painter.fontMetrics().width(" ");

    linesPerPage_ = (size().height() - ymargin_) / cy_ +
                    ((size().height() - ymargin_) % cy_ ? 1 : 0);

    TRACE_OUT;

    return true;
}

QRegion QBinaryDataViewViewport::calculateAddressBarRegion(QPainter &painter)
{
    TRACE_IN;

    QRect wrect = rect();
    QRect boundingRect = painter.boundingRect(wrect,0,"DDDDDDD+DD");
    boundingRect.setHeight(wrect.height());
    boundingRect.adjust(0,0,ymargin_,0);

    TRACE_OUT;

    return QRegion(boundingRect);
}

void QBinaryDataViewViewport::paintAddressBar(QPainter &painter, const QStringList &addresses)
{
    TRACE_IN;

    painter.fillRect(painter.clipBoundingRect(),style()->standardPalette().brush(QPalette::Window));

    int x = 0,y = 0;
    for (int i = 0; i < addresses.count(); ++i) {
        y = i * cy_;
        x = painter.clipBoundingRect().left();

        QRect itemRect = QRect(x,y,painter.clipBoundingRect().width(),cy_);

#if DEBUG_GEOMETRY >= 1
        painter.drawRect(itemRect);
#endif/**/

        QString address = addresses[i];

        painter.save();
        if (currentCursorPosition_.row() == i + topRow_)
        {
            painter.fillRect(itemRect,style()->standardPalette().brush(QPalette::Highlight));
            painter.setPen(style()->standardPalette().color(QPalette::HighlightedText));
            address += QString().sprintf("+%02X", currentCursorPosition_.column()
                                          % totalColumnCount_);
        }

        painter.drawText(itemRect,
                         Qt::AlignCenter,
                         address);
        painter.restore();

        //qDebug("x:%d y: %d addresses[%d]: %s", x, y, i, qPrintable(addresses[i]));
    }

    TRACE_OUT;
}

QRegion QBinaryDataViewViewport::calculatePresentationBarRegion(QPainter &painter)
{
    TRACE_IN;

    QRect wrect = rect();
    QString testString;
    int columnCnt = totalColumnCount_;

    if (columnCnt > 0)
    {
        for (int i = 0; i < columnCnt; ++i) testString.append("W");
    }
    testString.append("W");

    QRect boundingRect = painter.boundingRect(wrect,0,testString);
    boundingRect.setLeft(wrect.right() - boundingRect.width());
    boundingRect.setRight(wrect.right());
    boundingRect.setHeight(wrect.height());

    TRACE_OUT;

    return QRegion(boundingRect);
}

void QBinaryDataViewViewport::paintPresenationBar(QPainter &painter, const QStringList &presenations)
{
    TRACE_IN;

    QBrush highlight        = style()->standardPalette().brush(QPalette::Highlight);
    QColor highlightedText  = style()->standardPalette().color(QPalette::HighlightedText);

    painter.fillRect(painter.clipBoundingRect(),style()->standardPalette().brush(QPalette::Window));

    int x = 0,y = 0;
    for (int i = 0; i < presenations.count(); ++i) {
        y = i * cy_;
        x = xmargin_ + painter.clipBoundingRect().left();

        QRect itemRect = QRect(x,y,painter.clipBoundingRect().width(),cy_);
        itemRect = itemRect.adjusted(-xmargin_,0,0,0);

#if DEBUG_GEOMETRY >= 1
        painter.drawRect(itemRect);
#endif/**/

        painter.save();
        if (currentCursorPosition_.row() == i + topRow_)
        {
            painter.fillRect(itemRect,highlight);
            painter.setPen(highlightedText);
        }

        itemRect = itemRect.adjusted(xmargin_,0,-xmargin_,0);
        QString line = presenations[i];
        int j = 0;
        foreach(QChar ch, line)
        {
            int w = itemRect.width() / dataSource_->viewWidth();
            QRect chRect(itemRect.x() + j*w, itemRect.y(), w, itemRect.height());

#if DEBUG_GEOMETRY >= 1
            painter.drawRect(chRect);
#endif/**/
            if (currentCursorPosition_.column() == j &&
                currentCursorPosition_.row() == (i + topRow_))
            {
                painter.drawRect(chRect);
            }

            painter.drawText(chRect,
                             Qt::AlignCenter | Qt::AlignVCenter,
                             ch);

            ++j;
        }

        painter.restore();

        //qDebug("x:%d y: %d presentation[%d]: %s", x, y, i, qPrintable(presenations[i]));

    }

    TRACE_OUT;
}

void QBinaryDataViewViewport::paintViewport(QPainter &painter,
                                            QStringList &addresses,
                                            QStringList &presentations
                                            )
{
    TRACE_IN;

    QPalette currentPalette = style()->standardPalette();

    painter.setBackground(currentPalette.base());

    /* clear viewport */
    painter.fillRect(rect(), painter.background());

    /* calculate lines per page */
    if (size().height() && cy_) {
        linesPerPage_ = (size().height() - ymargin_) / cy_ +
                        ((size().height() - ymargin_) % cy_ ? 1 : 0);
    } else {
        linesPerPage_ = 1;
    }

    /* retrieve data to render */
    dataOnTheScreen_.clear();
    QList<ViewportItemData> dataToRender = getDataToRender(linesPerPage_);
    if (dataToRender.isEmpty()) {
        TRACE_OUT;
        return;
    }

    /* total rows count */
    int rows = dataToRender.count();
    /* total columns count */
    int columnCount = totalColumnCount_;

    QString lineOffsetText;
    /* Raw view buffer */
    QString rawViewData;

    Cursor_resetItemVisibility();

    int rightColumn = -1;

    int i,j;
    int x,y;
    for (i = 0; i < rows; ++i) {

        // Viewport yoffset for the first item in the line
        y = (i * cy_);
        // Viewport xoffset for the first item in the line
        x = painter.clipBoundingRect().left() + xmargin_;

        for (j = 0; j < columnCount; ++j)
        {

            if (j != leftColumn_ && !(j%groupSize_)) {
                if (leftColumn_ <= j) {
                   x += 2*xmargin_;
                }
            }

            /* item data offset */
            int offset = (i* columnCount) + j;            
            if (offset < dataToRender.count())
            {
                ViewportItemData itemData = dataToRender.at(offset);
                itemData[Offset] = offset;

                // address for the first element in line
                if (lineOffsetText.isEmpty()) {
                    lineOffsetText = lineOffsetText.sprintf("%08X", itemData[RawOffset].toUInt());

                }

                if (leftColumn_ <= j)
                {
                    itemData.remove(ViewportRect);

                    QRect itemRect = QRect(x,y,cx_,cy_);
                    QRegion intersectedRegion = painter.clipRegion().intersected(itemRect);
                    if ( !intersectedRegion.isEmpty() &&
                         QRegion(itemRect) == intersectedRegion)
                    {
                        ScopedPainter p(&painter);
                        paintItem(painter, itemRect, itemData);

                        itemData[ViewportRect] = itemRect;

                        Cursor_setItemVisibility(itemData);

                        // Cache rendered data
                        dataOnTheScreen_.append(itemData);

                        // Viewport xoffset for the next item
                        x += xmargin_ + cx_;

                        rightColumn = qMax(rightColumn, j + 1);
                    }
                }

                /* collecting raw data */
                int i = itemData[Qt::DisplayRole].toString().toInt(0, 16);
                QChar ch = QChar::fromLatin1(i);
                rawViewData.append(ch.isPrint()?ch:NOT_PRINTABLE_ITEM);
            }
        }

        if (rawViewData.isEmpty()) break;

        addresses.append(lineOffsetText);
        lineOffsetText.clear();
        presentations.append(rawViewData);
        rawViewData.clear();
    }

    if (rightColumn == -1)
    {
        // All columns not visible
        columnsOnView_ = 0;
    }
    else
    {
        columnsOnView_ = rightColumn - leftColumn_;
    }

    //qDebug() << "leftColumn_:" << leftColumn_ << "columnsOnView_:" << columnsOnView_ << "totalColumnCount_:" << totalColumnCount_;

    TRACE_OUT;
}


void QBinaryDataViewViewport::paintEvent(QPaintEvent * event)
{
    TRACE_IN;    

    QPainter painter(this);

    if (!updateGeometry(painter))
    {
        TRACE_OUT;
        return;
    }

    QStringList addresses;
    QStringList presentations;

    QRegion addressBarRegion;
    QRegion presentationBarRegion;
    QRegion viewportRegion;

    bool addressBarAutoVisible = true;
    bool presentationBarAutoVisible = true;
    do {
        viewportRegion = QRegion(rect());

        if (addressBarVisible_ && addressBarAutoVisible)
        {
            addressBarRegion = calculateAddressBarRegion(painter);
        }
        if (presentationBarVisible_ && presentationBarAutoVisible)
        {
            presentationBarRegion = calculatePresentationBarRegion(painter);
        }

        viewportRegion = viewportRegion.subtracted(addressBarRegion);
        viewportRegion = viewportRegion.subtracted(presentationBarRegion);

        if (viewportRegion.isEmpty() && presentationBarAutoVisible)
        {
            presentationBarAutoVisible = false;
            continue;
        }
        if (viewportRegion.isEmpty() && addressBarAutoVisible)
        {
            addressBarAutoVisible = false;
            continue;
        }
        if (viewportRegion.isEmpty() && !addressBarAutoVisible && !presentationBarAutoVisible)
        {
            // avoid infinity loop
            viewportRegion = QRegion(rect());
        }

    } while(viewportRegion.isEmpty());

    if (!event->region().intersected(viewportRegion).isEmpty()) {
        ScopedPainter p(&painter);
        painter.setClipRegion(viewportRegion);
        paintViewport(painter, addresses, presentations);
    }

    if ( currentCursorItemData_[ItemVisible].toBool() &&
         (cursorVisibility_ || !currentCursorInputBuffer_.isEmpty() || !hasFocus()) )
    {
        ScopedPainter p(&painter);
        Cursor_paint(painter);
    }

    if (addressBarVisible_ && addressBarAutoVisible &&
            !event->region().intersected(addressBarRegion).isEmpty())
    {
        ScopedPainter p(&painter);
        painter.setClipRegion(addressBarRegion);
        paintAddressBar(painter, addresses);
    }
    if (presentationBarVisible_ && presentationBarAutoVisible &&
            !event->region().intersected(presentationBarRegion).isEmpty())
    {
        ScopedPainter p(&painter);
        painter.setClipRegion(presentationBarRegion);
        paintPresenationBar(painter, presentations);
    }

    if (setupScrollBarsNeeded_)
    {
        setupScrollBarsNeeded_ = false;
        setupScrollBars();
    }

    // Accept event
    event->accept();

    TRACE_OUT;
}

void QBinaryDataViewViewport::focusInEvent(QFocusEvent * event)
{
    TRACE_IN;

    Q_UNUSED(event);

    if (isEditorMode_) {
        Cursor_startBlinking();
    }

    event->accept();

    TRACE_OUT;
}

void QBinaryDataViewViewport::focusOutEvent(QFocusEvent * event)
{
    TRACE_IN;

    Q_UNUSED(event);

    if (isEditorMode_) {
        Cursor_stopBlinking();
    }

    event->accept();

    TRACE_OUT;
}

void QBinaryDataViewViewport::mouseMoveEvent( QMouseEvent * event)
{
    TRACE_IN;

    //qDebug("MOUSE MOVE   x: %d y: %d", event->pos().x(), event->pos().y());

    QList<ViewportItemData>::const_iterator iter = dataOnTheScreen_.constBegin();
    while (iter != dataOnTheScreen_.constEnd()) {
        QRegion itemRegion = (*iter)[ViewportRect].toRect();
        if (itemRegion.contains(event->pos()))
        {
            //qDebug() << "ITEM   " << (*iter)[RawData].toChar();
            QModelIndex index = dataSource_->index((*iter)[RowIndex].toInt(),
                               (*iter)[ColumnIndex].toInt());
            Cursor_moveToIndex(index);

            break;
        }
        ++iter;
    }

    event->accept();

    TRACE_OUT;
}

void QBinaryDataViewViewport::mousePressEvent(QMouseEvent * event)
{
    TRACE_IN;

    //qDebug("MOUSE PRESS   x: %d y: %d", event->pos().x(), event->pos().y());
    switch (event->button())
    {
    case Qt::LeftButton:
    {
        Cursor_updatePositionByMouseEvent(event);
        Cursor_startSelection();

        event->accept();
    }
    break;
//    case Qt::RightButton:
//    {
//        emit queryForContextMenu(event->pos());
//    }
//    break;
    default:;
    }

    TRACE_OUT;
}

void QBinaryDataViewViewport::mouseReleaseEvent(QMouseEvent * event)
{
    TRACE_IN;

    //qDebug("MOUSE RELEASE   x: %d y: %d", event->pos().x(), event->pos().y());
    switch (event->button())
    {
    case Qt::LeftButton:
    {
        Cursor_updatePositionByMouseEvent(event);
        Cursor_finishSelection();

        event->accept();
    }
    break;
//    case Qt::RightButton:
//    {

//    }
//    break;
    default:;
    }

    TRACE_OUT;
}

void QBinaryDataViewViewport::keyPressEvent (QKeyEvent * event)
{
    TRACE_IN;

    Cursor_keyPressEvent(event);

    event->accept();

    TRACE_OUT;
}

void QBinaryDataViewViewport::keyReleaseEvent (QKeyEvent * event)
{
    TRACE_IN;

    Cursor_keyReleaseEvent(event);

    event->accept();

    TRACE_OUT;
}

bool QBinaryDataViewViewport::isSupportedKeyPressEvent(QKeyEvent * event) const
{
    TRACE_IN;

    bool res = false;

    if (isEditorMode_ && !event->text().isEmpty() && QString("0123456789abcdefABCDEF").indexOf(event->text()) >= 0)
    {
        res = true;
    }
    else
    {
        res = event->key() == Qt::Key_Left  ||
            event->key() == Qt::Key_Right   ||
            event->key() == Qt::Key_Up      ||
            event->key() == Qt::Key_Down    ||
            event->key() == Qt::Key_Escape  ||
            event->key() == Qt::Key_Shift    ;
    }

    TRACE_OUT;

    return res;
}

bool QBinaryDataViewViewport::isSupportedKeyReleaseEvent(QKeyEvent * event) const
{
    TRACE_IN;

    bool res = false;

    res = event->key() == Qt::Key_Shift;

    TRACE_OUT;

    return res;
}

bool QBinaryDataViewViewport::isVisibleIndex(const QModelIndex& index) const
{
    TRACE_IN;

    bool res = false;

    QList<ViewportItemData>::const_iterator iter = dataOnTheScreen_.constBegin();
    while (iter != dataOnTheScreen_.constEnd() && !res) {
        res =   (*iter)[RowIndex].toInt() == index.row() &&
                (*iter)[ColumnIndex].toInt() == index.column();
        ++iter;
    }

    TRACE_OUT;

    return res;
}

void QBinaryDataViewViewport::Cursor_initTimer()
{
    cursorPaintTimer_ = new QTimer(this);
    cursorPaintTimer_->setInterval(CURSOR_TIMEOUT);
    cursorPaintTimer_->setSingleShot(false);

    connect(cursorPaintTimer_, SIGNAL(timeout()), this, SLOT(Cursor_blinkingTrigger()));
}

void QBinaryDataViewViewport::Cursor_resetItemVisibility()
{
    currentCursorItemData_[ItemVisible] = false;
}

void QBinaryDataViewViewport::Cursor_setItemVisibility(const ViewportItemData &itemData)
{
    if (currentCursorPosition_.isValid() &&
        itemData[RowIndex] == currentCursorPosition_.row() &&
        itemData[ColumnIndex] == currentCursorPosition_.column()
        )
    {
        currentCursorItemData_              = itemData;
        currentCursorItemData_[ItemVisible] = true;
    }
}

void QBinaryDataViewViewport::Cursor_moveToIndex(const QModelIndex& index)
{
    QModelIndex prev = currentCursorPosition_;

    currentCursorItemData_.clear();
    currentCursorPosition_ = index;

    if (!isVisibleIndex(currentCursorPosition_))
    {
        scrollToIndex(currentCursorPosition_);
    }

    emit Cursor_positionChanged(prev, currentCursorPosition_);

    if (Selection == currentCursorMode_)
    {
        if (dataSource_->indexToOffset(currentCursorPosition_) > dataSource_->indexToOffset(savedCursorPosition_))
            emit Cursor_selectionChanged(savedCursorPosition_, currentCursorPosition_);
        else
            emit Cursor_selectionChanged(currentCursorPosition_, savedCursorPosition_);
    }

    // Force show cursor
    cursorVisibility_ = true;
    repaint();
}

void QBinaryDataViewViewport::Cursor_blinkingTrigger()
{
    TRACE_IN;

    repaint();

    if (hasFocus())
    {
        // {{ CURSOR invertVisibility
        cursorVisibility_ = !cursorVisibility_;
        // }} CURSOR
    }

    TRACE_OUT;
}

void QBinaryDataViewViewport::Cursor_startBlinking()
{
    TRACE_IN;

    if (hasFocus())
    {
        cursorPaintTimer_->start();
    }

    TRACE_OUT;
}

void QBinaryDataViewViewport::Cursor_stopBlinking()
{
    TRACE_IN;

    // {{ CURSOR setVisibility
    cursorVisibility_ = true;
    // }} CURSOR

    cursorPaintTimer_->stop();

    TRACE_OUT;
}

void QBinaryDataViewViewport::Cursor_updatePositionByMouseEvent(QMouseEvent * event)
{
    TRACE_IN;

    QList<ViewportItemData>::const_iterator iter = dataOnTheScreen_.constBegin();
    while (iter != dataOnTheScreen_.constEnd()) {
        QRect itemRect = (*iter)[ViewportRect].toRect();
        QRegion itemRegion = itemRect.adjusted(-((xmargin_/2)+1),0,(xmargin_/2)+1,0);
        if (itemRegion.contains(event->pos()))
        {
            QModelIndex prev = currentCursorPosition_;

            // {{ CURSOR setupCursor
            //qDebug() << "ITEM   " << (*iter)[RawData].toChar();
            currentCursorPosition_ = dataSource_->index((*iter)[RowIndex].toInt(),
                                                        (*iter)[ColumnIndex].toInt());
//            qDebug() << "CURSOR   ROW: " << currentCursorPosition_.row()
//                     << " COLUMN: " << currentCursorPosition_.column();

            // Force show cursor
            cursorVisibility_ = true;
            // }} CURSOR

            repaint();

            emit Cursor_positionChanged(prev, currentCursorPosition_);

            break;
        }
        ++iter;
    }

    TRACE_OUT;
}

QModelIndex QBinaryDataViewViewport::Cursor_addToInputBuffer(char inputData)
{
    QModelIndex newIndex;

    currentCursorInputBuffer_.append(inputData);
    if (currentCursorInputBuffer_.size() >= 2)
    {
        QString presentation = QString::fromAscii(currentCursorInputBuffer_);
        //qDebug() << "INPUT  presentation: '" << presentation << "'";
        if (dataSource_) {
            dataSource_->setData(currentCursorPosition_, presentation);
        }
        currentCursorInputBuffer_.clear();

        // Goto next position
        if (dataSource_) {
            newIndex = dataSource_->nextIndex(currentCursorPosition_);
        }
    }
    else
    {
        // Repaint
        repaint();
    }

    return newIndex;
}

QModelIndex QBinaryDataViewViewport::Cursor_clearInputBuffer()
{
    if (!currentCursorInputBuffer_.isEmpty())
    {
        currentCursorInputBuffer_.clear();
        // Repaint
        repaint();
    }

    return QModelIndex();
}

void QBinaryDataViewViewport::Cursor_resetSelection()
{
    currentSelection_.pos1  = QModelIndex();
    currentSelection_.pos2  = QModelIndex();

    update();

    emit Cursor_selectionCanceled();
}

void QBinaryDataViewViewport::Cursor_startSelection()
{
    Cursor_resetSelection();

    currentCursorMode_      = Selection;
    savedCursorPosition_    = currentCursorPosition_;

    update();
}

void QBinaryDataViewViewport::Cursor_finishSelection()
{
    if (dataSource_)
    {
        if (dataSource_->indexToOffset(savedCursorPosition_)
            <= dataSource_->indexToOffset(currentCursorPosition_))
        {
            currentSelection_.pos1  = savedCursorPosition_;
            currentSelection_.pos2  = currentCursorPosition_;
        }
        else
        {
            currentSelection_.pos1  = currentCursorPosition_;
            currentSelection_.pos2  = savedCursorPosition_;
        }
        currentCursorMode_      = Normal;
        savedCursorPosition_    = QModelIndex();

        update();

        emit Cursor_selectionDone(currentSelection_.pos1, currentSelection_.pos2);
    }
}

void QBinaryDataViewViewport::Cursor_keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Shift:
        Cursor_finishSelection();
    break;
    }
}

void QBinaryDataViewViewport::Cursor_keyPressEvent(QKeyEvent *event)
{
    QModelIndex newIndex;

    if (!event->text().isEmpty() && QString("0123456789abcdefABCDEF").indexOf(event->text()) >= 0)
    {
        if (dataSource_ && (dataSource_->flags(currentCursorPosition_) & Qt::ItemIsEditable))
        {
            newIndex = Cursor_addToInputBuffer(event->text().toUpper().at(0).toAscii());
        }
    }
    else
    {
        switch (event->key())
        {
        case Qt::Key_Escape:
            Cursor_resetSelection();
            newIndex = Cursor_clearInputBuffer();
        break;
        case Qt::Key_Shift:
            Cursor_startSelection();
        break;
        case Qt::Key_Left:
            if (dataSource_) newIndex = dataSource_->prevIndex(currentCursorPosition_);
        break;
        case Qt::Key_Right:
            if (dataSource_) newIndex = dataSource_->nextIndex(currentCursorPosition_);
        break;
        case Qt::Key_Up:
            if (dataSource_) newIndex = dataSource_->index(currentCursorPosition_.row()-1,currentCursorPosition_.column());
        break;
        case Qt::Key_Down:
            if (dataSource_) newIndex = dataSource_->index(currentCursorPosition_.row()+1,currentCursorPosition_.column());
        break;
        default:;
        }
    }

    if (newIndex.isValid())
    {
        Cursor_moveToIndex(newIndex);
    }
}

QModelIndex QBinaryDataViewViewport::Cursor_position(void) const
{
    TRACE_IN;

    TRACE_OUT;

    return currentCursorPosition_;
}

void QBinaryDataViewViewport::Cursor_setPosition(const QModelIndex &position)
{
    TRACE_IN;

    if (position.isValid())
    {
        currentCursorItemData_.clear();
        currentCursorPosition_ = position;

        // Force show cursor
        cursorVisibility_ = true;
        repaint();
    }

    TRACE_OUT;
}

QBinaryDataViewViewport::DataSelection QBinaryDataViewViewport::selection() const
{
    return currentSelection_;
}

bool QBinaryDataViewViewport::addressBarVisible() const
{
    return addressBarVisible_;
}

bool QBinaryDataViewViewport::presentationBarVisible() const
{
    return presentationBarVisible_;
}

bool QBinaryDataViewViewport::isEditorMode() const
{
    return isEditorMode_;
}

void QBinaryDataViewViewport::setAddressBarVisible(bool value)
{
    addressBarVisible_ = value;

    update();
}

void QBinaryDataViewViewport::setPresentationBarVisible(bool value)
{
    presentationBarVisible_ = value;

    update();
}

void QBinaryDataViewViewport::setEditorMode(bool value)
{
    isEditorMode_ = value;

    update();
}
