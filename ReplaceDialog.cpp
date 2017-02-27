#include "ReplaceDialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent, QPlainTextEdit* pText) :
    FindDialog(parent, pText)
{
    initControl ();
    connectSlots ();
}


void ReplaceDialog::initControl()
{
    setWindowTitle ("Replace");

    m_repLbl.setText ("Replace to: ");
    m_repBtn.setText ("Replace");
    m_repAllBtn.setText ("Replace All");

    gLayout.removeWidget (&m_matchBox);
    gLayout.removeWidget (&radioGrpBox);
    gLayout.removeWidget (&m_closeBtn);

    gLayout.addWidget (&m_repLbl, 1, 0);
    gLayout.addWidget (&m_repEdit, 1, 1);
    gLayout.addWidget (&m_repBtn, 1, 2);
    gLayout.addWidget (&m_repAllBtn, 2, 2);
    gLayout.addWidget (&m_matchBox, 3, 0);
    gLayout.addWidget (&radioGrpBox, 3, 1);
    gLayout.addWidget (&m_closeBtn, 3, 2);

}

void ReplaceDialog::connectSlots()
{
    connect(&m_repBtn, SIGNAL(clicked()), this, SLOT(onRepBtnClicked()));
    connect (&m_repAllBtn, SIGNAL(clicked()), this, SLOT(onRepAllBtnClicked()));
}

void ReplaceDialog::onRepBtnClicked()
{
    QString target = m_findEdit.text ();
    QString to = m_repEdit.text ();

    if( m_pTextEdit != NULL && target != "" && to != "" )
    {
        QString selText = m_pTextEdit->textCursor ().selectedText ();

        if( selText == target )
        {
            m_pTextEdit->insertPlainText (to);
        }
        onFindBtnClicked ();
    }
}

void ReplaceDialog::onRepAllBtnClicked()
{
    QString target = m_findEdit.text ();
    QString to = m_repEdit.text ();

    if(m_pTextEdit != NULL && target != "" && to != "" )
    {
        QString text = m_pTextEdit->toPlainText ();
        text.replace (target, to, m_matchBox.isChecked () ? Qt::CaseSensitive : Qt::CaseInsensitive);
        m_pTextEdit->setPlainText (text);
    }
}
