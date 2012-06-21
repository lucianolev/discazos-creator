#include <QDebug>
#include <QFileDialog>

#include "discform.h"
#include "ui_discform.h"

DiscForm::DiscForm(Disc *disc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiscForm)
{
    ui->setupUi(this);

    m_tracks = disc->tracks();
    ui->filesTable->setModel(m_tracks);
    connect(ui->addFilesButton, SIGNAL(clicked()), this, SLOT(addMP3Files()));
}

DiscForm::~DiscForm()
{
    delete ui;
}

void DiscForm::addMP3Files() {
   QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Add files"),
                                            QString::null, tr("MP3 Files (*.mp3)"));

   m_tracks->addTracks(fileNames);

   ui->filesTable->resizeColumnsToContents();
}
