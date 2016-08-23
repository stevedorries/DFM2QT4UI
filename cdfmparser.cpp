/**
 * @file CDfmParser.cpp
 * @authors Robert Doering
 *          Steven Dorries
 * @date 11.08.2009
 *
 */

#include "cdfmparser.h"
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <QMessageBox>


CDfmParser::CDfmParser()
{
}


CDfmParser::~CDfmParser()
{
}


/**
 * @brief Parse given file into given XML document.
 *
 * @param filename Name of DFM file to parse.
 * @param guiTree CGuiTreeDomDocument to fill with widget from the file.
 *
 * @return 0 = ok
 * @return -1 = error
 */
int CDfmParser::parseFile(CGuiTreeDomDocument *guiTree, const char *filename)
{
    // Set domDoc to given guiTree
    domDoc = guiTree;

    // Set filename.
    if(setDfmFileName(filename))
        return(-1);

    return(parseFile());
}


/**
 * @brief Start parsing file, which filename was set up before.
 * @brief Make sure the domDoc and dfmFileName was set before.
 *
 * @return 0 = ok
 * @return -1 = error
 */
int CDfmParser::parseFile()
{
    string str;
    string substr;
    string menuitem;
    stringstream sstr;

    CGuiTreeDomElement curDomElm;
    QDomElement tmpDomElm;
    QDomText tmpDomTxt;
    QDomElement rootDomElm;

    string pairKey;
    string pairValue;
    enumDfmValueInfo pairValueInfo;
    int parserReturnCode = 0;
    int rc;  // tmp return code

    // Repare XML document
    rootDomElm = domDoc->createElement("guiRoot");
    domDoc->appendChild(rootDomElm);
    curDomElm = rootDomElm;

    dfmFileStream.open(dfmFileName.data());
    if(!dfmFileStream.is_open())
    {
        logging("Can't open DFM file.");
        return(-1);
    }
    lineNum = 0;

    while(getline(dfmFileStream, str))
    {
        lineNum++;
        trimSpaces(str);

        if(isDfmObjectLine(str))
        {
            tmpDomElm = domDoc->createElement("guiObject");
            curDomElm.appendChild(tmpDomElm);
            curDomElm = tmpDomElm;  // set new node as current node
            curDomElm.setAttribute("name",QString(getDfmObjectName(str).c_str()));
            curDomElm.setAttribute("class", QString(getDfmObjectType(str).c_str()));

            curDomElm.setDomProperty("name", getDfmObjectName(str).c_str());
            curDomElm.setDomProperty("class", getDfmObjectType(str).c_str());
        }
        else if(isDfmObjectEndLine(str))
        {
            tmpDomElm = curDomElm.parentNode().toElement();
            if(curDomElm == rootDomElm)
            {
                logging(QString("Error: more object closed by \"end\" as opened before. (line %1)").arg(lineNum));
                parserReturnCode = -1;
            }
            else
                curDomElm = tmpDomElm;
        }
        else if(isDfmKeyValuePair(str))
        {
            pairKey = getDfmPairKey(str);
            pairValue = getDfmPairValue(str);

            pairValueInfo = getDfmValueInfo(pairValue);
            switch(pairValueInfo)
            {
            case dviNormal:
                curDomElm.setDomProperty(QString(pairKey.data()), QString(pairValue.data()));
                break;
            case dviBinStart:
                rc = parseBinaryValueLines(domDoc, curDomElm, QString(pairKey.data()), QString(pairValue.data()));
                    if(rc != 0)
                        parserReturnCode = -1;
                break;
            case dviPanelItemsStart:
                rc = parsePanelItemsValueLines(domDoc, curDomElm, QString(pairKey.data()), QString(pairValue.data()));
                if(rc != 0)
                    parserReturnCode = -1;
                break;
            case dviStringItemsStart:
                rc = parseStringItemsValueLines(domDoc, curDomElm, QString(pairKey.data()), QString(pairValue.data()));
                if(rc != 0)
                    parserReturnCode = -1;
                break;
            case dviNextLine:
                rc = parseLongTextValueLines(domDoc, curDomElm, QString(pairKey.data()), QString(pairValue.data()));
                if(rc != 0)
                    parserReturnCode = -1;
                break;
            default:
                logging(QString("Can't understand value of line %1 with \"%2\".").arg(lineNum).arg(QString(str.data())));
                break;
            }
        }
        else
        {
            logging(QString("Can't understand line %1 with \"%2\".").arg(lineNum).arg(QString(str.data())));
            parserReturnCode = -1;
        }
    }

    // close files
    dfmFileStream.close();

    return(parserReturnCode);
}


