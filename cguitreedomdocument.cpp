
#include <stdio.h>
#include <QtDebug>
#include "cguitreedomdocument.h"

CGuiTreeDomDocument::CGuiTreeDomDocument()
{
    QDomImplementation impl;
    impl.setInvalidDataPolicy(QDomImplementation::ReturnNullNode);
}


/**
 * Get first "guiObject" located in "guiRoot".
 *
 * @return Node element of first guiObject or and empty element node there is no one.
 **/
CGuiTreeDomElement CGuiTreeDomDocument::getFirstGuiObjectElement()
{
    CGuiTreeDomElement domElmGuiTree;

    domElmGuiTree   = this->firstChildElement("guiRoot");
    if(domElmGuiTree.isNull())
        return(domElmGuiTree);
    return(domElmGuiTree.firstChildElement("guiObject"));
}
