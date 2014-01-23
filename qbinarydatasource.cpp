#include "qbinarydatasource.h"
#include "qbinarydatasourceproxy.h"

#include <limits>

#define TRACE(x)
//#define TRACE(x) x

/******************************************************************************/
QBinaryDataSource::QBinaryDataSource(QObject *parent)
    : QAbstractBinaryDataSource(parent)
    , ioDevice_(0)
{
}

/******************************************************************************/
QBinaryDataSource::~QBinaryDataSource(void)
{
}

/******************************************************************************/
/* IO source */
void QBinaryDataSource::attachTo(QIODevice *input)
{
    Q_ASSERT_X(!input->isSequential(), __FUNCTION__, "Device should be not sequental");
    ioDevice_ = input;
}

/******************************************************************************/
QIODevice *QBinaryDataSource::detachFrom(void)
{
    QIODevice *res = ioDevice_;

    res = ioDevice_;

    reset();

    return res;
}

/******************************************************************************/
/* QAbstractItemModel */
QModelIndex QBinaryDataSource::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return QModelIndex();
}

/******************************************************************************/
QModelIndex QBinaryDataSource::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    //Q_ASSERT(ioDevice_ != 0);

    if (!ioDevice_)
    {
        return QModelIndex();
    }

    quint64 pos = row * viewWidth() + column;
    quint64 size = ioDevice_->size();
    if (pos >= size)
    {
        return QModelIndex();
    }

    return createIndex(row, column, 0);
}

/******************************************************************************/
QModelIndex QBinaryDataSource::nextIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    /* real position */
    quint64 pos = index.row() * viewWidth() + index.column();
    ++pos;

    quint64 size = ioDevice_->size();
    if ( size - 1 < pos )
        return QModelIndex();

    return offsetToIndex(pos);
}

/******************************************************************************/
QModelIndex QBinaryDataSource::prevIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    if (index.row() * viewWidth() + index.column() - 1  < 0)
        return QModelIndex();

    quint64 pos = index.row() * viewWidth() + index.column();
    --pos;

    return offsetToIndex(pos);
}

/******************************************************************************/
QModelIndex QBinaryDataSource::offsetToIndex(quint64 offset) const
{
    int row     = offset / viewWidth();
    int column  = offset % viewWidth();

    return createIndex(row, column, 0);
}

/******************************************************************************/
quint64 QBinaryDataSource::indexToOffset(QModelIndex index) const
{
    if (!index.isValid())
        return 0;

    if (index.row() * viewWidth() + index.column() - 1  < 0)
        return 0;

    quint64 pos = index.row() * viewWidth() + index.column();

    quint64 size = ioDevice_->size();
    if ( size - 1 < pos )
        return 0;

    return pos;
}

/******************************************************************************/
int QBinaryDataSource::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);

    int result = 0;

    if (ioDevice_) {
        /* to support sources with non determined size */
        result = std::numeric_limits<int>::max();

        Q_ASSERT_X(ioDevice_->isOpen(), __FUNCTION__, "ioDevice should be opened");
        Q_ASSERT_X(viewWidth() > 0, __FUNCTION__, "viewWidth_ is zero");

        quint64 size = ioDevice_->size();
        result = size / viewWidth() +
             (size % viewWidth() ? 1 : 0);
    }

    return result;
}

/******************************************************************************/
int QBinaryDataSource::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);

    if (!ioDevice_)
        return 0;

    Q_ASSERT_X(viewWidth() > 0, __FUNCTION__, "viewWidth_ is zero");

    return viewWidth();
}

/******************************************************************************/
QVariant QBinaryDataSource::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return QString().sprintf("+0x%02X", section);
    } else if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return QString().sprintf("0x%08X", section*viewWidth());
    }
    return QVariant();
}

