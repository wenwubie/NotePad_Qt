#include "AppConfig.h"
#include <QFile>
#include <QDataStream>
#include <QApplication>
AppConfig::AppConfig(QObject *parent) : QObject(parent)
{
    m_isValid = Restore ();
}

AppConfig::AppConfig(QFont font, QPoint point, QSize size, bool istbVisible, bool issbVisible, bool isAutoWrap, QObject *parent)
{
    m_mainEditFont = font;
    m_windowPoint = point;
    m_windowSize = size;
    m_isToolBarVisible =istbVisible;
    m_isStatusBarVisible = issbVisible;
    m_isAutoWrap =isAutoWrap;
    m_isValid = true;
}

bool AppConfig::Restore()
{
    bool ret = true;
    QFile file(QApplication::applicationDirPath () + "/app.config");

    if( file.open (QIODevice::ReadOnly) )
    {
        QDataStream in(&file);

        in.setVersion (QDataStream::Qt_4_7);

        in >> m_mainEditFont;
        in >> m_windowPoint;
        in >> m_windowSize;
        in >> m_isToolBarVisible;
        in >> m_isStatusBarVisible;
        in >> m_isAutoWrap;

        file.close ();
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool AppConfig::Store()
{
    bool ret = true;
    QFile file(QApplication::applicationDirPath () + "/app.config");

    if( file.open (QIODevice::WriteOnly) )
    {
        QDataStream out(&file);

        out.setVersion (QDataStream::Qt_4_7);

        out << m_mainEditFont;
        out << m_windowPoint;
        out << m_windowSize;
        out << m_isToolBarVisible;
        out << m_isStatusBarVisible;
        out << m_isAutoWrap;

        file.close ();
    }
    else
    {
        ret = false;
    }

    return ret;
}

QFont AppConfig::mainEditFont()
{
   return m_mainEditFont;
}

QPoint AppConfig::windowPoint()
{
    return m_windowPoint;
}

QSize AppConfig::windowSize()
{
    return m_windowSize;
}

bool AppConfig::isToolBarVisible()
{
   return m_isToolBarVisible;
}

bool AppConfig::isStatusBarVisible()
{
    return m_isStatusBarVisible;
}

bool AppConfig::isAutoWrap()
{
    return m_isAutoWrap;
}

bool AppConfig::isValid()
{
    return m_isValid;
}
