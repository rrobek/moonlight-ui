#ifndef MOONLIGHTTASK_H
#define MOONLIGHTTASK_H

#include <QObject>
#include <QProcess>

class MoonlightTask : public QObject
{
    Q_OBJECT
public:
    explicit MoonlightTask(QObject *parent, const QString& cmd, const QStringList& args);
    void run();

    bool isStreamCommand();
    bool isStreaming();

signals:
    void logMessage(QString msg);
    void dialogMessage(QString msg);
    void dialogError(QString err);
    void serverInfo(QString server);
    void graphicsInfo(QString graphics);
    void streamInfo(QString stream);
    void appInfo(QStringList apps);
    void finished(QString task);
public slots:
    void procError(QProcess::ProcessError err);
    void finished(int code, QProcess::ExitStatus);
    void readyReadStdErr();
    void readyReadStdOut();
private:
    QProcess* proc;
    QByteArray stderrBuffer; // we must implement line splitting for stderr ourselves

protected:
    QString cmd;
    QStringList args;

    QStringList appNames;
};

#endif // MOONLIGHTTASK_H
