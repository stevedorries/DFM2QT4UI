#include "formpreview.h"
#include "ui_formpreview.h"
#include <QFile>
#include <QVBoxLayout>
#include <QFormBuilder>


FormPreview::FormPreview(QWidget *parent) :
        QWidget(parent),
        mLoadWidget(NULL),
        ui(new Ui::FormPreview)
{
    ui->setupUi(this);
}


FormPreview::~FormPreview()
{
    delete ui;
}


int FormPreview::loadUiFile(QString &aFileName, QString *aErrorMsg)
{
    QFormBuilder builder;
    QWidget *childWidget;
    QFile file;
    QVBoxLayout *layout;
    QRect rectChildWidget;

    // clear widget loaded before
    if(mLoadWidget != NULL)
    {
        delete(mLoadWidget);
        mLoadWidget = NULL;
    }

    file.setFileName(aFileName);
    if(!file.open(QFile::ReadOnly))
    {
        *aErrorMsg = QString("The file ") + aFileName + " could not be opened.";
        return(-1);
    }

    childWidget = builder.load(&file, this);
    file.close();
    if(childWidget == NULL)
    {
        *aErrorMsg = QString("The file ") + aFileName + " could not be loaded.";
        return(-1);
    }
    mLoadWidget = childWidget;

    rectChildWidget = childWidget->geometry();
    this->setWindowTitle(childWidget->windowTitle());
    this->resize(rectChildWidget.width(), rectChildWidget.height());

    layout = new QVBoxLayout;
    layout->addWidget(childWidget);
    setLayout(layout);

    *aErrorMsg = "";
    return(0);
}
