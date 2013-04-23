#include <QDebug>
#include "cuidomelement.h"
#include "cuidomdocument.h"

CUiDomElement::CUiDomElement()
{
}


CUiDomElement::CUiDomElement( const QDomElement & x ) :QDomElement(x)
{
}

CUiDomElement CUiDomElement::getRootWidget()
{
    CUiDomElement e;

    e = this->ownerDocument().firstChildElement("ui");
    if(e.isNull())
    {
        //qDebug() << "no 'ui' element found";
        return(e);
    }
    e = e.firstChildElement("widget");
    if(e.isNull())
    {
        //qDebug() << "no 'widget' element found";
        return(e);
    }

    return(e);
}



/**
 * Create a QDomElement instance as ui widget and append it to givin uiElm.
 *
 * @param uiElm ui element
 * @param classname Name of class.
 * @param name Name of instance of class.
 * @return On Success isNull() returns false.
 **/
CUiDomElement CUiDomElement::createUiWidget(const QString & classname, const QString & name)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement tmpElm;

    // create element node
    tmpElm = domDoc.createElement("widget");
    if(tmpElm.isNull())
        return(tmpElm);

    // create attributes
    tmpElm.setAttribute("class", classname);
    tmpElm.setAttribute("name", name);

    // append DOM element to uiElm
    this->appendChild(tmpElm);

    return(tmpElm);
}

/**
 * Create a QDomElement instance as ui widget and append it to givin uiElm.
 *
 * @param uiElm ui element
 * @param classname Name of class.
 * @param name Name of instance of class.
 * @return On Success isNull() returns false.
 **/
CUiDomElement CUiDomElement::createUiItem()
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement tmpElm;

    // create element node
    tmpElm = domDoc.createElement("item");
    if(tmpElm.isNull())
        return(tmpElm);

    // append DOM element to uiElm
    this->appendChild(tmpElm);

    return(tmpElm);
}


/**
 * Create an DOM element with text content and add  this to a givin element.
 *
 * @param key Name of new DOM element.
 * @param value The text content of new DOM element
 * @return 0 on Success else -1.
 **/
int CUiDomElement::setUiDomKeyValuePair(const QString &key, const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement tmpDomElm = domDoc.createElement(key);
    if(tmpDomElm.isNull())
        return(-1);

    QDomText tmpDomTxt = domDoc.createTextNode(value);
    if(tmpDomTxt.isNull())
    {
        qDebug() << "setUiDomKeyValuePair(): Failed to create TextNode: value: " << value << "";
    }
    else
        tmpDomElm.appendChild(tmpDomTxt);
    this->appendChild(tmpDomElm);
    return(0);
}


/**
 * Create an DOM element with text content and add  this to a givin element.
 *
 * @param name Name of new DOM element.
 * @param value The text content of new DOM element
 * @return 0 on Success else -1.
 *
 *
 * @example
 *   <attribute name="title">
 *    <string>name</string>
 *   </attribute>
 **/
int CUiDomElement::setUiAttributeTitle(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement tmpDomElm = domDoc.createElement("attribute");
    if(tmpDomElm.isNull())
        return(-1);
    tmpDomElm.setAttribute("name", "title");

    if(0 != tmpDomElm.setUiDomKeyValuePair("string", value))
    {
        tmpDomElm.clear();
        return(-1);
    }

    this->appendChild(tmpDomElm);
    return(0);
}


