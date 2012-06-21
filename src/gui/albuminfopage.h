#ifndef LOADFILESPAGE_H
#define LOADFILESPAGE_H

#include <QWizardPage>

#include "../models/albuminfo.h"

namespace Ui {
    class AlbumInfoPage;
}

class AlbumInfoPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit AlbumInfoPage(QWidget *parent = 0);
    ~AlbumInfoPage();

    AlbumInfo* albumInfo();

public slots:
    void setNumDiscTabs(QString newValue);
    void setTitle(QString title);

private:
    Ui::AlbumInfoPage *ui;
    AlbumInfo *m_albumInfo;
    int m_currentNumDiscTabs;
};

#endif // LOADFILESPAGE_H
