#include "menuclassic.h"
#include "utils/mystyle.h"
#include "utils/desktopfile.h"
#include  <QDesktopWidget>

MenuClassic::MenuClassic(QWidget *parent):
    QMenu(parent)
{
setupMenu();
}

//------------------------------------------------------------------
void MenuClassic::setupMenu()
{


   mFindButton=new FindButton;
 connect(mFindButton,SIGNAL(findTextChanged(QString)),this,SLOT(findText(QString)));
    menuProgrammes=new MenuProgrammes(this);
    connect(menuProgrammes,SIGNAL(menuRecharged()),this,SLOT(rechargeMenu()));
    mMenuRecent=new MenuRecent(this);
     connect(menuProgrammes,SIGNAL(actionExecuted(QAction*)),mMenuRecent,SIGNAL(actionAdded(QAction*)));
   menuPower=new MenuSystem();

    menuFolders=new MenuFolders(this);
    mnuFind=new QMenu(this);
    mnuFind->setTitle(tr("Find"));
    mnuFind->setIcon(QIcon::fromTheme("edit-find"));
    connect(mnuFind,SIGNAL(aboutToShow()),mFindButton,SLOT(setFocus()));




    rechargeMenu();
    loadSettings();

}

//------------------------------------------------------------------
void MenuClassic::rechargeMenu()
{

 //clear();
     this->clear();

     mnuFind->clear();

     mnuFind->addAction(mFindButton->actionfind());

     mnuFind->addSeparator();
 //add menus
     this->addMenu(mnuFind);

    this->addSeparator();
    this->addMenu(mMenuRecent);
    foreach (QMenu *m, menuProgrammes->menus) {
        this->addMenu(m);

    }

    this->addSeparator();
    this->addMenu(menuFolders);
    this->addSeparator();

    this->addMenu(menuPower);



}


//------------------------------------------------------------------
void MenuClassic::loadSettings()
{


    QSettings setting;
    setting.beginGroup("Main");
    QString mparentColor=setting.value("BgColor","#404244").toString();
    QString mparentFColor=setting.value("FgColor","#FFFFFF").toString();
    QString fontName=setting.value("FontName").toString();
  //  int fontSize=setting.value("FontSize").toInt();
    setting.endGroup();

//    QFont font;
//    font.setFamily(fontName);
//    font.setPointSize(fontSize);
//    setFont(font);

    DesktopFile xdg(setting.fileName(),"Menu");
     QString MenuBgColor=xdg.value("MenuBgColor","#404244").toString();
    QString MenuFgColor=xdg.value("MenuFgColor","#FFFFFF").toString();
    QString MenuBorderColor=xdg.value("MenuBorderColor","#666666").toString();

    int radius=xdg.value("BoderRadius",0).toInt();



    this->setContentsMargins(radius,radius,radius,radius);
    QString stMenu=MyStyle:: menuColor()
            .arg(MenuBgColor)
            .arg(MenuFgColor)
            .arg(MenuBorderColor)
            .arg(QString::number(radius));

/*
    setStyleSheet(stMenu);
//    this-> setPalette(Qt::transparent);
//    this->setAttribute(Qt::WA_TranslucentBackground,true);
    foreach (QMenu *m,menuProgrammes->menus) {
        m->setStyleSheet(stMenu);
    }

menuPower->setStyleSheet(stMenu);
*/
}

//------------------------------------------------------------------
void MenuClassic::execApplication()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action){
        QString data=action->data().toString();
        QString exec=data.section("|",0,0);
        QProcess process;
        process.setWorkingDirectory(QDir::homePath());

        process.startDetached(exec);
        //   qDebug()<<exec;
    }
}

//------------------------------------------------------------------Find
void MenuClassic::findText(QString text)
{

    foreach (QAction *a, mnuFind->actions()) {

        if(a->objectName()=="ActionWidget")
            continue;

        mnuFind->removeAction(a);
    }

    if (text.isEmpty()) return;

    foreach (QMenu *m, menuProgrammes->menus) {
        foreach (QAction *a,   m->actions()) {
            QStringList data=a->data().toStringList();
            QString exec=data.at(0);
            if(exec.toLower().contains(text)
                    ||a->text().toLower().contains(text))
            {
                mnuFind->addAction(a);
            }
        }
    }

}




