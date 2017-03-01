#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>

class AboutDialog : public QDialog
{
    Q_OBJECT

    QLabel m_iconLbl;
    QLabel m_QQGrp;
    QPlainTextEdit m_infoEdit;
    QPushButton m_closeBtn;

public:
    explicit AboutDialog(QWidget *parent = 0);
    
    
};

#endif // ABOUTDIALOG_H
