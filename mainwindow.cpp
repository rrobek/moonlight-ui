#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "moonlighttask.h"
#include "optionsdialog.h"

#include <QMessageBox>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    // additional UI which can't be created in the designer:
    toolMenu = new QMenu(this);
    toolMenu->addAction(actionOptions);
    toolMenu->addSeparator();
    toolMenu->addAction(actionPair);
    toolMenu->addAction(actionUnpair);
    optionsButton->setMenu(toolMenu);
    // setting up the log window: min. size:
    QSize desktopSize = qApp->desktop()->size();
    int minw = logList->fontMetrics().width(QString(80, 'x'));
    logList->setMinimumWidth(qMin(desktopSize.width()-20, minw));
    // settomg up item size:
    // setUniformItemSizes() is important to ensure a performant list
    // however, the first item's size must be chosen carefully...
    logList->clear();
    QString strTest(120, QLatin1Char(0xA0)); // 120 chars with average width should be displayable
    logList->addItem(strTest);
    logList->setUniformItemSizes(true);

    resize(sizeHint());

    // start filling the app list
    refreshApps();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_actionOptions_triggered()
{
    OptionsDialog dlg(this);
    if(dlg.exec() == QDialog::Accepted)
        refreshApps();
}

void MainWindow::on_actionPair_triggered()
{
    runTask("pair");
}

void MainWindow::on_actionUnpair_triggered()
{
    runTask("unpair");
}

void MainWindow::on_appList_itemActivated(QListWidgetItem *item)
{
    QStringList args;
    if(item)
        args << "-app" << item->text();

    runTask("stream", args);

}

void MainWindow::on_runButton_clicked()
{
    QListWidgetItem* selItem = appList->currentItem();
    on_appList_itemActivated(selItem);
}

void MainWindow::logMessage(QString msg)
{
    logList->addItem(msg.trimmed());
    logList->scrollToBottom();
}

void MainWindow::dialogMessage(QString msg)
{
    if(activeTask && activeTask->isStreaming())
        logMessage(QString("MESSAGE: ") + msg);
    else
        QMessageBox::information(this, qApp->applicationName(), msg.trimmed());
}

void MainWindow::dialogError(QString err)
{
    if(activeTask && activeTask->isStreaming())
        logMessage(QString("ERROR: ") + err.trimmed());
    else
        QMessageBox::critical(this, qApp->applicationName(), err.trimmed());
}

void MainWindow::serverInfo(QString server)
{
    nameServer->setText(server.trimmed());
}

void MainWindow::graphicsInfo(QString graphics)
{
    nameGraphics->setText(graphics.trimmed());
}

void MainWindow::streamInfo(QString stream)
{
    nameStream->setText(stream.trimmed());
}

void MainWindow::appInfo(QStringList apps)
{
    appList->clear();
    appList->addItems(apps);
}

void MainWindow::taskFinished(QString task)
{
    activeTask = NULL;
    runButton->setEnabled(true);
    if(task == "pair")
        refreshApps();
}

void MainWindow::refreshApps()
{
    // We expect they are no longer valid now:
    appList->clear();
    nameServer->clear();
    nameGraphics->clear();
    nameStream->clear();
    // Fill the list.
    runTask("list");
}

void MainWindow::runTask(const QString& cmd, const QStringList& args)
{
    MoonlightTask* task = new MoonlightTask(this, cmd, args);
    connect(task, SIGNAL(logMessage(QString)), SLOT(logMessage(QString)));
    connect(task, SIGNAL(dialogMessage(QString)), SLOT(dialogMessage(QString)));
    connect(task, SIGNAL(dialogError(QString)), SLOT(dialogError(QString)));
    connect(task, SIGNAL(serverInfo(QString)), SLOT(serverInfo(QString)));
    connect(task, SIGNAL(graphicsInfo(QString)), SLOT(graphicsInfo(QString)));
    connect(task, SIGNAL(streamInfo(QString)), SLOT(streamInfo(QString)));
    connect(task, SIGNAL(appInfo(QStringList)), SLOT(appInfo(QStringList)));
    connect(task, SIGNAL(finished(QString)), SLOT(taskFinished(QString)));
    activeTask = task;
    runButton->setEnabled(false);
    task->run();
}
