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

#ifndef MENUPROGRAMMES_H
#define MENUPROGRAMMES_H

#include "menugen.h"
#include <QFileSystemWatcher>
#include <QMenu>
#include <QDomDocument>

class XdgAction : public QAction
{
     Q_OBJECT
public:
    XdgAction(const QString &file,QObject *parent);

private:
    QString mExec;
    QString mIconName;
public slots:
 //   void refreshIcon();
private slots:
    void lanchApplication();
};


class MenuProgrammes : public QMenu
{
        Q_OBJECT
public:
        explicit MenuProgrammes( QWidget *parent = 0);
        QList<QMenu *>menus;

signals:
       void menuRecharged();
       void actionExecuted(QAction *act);
private:
      MenuGen *m_menuGen;

 QHash<QString, QMenu *> hashMenu;

 QDomDocument m_doc;
public slots:
   void refreshIcons();
   void rechargeAppDesktop();
private slots:
        void chargeAppDesktop();

      void  chargeMenu();
      void addMenuDirectory(QString directory, QString category);
      QStringList openDesktopDerectory(QString directory);
      void lanchApplication();


        
};

#endif // MENUPROGRAMMES_H
