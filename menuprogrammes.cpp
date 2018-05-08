/***************************************************************************
 *   elokab Copyright (C) 2014 AbouZakaria <yahiaui@gmail.com>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "menuprogrammes.h"
#include "utils/desktopfile.h"
#include "openexec.h"
#include "utils/edir.h"
#include <QFile>

#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QProcess>

#include <QApplication>

XdgAction::XdgAction(const QString &file,QObject *parent): QAction(parent)
{
    DesktopFile dsg(file,"Desktop Entry");
    QString iconName=dsg.value("Icon").toString();
    setIcon(QIcon::fromTheme(iconName));
    setText(dsg.value("Name").toString());
    setToolTip(dsg.value("GenericName").toString());
    setData(file);
    mExec=dsg.value("Exec").toString();
    mIconName=iconName;
    connect(this,SIGNAL(triggered()),this,SLOT(lanchApplication()));
}

void XdgAction::lanchApplication()
{

    QString exec=mExec;
    //        kdesudo

    if(exec.contains("kdesudo")||exec.contains("gksudo")){
        exec.remove("kdesudo")   ;
        exec.remove("gksudo")   ;
        exec=QString("ekbsudo %1 -i %2").arg(exec).arg(mIconName);
    }
    QProcess process;
    process.setWorkingDirectory(QDir::homePath());
    qDebug()<<"XdgAction::lanchApplication()===================="<<exec;

    process.startDetached(exec);


}


MenuProgrammes::MenuProgrammes(QWidget *parent) :
    QMenu(parent)
{

    QStringList list=  openDesktopDerectory("elokab-menu-applications.directory");

    QString text=list.at(0);

    QString icon=list.at(2);
    setTitle(text);
//    QIcon icnF=EIcon::fromTheme(windowIconText(),"folder");
//    setIcon(icnF);
    setWindowIconText(icon);
    m_menuGen =new MenuGen(this->locale().name().section("_",0,0),this);

    connect(m_menuGen,SIGNAL(appDesktopChanged()) ,this,SLOT(rechargeAppDesktop()));
    chargeMenu();
}
//! [0]
void MenuProgrammes::chargeMenu()
{
    //m_sWatcher->blockSignals(true);
    foreach (QMenu *mn,hashMenu){
        foreach (QAction *a, mn->actions()) {

            delete a;
        }
        delete mn;
    }


    menus.clear();
    this->clear();

    hashMenu.clear();
    QString xmlPath=QDir::homePath()+"/.config/menus/elokab-applications.menu";
    if(!QFile::exists(xmlPath)){
        QDir appDir(QApplication::applicationDirPath());
        appDir.cdUp();
        appDir.cdUp();
        xmlPath=  appDir.absolutePath()+"/etc/xdg/menus/elokab-applications.menu";
        //  xmlPath="/etc/xdg/menus/elokab-applications.menu";xfce-applications.menu
    }

    QFile file(xmlPath);

  qDebug()<<file.fileName()<<"===================";

    if (!file.open(QIODevice::ReadOnly)){
        qDebug()<<file.errorString();
        return;
    }
    if (!m_doc.setContent(&file)) {
        qDebug()<<"MenuProgrammes::chargeMenu() error xmlContent";
        file.close();
        return;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = m_doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement menu = n.toElement(); // try to convert the node to an element.
        if(!menu.isNull()) {
            //   qDebug()<<menu.tagName(); // the node really is an element.
            if(menu.tagName()=="Menu"){
                //  QString name=   menu.firstChildElement("Name").text();
                QString directory=  menu.firstChildElement("Directory").text().trimmed();
                QString category;

                QDomNode Include = menu.firstChildElement("Include");
                QDomNode And = Include.firstChildElement("And");
                QDomElement Cat = And.toElement();
                if(!Cat.isNull()) {
                    category=   Cat.firstChildElement("Category").text().trimmed();

                }

                addMenuDirectory(directory,category);
            }
        }
        n = n.nextSibling();
    }

    chargeAppDesktop();
    //  m_sWatcher->blockSignals(false);
}
//! [0][1]
void MenuProgrammes::addMenuDirectory( QString directory,QString category)
{

    QStringList list=  openDesktopDerectory( directory);

    QString text=list.at(0);

    QString comment=list.at(1);
    QString icon=list.at(2);


    QMenu *menu=new QMenu;
    menu->setTitle(text);
   // menu->setToolTip(comment);
    //حفظ اسم الايقونة لاستخدامه عند التحديث
    menu->setObjectName(icon);

    menus.append(menu);
    hashMenu.insert(category,menu);

    //hashMenu[category] = menu;


    QIcon icnF=QIcon::fromTheme(icon,QIcon::fromTheme("folder"));
    menu->setIcon(icnF);

}

QStringList MenuProgrammes::openDesktopDerectory(QString directory)
{
    QString directoryPath=QDir::homePath()+"/.local/share/desktop-directories/"+directory;
    //   qDebug()<<directoryPath<<"----------------------------------------------------------------";

    if(!QFile::exists(directoryPath)){
        QDir appDir(QApplication::applicationDirPath());
        appDir.cdUp();

        directoryPath=  appDir.absolutePath()+"/share/desktop-directories/"+directory;

    }
    QString lc=this->locale().name().section("_",0,0);


    DesktopFile xdg(directoryPath,"Desktop Entry",lc);
    QString oldname=xdg.value("Name").toString();
    QString text=xdg.value("Name["+lc+"]",oldname).toString();
    QString icon=xdg.value("Icon").toString();
    QString oldComment=xdg.value("Comment").toString();
    QString comment=xdg.value("Comment["+lc+"]",oldComment).toString();
    // qDebug()<<directoryPath<<"--------------------------noexist--------------------------------------";

    //    QString lc=this->locale().name().section("_",0,0);
    //    QSettings settings(directoryPath,QSettings::IniFormat);
    //    settings.setIniCodec("UTF-8");
    //    settings.beginGroup("Desktop Entry");
    //    QString textOld=settings.value("Name").toString();
    //    QString text=settings.value("Name["+lc+"]",textOld).toString();
    //    QString commentOld=settings.value("Comment").toString();
    //    QString comment=settings.value("Comment["+lc+"]",commentOld).toString();
    //    QString icon=settings.value("Icon").toString();

    //    settings.endGroup();

    //  qDebug()<<text<<comment<<icon<<"----------------------------------------------------------------";
    return QStringList()<<text<<comment<<icon;
}

//! [0][2]
void MenuProgrammes::chargeAppDesktop()
{

 /**********************************************
 *******************new menu*************
 **********************************************/

    QFile file(Edir::configDir()+"/applictions");
    if(!file.exists()||QFileInfo(file).size()==0)
        m_menuGen->genirateAppFile();

    if (!file.open(QFile::ReadOnly)){
        qDebug()<<file.fileName()+file.errorString();
        return ;
    }
    QTextStream textStream(&file);
    QString line;
    QString lc=this->locale().name().section("_",0,0);
    line= textStream.readLine();//premier line

    while (!line.isNull()) {
        QString filePath=line.section("|",0,0);

        DesktopFile dsg(filePath,"Desktop Entry",lc);

        QString oldname=dsg.value("Name").toString();
        QString name=dsg.value("Name["+lc+"]",oldname).toString();
        QString exec=dsg.value("Exec").toString().trimmed();

        QString category=dsg.value("Categories","Other").toString().trimmed();
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

        connect(act, SIGNAL(triggered()), this, SLOT(lanchApplication()));

        QStringList listCat=category.split(";", QString::SkipEmptyParts);
        bool exist=false;
        foreach (QMenu *mn,hashMenu){
            QString cat=hashMenu.key(mn);

            foreach(QString s,listCat){
                if(s.contains(cat)&&exist==false){

                    exist=true;
                    mn->addAction(act);

                }

            }


        }
        if(exist==false){
            if(hashMenu.contains("Other")){
                QMenu *mn=hashMenu.value("Other");
                mn->addAction(act);

            }
        }



        line= textStream.readLine();
    }
    file.close();



    foreach (QMenu *mn,menus){

        if(mn->actions().count()>0){
            this->addMenu(mn);

        }else{
            menus.removeOne(mn);
        }
    }
}


