#include <QFileDialog>
#include <QMessageBox>

#include "savediscazopage.h"
#include "ui_savediscazopage.h"
#include "albuminfopage.h"

SaveDiscazoPage::SaveDiscazoPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SaveDiscazoPage)
{
    ui->setupUi(this);

    connect(ui->chooseDirectoryButton, SIGNAL(clicked()), this, SLOT(chooseDirectory()));
    connect(ui->saveDiscazoButton, SIGNAL(clicked()), this, SLOT(saveDiscazo()));
}

SaveDiscazoPage::~SaveDiscazoPage()
{
    delete ui;
}

void SaveDiscazoPage::chooseDirectory() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QString::null,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->saveDirectoryLineEdit->setText(dir);
}

void SaveDiscazoPage::saveDiscazo() {
    QString dirPath = ui->saveDirectoryLineEdit->text();
    QDir dirToSave = QDir(dirPath);
    AlbumInfoPage* albumInfoPage = (AlbumInfoPage*)wizard()->page(1);
    QString albumTitle = albumInfoPage->albumInfo()->getTitle();
    dirToSave.mkdir(albumTitle);
    dirToSave.cd(albumTitle);

    //Save AlbumInfo in XML file
    albumInfoPage->albumInfo()->exportInfo(dirToSave.absolutePath());
    //Save audio data to file
    albumInfoPage->albumInfo()->exportAudio(dirToSave.absolutePath());

    QMessageBox msgBox;
    msgBox.setText(tr("The discazo has been saved succesfully"));
    msgBox.exec();
}
