#include <taglib/tag.h>

#include "mp3track.h"

MP3Track::MP3Track(QString filename, QObject *parent) :
    QObject(parent)
{
    m_tagFile = new TagLib::MPEG::File(filename.toUtf8().constData());
    m_file = new MP3File(filename.toUtf8().constData());
    m_file->analyze();
}

MP3Track::~MP3Track()
{
    delete m_file;
    delete m_tagFile;
}

unsigned int MP3Track::number() {
    TagLib::Tag *tag = m_tagFile->tag();
    if(tag) {
        unsigned int number = tag->track();
        return number;
    } else {
        return 0;
    }
}

bool MP3Track::notAnMP3() {
    return !m_file->isMP3();
}

bool MP3Track::containErrors() {
    return !m_file->isValid();
}

bool MP3Track::notAcceptableQuality() {
    return (m_file->sampleRate() != 44100 || m_file->bitrate() < 128);
}

int MP3Track::bitrate() {
    TagLib::MPEG::Properties *properties = m_tagFile->audioProperties();
    if(properties) {
        return properties->bitrate();
    } else {
        return 0;
    }
}

QString MP3Track::title() {
    TagLib::Tag *tag = m_tagFile->tag();
    if(tag) {
        TagLib::String title = tag->title();
        return QString(title.toCString());
    } else {
        return QString("No Tags");
    }
}

QString MP3Track::artist() {
    TagLib::Tag *tag = m_tagFile->tag();
    if(tag) {
        TagLib::String artist = tag->artist();
        return QString(artist.toCString());
    } else {
        return QString("No Tags");
    }
}

int MP3Track::length() {
    TagLib::MPEG::Properties *properties = m_tagFile->audioProperties();
    if(properties) {
        return properties->length();
    } else {
        return 0;
    }
}

unsigned int MP3Track::lengthInMs() {
    return this->mp3file()->lengthInMs();
}

MP3File* MP3Track::mp3file() {
   return m_file;
}