/**
 * Set Property (Geometry) at ui by guitree or default values (x=0, y=0, w=10, h=10).
 *
 * @param x x-value.
 * @param y y-value
 * @param w width
 * @param h height
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyGeometry(const QRect &aGeo)
{
    return(setUiPropertyGeometry(QString::number(aGeo.x()), QString::number(aGeo.y()), QString::number(aGeo.width()), QString::number(aGeo.height())));
}


/**
 * Set Property (Geometry) at ui by guitree or default values (x=0, y=0, w=10, h=10).
 *
 * @param x x-value.
 * @param y y-value
 * @param w width
 * @param h height
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyGeometry(const QString &x, const QString &y, const QString &w, const QString &h)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;
    CUiDomElement rectElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "geometry");

    // create element node
    rectElm = domDoc.createElement("rect");
    if(rectElm.isNull())
    {
        propertyElm.clear();
        return(-1);
    }

    // set values
    if(0 != rectElm.setUiDomKeyValuePair("x", x) ||
       0 != rectElm.setUiDomKeyValuePair("y", y) ||
       0 != rectElm.setUiDomKeyValuePair("width", w) ||
       0 != rectElm.setUiDomKeyValuePair("height", h) )
    {
        propertyElm.clear();
        rectElm.clear();
        return(-1);
    }

    // append rect to property
    propertyElm.appendChild(rectElm);
    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyFrameShape(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "frameShape");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("enum", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyFrameShadow(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "frameShadow");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("enum", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyAlignment(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "alignment");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("enum", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyLayoutDirection(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "layoutDirection");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("enum", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}



/**
 * Set property "currentIndex" at ui DOM-tree.
 * The number represent the selected tabsheet and begins
 * with 0.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyCurrentIndex(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "currentIndex");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("number", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyText(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "text");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("string", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyTitle(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "title");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("string", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyEnabled(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "enabled");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("bool", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyChecked(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "checked");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("bool", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}



/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyCheckable(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "checkable");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("bool", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property "checkState" at ui by guitree.
 *
 * possible enums (values) are:
 *      "Checked"
 *      "Unchecked"
 *      "PartiallyChecked"
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyCheckState(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "checkState");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("enum", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyMinimum(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "minimum");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("number", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyMaximum(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "maximum");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("number", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertySingleStep(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "singleStep");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("number", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyValue(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "value");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("number", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyOrientation(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "orientation");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("enum", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyTextVisible(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "textVisible");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("bool", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyTabPosition(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "tabPosition");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("enum", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyFlat(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "flat");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("bool", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyToolTip(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "toolTip");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("string", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *

 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyPlainText(const QString &value)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "plainText");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("string", value) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 * Currently this method overwrite all existing palettes, this means thas you can only use one role.
 *
 * @param value Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyPalette(const QString &aRole, const QColor &aColor)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;
    CUiDomElement palette;
    CUiDomElement active;
    CUiDomElement inactive;
    CUiDomElement disabled;
    CUiDomElement activeColorrole;
    CUiDomElement inactiveColorrole;
    CUiDomElement disabledColorrole;
    CUiDomElement activeBrush;
    CUiDomElement inactiveBrush;
    CUiDomElement disabledBrush;
    CUiDomElement activeColor;
    CUiDomElement inactiveColor;
    CUiDomElement disabledColor;


    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "palette");

    // create palette
    palette = domDoc.createElement("palette");
    if(palette.isNull())
    {
        propertyElm.clear();
        return(-1);
    }
    propertyElm.appendChild(palette);


    // create active, inactive and diasabled
    active = domDoc.createElement("active");
    if(active.isNull())
    {
        propertyElm.clear(); // all children will also deleted
        return(-1);
    }
    palette.appendChild(active);

    inactive = domDoc.createElement("inactive");
    if(inactive.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    palette.appendChild(inactive);

    disabled = domDoc.createElement("disabled");
    if(disabled.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    palette.appendChild(disabled);


    // create colorroles
    activeColorrole = domDoc.createElement("colorrole");
    if(activeColorrole.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    activeColorrole.setAttribute("role", aRole);
    active.appendChild(activeColorrole);

    inactiveColorrole = domDoc.createElement("colorrole");
    if(inactiveColorrole.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    inactiveColorrole.setAttribute("role", aRole);
    inactive.appendChild(inactiveColorrole);

    disabledColorrole = domDoc.createElement("colorrole");
    if(disabledColorrole.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    disabledColorrole.setAttribute("role", aRole);
    disabled.appendChild(disabledColorrole);

    // create brushes
    activeBrush = domDoc.createElement("brush");
    if(activeBrush.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    activeBrush.setAttribute("brushstyle", "SolidPattern");
    activeColorrole.appendChild(activeBrush);

    inactiveBrush = domDoc.createElement("brush");
    if(inactiveBrush.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    inactiveBrush.setAttribute("brushstyle", "SolidPattern");
    inactiveColorrole.appendChild(inactiveBrush);

    disabledBrush = domDoc.createElement("brush");
    if(disabledBrush.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    disabledBrush.setAttribute("brushstyle", "SolidPattern");
    disabledColorrole.appendChild(disabledBrush);

    // create colors
    activeColor = domDoc.createElement("color");
    if(activeColor.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    activeColor.setAttribute("alpha", "255");
    activeBrush.appendChild(activeColor);

    inactiveColor = domDoc.createElement("color");
    if(inactiveColor.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    inactiveColor.setAttribute("alpha", "255");
    inactiveBrush.appendChild(inactiveColor);

    disabledColor = domDoc.createElement("color");
    if(disabledColor.isNull())
    {
        propertyElm.clear();  // all children will also deleted
        return(-1);
    }
    disabledColor.setAttribute("alpha", "255");
    disabledBrush.appendChild(disabledColor);


    // set values
    if(0 != activeColor.setUiDomKeyValuePair("red", QString::number(aColor.red())))
    {
        propertyElm.clear();
        return(-1);
    }

    if(0 != inactiveColor.setUiDomKeyValuePair("red", QString::number(aColor.red())))
    {
        propertyElm.clear();
        return(-1);
    }

    if(0 != disabledColor.setUiDomKeyValuePair("red", QString::number(aColor.red())))
    {
        propertyElm.clear();
        return(-1);
    }

    if(0 != activeColor.setUiDomKeyValuePair("green", QString::number(aColor.green())))
    {
        propertyElm.clear();
        return(-1);
    }

    if(0 != inactiveColor.setUiDomKeyValuePair("green", QString::number(aColor.green())))
    {
        propertyElm.clear();
        return(-1);
    }

    if(0 != disabledColor.setUiDomKeyValuePair("green", QString::number(aColor.green())))
    {
        propertyElm.clear();
        return(-1);
    }

    if(0 != activeColor.setUiDomKeyValuePair("blue", QString::number(aColor.blue())))
    {
        propertyElm.clear();
        return(-1);
    }

    if(0 != inactiveColor.setUiDomKeyValuePair("blue", QString::number(aColor.blue())))
    {
        propertyElm.clear();
        return(-1);
    }

    if(0 != disabledColor.setUiDomKeyValuePair("blue", QString::number(aColor.blue())))
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * posible values are:
 *  "QSlider::TicksBothSides"
 *  "QSlider::TicksBelow"       // this is also right
 *  "QSlider::TicksAbove"       // this is also left
 *
 * @param aPos Ui value, this is an enum value.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyTickPosition(const QString &aPos)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "tickPosition");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("enum", aPos) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Set Property at ui by guitree.
 *
 * @param aTitle Window title
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::setUiPropertyWindowTitle(const QString &aTitle)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement propertyElm;

    // create element node
    propertyElm = domDoc.createElement("property");
    if(propertyElm.isNull())
        return(-1);

    // create attributes
    propertyElm.setAttribute("name", "windowTitle");

    // set values
    if(0 != propertyElm.setUiDomKeyValuePair("string", aTitle) )
    {
        propertyElm.clear();
        return(-1);
    }

    // append property to givin uiElm
    this->appendChild(propertyElm);

    return(0);
}


/**
 * Add items to this node.
 *
 * @param aListItems Givin items.
 * @param aCheckState CheckState of all items, for possible values see setUiPropertyCheckState(). If this is NULL, than the property vanish.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::addUiItemList(const QStringList &aListItems, const QString &aCheckState)
{
    CUiDomElement itemNode;
    int i;
    int rc;
    bool writeCheckState = true;

    if(aCheckState.isNull())
        writeCheckState = false;

    for(i=0; i < aListItems.size(); i++)
    {
        itemNode = this->createUiItem();
        if(itemNode.isNull())
            return(-1);

        rc = itemNode.setUiPropertyText(aListItems.at(i));
        if(rc != 0)
            return(-1);

        if(writeCheckState)
        {
            rc = itemNode.setUiPropertyCheckState(aCheckState);
            if(rc != 0)
                return(-1);
        }
    }
    return(0);
}


/**
 * Add items to this node.
 *
 * @param aListItems Givin items.
 * @param aCheckState CheckState of all items, for possible values see setUiPropertyCheckState().
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::addUiItemList(const QStringList &aListItems)
{
    return(addUiItemList(aListItems, NULL));
}


/**
 * Add an "addaction" node
 *
 * @param aName Content of "name" attribute.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::addUiAddAction(const QString &aName)
{
    QDomDocument domDoc = this->ownerDocument();
    CUiDomElement n;

    // create element node
    n = domDoc.createElement("addaction");
    if(n.isNull())
        return(-1);

    // create attributes
    n.setAttribute("name", aName);


    // append property to givin uiElm
    this->appendChild(n);

    return(0);
}


/**
 * Add an "action" element node as child to root widget, because of this it becomes global.
 *
 * @param aName Content of "name" attribute.
 * @param aText Content of "text" property.
 * @return 0 on Success and -1 if it failed.
 **/
