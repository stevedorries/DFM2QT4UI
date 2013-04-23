#include "cguitreedomelement.h"
#include <QDebug>
#include <QDomDocument>

CGuiTreeDomElement::CGuiTreeDomElement()
{
}


CGuiTreeDomElement::CGuiTreeDomElement( const QDomElement & aX ) : QDomElement(aX)
{
}


CGuiTreeDomElement CGuiTreeDomElement::parentNode() const
{
    CGuiTreeDomElement rc = QDomNode::parentNode().toElement();
    return(rc);
}


bool CGuiTreeDomElement::hasGuiObjectChildNodes()
{
    QDomElement e = this->firstChildElement("guiObject");
    if(e.isNull())
        return(false);
    return(true);
}


/**
 * Return root guiObject of this element, that is not the "guiRoot" element.
 *
 * @returns CGuiTreeDomElement of first guiObject or a Null-Node if it doesn't exists.
 */
CGuiTreeDomElement CGuiTreeDomElement::getRootGuiObject()
{
    CGuiTreeDomElement rootObject;
    CGuiTreeDomElement guiObject;
    QDomDocument domDoc;

    domDoc = this->ownerDocument();
    rootObject = domDoc.firstChildElement("guiRoot");
    guiObject = rootObject.firstChildElement("guiObject");

    return(guiObject);
}


/**
 * Read a value from givin gui object node for a givin key This is the secure way to do this.
 *
 * DOM element sample:
 * <guiObject>
 *  <name>Bevel1</name>
 *  <class>TBevel</class>
 *  <Left>16</Left>
 *  <Top>48</Top>
 *  <Width>641</Width>
 *  <Height>16</Height>
 *  <Anchors>[akLeft, akTop, akRight]</Anchors>
 *  <Shape>bsBottomLine</Shape>
 * </guiObject>
 *
 * @param value Storage for value for givin key.
 * @param key Name of childnode, which contain the value.
 * @param gutTreeElm Parentnode which is containing the key and value.
 * @return 0 = ok, -1 = Failed
 **/
int CGuiTreeDomElement::getDomPropertySecure(QString &value, const QString &key)
{
    QDomElement guiTreeElm =  this->firstChildElement(key);
    if(guiTreeElm.isNull())
    {
        // qDebug() << "key doesnt exist";
        return(-1);
    }

    QDomNode guiTreeNode = guiTreeElm.firstChild();

    // Look for value which have to be the one and only child node.
    if(!guiTreeNode.isText())
    {
        qDebug() << "child isnt a text";
        return(-1);
    }

    // Convert value to QString
    QDomText valueNode = guiTreeNode.toText();
    if(valueNode.isNull())
    {
        qDebug() << "value is empty";
        return(-1);
    }
    value = valueNode.data();

    return(0);
}





/**
 * Read a value from givin gui object node for a givin key.
 *
 * DOM element sample:
 * <guiObject>
 *  <name>Bevel1</name>
 *  <class>TBevel</class>
 *  <Left>16</Left>
 *  <Top>48</Top>
 *  <Width>641</Width>
 *  <Height>16</Height>
 *  <Anchors>[akLeft, akTop, akRight]</Anchors>
 *  <Shape>bsBottomLine</Shape>
 * </guiObject>
 *
 * @param  domElm Givin GUI widget DOM element.
 * @param  key name property.
 * @return Value as string if exist or "" if isn't.
 **/
QString CGuiTreeDomElement::getDomProperty(const QString key)
{
    QString str;

    if(0 != getDomPropertySecure(str, key))
        str = "";  // it doesn't matter, use empty string
    return(str);
}


/**
 * Read a value from givin gui object node for a givin key.
 *
 * DOM element sample:
 * <guiObject>
 *  <name>Bevel1</name>
 *  <class>TBevel</class>
 *  <Left>16</Left>
 *  <Top>48</Top>
 *  <Width>641</Width>
 *  <Height>16</Height>
 *  <Anchors>[akLeft, akTop, akRight]</Anchors>
 *  <Shape>bsBottomLine</Shape>
 * </guiObject>
 *
 * @param  domElm Givin GUI widget DOM element.
 * @param  key name property.
 * @param  defaultValue Value, used if key does not exist.
 * @return Value as string if exist or defaultValue if isn't.
 **/
