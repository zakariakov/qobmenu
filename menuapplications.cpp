#include "menuapplications.h"
#include "utils/mystyle.h"
#include "utils/desktopfile.h"
//#include "utils/x11utills.h"
#include <QDesktopWidget>
#include <QDebug>
#include<QSettings>


#define  Classic_Menu 0
#define  Form_Menu 1
//------------------------------------------------------------------
MenuApplications::MenuApplications(QObject *parent) :
    QObject(parent),menuStyle(0),/*mMenuForm(0),*/mMenuClassic(0),mX(0),mY(0)

{
    // qDebug()<<"aime here-----------";

    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus ,false);
    mFileSystemWatcher=new QFileSystemWatcher;
    QSettings setting;
    mFileSystemWatcher->addPath(setting.fileName());
    connect(mFileSystemWatcher,SIGNAL(fileChanged(QString)),this,SLOT(loadSettings()));
    loadSettings();

    if(menuStyle==Classic_Menu){
        mMenuClassic=new MenuClassic;

    } else{
        //  mMenuForm=new MenuForm;


    }
}

void MenuApplications::showForm()
{

    //    if(mMenuForm)
    //        mMenuForm->open(QCursor::pos(),0);

}

//------------------------------------------------------------------
void MenuApplications::loadSettings()
{


    QSettings setting;

    // MenuStyle 0=classic 1=formMenu Default=0
    menuStyle=setting.value("MenuStyle",0).toInt();

    //MenuPos position of menu
    //0= cursorPos 1=topLeft 2=topRight 3=botLeft 4=botRight  Default =0
    menuPos=setting.value("MenuPos",0).toInt();

    //

    mX=setting.value("At-X",0).toInt();
    mY=setting.value("At-Y",0).toInt();



}

//------------------------------------------------------------------
void MenuApplications::afterMenuActivated()
{
    QPoint mpos=QCursor::pos();

    QRect screen = QApplication::desktop()->availableGeometry(mpos);

    int h=mMenuClassic->height();
    int w=mMenuClassic->width();

    switch (menuPos) {
    case 1:
        mpos=screen.topLeft()+QPoint(mX,mY);
        break;
    case 2:
        mpos=QPoint(screen.right()-w-mX,screen.top()+mY);
        break;
    case 3:
        mpos=QPoint(screen.left()+mX,screen.bottom()-h-mY);
        break;
    case 4:
        mpos=QPoint(screen.right()-w-mX,screen.bottom()-h-mY);
        break;

    default:
        mpos=QCursor::pos()+QPoint(mX,mY);
        break;
    }


    qDebug()<<mpos;
    if(menuStyle==Classic_Menu){
        if(!mMenuClassic)return;
        mMenuClassic->popup(mpos);
        qDebug()<<"pos2"<<  mMenuClassic->pos();
        //mMenuClassic->showTearOffMenu(mpos);
        mMenuClassic->activateWindow();
        //       X11UTILLS::raiseWindow(mMenuClassic->winId());
        mMenuClassic->setFocus();


    }else{
        //         if(!mMenuForm)return;
        //         showForm();
        //         mMenuForm->setFocus();
    }
}


//------------------------------------------------------------------
void MenuApplications::showMenu()
{

    if(menuStyle==Classic_Menu){
        if(!mMenuClassic)return;
        mMenuClassic->show();

        mMenuClassic->activateWindow();
        mMenuClassic->setFocus();
        mMenuClassic->hide();
    }else{
        //        if(!mMenuForm)return;
        //        mMenuForm ->activateWindow();
        //        mMenuForm->setFocus();
    }

    QTimer::singleShot(10, this, SLOT(afterMenuActivated()));
}

