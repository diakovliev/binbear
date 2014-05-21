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