QString CGuiTreeDomElement::getDomProperty(const QString key, const QString defaultValue)
{
    QString str;

    if(0 != getDomPropertySecure(str, key))
    {
        // qDebug() << QString("cant find ") << key << defaultValue << QString(".");
        str = defaultValue;  // it doesn't matter, use empty string
    }
    return(str);
}


/**
 * Create an DOM element with text content and add  this to a givin element.
 *
 * @param domDoc Givin document object.
 * @param key Name of new DOM element.
 * @param value The text content of new DOM element
 * @return 0 on Success else -1.
 **/
int CGuiTreeDomElement::setDomProperty(const QString key, const QString value)
{
    QDomDocument domDoc = this->ownerDocument();
    QDomElement tmpDomElm = domDoc.createElement(key);
    if(tmpDomElm.isNull())
        return(-1);
    QDomText tmpDomTxt = domDoc.createTextNode(value);
    if(tmpDomTxt.isNull())
    {
        tmpDomElm.clear();
        return(-1);
    }
    tmpDomElm.appendChild(tmpDomTxt);
    this->appendChild(tmpDomElm);
    return(0);
}


/**
 * Find position of first child-guiObject with matching name.
 *
 * @param domElm Givin DOM element.
 * @param name Value of guiObject property to search for.
 * @return Position as Integer or 0.
 **/
int CGuiTreeDomElement::getNumberOfGuiObject(const QString &name)
{
    QDomElement e;
    QDomNode n;
    QDomElement s;
    QDomNode c;
    QDomText valueNode;
    QDomNodeList eList;
    int rc = 0;

    e = this->firstChildElement("guiObject");
    while(!e.isNull())
    {
        s = e.firstChildElement("name");
        if(s.isNull())
        {
            e = e.nextSiblingElement("guiObject");
            continue;
        }

        c = s.firstChild();
        // Look for value which have to be the one and only child node.
        if(!c.isText())
        {

            e = e.nextSiblingElement("guiObject");
            continue;
        }

        valueNode = c.toText();
        if(valueNode.data() == name)
        {
            return(rc);
        }

        // increace
        rc++;
        e = e.nextSiblingElement("guiObject");
    }

    return(0);
}


/**
 * Read out item into a QStringList.
 *
 * @param aName Value of guiObject property to search for.
 * @return Position as Integer or 0.
 **/
QStringList CGuiTreeDomElement::getItemListOf(const QString &name)
{
    QStringList list;
    QDomElement e;
    QDomElement item;
    QDomNode child;
    QDomText childText;

    e = this->firstChildElement(name);
    if(e.isNull())
        return(list);

    item = e.firstChildElement("item");
    while(!item.isNull())
    {
        child = item.firstChild();
        // Look for value which have to be the one and only child node.
        if(!child.isText())
        {
            item = item.nextSiblingElement();
            continue;
        }

        // add to text
        childText = child.toText();
        list.append(childText.data());;

        // increace
        item = item.nextSiblingElement();
    }

    return(list);
}


/**
 * Read out item into a QString separated by \n.
 *
 * @param name Value of guiObject property to search for.
 * @return Position as Integer or 0.
 **/
QString CGuiTreeDomElement::getItemsOf(const QString &name)
{
    QString text;
    QDomElement e;
    QDomElement item;
    QDomNode child;
    QDomText childText;
    bool isFirst = true;

    e = this->firstChildElement(name);
    if(e.isNull())
        return(text);

    item = e.firstChildElement("item");
    while(!item.isNull())
    {
        child = item.firstChild();
        // Look for value which have to be the one and only child node.
        if(!child.isText())
        {
            item = item.nextSiblingElement();
            continue;
        }

        // add new line if neccessary
        if(isFirst)
            isFirst = false;
        else
            text += "\n";

        // add to text
        childText = child.toText();
        text += childText.data();

        // increace
        item = item.nextSiblingElement();
    }

    return(text);
}


