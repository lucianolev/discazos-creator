#ifndef DISC_H
#define DISC_H

#include <QObject>

#include "mp3tracksmodel.h"

class Disc : public QObject
{
    Q_OBJECT
public:
    explicit Disc(QObject *parent = 0);
    ~Disc();

    QString name();
    MP3TracksModel* tracks();

signals:

public slots:

private:
    QString m_name;
    MP3TracksModel *m_tracks;

};

#endif // DISC_H
