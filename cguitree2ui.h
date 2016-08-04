/**
 * cguitree2ui.h
 *
 * @class Convert an domDoc object to an QT4 object.
 *
 * @TODO: add loggin signal
 *
 *
 *  Created on: 11.08.2009
 *      Author: Robert Doering
 */

#ifndef CGUITREE2UI_H
#define CGUITREE2UI_H

#include <QObject>
#include <QDomDocument>  // QDomDocument
#include "cuidomdocument.h"
#include "cuidomelement.h"
#include "cguitreedomdocument.h"
#include "cguitreedomelement.h"

class CGuiTree2Ui : public QObject
{
    Q_OBJECT

public:
    CGuiTree2Ui();
    ~CGuiTree2Ui();

    int convert(CUiDomDocument *domDocUi, CGuiTreeDomDocument *domDocGuiTree);


signals:
    /**
     * Signal: Logging message.
     */
    void logging(const QString &text);

  private:
    //QDomDocument * domDocTranslation;
    int convertTRadioGroup(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);

    int convertTOvcDateEdit(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTDBGrid(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertGuiObject(CUiDomElement &uiElm, CGuiTreeDomElement &guiTreeElm, bool guiTreeElmIsFrist);
    int convertUnknownWidget(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTBevel(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertForm(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTTabControl(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTPageControl(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTTabSheet(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTButton(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTLabel(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTEdit(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTPanel(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTGroupBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTCheckBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTRadioButton(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTImage(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTStatusBar(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTScrollBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTCSpinEdit(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTActionList(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTAction(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTMainMenu(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTToolBar(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTMenuItem(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTProgressBar(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTCGauge(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTCSpinBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTScrollBar(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTMemo(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTTrackBar(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTTreeView(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTCheckListBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTComboBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);
    int convertTListBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName);

};

#endif // CGUITREE2UI_H