/**
 * Read out color as QColor.
 *
 * TColor format is 0xRRGGBBSS R=red G=green B=blue S=system color (this is not supported by QT)
 *
 * Supported Colors are:
 *   clBlack    = 000000
 *   clMaroon   = 000080
 *   clGreen    = 000080
 *   clOlive    = 008080
 *   clNavy     = 000080
 *   clPurple   = 800080
 *   clTeal     = 008080
 *   clGray     = 808080
 *   clSilver   = c0c0c0
 *   clRed      = 0000ff
 *   clLime     = 0000ff
 *   clYellow   = 00ffff
 *   clBlue     = 0000ff
 *   clFuchsia  = ff00ff
 *   clAqua     = 00ffff
 *   clLtGray   = c0c0c0
 *   clDkGray   = 808080
 *   clWhite    = ffffff
 *
 * @param[out] aColor Color of element
 * @param[in] name tag name of color element.
 * @return 0 on success and else if not.
 **/
int CGuiTreeDomElement::getColor(QColor &aColor, const QString &aName)
{
    QString colorText;
    qint64 colorInt;
    bool colorIntOk = false;
    QString colorHexText;
    unsigned char colorByte;

    QDomElement e;
    QColor color;
    QDomNode child;
    QDomText childText;

    e = this->firstChildElement(aName);
    if(e.isNull())
        return(-1);

    child = e.firstChild();
    // Look for value which have to be the one and only child node.
    if(!child.isText())
        return(-1);

    colorText = child.toText().data();

    if(colorText == "clBlack")        color.setNamedColor("#000000");
    else if(colorText == "clMaroon")  color.setNamedColor("#000080");
    else if(colorText == "clGreen")   color.setNamedColor("#000080");
    else if(colorText == "clOlive")   color.setNamedColor("#008080");
    else if(colorText == "clNavy")    color.setNamedColor("#000080");
    else if(colorText == "clPurple")  color.setNamedColor("#800080");
    else if(colorText == "clTeal")    color.setNamedColor("#008080");
    else if(colorText == "clGray")    color.setNamedColor("#808080");
    else if(colorText == "clSilver")  color.setNamedColor("#c0c0c0");
    else if(colorText == "clRed")     color.setNamedColor("#0000ff");
    else if(colorText == "clLime")    color.setNamedColor("#0000ff");
    else if(colorText == "clYellow")  color.setNamedColor("#00ffff");
    else if(colorText == "clBlue")    color.setNamedColor("#0000ff");
    else if(colorText == "clFuchsia") color.setNamedColor("#ff00ff");
    else if(colorText == "clAqua")    color.setNamedColor("#00ffff");
    else if(colorText == "clLtGray")  color.setNamedColor("#c0c0c0");
    else if(colorText == "clDkGray")  color.setNamedColor("#808080");
    else if(colorText == "clWhite")   color.setNamedColor("#ffffff");
    else
    {
        colorInt = colorText.toInt(&colorIntOk, 10);
        if(colorIntOk)  // colorText contains an integer value in decimal notation
        {
            colorByte = colorInt & 0xFF;
            colorHexText = QString().sprintf("#%02X", colorByte);
            colorByte = (colorInt >> 8) & 0xFF;
            colorHexText += QString().sprintf("%02X", colorByte);
            colorByte = (colorInt >> 16) & 0xFF;
            colorHexText += QString().sprintf("%02X", colorByte);

            color.setNamedColor(colorHexText);
        }
    }

    aColor = color;
    return(0);
}


/**
 * Remove quotes and spaces at begin and end of givin string.
 *
 * @param text Text to process.
 * @return text without quotes
 *
 **/
QString CGuiTreeDomElement::removeQuotes(QString text)
{
    int begin, end;
    int n;
    //qDebug() << "removeQuotes:" << text;

    begin = text.indexOf(QRegExp("^'")) + 1;
    if(begin <= 0)
        begin = 0;

    end = text.indexOf(QRegExp("'$")) - 1;
    if(end < 0)
        end = text.length();
    //qDebug() << "Res:" << text.mid(begin, n);


    return(text.mid(begin, end));
}


