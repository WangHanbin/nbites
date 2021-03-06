#include "OfflineDataFinder.h"

#include <iostream>
#include <QCoreApplication>
#include <QLineEdit>

#include "NaoPaths.h"
#include "io/FileInProvider.h"

namespace qtool {
namespace data {

using namespace common::io;
using namespace common::paths;

static const int DEFAULT_NAME_COLUMN_WIDTH = 300;

OfflineDataFinder::OfflineDataFinder(QWidget* parent) :
    DataFinder(parent) {
    QLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);

    setupFSModel();
    setupFSBrowser();
    setupLogLabels();
    layout->addWidget(logLabel);
    layout->addWidget(logPathLabel);
    layout->addWidget(fsBrowser);

    this->setLayout(layout);
}

OfflineDataFinder::~OfflineDataFinder() {
}

void OfflineDataFinder::setupLogLabels() {

    logLabel = new QLabel(this);
    logLabel->setText("Current log:");

    logPathLabel = new QLineEdit(this);
    logPathLabel->setReadOnly(true);
}

void OfflineDataFinder::setupFSModel() {

    fsModel = new QFileSystemModel(this);
    fsModel->setRootPath(QDir::homePath());
    fsModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
}

void OfflineDataFinder::setupFSBrowser() {

    fsBrowser = new QTreeView(this);
    fsBrowser->setModel(fsModel);
    fsBrowser->setRootIndex(fsModel->index(QString(NBITES_DIR) + "/data/logs"));
    fsBrowser->expand(fsModel->index(QDir::currentPath()));

    fsBrowser->setColumnWidth(0, DEFAULT_NAME_COLUMN_WIDTH);
    fsBrowser->setColumnHidden(1, true);
    fsBrowser->setColumnHidden(2, true);
    fsBrowser->setColumnHidden(3, true);

    connect(fsBrowser, SIGNAL(expanded(const QModelIndex&)),
            this, SLOT(folderChanged(const QModelIndex&)));
}

void OfflineDataFinder::folderChanged(const QModelIndex& index) {
    this->scanFolderForLogs(fsModel->filePath(index));
}

void OfflineDataFinder::scanFolderForLogs(QString path) {
    QString filter = QString(("*" + NAO_LOG_EXTENSION).data());
    QDir dir(path, filter);
    QFileInfoList list = dir.entryInfoList();

    if (list.size() == 0) {
        return;
    }

    logPathLabel->setText(path); // notify the user of the new log
    emit signalNewDataSet();

    for (int i = 0; i < list.size(); i++) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.size() != 0) {
            std::string path = fileInfo.absoluteFilePath().toStdString();
            InProvider::ptr file_in(new FileInProvider(path));
            emit signalNewInputProvider(file_in, fileInfo.baseName().toStdString());
        }
    }
}

}
}
