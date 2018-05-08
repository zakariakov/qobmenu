#ifndef POWERDLG_H
#define POWERDLG_H

#include <QDialog>
#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSpacerItem>


//namespace Ui {
//class PowerDlg;
//}
/**
 * @brief The PowerDlg class نافذة تخبر برسالة لوقف الجهاز مع مؤقت زمني
 */
class PowerDlg : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief PowerDlg نافذة الرسالة
     * @param act الامر
     * @param parent الاب
     */
    explicit PowerDlg(const QString &txt, QIcon icon, QWidget *parent = 0);
    ~PowerDlg();




public slots:
    /*!< عداد تنازلي لاجراء الامر بالغلق او الخروج.... */
    void countDown();

private slots:
    /*!< تكوين الادوات والايقونات */
    void setupMe();
    void loadService();
private:
    //    Ui::PowerDlg *ui;
    int m_Counter;
    int m_action;
    QString m_ActName;
    QTimer *m_Timer;
    //--Gui---
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QLabel *quitIcon;
    QSpacerItem *verticalSpacer;
    QLabel *question;
    QLabel *reminder;
    QString message;
};

#endif // POWERDLG_H