/******************************************************************************/
QVariant QBinaryDataSource::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    quint64 pos = index.row() * viewWidth() + index.column();
    char byte = 0;

    Q_ASSERT_X(ioDevice_->isOpen(), __FUNCTION__, "ioDevice should be opened");
    Q_ASSERT_X(viewWidth() > 0, __FUNCTION__, "viewWidth_ is zero");

    /* Qt::DisplayRole */
    if (role == Qt::UserRole){

        /* Data */
        quint64 size = ioDevice_->size();
        if ( size - 1 < pos )
            return QVariant();

        if (ioDevice_->seek(pos)) {
            if (ioDevice_->read(&byte, 1) != 1) {
                qCritical("Data read: Error on reading byte from pos 0x%08X", pos);
            }
            TRACE( else { qDebug("Data read: %02X <- 0x%08X", (quint8)byte, pos);} )
        } else {
            qCritical("Data read: Error on seeking to pos 0x%08X", pos);
        }

        QMap<QString,QVariant> userData;
        userData["char"] = QChar::fromLatin1(byte);
        userData["pos"] = pos;
        return userData;

    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {

        /* Data */
        quint64 size = ioDevice_->size();
        if ( size - 1 < pos )
            return QVariant();

        if (ioDevice_->seek(pos)) {
            if (ioDevice_->read(&byte, 1) != 1) {
                qCritical("Data read: Error on reading byte from pos 0x%08X", pos);
            }
            TRACE( else { qDebug("Data read: %02X <- 0x%08X", (quint8)byte, pos);} )
        } else {
            qCritical("Data read: Error on seeking to pos 0x%08X", pos);
        }

        return QString().sprintf("%02X",(quint8)byte);

    } else if (role == Qt::ToolTipRole || role == Qt::StatusTipRole) {

        /* Address */

        quint64 pos = index.row() * viewWidth() + index.column();

        return QString().sprintf("%08X",pos);

    } else if (role == Qt::TextAlignmentRole) {
        /* Text aligment */

        return Qt::AlignCenter;
    }

    return QVariant();
}

/******************************************************************************/
bool QBinaryDataSource::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (!index.isValid())
        return false;

    if (role != Qt::EditRole)
        return false;

    quint64 pos = index.row() * viewWidth() + index.column();
    char byte = 0;

    Q_ASSERT_X(ioDevice_->isOpen(), __FUNCTION__, "ioDevice should be opened");
    Q_ASSERT_X(viewWidth() > 0, __FUNCTION__, "viewWidth_ is zero");

    /* byte hex presentation */
    QString p = value.toString();
    int i = p.toInt(0, 16);
    if (i >= 0 && i <= 0xFF) {
        byte = (char)i;
        /* write to ioDevice_ */
        if (ioDevice_->seek(pos)) {
            if (ioDevice_->write(&byte, 1) != 1) {
                qCritical("Data write: Error on reading byte from pos 0x%08X", pos);
                return false;
            } else {
                TRACE( qDebug("Data write: %02X -> 0x%08X", (quint8)byte, pos); )
                return true;
            }
        } else {
            qCritical("Data write: Error on seeking to pos 0x%08X", pos);
            return false;
        }
    }
    return false;
}

/******************************************************************************/
Qt::ItemFlags QBinaryDataSource::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

/******************************************************************************/
void QBinaryDataSource::setInterpretation(QBinaryDataInterpretation *interptretation)
{
    Q_UNUSED(interptretation);
}

/******************************************************************************/
QBinaryDataInterpretation *QBinaryDataSource::interpretation() const
{
    return 0;
}

/******************************************************************************/
QBinaryDataSourceProxy *QBinaryDataSource::createProxy()
{
    return new QBinaryDataSourceProxy(this);
}

///******************************************************************************/
//QBinaryDataSourceSelection_List *QBinaryDataSource::createSelection(const QModelIndex &index)
//{
//    return 0;
//}

///******************************************************************************/
//QBinaryDataSourceSelection_Range *QBinaryDataSource::createSelection(const QModelIndex &begin, const QModelIndex &end)
//{
//    return 0;
//}
