#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>

#include "ui_mainwindow.h"

class MoonlightTask;

class MainWindow : public QMainWindow, private Ui_MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_actionOptions_triggered();
    void on_actionPair_triggered();
    void on_actionUnpair_triggered();
    void on_runButton_clicked();
    // messages from the task
    void logMessage(QString msg);
    void dialogMessage(QString msg);
    void dialogError(QString err);
    void serverInfo(QString server);
    void graphicsInfo(QString graphics);
    void streamInfo(QString stream);
    void appInfo(QStringList apps);
    void taskFinished();
private:
    QMenu* toolMenu;
    MoonlightTask* activeTask;

    void runTask(const QString &cmd, const QStringList &args = QStringList());
};

#endif // MAINWINDOW_H
