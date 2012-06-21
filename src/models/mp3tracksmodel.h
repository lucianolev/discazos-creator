#ifndef MP3FILESMODEL_H
#define MP3FILESMODEL_H

#include <QAbstractTableModel>

#include "mp3track.h"

class MP3TracksModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MP3TracksModel(QObject *parent = 0);
    ~MP3TracksModel();

    void addTracks(const QStringList filenames);

    const QList< MP3Track* >& tracksList();

signals:

public slots:

private:
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    const QString m_fileStatusText(MP3Track* track) const;

    QList< MP3Track* > m_files;
};

#endif // MP3FILESMODEL_H
