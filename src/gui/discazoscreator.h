#ifndef DISCAZOSCREATOR_H
#define DISCAZOSCREATOR_H

#include <QWizard>

class DiscazosCreator : public QWizard
{
    Q_OBJECT
public:
    enum { Page_Intro, Page_AlbumInfo, Page_SaveDiscazo };

    explicit DiscazosCreator(QWidget *parent = 0);

signals:

public slots:

};

#endif // DISCAZOSCREATOR_H
