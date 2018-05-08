#ifndef FINDBUTTON_H
#define FINDBUTTON_H

#include <QObject>
#include <QWidgetAction>
#include <QAction>
#include <QLineEdit>
class FindButton : public QObject
{
      Q_OBJECT
public:
  explicit  FindButton();
  QAction *actionfind(){return  actionWidget;}

signals:
  void findTextChanged(QString);
private:
     QLineEdit *findLineEdite;
     QWidgetAction  *actionWidget;
     QStringList m_wordList;

private slots:
     void chargeCompliterDirs(QString dir);
     void chargeCompliterFiles(QString dir);
     void execFindApplication();
     void setFocus();
};

#endif // FINDBUTTON_H
