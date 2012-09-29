#include <QDebug>
#include <QFile>
#include <QDir>
#include <QXmlStreamWriter>

#include "../libmp3basics/mp3_file_reader.h"

#include "albuminfo.h"
#include "disc.h"

AlbumInfo::AlbumInfo(QObject *parent) :
    QObject(parent)
{
    m_albumType = "Artist Album";
    m_recType = "Studio";
    m_audiofile_size = 0;
    addNewDisc();
}

void AlbumInfo::setTitle(QString title) {
    m_title = title;
}

QString AlbumInfo::getTitle() {
    return m_title;
}

void AlbumInfo::addNewDisc() {
    Disc *cd = new Disc();
    m_discs.append(cd);
}

void AlbumInfo::removeDisc(int discNum) {
    m_discs.removeAt(discNum);
}

Disc* AlbumInfo::getDisc(int discNum) {
    qDebug() << m_discs.size();
    if(discNum == 0 || discNum > m_discs.size()) {
        return m_discs.last();
    } else {
        return m_discs.at(discNum-1);
    }
}

bool AlbumInfo::exportInfo(QString dirPath) {
    QXmlStreamWriter xml;
    xml.setAutoFormatting(true);

    QString fileName = dirPath + "/" + "info.xml";
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }
    xml.setDevice(&file);

    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE discazos>");

    xml.writeStartElement("discazos-disc-creator");
    xml.writeAttribute("version", XML_FILE_VERSION);
    xml.writeEmptyElement("album");
    xml.writeAttribute("title", m_title);

    QString number;
    for (int i = 0; i < m_discs.size(); ++i) {
        Disc* currentDisc = m_discs.at(i);
        xml.writeStartElement("disc");
        xml.writeAttribute("number", number.setNum(i+1));
        xml.writeAttribute("title", currentDisc->name());
        const QList< MP3Track* >& currentTrackList = currentDisc->tracks()->tracksList();
        for (int j = 0; j < currentTrackList.size(); ++j) {
            MP3Track* currentTrack = currentTrackList.at(j);
            xml.writeStartElement("track");
            xml.writeAttribute("number", number.setNum(j+1));
            xml.writeAttribute("length", number.setNum(currentTrack->lengthInMs()));
            xml.writeEmptyElement("artist");
            xml.writeAttribute("name", currentTrack->artist());
            xml.writeEmptyElement("song");
            xml.writeAttribute("name", currentTrack->title());
            xml.writeEndElement();
        }
        xml.writeEndElement();
    }

    xml.writeEmptyElement("audiofile");
    xml.writeAttribute("size", QString::number(m_audiofile_size));

    xml.writeEndDocument();
    return true;
}

QString AlbumInfo::m_randomFilename() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static const int max_len = 20;
    static const int min_len = 5;

    QString s;
    int len = (rand() % (max_len-min_len)) + min_len;
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return s;
}

bool AlbumInfo::exportAudio(QString dirPath) {
    QString filename = dirPath + "/" + m_randomFilename() + ".mp3";
    QFile mp3out(filename);
    bool fileOpened = mp3out.open(QIODevice::WriteOnly);
    if(!fileOpened) {
        qDebug() << "Cannot open file for writing!";
        return false;
    }

    QDataStream mp3stream(&mp3out);
    for (int i = 0; i < m_discs.size(); ++i) {
        Disc* currentDisc = m_discs.at(i);
        const QList< MP3Track* >& currentTrackList = currentDisc->tracks()->tracksList();
        qDebug() << "Merging disc " << i << "...";
        for (int j = 0; j < currentTrackList.size(); ++j) {
            qDebug() << "--Merging track " << j << "...";
            MP3Track* currentTrack = currentTrackList.at(j);
            MP3FileReader reader(currentTrack->mp3file());
            while(!reader.eof()) {
              char* frameData = reader.readNextFrame();
              mp3stream.writeRawData(frameData, reader.lastReadCount());
            }
        }
    }

    mp3out.close();
    m_audiofile_size = mp3out.size();

    return true;
}
