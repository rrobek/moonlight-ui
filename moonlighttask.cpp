#include "moonlighttask.h"
#include "optionsdialog.h"

MoonlightTask::MoonlightTask(QObject *parent, const QString &cmd, const QStringList &args) :
    QObject(parent), cmd(cmd), args(args)
{
}

void MoonlightTask::run()
{
    Options o = OptionsDialog::getOptions(isStreamCommand());
    proc = new QProcess(this);
    QString prog = o.executable; // TODO moonlight path
    QStringList allArgs;
    allArgs << cmd;
    allArgs << "-uifriendly";
    allArgs << args;
    allArgs << o.args;
    allArgs << o.server;
    // TODO additional args as configured in options
    proc->setReadChannel(QProcess::StandardOutput);
    // signals:
    connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(finished(int,QProcess::ExitStatus)));
    connect(proc, SIGNAL(error(QProcess::ProcessError)), SLOT(procError(QProcess::ProcessError)));
    connect(proc, SIGNAL(readyRead()), SLOT(readyReadStdOut()));
    connect(proc, SIGNAL(readyReadStandardError()), SLOT(readyReadStdErr()));

    // actually start:
    proc->start(prog, allArgs, QIODevice::ReadOnly | QIODevice::Text);
}

bool MoonlightTask::isStreamCommand()
{
    return cmd == "stream";
}
bool MoonlightTask::isStreaming()
{
    return isStreamCommand() && proc && proc->state() == QProcess::Running;
}

void MoonlightTask::procError(QProcess::ProcessError err)
{
    QString msg;
    switch(err) {
        case QProcess::FailedToStart:	msg = "The task failed to start."; break;
        case QProcess::Crashed:	msg = "The task crashed some time after starting successfully."; break;
        case QProcess::Timedout: msg = "A waiting operation timed out."; break;
        case QProcess::WriteError: msg = "An error occurred while writing to the task."; break;
        case QProcess::ReadError: msg = "An error occurred while reading from the task."; break;
        case QProcess::UnknownError: msg = "An unknown error occurred with the task."; break;
    }
    emit dialogError(msg);
    emit finished();
}

void MoonlightTask::finished(int code, QProcess::ExitStatus)
{
    QString msg(QString("Task %1 completed with code %2.").arg(cmd).arg(code));
    emit logMessage(msg);
    emit finished();
}

void MoonlightTask::readyReadStdErr()
{
    stderrBuffer.append(proc->readAllStandardError());
    int idx = -1;
    while((idx = stderrBuffer.indexOf('\n')) >= 0) {
        QString str(stderrBuffer.left(idx));
        stderrBuffer.remove(0, idx+1);
        emit dialogError(str);
    }
}

void MoonlightTask::readyReadStdOut()
{
    while(proc->canReadLine()) {
        QString line = proc->readLine();
        // TODO parse the line
        emit logMessage(line);
    }
}