/**
 * @brief Parse Lines for an binary key-value-pair.
 *
 * @brief Key and value will be added to the domDocument as property
 * @brief for given domElm.
 * @example
    Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    000000008000008000000080800080000000800080008080000080808000C0C0
    C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF006CC}

 * @return 0 ok
 * @return -1 Failed
 */
int CDfmParser::parseBinaryValueLines(CGuiTreeDomDocument *domDoc, CGuiTreeDomElement &domElm, QString key, QString value)
{
    string line;
    enumDfmValueInfo pairValueInfo;

    while(getline(dfmFileStream, line))
    {
        lineNum++;
        trimSpaces(line);
        pairValueInfo = getDfmValueInfo(line);
        switch(pairValueInfo)
        {
        case dviNormal:
            value.append(QString(line.data())+"\n");
            break;
        case dviBinEnd:
            value += QString(line.data());
            value.replace(QRegExp("[{}]"),"");
            // append key-value-pair to dom object
            if(ConvertBinaryData)
                domElm.setDomProperty(key, value);
            return(0);
            break;
        case dviPanelItemsEnd:
        case dviStringItemsEnd:
        case dviBinStart:
        case dviPanelItemsStart:
        case dviStringItemsStart:
        default:
            logging(QString("Error: Unexpected end of binary value in line %1").arg(lineNum));
            return(-1);
            break;
        }
    }
    logging(QString("Error: Unexpected end of file, while parsing binary-values at line %1.").arg(lineNum));
    return(-1);
}


/**
 * Parse Lines for an panel items key-value-pair.
 *
 * Key and value will be added to the domDocument as properties
 * for given domElm.
 * @example
    Panels = <
      item
        Width = 350
      end
      item
        Width = 350
      end
      item
        Width = 160
      end
      item
        Width = 80
      end
      item
        Width = 70
      end>


 * @return 0 ok
 * @return -1 Failed
 */
int CDfmParser::parsePanelItemsValueLines(CGuiTreeDomDocument *domDoc, CGuiTreeDomElement &domElm, QString key, QString value)
{
    string line;
    enumDfmValueInfo pairValueInfo;
    CGuiTreeDomElement domSubElm;

    // Create child node with key as name
    domSubElm = domDoc->createElement(key);
    domElm.appendChild(domSubElm);

    while(getline(dfmFileStream, line))
    {
        lineNum++;
        trimSpaces(line);
        if(isDfmKeyValuePair(line))
            domSubElm.setDomProperty(QString(getDfmPairKey(line).data()), QString(getDfmPairValue(line).data()));
        else
        {
            pairValueInfo = getDfmValueInfo(line);
            switch(pairValueInfo)
            {
            case dviPanelItemsEnd:
                return(0);
                break;
            case dviNormal: break;  // "item" and "end"
            case dviBinEnd:
            case dviStringItemsEnd:
            case dviBinStart:
            case dviPanelItemsStart:
            case dviStringItemsStart:
            default:
                logging(QString("Error: Unexpected end of panel array in line %1").arg(lineNum));
                return(-1);
                break;
            }
        }
    }
    logging(QString("Error: Unexpected end of file, while parsing panel-items at line %1.").arg(lineNum));
    return(-1);
}


