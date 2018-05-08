#include "findbutton.h"
#include <QDir>
#include <QCompleter>
#include <QProcess>

FindButton::FindButton()
{

    actionWidget = new QWidgetAction(this);
    actionWidget->setObjectName("ActionWidget");

    findLineEdite = new QLineEdit;
    connect(findLineEdite,SIGNAL(textChanged(QString)),this,SIGNAL(findTextChanged(QString)));
    connect(findLineEdite,SIGNAL(returnPressed()),this,SLOT(execFindApplication()));

    findLineEdite->setFrame(false);
    findLineEdite->setStyleSheet("background-color: transparent;");
    findLineEdite->setPlaceholderText("type text");
   // QStringList dirs = QString(getenv("PATH")).split(":");

//    foreach(QString dir,dirs)
//    {
//        //         qDebug()<<dir;
//        chargeCompliterDirs( dir);
//    }

 //   m_wordList.removeDuplicates();
 //   QCompleter *completer = new QCompleter(m_wordList, this);
 //   completer->setCaseSensitivity(Qt::CaseInsensitive);
 //   completer->setCompletionMode(QCompleter::InlineCompletion);
   // findLineEdite->setCompleter(completer);
    actionWidget->setDefaultWidget(findLineEdite);
}

//------------------------------------------------------------------
void FindButton::execFindApplication()
{
    QString exec=findLineEdite->text();
    if(exec.isEmpty())return;

        QProcess process;
        process.setWorkingDirectory(QDir::homePath());

        process.startDetached(exec,QStringList(),QDir::homePath());
        // qDebug()<<exec;

}

/************************************
 ***تحميل قائمة بالبرامج المثبتة*****
 ************************************/

void FindButton::chargeCompliterDirs(QString dir)
{
          chargeCompliterFiles(dir);
          //qDebug()<<dir;
          QString subdir;
          QDir dirS(dir);
          foreach ( subdir, dirS.entryList(QDir::AllDirs| QDir::NoSymLinks| QDir::NoDotAndDotDot )){
                    chargeCompliterDirs(dir+"/"+subdir);

          }
}

void FindButton::chargeCompliterFiles(QString dir)
{
          QString subfile;
          QDir dirS(dir);
          foreach ( subfile, dirS.entryList(QDir::AllEntries)){
                    QFileInfo fi(dir+"/"+subfile);

                    if(fi.isExecutable()&&fi.isFile()){
                          QString name=fi.baseName();
                           m_wordList.append(name);

                    }
          }
}

void FindButton::setFocus()
{
    findLineEdite->setFocus();
}
