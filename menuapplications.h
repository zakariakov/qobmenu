#ifndef MENUAPPLICATIONS_H
#define MENUAPPLICATIONS_H


#include "menuclassic.h"

//#include "menuform.h"

#include <QObject>
#include <QFileSystemWatcher>

class MenuApplications : public QObject
{
    Q_OBJECT

public:
    explicit MenuApplications(QObject *parent = 0);


public slots:

    void showMenu();

    void loadSettings();

signals:

private slots:
void showForm();
    void afterMenuActivated();




private:



//MenuForm *mMenuForm;
MenuClassic *mMenuClassic;
int menuStyle;
int menuPos;
int mX;
int mY;
   // QLineEdit *findLineEdite;


QFileSystemWatcher *mFileSystemWatcher;
};



#endif // MENUAPPLICATIONS_H
