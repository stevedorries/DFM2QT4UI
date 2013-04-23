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
 * DFM to standard GUI tree converter class.
 *
 * This is an QT class to use also the signal-slot methods.
 *
 *
 */
class CDfm2GuiTree : public QObject
{
    Q_OBJECT

public:
    CDfm2GuiTree();
    virtual ~CDfm2GuiTree();

    int parseFile(CGuiTreeDomDocument *guiTree, const char *filename);
    int parseFile();
    int setDfmFileName(const char *filename);

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
     * @var dviNormal Value contains complue value data-
     * @var dviBinStart Begin of an binary value, first data are at the next line without a key (example: "Picture.Data = {")
     * @var dviBinEnd End of an binary value, data are available at this line, too.
     * @var dviPanelItemsStart Begin of panel items, first item begin in next line.
     * @var dviPanelItemsEnd End of panel items, data are also in this line.
     * @var dviStringItemsStart Begin of string items, first item begin in next line.
     * @var dviStringItemsEnd End of string items, data are also in this line.
     * @var dviNextLine Value is at the next line.
     */
    enum enumDfmValueInfo{dviNormal=0, dviBinStart, dviBinEnd, dviPanelItemsStart,
                          dviPanelItemsEnd, dviStringItemsStart, dviStringItemsEnd,dviNextLine};

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
