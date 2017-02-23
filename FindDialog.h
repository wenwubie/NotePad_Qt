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
#include <QPlainTextEdit>
#include <QPointer>

class FindDialog : public QDialog
{
    Q_OBJECT

protected:
    // �������hLayout֮ǰ����,��Ϊ�������hLayout����
    // ������ʱ,��������hLayout,������grpBox,Ȼ����滹������һ��hLayout
    // ����������������hLayout����
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

    QPointer<QPlainTextEdit> m_pTextEdit;

    void initControl();
    void connectSlots();

protected slots:
    void onFindBtnClicked();
    void onCloseBtnClicked();

public:
    explicit FindDialog(QWidget* parent = 0, QPlainTextEdit* pTextEdit = 0);

    void setTextEdit(QPlainTextEdit* textEdit);
    QPlainTextEdit* getTextEdit(void);

    bool event(QEvent* evt);

signals:
    
public slots:
    
};

#endif // FINDDIALOG_H
