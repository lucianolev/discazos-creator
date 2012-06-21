#ifndef SAVEDISCAZOPAGE_H
#define SAVEDISCAZOPAGE_H

#include <QWizardPage>

namespace Ui {
    class SaveDiscazoPage;
}

class SaveDiscazoPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit SaveDiscazoPage(QWidget *parent = 0);
    ~SaveDiscazoPage();

public slots:
    void chooseDirectory();
    void saveDiscazo();

private:
    Ui::SaveDiscazoPage *ui;

};

#endif // SAVEDISCAZOPAGE_H
