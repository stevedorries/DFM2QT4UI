#ifndef CUIDOMDOCUMENT_H
#define CUIDOMDOCUMENT_H

#include <QDomDocument>
#include "cuidomelement.h"

class CUiDomDocument : public QDomDocument
{
public:
    CUiDomDocument();

    CUiDomElement init();
};

#endif // CUIDOMDOCUMENT_H
