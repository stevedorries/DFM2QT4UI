#include "clogger.h"

CLogger::CLogger(QWidget *parent) :
    QListView(parent)
{


}


int CLogger::init()
{
    mLogModel = new QStandardItemModel(5, 4, this);
    mLogModel->setHeaderData(0, Qt::Horizontal, "Name");
    mLogModel->setHeaderData(1, Qt::Horizontal, "DFM-Type");
    mLogModel->setHeaderData(2, Qt::Horizontal, "UI-Type");
    mLogModel->setHeaderData(3, Qt::Horizontal, "Description");

    mLogModel->insertRows(0, 1, QModelIndex());
    mLogModel->setData(mLogModel->index(0, 0, QModelIndex()), "btnEdit");
    mLogModel->setData(mLogModel->index(0, 1, QModelIndex()), "TButton");
    mLogModel->setData(mLogModel->index(0, 2, QModelIndex()), "QPushButton");
    mLogModel->setData(mLogModel->index(0, 3, QModelIndex()), "Failed to create something.");

    mLogModel->insertRows(0, 1, QModelIndex());
    mLogModel->setData(mLogModel->index(0, 0, QModelIndex()), "lbWhat");
    mLogModel->setData(mLogModel->index(0, 1, QModelIndex()), "TLabel");
    mLogModel->setData(mLogModel->index(0, 2, QModelIndex()), "QLabel");
    mLogModel->setData(mLogModel->index(0, 3, QModelIndex()), "Failed to create something.");

    this->setModel(mLogModel);

    return(0);
}
