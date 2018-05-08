#ifndef OPENEXEC_H
#define OPENEXEC_H

#include <QObject>

class OpenExec : public QObject
{
    Q_OBJECT
public:
    explicit OpenExec(QObject *parent = 0);
static void execFile(const QString &fileName);
static void execFolder(const  QString &localDir);

static QString defaultTerminal();
signals:

public slots:
};

#endif // OPENEXEC_H
