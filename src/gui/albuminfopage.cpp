#include <QDebug>

#include "albuminfopage.h"
#include "ui_albuminfopage.h"
#include "discform.h"

AlbumInfoPage::AlbumInfoPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::AlbumInfoPage)
{
    ui->setupUi(this);

    m_albumInfo = new AlbumInfo();

    m_currentNumDiscTabs = 1;
    DiscForm *df = new DiscForm(m_albumInfo->getDisc(1), this);
    ui->discsTabWidget->addTab(df, QString("CD 1"));

    connect(ui->numDiscs, SIGNAL(currentIndexChanged(QString)), this, SLOT(setNumDiscTabs(QString)));
    connect(ui->title, SIGNAL(textChanged(QString)), this, SLOT(setTitle(QString)));
}

AlbumInfoPage::~AlbumInfoPage()
{
    delete ui;
    delete m_albumInfo;
}

void AlbumInfoPage::setNumDiscTabs(QString newValue) {
    int newNumTabs = newValue.toInt();
    if (newNumTabs < m_currentNumDiscTabs) {
        int tabsToRemove = m_currentNumDiscTabs - newNumTabs;
        for (int i = 0; i < tabsToRemove; i++) {
            int iTabToRemove = m_currentNumDiscTabs - i - 1;
            //Removes the tab
            ui->discsTabWidget->removeTab(iTabToRemove);
            //Deletes the disc from the model
            m_albumInfo->removeDisc(iTabToRemove);
        }
    } else if (newNumTabs > m_currentNumDiscTabs) {
        int tabsToAdd = newNumTabs - m_currentNumDiscTabs;
        for (int i = 0; i < tabsToAdd; i++) {
            int discNum = m_currentNumDiscTabs + i + 1;
            //Adds a new disc to the model
            m_albumInfo->addNewDisc();
            //Adds the tab
            DiscForm *df = new DiscForm(m_albumInfo->getDisc(discNum), this);
            ui->discsTabWidget->addTab(df, QString("CD %1").arg(discNum));
        }
    }
    m_currentNumDiscTabs = newNumTabs;
}

void AlbumInfoPage::setTitle(QString title) {
    m_albumInfo->setTitle(title);
}

AlbumInfo* AlbumInfoPage::albumInfo() {
    return m_albumInfo;
}
