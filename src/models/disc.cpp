#include "disc.h"
#include "mp3tracksmodel.h"

Disc::Disc(QObject *parent) :
    QObject(parent)
{
    m_tracks = new MP3TracksModel();
}

Disc::~Disc()
{
    delete m_tracks;
}

QString Disc::name() {
    return m_name;
}

MP3TracksModel* Disc::tracks() {
    return m_tracks;
}
