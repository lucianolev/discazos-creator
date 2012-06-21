#include <QStringList>

#include "mp3tracksmodel.h"

MP3TracksModel::MP3TracksModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

MP3TracksModel::~MP3TracksModel()
{
    for (int i = 0; i < m_files.size(); ++i) {
        delete m_files.at(i);
    }
}

int MP3TracksModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_files.size();
}

int MP3TracksModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant MP3TracksModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole) {
        QString number;
        switch (index.column()) {
            case 0:
                return number.setNum(m_files.at(index.row())->number());
            case 1:
                return m_files.at(index.row())->artist();
            case 2:
                return m_files.at(index.row())->title();
            case 3:
                return number.setNum(m_files.at(index.row())->length()) + QString(" secs");
            case 4:
                return number.setNum(m_files.at(index.row())->bitrate()) + QString(" kbps");
            case 5:
                return m_fileStatusText(m_files.at(index.row()));
        }
    }

    return QVariant();
}

QVariant MP3TracksModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("");
            case 1:
                return tr("Artist");
            case 2:
                return tr("Title");
            case 3:
                return tr("Length");
            case 4:
                return tr("Bitrate");
            case 5:
                return tr("Status");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

void MP3TracksModel::addTracks(const QStringList files) {
    int currentNumRows = m_files.size();
    int lastNumRowAfterInsertion = currentNumRows + files.size() - 1;
    beginInsertRows(QModelIndex(), currentNumRows, lastNumRowAfterInsertion);
    for (int i = 0; i < files.size(); ++i) {
        MP3Track *mp3track = new MP3Track(files.at(i));
        m_files.append(mp3track);
    }
    endInsertRows();
}

const QList< MP3Track* >& MP3TracksModel::tracksList() {
    return m_files;
}

const QString MP3TracksModel::m_fileStatusText(MP3Track* track) const {
    QString statusText;

    if(track->notAnMP3()) {
        statusText = tr("Error: Not an MP3 File!");
    } else if(track->containErrors()) {
        statusText = tr("Error: The track contain errors!");
    }

    if(!track->containErrors()) {
        if(track->notAcceptableQuality()) {
            statusText = tr("Error: Audio quality is too low.");
        } else {
            statusText = tr("File OK.");
            if(track->bitrate() > 128) {
                statusText += " ";
                statusText += tr("Needs transconding to CBR 128 kbps.");
            }
        }
    }

    return statusText;
}
