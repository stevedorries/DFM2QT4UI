#include "cuidomdocument.h"
#include <QtDebug>

CUiDomDocument::CUiDomDocument()
{
    QDomImplementation impl;
    impl.setInvalidDataPolicy(QDomImplementation::ReturnNullNode);
}


/**
 * Initialize QT UI DOM and return root element node.
 * Attention all data will be removed before.
 *
 * @return Root element node.
 **/
CUiDomElement CUiDomDocument::init()
{
    QDomProcessingInstruction  domDocUiHeader;
    QDomAttr                   domDocUiAttr;
    CUiDomElement              domDocUiRoot;

    // Mark file as xml file
    domDocUiHeader = this->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    this->appendChild(domDocUiHeader);

    // create root doument and set version attribute for it
    domDocUiRoot = this->createElement( "ui" );
    this->appendChild(domDocUiRoot);
    domDocUiAttr = this->createAttribute("version");
    domDocUiAttr.setValue("4.0");
    domDocUiRoot.setAttributeNode(domDocUiAttr);

    return(domDocUiRoot);
}
