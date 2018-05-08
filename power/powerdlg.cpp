#include "powerdlg.h"
//#include "Action.h"
//#include "ui_powerdlg.h"
#include <QTimer>
#include <QIcon>
#include <QDebug>
#include "utils/edir.h"
#include <QSettings>
#include <QTextCodec>
PowerDlg::PowerDlg(const QString &txt, QIcon icon, QWidget *parent) :
    QDialog(parent)/*,
    ui(new Ui::PowerDlg)*/
{

   // setupUi(this);
    setupMe();
  //  m_action = txt;
    quitIcon->setPixmap(icon.pixmap(QSize(128, 128)));
    m_ActName=txt;
loadService();
//    question->setText(QString(tr("Are you sure you want to quit all applications and %1 now ?")).arg(m_ActName));
//    reminder->setText(QString(tr("If you do nothing, the system will %1 automatically in 30 seconds.")).arg(m_ActName));
qDebug()<<m_ActName;
setWindowTitle(m_ActName);
    m_Counter = 30;
    m_Timer = new QTimer(this);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(countDown()));
    m_Timer->start(1000);
}
void PowerDlg::loadService()
{
    QDir serviceDir(Edir::dataDir()+"/elokab-syspower");
//    serviceDir.cdUp();
//    serviceDir.cd( serviceDir.absolutePath()+"/share/elokab/elokab-syspower") ;
    QString path=  serviceDir.absoluteFilePath("Message.desktop");

    QString mQuestion="Are you sure you want to quit all applications and %1 now ?";
    message="If you do nothing, the system will %1 automatically in %2 seconds.";

    QString lc=locale().name().section("_",0,0);

    QSettings gs(path,QSettings::IniFormat);
    gs.setIniCodec(QTextCodec::codecForName("UTF-8"));
    gs.beginGroup("Desktop Entry");

    QString QuestionOld=gs.value("Question",mQuestion).toString();
    mQuestion=gs.value("Question["+lc+"]",QuestionOld).toString();

    QString messageOld=gs.value("Reminder",message).toString();
    message=gs.value("Reminder["+lc+"]",messageOld).toString();

    gs.endGroup();

    question->setText(QString(mQuestion).arg(m_ActName));
    reminder->setText(QString(message).arg(m_ActName).arg("30"));

}

PowerDlg::~PowerDlg()
{
    m_Timer->stop();
    delete m_Timer;
    //delete ui;

}

void PowerDlg::countDown()
{


    m_Counter -= 1;


    reminder->setText(QString(message).arg(m_ActName).arg(m_Counter));


    if (m_Counter == 0) accept();
}


void PowerDlg::setupMe()
{
    this->resize(395, 130);
           gridLayout = new QGridLayout(this);
           gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
           buttonBox = new QDialogButtonBox(this);
           buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
           buttonBox->setOrientation(Qt::Horizontal);
           buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

           gridLayout->addWidget(buttonBox, 3, 1, 1, 3);

           quitIcon = new QLabel(this);
           quitIcon->setObjectName(QString::fromUtf8("quitIcon"));
           QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
           sizePolicy.setHorizontalStretch(0);
           sizePolicy.setVerticalStretch(0);
           sizePolicy.setHeightForWidth(quitIcon->sizePolicy().hasHeightForWidth());
           quitIcon->setSizePolicy(sizePolicy);
           quitIcon->setMaximumSize(QSize(64, 64));
           QFont font;
           font.setKerning(false);
           quitIcon->setFont(font);
           quitIcon->setScaledContents(true);

           gridLayout->addWidget(quitIcon, 0, 0, 2, 1);

           verticalSpacer = new QSpacerItem(20, 33, QSizePolicy::Minimum, QSizePolicy::Expanding);

           gridLayout->addItem(verticalSpacer, 2, 2, 1, 1);

           question = new QLabel(this);
           question->setObjectName(QString::fromUtf8("question"));
           QFont font1;
           font1.setBold(true);
           font1.setWeight(75);
           question->setFont(font1);
           question->setWordWrap(true);

           gridLayout->addWidget(question, 0, 1, 1, 3);

           reminder = new QLabel(this);
           reminder->setObjectName(QString::fromUtf8("reminder"));
           reminder->setWordWrap(true);

           gridLayout->addWidget(reminder, 1, 1, 1, 3);



           QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
           QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}
