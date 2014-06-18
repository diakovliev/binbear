#include "qbinarydataview.h"
#include "qbinarydataviewviewport.h"

#include <QScrollBar>
#include <QPainter>
#include <QStyle>
#include <QKeyEvent>

QBinaryDataView::QBinaryDataView(QWidget *parent)
    : QAbstractScrollArea(parent)
    , viewport_(new QBinaryDataViewViewport(this))
{
    Q_ASSERT(viewport_);
    setViewport(viewport_);
}

QBinaryDataView::~QBinaryDataView(void)
{
}

void QBinaryDataView::setDataSource(QAbstractBinaryDataSource *newDataSource)
{
    Q_ASSERT(viewport_);
    viewport_->setDataSource(newDataSource);
    viewport_->initScrollArea(this);
}

QAbstractBinaryDataSource *QBinaryDataView::dataSource() const
{
    Q_ASSERT(viewport_);
    return viewport_->dataSource();
}

void QBinaryDataView::resizeEvent(QResizeEvent * event)
{
    Q_ASSERT(viewport_);
    viewport_->resizeEvent(event);
}

void QBinaryDataView::paintEvent(QPaintEvent * event)
{
    Q_ASSERT(viewport_);
    viewport_->paintEvent(event);
}

void QBinaryDataView::mouseMoveEvent( QMouseEvent * event)
{
    Q_ASSERT(viewport_);
    viewport_->mouseMoveEvent(event);
}

void QBinaryDataView::mousePressEvent( QMouseEvent * event)
{
    Q_ASSERT(viewport_);
    viewport_->mousePressEvent(event);
}

void QBinaryDataView::mouseReleaseEvent( QMouseEvent * event)
{
    Q_ASSERT(viewport_);
    viewport_->mouseReleaseEvent(event);
}

void QBinaryDataView::focusInEvent(QFocusEvent * event)
{
    Q_ASSERT(viewport_);
    viewport_->focusInEvent(event);
}

void QBinaryDataView::focusOutEvent(QFocusEvent * event)
{
    Q_ASSERT(viewport_);
    viewport_->focusOutEvent(event);
}

void QBinaryDataView::keyPressEvent(QKeyEvent * event)
{
    Q_ASSERT(viewport_);
    if (viewport_->isSupportedKeyPressEvent(event))
    {
        viewport_->keyPressEvent(event);
    }
    else
    {
        QAbstractScrollArea::keyPressEvent(event);
    }
}

void QBinaryDataView::keyReleaseEvent(QKeyEvent * event)
{
    Q_ASSERT(viewport_);
    if (viewport_->isSupportedKeyReleaseEvent(event))
    {
        viewport_->keyReleaseEvent(event);
    }
    else
    {
        QAbstractScrollArea::keyReleaseEvent(event);
    }
}

void QBinaryDataView::gotoAddress(quint64 pos)
{
    Q_ASSERT(viewport_);

    if (!viewport_->dataSource())
        return;

    QModelIndex index = viewport_->dataSource()->offsetToIndex(pos);
    if (index.isValid())
    {
        viewport_->Cursor_moveToIndex(index);
    }
}

bool QBinaryDataView::addressBarVisible() const
{
    Q_ASSERT(viewport_);
    return viewport_->addressBarVisible();
}

bool QBinaryDataView::presentationBarVisible() const
{
    Q_ASSERT(viewport_);
    return viewport_->presentationBarVisible();
}

bool QBinaryDataView::isEditorMode() const
{
    Q_ASSERT(viewport_);
    return viewport_->isEditorMode();
}

unsigned int QBinaryDataView::groupSize() const
{
    Q_ASSERT(viewport_);
    return viewport_->groupSize();
}

void QBinaryDataView::setAddressBarVisible(bool value)
{
    Q_ASSERT(viewport_);
    viewport_->setAddressBarVisible(value);
}

void QBinaryDataView::setPresentationBarVisible(bool value)
{
    Q_ASSERT(viewport_);
    viewport_->setPresentationBarVisible(value);
}

void QBinaryDataView::setEditorMode(bool value)
{
    Q_ASSERT(viewport_);
    viewport_->setEditorMode(value);
}

void QBinaryDataView::setGroupSize(unsigned int newGroupSize)
{
    Q_ASSERT(viewport_);
    viewport_->setGroupSize(newGroupSize);
}

void QBinaryDataView::setProperties(const Properties &properties)
{
    Q_ASSERT(viewport_);

    viewport_->setProperty("addressBarVisible", properties.addressBarVisible);
    viewport_->setProperty("presentationBarVisible", properties.presentationBarVisible);
    viewport_->setProperty("isEditorMode", properties.isEditorMode);
    viewport_->setProperty("groupSize", properties.groupSize);
}

void QBinaryDataView::getProperties(Properties &properties)
{
    Q_ASSERT(viewport_);

    properties.addressBarVisible        = viewport_->property("addressBarVisible").toBool();
    properties.presentationBarVisible   = viewport_->property("presentationBarVisible").toBool();
    properties.isEditorMode             = viewport_->property("isEditorMode").toBool();
    properties.groupSize                = viewport_->property("groupSize").toInt();
}
