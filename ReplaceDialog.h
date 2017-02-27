#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QPlainTextEdit>
#include "FindDialog.h"

class ReplaceDialog : public FindDialog
{
    Q_OBJECT

    QLabel m_repLbl;
    QLineEdit m_repEdit;
    QPushButton m_repBtn;
    QPushButton m_repAllBtn;

public:
    explicit ReplaceDialog(QWidget *parent = 0, QPlainTextEdit* pText = 0);
    void initControl();
    void connectSlots();

protected slots:
    void onRepBtnClicked();
    void onRepAllBtnClicked();
    
};

#endif // REPLACEDIALOG_H