/**
 * Parse Lines for an string items key-value-pair.
 *
 * Key and value will be added to the domDocument as properties
 * for given domElm.
 * @example
    Items.Strings = (
        'electronic Chart')
 *
 * @return 0 ok
 * @return -1 Failed
 */
int CDfmParser::parseStringItemsValueLines(CGuiTreeDomDocument *domDoc, CGuiTreeDomElement &domElm, QString key, QString value)
{
    string line;
    enumDfmValueInfo pairValueInfo;
    CGuiTreeDomElement domSubElm;
    QString item;

    // Create child node with key as name
    domSubElm = domDoc->createElement(key);
    domElm.appendChild(domSubElm);

    while(getline(dfmFileStream, line))
    {
        lineNum++;
        trimSpaces(line);

        pairValueInfo = getDfmValueInfo(line);
        switch(pairValueInfo)
        {
        case dviNormal:
        case dviStringItemsEnd:
            item = QString(line.data());
            item.replace(QRegExp("[()']"),"");
            domSubElm.setDomProperty("item", item);

            if(pairValueInfo == dviStringItemsEnd)
                return(0);
            break;
        case dviBinEnd:
        case dviPanelItemsEnd:
        case dviBinStart:
        case dviPanelItemsStart:
        case dviStringItemsStart:
        default:
            logging(QString("Error: Unexpected end of string array in line %1").arg(lineNum));
            return(-1);
            break;
        }
    }
    logging(QString("Error: Unexpected end of file, while parsing string-items at line %1.").arg(lineNum));
    return(-1);
}


/**
 * @brief Parse Lines for long text key-value-pair.
 *
 * @brief Key and value will be added to the domDocument as properties
 * @brief for given domElm.
 * @example
    Items.Strings = (
        'electronic Chart')
 *
 * @return 0 ok
 * @return -1 Failed
 */
int CDfmParser::parseLongTextValueLines(CGuiTreeDomDocument *domDoc, CGuiTreeDomElement &domElm, QString key, QString value)
{

    QString nvalue;
    string line;
    bool runLoop = true;

    do
    {
        // read in line
        getline(dfmFileStream, line);
        if(dfmFileStream.eof())
        {
            logging(QString("Error: Unexpected end of file, while parsing multiline String at line %1.").arg(lineNum));
            return(-1);
        }
        lineNum++;
        trimSpaces(line);

        // look  for " +"
        if(0 == line.compare( line.length() - 2, 2, " +", 0, 2))
        {
            // remove " +"
            line.erase(line.length() - 2, 2);
        }
        else // finished reading
            runLoop = false;

        // remove last '\'' char of nvalue and first '\'' char of line, if
        if(!nvalue.isEmpty())
        {
            nvalue.chop(1);
            line.erase(0, 1);
        }

        // append
        nvalue.append(line.c_str());

    }while(runLoop);

    // write guitree
    domElm.setDomProperty(key, nvalue);

    return(0);

}


/**
 * Set DFM file name.
 *
 * @return 0 = ok
 * @return -1 = Failed
 */
int CDfmParser::setDfmFileName(const char *filename)
{
    if(filename == NULL)
        return(-1);

    dfmFileName = filename;

    return(0);
}

/**
 * @brief Set the value of ConvertBinaryData
 * @param value
 */
void CDfmParser::setConvertBinaryData(bool value)
{
    if(value == NULL)
        this->ConvertBinaryData = false;
    else
        this->ConvertBinaryData = value;
}

bool CDfmParser::isDfmObjectLine(const string str)
{
    if(str.find("object", 0) != string::npos &&
            str.find(":", 0) != string::npos)
        return(true);
    return(false);
}

bool CDfmParser::isDfmObjectEndLine(const string str)
{
    string s = str;
    trimSpaces(s);
    if(s.compare("end") == 0) // equal?
        return(true);
    return(false);
}

string CDfmParser::getDfmObjectName(const string line)
{
    string name = line.substr((line.find_first_of("t", 0)+2), line.length());
    name = name.substr(0, name.find_first_of(":", 0));
    return(name);
}

