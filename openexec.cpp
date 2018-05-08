#include "openexec.h"
#include "utils/desktopfile.h"
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QSettings>
OpenExec::OpenExec(QObject *parent) : QObject(parent)
{

}
void OpenExec::execFile(const QString &fileName)
{

    QProcess process;
    process.setWorkingDirectory(QDir::homePath());
    QDir::setCurrent(QDir::homePath());
//dir.setCurrent(QDir::homePath());
    //------------------------------------------------------------non desktop
    if(!fileName.endsWith(".desktop")){
        qDebug()<<fileName;
        process.startDetached(fileName.trimmed()/*,QStringList(),QDir::homePath()*/);
        return;
    }

    //--------------------------------------------------------------desktop
    DesktopFile ds(fileName,"Desktop Entry");

    QString exec=ds.value("Exec").toString();

    QRegExp ex("\\%[A-Z]");
    QRegExp ex2("\\%[a-z]");
    exec.remove(ex);
    exec.remove(ex2);
    exec.remove("-caption");

    bool terminal=ds.value("Terminal",false).toBool();

    QString term=defaultTerminal();
    if(terminal)
        process.startDetached(QString("%1 -e %2").arg(term).arg(exec.trimmed()));//Todo remplace xfce4-terminal
    else
        process.startDetached(exec.trimmed());


}

bool appExists(const QString &fm)
{
    if(fm.isEmpty())return false;
    QStringList dirs = QString(getenv("PATH")).split(":");

    foreach(QString dir,dirs)
    {

            if(QFile::exists(dir+"/"+fm)){

                qDebug()<<"exist"<<dir+"/"+fm;
                 return true;
            }


    }
    return false;
}

void OpenExec::execFolder(const  QString &localDir)
{


  if(localDir.isEmpty())return;

  //var for browser name
  QString costumBrowser;

  //search in user setting
  QSettings setting;
  //setting.beginGroup("Menu");
  costumBrowser=(setting.value("Browser").toString());
  //setting.endGroup();
  if(!appExists(costumBrowser))
      costumBrowser=QString();

  qDebug()<<"costumBrowser user"<<costumBrowser;

  //search in envirenement
  if(costumBrowser.isEmpty()){
      QByteArray sS=qgetenv("DESKTOP_SESSION");
      if(sS=="elokab-session"){

          QSettings setting("elokab","elokabsettings");
          setting.beginGroup("DefaultBrowser");
          costumBrowser=(setting.value("BROWSER","elokab-fm").toString());
          setting.endGroup();

      }
      //search in enverenment
      else if(sS=="xfce")
          costumBrowser="thunar";
      else if(sS==" plasma-wayland-session"||sS==" plasma-session"||sS.contains("plasma"))
          costumBrowser="dolphin";
      else if(sS=="gnome-session"||sS.contains("gnome"))
          costumBrowser="nautilus";
      else if(sS=="cinnamon-session")
          costumBrowser="nimo";
      else if(sS==("lxsession"))
          costumBrowser="pcmanfm";
      //TODO add env
  }
  qDebug()<<"costumBrowser env"<<costumBrowser;

  if(!appExists(costumBrowser))
      costumBrowser=QString();

  //search in list
  if(costumBrowser.isEmpty()){
      //TODO add other browser
      QStringList list;
      list<<"elokab-fm"<<"caja"<<"dolphin"<<"thunar"<<"qtfm"<<"nemo"
         <<"deepin-file-manager"<<" emelfm2"<<" nautilus"
        << "konqueror"<<"pcmanfm"<<"spacefm"<<"worker" ;


          foreach (QString fm, list) {

              if(appExists(fm)){
                  costumBrowser=fm;
                 break;
              }


          }



  }
  qDebug()<<"costumBrowser list"<<costumBrowser;
  //
  if(costumBrowser.isEmpty()) {

      qDebug()<<"defaultBrowser xdg open"<<localDir;

      QProcess p;
      p.startDetached(QString("xdg-open \"%1\"").arg(localDir));

  }else{
     qDebug()<<"defaultBrowser prosess"<<costumBrowser;
      QStringList list;

      list<<localDir;
      QProcess process;
      process.startDetached(costumBrowser,list);
      qDebug()<<costumBrowser<<list ;

  }

    //   this->hide();
}




//############################# get prefired terminam ###########################
QString OpenExec::defaultTerminal()
{
      QString terminal;
     QSettings setting;
     terminal=setting.value("Terminal").toString();

      if(!terminal.isEmpty() && appExists(terminal))
          return terminal;

    QByteArray sS=qgetenv("DESKTOP_SESSION");
    qDebug()<<"envirenment"<<sS;

    if(sS=="elokab-session"){
        QSettings setting("elokab","elokabsettings");
        setting.beginGroup("Terminal");
        terminal=  setting.value("Default","xterm").toString();
        setting.endGroup();
        return terminal;
    }

    //search in enverenment
    if(sS=="xfce")
        terminal="xfce4-terminal";
    else if(sS=="Enlightenment"||sS.contains("enlightenment"))
        terminal="terminology";
    else if(sS==" plasma-wayland-session"||sS==" plasma-session"||sS.contains("plasma"))
        terminal="konsole";
    else if(sS=="gnome-session"||sS.contains("gnome"))
        terminal="gnome-terminal";
    else if(sS=="cinnamon-session")
        terminal="gnome-terminal";
    else if(sS.contains("deepin"))
        terminal="gnome-terminal";
    else if(sS==("lxsession"))
        terminal="lxterminal";
    //search list of terminal

    if(terminal.isEmpty()){
        //TODO remplace this list
        QStringList list;
        list<<"elokab-terminal"<<"gnome-terminal"<<"konsole"<<"termite"<<"deepin-terminal"<<"terminology"<<"xfce4-terminal"
           << "lxterminal"<<"qterminal"<<"mate-terminal"<<"pantheon-terminal"<<"terminator"<<"theterminal"<<"aterm"
           <<"urxvt"<<"eterm"<<"mlterm"<<" tilda";

            foreach (QString term, list) {
                if( appExists(term)){
                    terminal=term;
                    qDebug()<<"terminal exist"<<term;
                    break;
                }
            }

    }
    if(terminal.isEmpty())
        terminal="xtem";

    qDebug()<<"defaultTerminal"<<terminal;
    return terminal;

}

