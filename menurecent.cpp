#include "menurecent.h"
#include "utils/edir.h"
#include "openexec.h"
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <QTextCodec>
#include "utils/desktopfile.h"
MenuRecent::MenuRecent(QWidget *parent) :
    QMenu(parent)
{
   this->setTitle(tr("recently used"));
    this->setIcon(QIcon::fromTheme("document-open-recent"));
connect(this,SIGNAL(actionAdded(QAction*)),this,SLOT(addRecentAct(QAction*)));
  loadService();
    loadSettings();
}
void MenuRecent::loadService()
{
    QDir serviceDir(Edir::dataDir()+"/elokab-menu");
    QString path=serviceDir.absoluteFilePath("elokab-recent.desktop");

    QString lc=locale().name().section("_",0,0);
    QSettings gs(path,QSettings::IniFormat);
    gs.setIniCodec(QTextCodec::codecForName("UTF-8"));
    gs.beginGroup("Desktop Entry");
    QString nameOld=gs.value("Name","recently used").toString();
    QString name=gs.value("Name["+lc+"]",nameOld).toString();
    QString iconName=gs.value("Icon").toString();

    this->setTitle(name);
    this->setIcon(QIcon::fromTheme(iconName,QIcon::fromTheme("folder")));
    gs.endGroup();

}
void MenuRecent::addRecentAct(QAction *act)
{
    foreach ( QAction *action, actions()) {

        if(action->data()==act->data()){
           swapAction(action);
         return;
        }

    }
    qDebug()<<"MenuRecent::addRecentAct"<<act->text();
    QAction *action=new QAction(this);

    action->setData(act->data());
    action->setText(act->text());
    action->setToolTip(act->toolTip());
    action->setIcon(act->icon());

    insertAction(this->actions().first(),action);
  //  this->actions().swap(0,actions().count()-1);
    connect(action, SIGNAL(triggered()), this, SLOT(lanchApplication()));

    if(actions().count()>21)
       delete actions().last();

    saveSettings();
}


void MenuRecent::lanchApplication()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action){
        QStringList data=action->data().toStringList();
        QString exec=data.at(2).trimmed();
        qDebug()<<exec;
        OpenExec::execFile(exec);

      swapAction(action);
    }
}

void MenuRecent::swapAction(QAction *action)
{
    if(actions().first()==action)
        return;

    this->removeAction(action);
    this->insertAction(actions().first(),action);
    connect(action, SIGNAL(triggered()), this, SLOT(lanchApplication()));
    saveSettings();
}

void MenuRecent::loadSettings()
{
    QSettings settings(QApplication::organizationName(),"menuRecents");
     int size = settings.beginReadArray("Actions");
     for (int i = 0; i < size; ++i) {
         settings.setArrayIndex(i);

        QString file = settings.value("File").toString();
        if(QFile::exists(file))
        chargeDesktopApp(file);
     }
     settings.endArray();
}
void MenuRecent::saveSettings()
{
    QSettings settings(QApplication::organizationName(),"menuRecents");
     settings.beginWriteArray("Actions");
     for (int i = 0; i < actions().count(); ++i) {
        QStringList list=actions().at(i)->data().toStringList();

         settings.setArrayIndex(i);
         settings.setValue("File",  list.at(2).trimmed());

     }
     settings.endArray();
}
void MenuRecent::chargeDesktopApp(const QString &filePath)
{
     QString lc=this->locale().name().section("_",0,0);

    DesktopFile dsg(filePath,"Desktop Entry",lc);

    QString oldname=dsg.value("Name").toString();
    QString name=dsg.value("Name["+lc+"]",oldname).toString();
    QString exec=dsg.value("Exec").toString().trimmed();

    QString icon=dsg.value("Icon").toString();

    QString oldgenericName=dsg.value("GenericName").toString();
    QString genericName=dsg.value("GenericName["+lc+"]",oldgenericName).toString();

    QString oldcomment=dsg.value("Comment",genericName).toString();
    QString comment=dsg.value("Comment["+lc+"]",oldcomment).toString();

    QAction *act=new QAction(this);
    QStringList list;

    QRegExp ex("\\%[A-Z]");
    QRegExp ex2("\\%[a-z]");
    exec.remove(ex);
    exec.remove(ex2);
    exec.remove("-caption");

    list<<exec<<icon.trimmed()<<filePath;

    act->setData(list);
    act->setText(name);
    act->setToolTip(comment);
    act->setIcon(QIcon::fromTheme(icon,QIcon::fromTheme("application-x-executable")));
    addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(lanchApplication()));
}
