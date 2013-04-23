#include "cfrontend2.h"
#include "ui_cfrontend2.h"
#include <QDebug>
#include <QFileDialog>

#include "cdfm2guitree.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include "cguitree2ui.h"
#include "cuidomdocument.h"
#include "cguitreedomdocument.h"
#include <QFileInfo>

#define APPNAME "DFM2QT4"
#define ORGNAME "DFM2QT4"


CFrontend2::CFrontend2(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::CFrontend2)
{
    ui->setupUi(this);

    mPreviewBgColor = QColor(53, 227, 22, 50);

    // setup logger
    mLogModel = new QStandardItemModel(0, 4, this);
    mLogModel->setHeaderData(0, Qt::Horizontal, "Name");
    mLogModel->setHeaderData(1, Qt::Horizontal, "DFM-Type");
    mLogModel->setHeaderData(2, Qt::Horizontal, "UI-Type");
    mLogModel->setHeaderData(3, Qt::Horizontal, "Description");
    ui->logger->setModel(mLogModel);

    // create qdesigner workbench as preview
    mFormPreview = new FormPreview();

    loadStettings();
}

CFrontend2::~CFrontend2()
{
    if(mFormPreview)
        delete(mFormPreview);
    delete(mLogModel);
    delete ui;
}

void CFrontend2::on_btnAddFile_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), mLastDfmDirPath, tr("Borland Form (*.dfm);;All (*)"));
    if(fileNames.count() <= 0)
        return;

    setDfmFilePathList(fileNames);
    storeSettings();
}

void CFrontend2::on_btnRemoveFile_clicked()
{
    QList<QListWidgetItem *>selectedItems;

    selectedItems = ui->listToConvert->selectedItems();
    if(selectedItems.count() < 1)
        return;

    for(int i=ui->listToConvert->count() -1 ; i >= 0; i--)
        if(ui->listToConvert->item(i)->isSelected())
            ui->listToConvert->takeItem(i);

    if(ui->listToConvert->count() <= 0)
        ui->btnConvert->setEnabled(false);
}


void CFrontend2::on_listToConvert_itemSelectionChanged()
{
    QList<QListWidgetItem *>selectedItems;

    selectedItems = ui->listToConvert->selectedItems();
    if(selectedItems.count() <= 0)
        ui->btnConvert->setText(tr("Convert All"));
    else
        ui->btnConvert->setText(tr("Convert This"));

    if(selectedItems.count() == 1 && selectedItems[0]->backgroundColor() == mPreviewBgColor )
        ui->btnPreview->setEnabled(true);
    else
        ui->btnPreview->setEnabled(false);
}



/**
 * Write giving text into the frontend.
 */
void CFrontend2::logText(const QString & text)
{
    QStandardItem *logMsg;
    QStandardItem *logLine;
    QList<QStandardItem*> logRow;

    logLine = new QStandardItem();
    logLine->setText("2105");

    logMsg = new QStandardItem();
    logMsg->setText(text);

    logRow.append(logMsg);
    logRow.append(logLine);
    mLogModel->appendRow(logRow);
    //mLogModel->setData(mLogModel->index(logMsg->index(), 1, QModelIndex()), "Hallo");

    //ui.lwDetails->addItem(text);
}


/**
 *
 *
 **/
void CFrontend2::on_btnConvert_clicked()
{
    convert();
}

/**
 *
 *
 **/
void CFrontend2::convert()
{
    int rc;
    CGuiTreeDomDocument *guiTree;
    QString dfmFilePath, uiFilePath, xmlFilePath;
    QStringList pathList;

    /*  Init.  */

    // clear log output
    mLogModel->clear();

    // get list of DFM files
    pathList = this->getListToConvert();

    // Alloc mem for GUI tree with standardlized widgets
    guiTree = new CGuiTreeDomDocument();

    // Alloc mem for QT UI tree
    CUiDomDocument *qtDoc = new CUiDomDocument();

    // setup parser
    CDfm2GuiTree *parser = new CDfm2GuiTree();
    connect(parser, SIGNAL(logging(const QString &)), this, SLOT(logText(const QString &)));

    // Alloc converter
    CGuiTree2Ui *converter = new CGuiTree2Ui();
    connect(converter, SIGNAL(logging(const QString &)), this, SLOT(logText(const QString &)));

    for(int i = 0; i < pathList.count(); i++)
    {
        dfmFilePath = pathList[i];
        uiFilePath = pathList[i];
        uiFilePath.remove(QRegExp("\.dfm$"));
        uiFilePath.append(".ui");
        xmlFilePath = pathList[i];
        xmlFilePath.remove(QRegExp("\.dfm$"));
        xmlFilePath.append(".xml");

        qtDoc->clear();
        guiTree->clear();

        logText("--- " + dfmFilePath + " ---");

        // parse file
        rc = parser->parseFile(guiTree, dfmFilePath.toLatin1().constData());
        if(rc != 0)
        {
            logText("Failed to parse DFM file.");
            continue;
        }
        logText("Parsed DFM file successfully");

        /* Conver guiTree to an QT4 UI file */
        if(0 != converter->convert(qtDoc, guiTree))
            logText("Failed to convert into UI format");
        else
        {
            this->enablePreview(dfmFilePath);
            logText("Converted successfully");
        }

        // write out
        logText("Write " + uiFilePath);
        dumpDomDoc(qtDoc, uiFilePath.toLatin1().constData());

        logText("Write " + xmlFilePath);
        dumpDomDoc(guiTree, xmlFilePath.toLocal8Bit());
    }

    // free mem
    delete(parser);
    delete(guiTree);
    delete(qtDoc);
    delete(converter);

    // refresh view
    on_listToConvert_itemSelectionChanged();
}