string CDfmParser::getDfmObjectType(const string line)
{
    string type = line.substr((line.find_first_of(":", 0)+2), line.length());
    return(type);
}



/**
 *
 * sample: Font.Charset = DEFAULT_CHARSET
 */
bool CDfmParser::isDfmKeyValuePair(const string line)
{
    size_t pos;
    bool found;

    // look for first non space char (key)
    for(pos=0, found=false; pos<line.length(); pos++)
    {
        if(isspace(line.at(pos)))
            continue;
        found = true;
        break;
    }
    if(!found)
        return(false);

    // look for first space after the key
    for(found=false; pos<line.length(); pos++)
    {
        if(!isspace(line.at(pos)))
            continue;
        found = true;
        break;
    }

    // Does line at position pos match " = "?
#if 1
    if(line.compare(pos, 2, " =", 2) == 0)
#else
    if(line.length() < pos + 3 ||
            line.at(pos + 0) != ' ' ||
            line.at(pos + 1) != '=' ||
            line.at(pos + 1) != ' ')
#endif
        return(true);
}

/**
 * Analyze given value from an DFM key-value-pair, maybe it has more value data at
 * the next line of DFM file.
 *
 * @param[in] value String with value information.
 * @return enumDfmValueInfo
 */
CDfmParser::enumDfmValueInfo CDfmParser::getDfmValueInfo(const string value)
{
    size_t pos;
    const char spaces[] = " \t\r\n";

    // Search for non-spaces at the end
    pos = value.find_last_not_of(spaces);
    if(pos == string::npos)
        return(dviNextLine);

    // look for binary data
    if(value[pos] == '}')
        return(dviBinEnd);

    // look for panel items
    if(value[pos] == '>')
        return(dviPanelItemsEnd);

    // look for string items
    if(value[pos] == ')')
        return(dviStringItemsEnd);

    // Search for non-spaces at the beginning
    pos = value.find_first_not_of(spaces);

    // look for binary data
    if(value[pos] == '{')
        return(dviBinStart);

    // look for panel items
    if(value[pos] == '<')
        return(dviPanelItemsStart);

    // look for string items
    if(value[pos] == '(')
        return(dviStringItemsStart);

    return(dviNormal);
}


/**
 *
 * @param[in] line DFM file line like "   Font.Name = 'MS Sans Serif'"
 */
string CDfmParser::getDfmPairKey(const string line)
{
    string rs;
    size_t pos;

    // look for pos of '=' char
    pos = line.find_first_of('=');
    if(pos == string::npos)
        return(rs);  // no key-value-pair

    // substring
    rs = line.substr(0, pos);

    // Remove white spaces at begin and end.
    trimSpaces(rs);

    return(rs);
}

/**
 *
 * @param[in] line DFM file line like "   Font.Name = 'MS Sans Serif'"
 */
string CDfmParser::getDfmPairValue(const string line)
{
    string rs;
    size_t pos;

    // look for pos of '=' char
    pos = line.find_first_of('=');
    if(pos == string::npos || pos >= line.length())
        return(rs);  // no key-value-pair

    // substring
    rs = line.substr(pos + 1, line.length() - (pos  + 1));

    // Remove white spaces at begin and end.
    trimSpaces(rs);

    return(rs);
}


/**
 * Remove leading and trailing spaces
 */
void CDfmParser::trimSpaces(string& strText)
{
    const char spaces[] = " \t\r\n";

    // Search spaces at the beginning
    string::size_type nFirst = strText.find_first_not_of(spaces);
    if(nFirst == string::npos)
    {
        // Only spaces. Empty the string
        strText.erase();
        return;
    }

    if(nFirst != 0) // Some spaces at the beginning. Remove them
        strText.erase(0, nFirst);

    // Search spaces at the end
    string::size_type nLast = strText.find_last_not_of(spaces);
    if(nLast != string::npos) // Some spaces at the end. Remove them
        strText.erase(nLast + 1);
}