int CUiDomElement::addUiGlobalAction(const QString &aName, const QString &aText)
{
    CUiDomElement rootWidget;
    CUiDomElement n;
    QDomDocument domDoc;
    int rc;

    rootWidget = this->getRootWidget();
    if(rootWidget.isNull())
        return(-1);


    domDoc = this->ownerDocument();

    // create element node
    n = domDoc.createElement("action");
    if(n.isNull())
        return(-1);

    // create attributes
    n.setAttribute("name", aName);

    rc = n.setUiPropertyText(aText);
    if(rc != 0)
    {
        n.clear();
        return(-1);
    }

    // append DOM element to uiElm
    rootWidget.appendChild(n);

    return(0);
}


/**
 * Create a randomized name.
 * This name contains 5 characters and a increasing number with 4 digits.
 *
 * @param aPrefix Added in front of randomized name.
 * @param aSuffix Added at end of ramdomized name.
 * @return Created name.
 **/
QString CUiDomElement::getRandName(QString aPrefix, QString aSuffix)
{
    QString characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString randName;
    static int number = 0;
    int i;

    for(i = 0; i < 5; i++)
        randName += characters[rand() % characters.size()];
    randName += QString().sprintf("%05d", number);

    number++;

    return(aPrefix + randName + aSuffix);
}


















