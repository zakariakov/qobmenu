/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)GPL3+
 *
 * elokab - a lightweight, Qt based, desktop toolset
 * https://sourceforge.net/project/elokab/
 *
 * Copyright: 2013-2014 elokab team
 * Authors:
 *   Abou Zakaria <yahiaui@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "menusystem.h"
#include "power/powerdlg.h"
//#include "Tawhid_interface.h"
#include <QtDBus>
#include <QProcess>
#include <QApplication>
#include <QMessageBox>
#include "utils/edir.h"
#include <QDebug>
#include <QSettings>
#include <QTextCodec>
MenuSystem::MenuSystem()
{
    QDir serviceDir(Edir::dataDir()+"/elokab-syspower");
//    serviceDir.cdUp();
//    serviceDir.cd( serviceDir.absolutePath()+"/share/elokab/elokab-syspower") ;
    qDebug()<<"syspower=============="<<serviceDir.absolutePath();

    actShutdown=new QAction(loadService(serviceDir.absoluteFilePath("Shutdown.desktop")),this);
    actReboot=new QAction(loadService(serviceDir.absoluteFilePath("Reboot.desktop")),this);
    actHibernate=new QAction(loadService(serviceDir.absoluteFilePath("Hibernate.desktop")),this);
    actSuspend=new QAction(loadService(serviceDir.absoluteFilePath("Suspend.desktop")),this);
    m_actLokScreen=new QAction(loadService(serviceDir.absoluteFilePath("Lock.desktop")),this);
    m_actLogout=new QAction(loadService(serviceDir.absoluteFilePath("Logout.desktop")),this);

    setTitle(actShutdown->text());

    connect(actShutdown, SIGNAL(triggered()), this, SLOT(shutdown()));
    actShutdown->setVisible(m_power.canAction(PowerMain::Shutdown));
    actShutdown->setData("Shutdown");

    connect(actReboot, SIGNAL(triggered()), this, SLOT(reboot()));
    actReboot->setVisible(m_power.canAction(PowerMain::Reboot));
    actReboot->setData("Reboot");

    connect(actHibernate, SIGNAL(triggered()), this, SLOT(hibernate()));
    actHibernate->setVisible(m_power.canAction(PowerMain::Hibernate));
   actHibernate->setData("Hibernate");


    connect(actSuspend, SIGNAL(triggered()), this, SLOT(suspend()));
    actSuspend->setVisible(m_power.canAction(PowerMain::Suspend));
   actSuspend->setData("Suspend");

    connect(m_actLokScreen, SIGNAL(triggered()), this, SLOT(lockScreen()));
    m_actLokScreen->setData("LokScreen");

    connect(m_actLogout, SIGNAL(triggered()), this, SLOT(logout()));
    m_actLogout->setVisible(m_power.canAction(PowerMain::Logout));
    m_actLogout->setData("Logout");

    //**************
    this->addAction(actShutdown);
    this->addAction(actReboot);
    this->addSeparator();
    this->addAction(actHibernate);
    this->addAction(actSuspend);
    this->addSeparator();
    this->addAction(m_actLokScreen);
    this->addAction(m_actLogout);
    //**************
refreshIcons();
}
QString MenuSystem::loadService(const QString &path)
{
    QFileInfo fi(path);

    QString lc=locale().name().section("_",0,0);
    QSettings gs(path,QSettings::IniFormat);
    gs.setIniCodec(QTextCodec::codecForName("UTF-8"));
     gs.beginGroup("Desktop Entry");
     QString nameOld=gs.value("Name",fi.baseName()).toString();
     QString name=gs.value("Name["+lc+"]",nameOld).toString();
     gs.endGroup();
     return name;
}

void MenuSystem::refreshIcons()
{
    setIcon(QIcon::fromTheme("system-shutdown",QIcon::fromTheme("Shutdown")));

    QIcon  pixmap5 =QIcon::fromTheme("system-lock-screen",QIcon::fromTheme("system-lock"));


    QIcon  pixmap1 = PowerMain::ICON(PowerMain::Shutdown);
    QIcon  pixmap2 = PowerMain::ICON(PowerMain::Reboot);
    QIcon  pixmap3 = PowerMain::ICON(PowerMain::Hibernate);
    QIcon  pixmap4 =PowerMain::ICON(PowerMain::Suspend);
    QIcon  pixmap6 =PowerMain::ICON(PowerMain::Logout);
    actShutdown->setIcon((pixmap1));
    actReboot->setIcon((pixmap2));
    actHibernate->setIcon((pixmap3));
    actSuspend->setIcon((pixmap4));
    m_actLokScreen->setIcon((pixmap5));
    m_actLogout->setIcon((pixmap6));

}


void MenuSystem::hibernate(){
    PowerDlg *dlg=new PowerDlg(actHibernate->text(),PowerMain::ICON(PowerMain::Hibernate));

    if(dlg->exec()!=QDialog::Accepted)
        return ;
                    m_power.doAction(PowerMain::Hibernate);

}


void MenuSystem::logout(){
    PowerDlg *dlg=new PowerDlg(m_actLogout->text(),PowerMain::ICON(PowerMain::Logout));
    if(dlg->exec()!=QDialog::Accepted)
        return ;
   m_power.doAction(PowerMain::Logout);
// qApp->quit();

}


void MenuSystem::lockScreen()
{

//          //-------------------
          QDBusInterface dbus("org.elokab.ScreenSaver",
                              "/org/elokab/ScreenSaver",
                              "org.elokab.ScreenSaver");

          if (!dbus.isValid()) {
              qDebug() << "QDBusInterface is not valid! starting xscreensaver-command";
                   QProcess process;
                  // process.startDetached("xscreensaver-command", QStringList() << "-lock");
                      process.startDetached("xdg-screensaver", QStringList() << "lock");

              return ;
          }
          QList<QVariant> args;
      //    args.append("Elokab ScreenSaver");       // Application Name

          dbus.callWithArgumentList(QDBus::NoBlock, "lock", args);


}


void MenuSystem::reboot()
{
    PowerDlg *dlg=new PowerDlg(actReboot->text(),PowerMain::ICON(PowerMain::Reboot));
    if(dlg->exec()!=QDialog::Accepted)
        return ;
               m_power.doAction(PowerMain::Reboot);

}


void MenuSystem::shutdown()
{
    PowerDlg *dlg=new PowerDlg(actShutdown->text(),PowerMain::ICON(PowerMain::Shutdown));
    if(dlg->exec()!=QDialog::Accepted)
        return ;
                   m_power.doAction(PowerMain::Shutdown);

}


void MenuSystem::suspend()
{
    PowerDlg *dlg=new PowerDlg(actSuspend->text(),PowerMain::ICON(PowerMain::Suspend));
    if(dlg->exec()!=QDialog::Accepted)
        return ;
                m_power.doAction(PowerMain::Suspend);

}

