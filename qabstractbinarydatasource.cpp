#include "qabstractbinarydatasource.h"

QAbstractBinaryDataSource::QAbstractBinaryDataSource(QObject *parent)
    : QAbstractItemModel(parent)
    , viewWidth_(20)
    , colorScheme_(0)
{
}

QAbstractBinaryDataSource::~QAbstractBinaryDataSource()
{

}

/******************************************************************************/
/* View management */
void QAbstractBinaryDataSource::setViewWidth(quint8 viewWidth)
{
    if (viewWidth_ != viewWidth) {
        viewWidth_ = viewWidth;
        reset();
    }
}

/******************************************************************************/
quint8 QAbstractBinaryDataSource::viewWidth(void) const
{
    return viewWidth_;
}

/******************************************************************************/
bool QAbstractBinaryDataSource::indexInRange(const QModelIndex &pos, const QModelIndex &p1, const QModelIndex &p2) const
{
    if (!pos.isValid() || !p1.isValid() || !p2.isValid())
        return false;

    quint64 po    = indexToOffset(pos);
    quint64 po1   = qMin(indexToOffset(p1), indexToOffset(p2));
    quint64 po2   = qMax(indexToOffset(p1), indexToOffset(p2));
    return po >= po1 && po <= po2;
}

/******************************************************************************/
void QAbstractBinaryDataSource::setColorScheme(QBinaryDataColorScheme *colorScheme)
{
    colorScheme_ = colorScheme;

    reset();
}

/******************************************************************************/
QBinaryDataColorScheme *QAbstractBinaryDataSource::colorScheme() const
{
    return colorScheme_;
}