/**
 * @brief Write out xml tree of an domDoc object.
 *
 * @param domDoc XML DOM document to write out
 */
void CFrontend2::dumpDomDoc(QDomDocument *domDoc, const char * fileName)
{
    ofstream uiFileStream;
    uiFileStream.open(fileName);
    if(!uiFileStream.is_open())
    {
        logText("Can't open UI file.");
    }
    else
    {
        // output
        uiFileStream << (string) domDoc->toString(1).toUtf8();

        // close files
        uiFileStream.close();
    }
}


/**
 * Get list of
 *
 *
 **/
QStringList CFrontend2::getListToConvert()
{
    QList<QListWidgetItem *> widgetList;
    QStringList fileList;
    int i;

    widgetList = ui->listToConvert->selectedItems();
    if(widgetList.count() > 0)
    {
        for(i = 0; i < widgetList.count(); i++)
            fileList.append(widgetList[i]->data(Qt::DisplayRole).toString());
    }
    else
    {
        for(i=0; i < ui->listToConvert->count(); i++)
            fileList.append(ui->listToConvert->item(i)->data(Qt::DisplayRole).toString());
    }

    return(fileList);
}


/**
 *
 *
 *
 **/
void CFrontend2::enablePreview(QString &filePath)
{
    QList <QListWidgetItem *>listItem;

    listItem = ui->listToConvert->findItems(filePath, Qt::MatchExactly);
    if(listItem.count() <= 0)
        return;

    for(int i = 0; i < listItem.count(); i++)
        listItem[0]->setBackgroundColor(mPreviewBgColor);
}


/**
 * Show a preview of converted file.
 * Name of file is UI-Name at GUI.
 */
void CFrontend2::on_btnPreview_clicked()
{
    QString fileName;
    QList<QListWidgetItem *>selectedItems;

    // init.
    selectedItems = ui->listToConvert->selectedItems();
    if(selectedItems.count() >= 1)
    {
        fileName = selectedItems[0]->data(Qt::DisplayRole).toString();
        fileName.remove(QRegExp("\.dfm$"));
        fileName.append(".ui");
        showPreview(fileName);
    }
    else
        ui->btnPreview->setEnabled(false);
}


/**
 * Show a preview of converted file.
 * Name of file is UI-Name at GUI.
 */
void CFrontend2::showPreview(QString fileName)
{
    QString errorMsg;
    int rc;

    rc = mFormPreview->loadUiFile(fileName, &errorMsg);
    if(rc == 0)
        mFormPreview->show();
    else
        this->logText(errorMsg);
}


/**
 *
 *
 */
void CFrontend2::setDfmFilePathList(QStringList list)
{
    QFileInfo fi;

    if(list.isEmpty())
        return;

    ui->listToConvert->insertItems(0, list);
    ui->btnConvert->setEnabled(true);

    // remember dir
    fi.setFile(list[0]);
    mLastDfmDirPath = fi.dir().path();
}


void CFrontend2::on_listToConvert_doubleClicked(QModelIndex index)
{
    if(ui->listToConvert->item(index.row())->backgroundColor() == mPreviewBgColor )
        showPreview(ui->listToConvert->item(index.row())->data(Qt::DisplayRole).toString().replace(".dfm", ".ui"));
}


/**
 *
 *
 *
 **/
void CFrontend2::loadLastDfms()
{
    QSettings s(ORGNAME, APPNAME);
    ui->listToConvert->addItems(s.value("gui/lastdfms", "").toStringList());

    // delete empty lines
    QListWidgetItem *item;
    for(int i=0; i < ui->listToConvert->count(); i++)
    {
        item = ui->listToConvert->item(0);
        if(item == NULL)
            break;
        if(item->text().isEmpty())
        {
            ui->listToConvert->takeItem(i);
            i--;
        }
    }

    // activate convert-btn if possible
    if(ui->listToConvert->count() > 0)
        ui->btnConvert->setEnabled(true);
}


/**
 * Load settings.
 *
 **/
void CFrontend2::loadStettings()
{
    bool rememberLastDfms;
    QSettings s(ORGNAME, APPNAME);
    mLastDfmDirPath = s.value("gui/lastdfmdirpath", ".").toString();
    rememberLastDfms = s.value("gui/rememberlastdfms", true).toBool();
    setRememberLastDfms(rememberLastDfms);
}


/**
 * Save settings.
 *
 **/
void CFrontend2::storeSettings()
{
    QStringList lastDfms;

    QSettings s(ORGNAME, APPNAME);
    s.setValue("gui/lastdfmdirpath", mLastDfmDirPath);
    s.setValue("gui/rememberlastdfms", mRememberLastDfms);
    if(mRememberLastDfms)
    {
        for(int i = 0; i < ui->listToConvert->count(); i++)
            lastDfms.append(ui->listToConvert->item(i)->data(Qt::DisplayRole).toString());
    }
    s.setValue("gui/lastdfms", lastDfms);
}


/**
 *
 *
 **/
void CFrontend2::setRememberLastDfms(bool state)
{
    QPalette palette;
    if(state == mRememberLastDfms)
        return;

    mRememberLastDfms = state;
    palette = ui->btnTogleRememberLastDfms->palette();
    if(mRememberLastDfms)
        palette.setColor(QPalette::ButtonText, Qt::black);
    else
        palette.setColor(QPalette::ButtonText, Qt::gray);
    ui->btnTogleRememberLastDfms->setPalette(palette);
}


/**
 *
 *
 **/
void CFrontend2::togleRememberLastDfms()
{
    setRememberLastDfms(!mRememberLastDfms);
    storeSettings();
}



