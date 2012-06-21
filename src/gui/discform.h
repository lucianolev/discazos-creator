#ifndef DISCFORM_H
#define DISCFORM_H

#include <QWidget>

#include "../models/disc.h"

namespace Ui {
    class DiscForm;
}

class DiscForm : public QWidget
{
    Q_OBJECT

public:
    explicit DiscForm(Disc *disc = 0, QWidget *parent = 0);
    ~DiscForm();

public slots:
    void addMP3Files();

private:
    Ui::DiscForm *ui;
    MP3TracksModel *m_tracks;

};

#endif // DISCFORM_H
