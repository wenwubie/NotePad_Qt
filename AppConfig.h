#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QFont>
#include <QSize>
#include <QPoint>

class AppConfig : public QObject
{
    Q_OBJECT
    QFont m_mainEditFont;
    bool m_isToolBarVisible;
    bool m_isStatusBarVisible;
    bool m_isAutoWrap;
    bool m_isValid;
    QPoint m_windowPoint;
    QSize m_windowSize;
    bool Restore();
public:
    explicit AppConfig(QObject *parent = 0);
    explicit AppConfig(QFont font, QPoint point, QSize size, bool istbVisible, bool issbVisible, bool isAutoWrap, QObject *parent = 0);
    bool Store();
    QFont mainEditFont();
    QPoint windowPoint();
    QSize windowSize();
    bool isToolBarVisible();
    bool isStatusBarVisible();
    bool isAutoWrap();
    bool isValid();


    
    
};

#endif // APPCONFIG_H
