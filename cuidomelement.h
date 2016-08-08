#ifndef CUIDOMELEMENT_H
#define CUIDOMELEMENT_H

#include <QDomElement>
#include <QRect>
#include <QColor>

class CUiDomElement : public QDomElement
{
public:
    CUiDomElement();
    CUiDomElement( const QDomElement & x );

    CUiDomElement createUiWidget(const QString & classname, const QString & name);
    CUiDomElement createUiItem();
    CUiDomElement getRootWidget();
    static QString getRandName(const QString aPrefix, const QString aSuffix);


    int setUiDomKeyValuePair(const QString &key, const QString &value);

    int setUiAttributeTitle(const QString &value);
    int setUiPropertyGeometry(const QString &x, const QString &y, const QString &w, const QString &h);
    int setUiPropertyGeometry(const QRect &aGeo);
    int setUiPropertyFrameShape(const QString &value);
    int setUiPropertyCurrentIndex(const QString &value);
    int setUiPropertyText(const QString &value);
    int setUiPropertyTitle(const QString &value);
    int setUiPropertyFrameShadow(const QString &value);
    int setUiPropertyAlignment(const QString &value);
    int setUiPropertyLayoutDirection(const QString &value);
    int setUiPropertyEnabled(const QString &value);
    int setUiPropertyChecked(const QString &value);
    int setUiPropertyCheckable(const QString &value);
    int setUiPropertyCheckState(const QString &value);
    int setUiPropertyMinimum(const QString &value);
    int setUiPropertyMaximum(const QString &value);
    int setUiPropertySingleStep(const QString &value);
    int setUiPropertyValue(const QString &value);
    int setUiPropertyOrientation(const QString &value);
    int setUiPropertyTextVisible(const QString &value);
    int setUiPropertyTabPosition(const QString &value);
    int setUiPropertyFlat(const QString &value);
    int setUiPropertyToolTip(const QString &value);
    int setUiPropertyPlainText(const QString &text);
    int setUiPropertyPalette(const QString &aRole, const QColor &aColor);
    int setUiPropertyTickPosition(const QString &aPos);
    int setUiPropertyWindowTitle(const QString &aTitle);
    int addUiItemList(const QStringList &aListItems, const QString &aCheckState);
    int addUiItemList(const QStringList &aListItems);
    int addUiAddAction(const QString &aName);
    int addUiGlobalAction(const QString &aName, const QString &aText);
    int addUiGlobalAction(const QString &aName, const QString &aText, const QString &aToolTip);

    /** @todo add xml element for "resources" and "connections" */
};

#endif // CUIDOMELEMENT_H
