#include "qbinarydataview_propertieseditor.h"
#include "ui_qbinarydataview_propertieseditor.h"

QBinaryDataView_PropertiesEditor::QBinaryDataView_PropertiesEditor(const QBinaryDataView::Properties &properties, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QBinaryDataView_PropertiesEditor)
    , currentProperties_()
{
    ui->setupUi(this);

    setProperties(properties);

    connect(ui->groupWidth, SIGNAL(valueChanged(int)), this, SLOT(groupWidthChanged(int)));
    connect(ui->adressBar, SIGNAL(toggled(bool)), this, SLOT(addressBarChanged(bool)));
    connect(ui->presentationBar, SIGNAL(toggled(bool)), this, SLOT(presentationBarChanged(bool)));
    connect(ui->editable, SIGNAL(toggled(bool)), this, SLOT(editableChanged(bool)));
}

QBinaryDataView_PropertiesEditor::~QBinaryDataView_PropertiesEditor()
{
    delete ui;
}

void QBinaryDataView_PropertiesEditor::setProperties(const QBinaryDataView::Properties &properties)
{
    currentProperties_.groupSize                = properties.groupSize;
    currentProperties_.addressBarVisible        = properties.addressBarVisible;
    currentProperties_.presentationBarVisible   = properties.presentationBarVisible;
    currentProperties_.isEditorMode             = properties.isEditorMode;

    ui->groupWidth->setValue(currentProperties_.groupSize);
    ui->adressBar->setChecked(currentProperties_.addressBarVisible);
    ui->presentationBar->setChecked(currentProperties_.presentationBarVisible);
    ui->editable->setChecked(currentProperties_.isEditorMode);
}

QBinaryDataView::Properties& QBinaryDataView_PropertiesEditor::properties()
{
    return currentProperties_;
}

void QBinaryDataView_PropertiesEditor::groupWidthChanged(int value)
{
    if (currentProperties_.groupSize != value)
    {
        currentProperties_.groupSize = value;
        emit propertiesChanged();
    }
}

void QBinaryDataView_PropertiesEditor::addressBarChanged(bool value)
{
    if (currentProperties_.addressBarVisible != value)
    {
        currentProperties_.addressBarVisible = value;
        emit propertiesChanged();
    }
}

void QBinaryDataView_PropertiesEditor::presentationBarChanged(bool value)
{
    if (currentProperties_.presentationBarVisible != value)
    {
        currentProperties_.presentationBarVisible = value;
        emit propertiesChanged();
    }
}

void QBinaryDataView_PropertiesEditor::editableChanged(bool value)
{
    if (currentProperties_.isEditorMode != value)
    {
        currentProperties_.isEditorMode = value;
        emit propertiesChanged();
    }
}
