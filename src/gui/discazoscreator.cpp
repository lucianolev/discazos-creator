#include "discazoscreator.h"
#include "intropage.h"
#include "albuminfopage.h"
#include "savediscazopage.h"

DiscazosCreator::DiscazosCreator(QWidget *parent) :
    QWizard(parent)
{
    setPage(Page_Intro, new IntroPage);
    setPage(Page_AlbumInfo, new AlbumInfoPage);
    setPage(Page_SaveDiscazo, new SaveDiscazoPage);

    setWindowTitle(tr("Discazos Creator 0.1"));
    resize(QSize(800, 600));
}
