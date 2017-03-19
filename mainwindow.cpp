#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "moonlighttask.h"

#include <QMessageBox>

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

    resize(sizeHint());

    // start filling the app list
    runTask("list");
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_actionOptions_triggered()
{

}

void MainWindow::on_actionPair_triggered()
{

}

void MainWindow::on_actionUnpair_triggered()
{

}

void MainWindow::on_runButton_clicked()
{

}

void MainWindow::logMessage(QString msg)
{
    logList->addItem(msg);
    logList->scrollToBottom();
}

void MainWindow::dialogMessage(QString msg)
{
    QMessageBox::information(this, qApp->applicationName(), msg);
}

void MainWindow::dialogError(QString err)
{
    QMessageBox::critical(this, qApp->applicationName(), err);
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

void MainWindow::taskFinished()
{
    activeTask = NULL;
    runButton->setEnabled(true);
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
    connect(task, SIGNAL(finished()), SLOT(taskFinished()));
    activeTask = task;
    runButton->setEnabled(false);
    task->run();
}
