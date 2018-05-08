#ifndef MENUCLASSIC_H
#define MENUCLASSIC_H



#include "findbutton.h"
#include "menuprogrammes.h"
#include "menufolders.h"
#include "menusystem.h"
#include "menurecent.h"
//#include "menuform.h"

#include <QWidget>
#include <QMenuBar>
#include <QToolButton>
#include <QFileSystemWatcher>
#include <QLayout>
#include <QWidgetAction>
#include <QLineEdit>
#include <QMenu>
#include <QObject>

class MenuClassic : public QMenu
{
    Q_OBJECT
public:
    MenuClassic(QWidget *parent =0);

public slots:



    void loadSettings();

signals:

private slots:

    void setupMenu();

    void rechargeMenu();

    void execApplication();

    void findText(QString text);

private:

    QMenu *mnuFile;

    MenuProgrammes *menuProgrammes;

    MenuFolders *menuFolders;

    MenuRecent *mMenuRecent;

    MenuSystem *menuPower;



    QMenu *mnuFind;


    FindButton *mFindButton;
   // QLineEdit *findLineEdite;



};

#endif // MENUCLASSIC_H
