#ifndef QBINARYDATAVIEW_PROPERTIESEDITOR_H
#define QBINARYDATAVIEW_PROPERTIESEDITOR_H

#include <QWidget>

#include "qbinarydataview.h"

namespace Ui {
class QBinaryDataView_PropertiesEditor;
}

class QBinaryDataView_PropertiesEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit QBinaryDataView_PropertiesEditor(const QBinaryDataView::Properties &properties, QWidget *parent = 0);
    ~QBinaryDataView_PropertiesEditor();

    QBinaryDataView::Properties& properties();
    void setProperties(const QBinaryDataView::Properties &properties);

protected slots:
    void groupWidthChanged(int value);
    void addressBarChanged(bool value);
    void presentationBarChanged(bool value);
    void editableChanged(bool value);

public slots:
signals:
    void propertiesChanged();

protected:
    
private:
    Ui::QBinaryDataView_PropertiesEditor *ui;
    QBinaryDataView::Properties currentProperties_;

};

#endif // QBINARYDATAVIEW_PROPERTIESEDITOR_H
