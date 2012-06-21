#ifndef MP3TRACK_H
#define MP3TRACK_H

#include <QObject>

#include <taglib/mpegfile.h>

#include "../libmp3basics/mp3_file.h"

class MP3Track : public QObject
{
    Q_OBJECT
public:
    explicit MP3Track(QString filename, QObject *parent = 0);
    ~MP3Track();

    unsigned int number();
    bool notAnMP3();
    bool containErrors();
    bool notAcceptableQuality();
    int bitrate();

    int length();
    QString artist();
    QString title();

    MP3File* mp3file();

signals:

public slots:

private:
    MP3File *m_file;
    TagLib::MPEG::File *m_tagFile;

};

#endif // MP3TRACK_H