/**
 * Tranlate ASCII-c-string with special chars into unicode string.
 *
 * Input : 'Lecture/Ecriture d'#39'une structure de type TTabHeader dans un fich'
 * Output: 'Lecture/Ecriture d'une structure de type TTabHeader dans un fich'
 *
 * @param text Text to process.
 * @return text with integrated special chars
 *
 **/
QString CGuiTreeDomElement::substituteAsciiSpecialChars(QString text)
{
    int idx, idxQuote;
    QString textCopy = text;
    int offset = 0;
    QString asciiStr;
    int c;

    do
    {
        // try to find encoded special char
        idx = textCopy.indexOf(QRegExp("'#[0-9]+'"), offset);
        if(idx < 0)
            break;

        // get ASCII-code
        asciiStr = textCopy.midRef(idx + 2).toString();

        // look for next quote, to add space before
        idxQuote = asciiStr.indexOf('\'');
        if(idxQuote >=0)
        {
            asciiStr = asciiStr.left(idxQuote);
        }

        // get char of ascii-code
        c = (asciiStr.toInt() & 0xFF);

        // insert char of ascii-code
        textCopy.insert(idx, QString(c));

        // set offset
        offset = idx + 2; // inserted char + one char of found regexp

    }while(true);

    textCopy.replace(QRegExp("'#[0-9]+'"), "");
    textCopy.replace(QRegExp("''"), "'");

    return(textCopy);
}


/**
 * Tranlate ASCII-c-string with special chars and quotes into unicode string without quodes.
 *
 * Input : 'Lecture/Ecriture d'#39'une structure de type TTabHeader dans un fich'
 * Output: Lecture/Ecriture d'une structure de type TTabHeader dans un fich
 *
 * @param text Text to process.
 * @return text with integrated special chars and without quotes
 *
 **/
QString CGuiTreeDomElement::translateToNonQuotedUnicode(QString text)
{
    return(substituteAsciiSpecialChars( removeQuotes(text) ));
}


/**
 * Analyse this node and return geometry.
 * Normaly the information of ordinate(x), absis(y), width(w) and height(h)
 * is stored in "Top" for x, "Left" for y, "Width" for w and "Height" for h.
 * But sometimes there is no "Widht" and "Height", in this case you have to use
 * "ClientWidth" for w and "ClientHeight" for h. To make this methos as reliable
 * as posible you have to use default values if a property does not exist.
 * The default values are 0 for x and y and 15 for w and h.
 *
 * @return Geometry of this element.
 *
 **/
QRect CGuiTreeDomElement::getPropertyGeometry()
{
    int x,y,w,h;
    QString tmp;


    if(0 != this->getDomPropertySecure(tmp, "Left"))  // doesn't exist?
        x = 0;
    else
        x = tmp.toInt(NULL);  // don't observe number, it has to be a number!

    if(0 != this->getDomPropertySecure(tmp, "Top"))  // doesn't exist?
        y = 0;
    else
        y = tmp.toInt(NULL);  // don't observe number, it has to be a number!

    if(0 != this->getDomPropertySecure(tmp, "Width") && this->getDomPropertySecure(tmp, "ClientWidth"))  // doesn't exist?
        w = 15;
    else
        w = tmp.toInt(NULL);  // don't observe number, it has to be a number!

    if(0 != this->getDomPropertySecure(tmp, "Height") && this->getDomPropertySecure(tmp, "ClientHeight"))  // doesn't exist?
        h = 15;
    else
        h = tmp.toInt(NULL);  // don't observe number, it has to be a number!

    return(QRect(x, y, w, h));
}


/**
 * Get list of tabs as string list. The list only contains the captions of tabs.
 *
 * @return A list of tab captions.
 **/
QStringList CGuiTreeDomElement::getTabStrings()
{
    QDomElement e;
    QDomElement item;
    QStringList list;
    QDomNode child;
    QDomText textNode;


    e = this->firstChildElement("Tabs.Strings");
    if(e.isNull())
        return(list);

    item = e.firstChildElement("item");
    while(!item.isNull())
    {
        child = item.firstChild();
        if(child.isText())
        {
            textNode = child.toText();
            list << textNode.data();
        }

        // increace
        item = item.nextSiblingElement("item");
    }

    return(list);
}






