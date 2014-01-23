#include "qabstractbinarydatasource.h"

QAbstractBinaryDataSource::QAbstractBinaryDataSource(QObject *parent)
    : QAbstractItemModel(parent)
    , viewWidth_(20)
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
