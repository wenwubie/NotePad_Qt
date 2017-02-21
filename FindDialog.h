#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>

class FindDialog : public QDialog
{
    Q_OBJECT

protected:
    // 必须放在hLayout之前申明,因为如果放在hLayout后面
    // 当析构时,先析构了hLayout,再析构grpBox,然后后面还再析构一次hLayout
    // 所导致析构了两次hLayout报错
    QGroupBox radioGrpBox;

    QGridLayout gLayout;
    QHBoxLayout hLayout;

    QLabel m_findLabel;
    QLineEdit m_findEdit;
    QPushButton m_findBtn;
    QPushButton m_closeBtn;
    QCheckBox m_matchBox;
    QRadioButton m_forwardBtn;
    QRadioButton m_backwardBtn;

public:
    explicit FindDialog(QWidget* parent = 0);
    bool event(QEvent* evt);
signals:
    
public slots:
    
};

#endif // FINDDIALOG_H