void MenuProgrammes::refreshIcons()
{


   // setIcon(EIcon::fromTheme(windowIconText(),"folder"));
    QAction *act;
    QList<QAction *> list;

    foreach (QMenu *mn,menus){

        list.append(mn->actions());
        QString icon=  mn->objectName();
        mn->setIcon(QIcon::fromTheme(icon,QIcon::fromTheme("folder")));

    }

    foreach(act,list)
    {
        QStringList listAct=act->data().toStringList();

        QString icon=listAct.at(1);

        QIcon mIcon=QIcon::fromTheme(icon,QIcon::fromTheme("application-x-executable"));

        act->setIcon(mIcon);
    }

    list.clear();

}

void MenuProgrammes::lanchApplication()
{


    QAction *action = qobject_cast<QAction *>(sender());
    if (action){
        QStringList data=action->data().toStringList();
        QString exec=data.at(2).trimmed();
        qDebug()<<exec;
        OpenExec::execFile(exec);
    }
    emit actionExecuted(action);
//        //  qDebug()<<"===================="<<action->data().toStringList();
//        QStringList data=action->data().toStringList();
//        QString exec=data.at(0).trimmed();
//        QString terminal=data.at(3).trimmed();
//        if(exec.contains("su-to-root")){
//            exec=QString("ekbsudo %1 -i %2").arg(exec).arg(data.at(1));
//        }
//        QProcess process;
//        process.setWorkingDirectory(QDir::homePath());
//     if(terminal=="true"){
//           process.startDetached(QString("xfce4-terminal -e %1").arg(exec));
//     }else
//      process.startDetached(exec,QStringList(),QDir::homePath());
//     //  QDesktopServices::openUrl( QUrl::fromLocalFile(exec) );
//     emit actionExecuted(action);
//    }

}

void MenuProgrammes::rechargeAppDesktop()
{
    m_menuGen->blockSignals(true);
    chargeMenu();

    m_menuGen->blockSignals(false);
    emit  menuRecharged();

    qDebug()<<"MenuProgrammes <<< emit menuRecharged====================================";
}
