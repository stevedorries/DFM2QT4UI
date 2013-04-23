#ifndef CGUITREEDOMELEMENT_H
#define CGUITREEDOMELEMENT_H

#include <QDomElement>
#include <QRect>
#include <QStringList>
#include <QColor>

class CGuiTreeDomElement : public QDomElement
{
public:
    CGuiTreeDomElement();
    CGuiTreeDomElement(const QDomElement & x);
    CGuiTreeDomElement parentNode() const;

    // input
    int setDomProperty(const QString key, const QString value);

    // output
    QString getDomProperty(const QString key);
    int getDomPropertySecure(QString &value, const QString &key);
    QString getDomProperty(const QString key, const QString defaultValue);
    int getNumberOfGuiObject(const QString &name);
    QRect getPropertyGeometry();
    QStringList getTabStrings();
    QString getItemsOf(const QString &aName);
    int getColor(QColor &aColor, const QString &aName);
    QStringList getItemListOf(const QString &aName);

    // process
    QString removeQuotes(QString text);
    QString substituteAsciiSpecialChars(QString text);
    QString translateToNonQuotedUnicode(QString text);
    bool hasGuiObjectChildNodes();
    CGuiTreeDomElement getRootGuiObject();
};

#endif // CGUITREEDOMELEMENT_H
