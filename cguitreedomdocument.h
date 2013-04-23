#ifndef CGUITREEDOMDOCUMENT_H
#define CGUITREEDOMDOCUMENT_H

#include <QDomDocument>
#include "cguitreedomelement.h"

class CGuiTreeDomDocument : public QDomDocument
{
public:
    CGuiTreeDomDocument();

    CGuiTreeDomElement getFirstGuiObjectElement();

};

#endif // CGUITREEDOMDOCUMENT_H
