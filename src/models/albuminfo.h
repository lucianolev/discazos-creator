#ifndef ALBUMINFO_H
#define ALBUMINFO_H

#include <QObject>

#include "disc.h"

const QString XML_FILE_VERSION = "0.2";

class AlbumInfo : public QObject
{
    Q_OBJECT

public:
    explicit AlbumInfo(QObject *parent = 0);

    bool exportInfo(QString dirPath);
    bool exportAudio(QString dirPath);

signals:

public slots:
    void setTitle(QString title);
    QString getTitle();

    void addNewDisc();
    void removeDisc(int discNum = 0);

    Disc* getDisc(int discNum = 0);

private:
    QString m_randomFilename();

    QString m_albumType;
    QString m_title;
    QString m_artist;
    QString m_recType;
    QList< Disc* > m_discs;
    qint64 m_audiofile_size;

};

#endif // ALBUMINFO_H
