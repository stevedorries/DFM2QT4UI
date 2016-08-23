/**
 * cdfm2guitree.h
 *
 *  Created on: 11.08.2009
 *      Author: Robert Doering
 */

#ifndef FORMPARSER_V2_H_
#define FORMPARSER_V2_H_

#include <QtGui/QWidget>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "cguitreedomdocument.h"
#include "cguitreedomelement.h"

using namespace std;

/**
 * \brief Parser that produces an XML representation of a Delphi text mode DFM file.
 *
 */
class CDfmParser : public QObject
{
    Q_OBJECT

public:
    CDfmParser();
    virtual ~CDfmParser();

public slots:
    int parseFile(CGuiTreeDomDocument *guiTree, const char *filename);
    int parseFile();
    int setDfmFileName(const char *filename);
    void setConvertBinaryData(bool value);

signals:
    /**
     * Signal: Logging message.
     */
    void logging(const QString & text);

private:
    string   dfmFileName;
    ifstream dfmFileStream;

    /**
     * @var lineNum line of DFM file that is currently parsed
     */
    size_t lineNum;

    /**
     * @enum enumDfmValueInfo Information of value part of an key-value-pair in a DFM file.
     * @var dviNormal Value contains complete value data
     * @var dviBinStart Begining of a binary value, first data are on the next line without a key (example: "Picture.Data = {")
     * @var dviBinEnd End of a binary value, data are available on this line, too.
     * @var dviPanelItemsStart Begining of Panel items, first item begins on the next line.
     * @var dviPanelItemsEnd End of Panel items, data are also on this line.
     * @var dviStringItemsStart Begining of String items, first item is on next line.
     * @var dviStringItemsEnd End of String items, data are also on this line.
     * @var dviNextLine Value is on the next line.
     */
    enum enumDfmValueInfo{dviNormal=0, dviBinStart, dviBinEnd, dviPanelItemsStart,
                          dviPanelItemsEnd, dviStringItemsStart, dviStringItemsEnd,dviNextLine};
    bool ConvertBinaryData;

    bool isDfmObjectLine(const string str);
    bool isDfmObjectEndLine(const string str);
    string getDfmObjectName(const string line);
    string getDfmObjectType(const string line);
    bool isDfmKeyValuePair(const string line);
    enumDfmValueInfo getDfmValueInfo(const string value);
    string getDfmPairKey(const string line);
    string getDfmPairValue(const string line);

    void trimSpaces(string& strText);
    int parseBinaryValueLines(CGuiTreeDomDocument *domDoc, CGuiTreeDomElement &domElm, QString key, QString value);
    int parsePanelItemsValueLines(CGuiTreeDomDocument *domDoc, CGuiTreeDomElement &domElm, QString key, QString value);
    int parseStringItemsValueLines(CGuiTreeDomDocument *domDoc, CGuiTreeDomElement &domElm, QString key, QString value);
    int parseLongTextValueLines(CGuiTreeDomDocument *domDoc, CGuiTreeDomElement &domElm, QString key, QString value);

    CGuiTreeDomDocument *domDoc;
};

#endif /* FORMPARSER_V2_H_ */
