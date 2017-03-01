#include "AboutDialog.h"
#include <QPixmap>
#include <QPalette>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent), m_iconLbl(this), m_QQGrp(this), m_infoEdit(this), m_closeBtn(this)
{
    QPixmap pm(":/Res/pic/dt.png");
    pm = pm.scaled (120, 120, Qt::KeepAspectRatio);

    m_iconLbl.setPixmap (pm);
    m_iconLbl.move (22, 30);
    m_iconLbl.resize (120, 120);

    m_QQGrp.setText ("QQ : 79444528");
    m_QQGrp.move(22, 200);
    m_QQGrp.resize (120, 40);

    QPalette p = m_infoEdit.palette ();
    p.setColor (QPalette::Active, QPalette::Base, palette().color (QPalette::Active, QPalette::Background));
    p.setColor (QPalette::Inactive, QPalette::Base, palette().color (QPalette::Inactive, QPalette::Background));
    m_infoEdit.setPalette (p);

    m_infoEdit.insertPlainText ("NotePad Project\n\nPlatform: Qt 4.7.4\n\nCopyright Danfinswe Solo\n\n");
    m_infoEdit.setReadOnly (true);
    m_infoEdit.setFrameShape (QFrame::NoFrame);
    m_infoEdit.move (240, 30);

    m_closeBtn.setText ("Close");
    m_closeBtn.move (400, 260);
    m_closeBtn.resize (100, 40);

    setFixedSize (600, 340);

    connect (&m_closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}
