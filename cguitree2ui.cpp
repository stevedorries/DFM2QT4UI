/**
 * @brief This module is used to convert a guiTree into a ui object.
 *
 *
 * @todo improve log concept
 *
 * Converted widgets are:
 * TBevel        - QFrame
 * TDBGrid       - QTableWidget
 * TTabControl   - QTabWidget
 * TPageControl  - QTabWidget
 * TTabSheet     - QFrame
 * TButton       - QPushButton
 * TBitBtn       - QPushButton
 * TLabel        - QLabel
 * TOvcDateEdit  - QDateEdit
 * TEdit         - QLineEdit
 * TPanel        - QFrame
 * TGroupBox     - QGroupBox
 * TCheckBox     - QCheckBox
 * TRadioButton  - QRadioButton
 * TImage        - QGraphicsView // picture will not be stored, maybe it schould be stored to file, but in this case we need also an *.qrc file and a *.pro which is linking both
 * TStatusBar    - QStatusBar
 * TCSpinEdit    - QSpinBox
 * TLMDSpinEdit  - QSpinBox
 * TScrollBox    - QScrollArea
 * TMainMenu     - QMenuBar
 * TMenuItem     - QMenu / QAction
 * TProgressBar  - QProgressBar
 * TCGauge       - QProgressBar
 * TCSpinButton  - QSpinBox
 * TTrackBar     - QSlider
 * TMemo         - QPlainTextEdit
 * TScrollBar    - QScrollBar
 * TTreeView     - QTreeWidget
 * TCheckListBox - QListWidget
 * TComboBox     - QComboBox
 *
 **/


#include "cguitree2ui.h"
#include <QDebug>


CGuiTree2Ui::CGuiTree2Ui()
{
}



CGuiTree2Ui::~CGuiTree2Ui()
{
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertUnknownWidget(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QFrame", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::StyledPanel");
    if(rc != 0)
    {
        logging("Error: Failed to create property 'frameShape'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertForm(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm;
    aNextUiDomElement.setUiDomKeyValuePair("class", aGuiTreeValueName);

    aNextUiDomElement = aUiElm.createUiWidget("QWidget", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode( aGuiTreeElm.getDomProperty("Hint") ));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyWindowTitle(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Caption","" /* no default*/ )));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'text'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTBevel(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    QString strBevelShape;
    int guiTreeTop, guiTreeLeft, guiTreeWidth, guiTreeHeight;

    strBevelShape = aGuiTreeElm.getDomProperty("Shape");
    if(strBevelShape.isEmpty())
        strBevelShape = "bsBox";

    aNextUiDomElement = aUiElm.createUiWidget("QFrame",aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    // read in origin geometry values
    guiTreeTop    = aGuiTreeElm.getDomProperty( "Top", "0").toInt();
    guiTreeLeft   = aGuiTreeElm.getDomProperty( "Left", "0").toInt();
    guiTreeWidth  = aGuiTreeElm.getDomProperty( "Width", "15").toInt();
    guiTreeHeight = aGuiTreeElm.getDomProperty( "Height", "15").toInt();

    // modify QFrame geometry property to handel different borland lines
    if(0 == strBevelShape.compare("bsBottomLine", Qt::CaseInsensitive))
    {
        rc = aNextUiDomElement.setUiPropertyFrameShape(QString("QFrame::HLine"));
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShape'.");
            return(-1);
        }

        guiTreeHeight = guiTreeHeight * 2;
    }
    else if(0 == strBevelShape.compare("bsTopLine", Qt::CaseInsensitive))
    {
        rc = aNextUiDomElement.setUiPropertyFrameShape(QString("QFrame::HLine"));
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShape'.");
            return(-1);
        }

        guiTreeTop    = guiTreeTop - guiTreeHeight;
        guiTreeHeight = guiTreeHeight * 2;
    }
    else if(0 == strBevelShape.compare("bsLeftLine", Qt::CaseInsensitive))
    {
        rc = aNextUiDomElement.setUiPropertyFrameShape(QString("QFrame::VLine"));
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShape'.");
            return(-1);
        }

        guiTreeLeft  = guiTreeLeft - guiTreeWidth;
        guiTreeWidth = guiTreeWidth * 2;
    }
    else if(0 == strBevelShape.compare("bsRightLine", Qt::CaseInsensitive))
    {
        rc = aNextUiDomElement.setUiPropertyFrameShape(QString("QFrame::VLine"));
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShape'.");
            return(-1);
        }

        guiTreeWidth = guiTreeWidth * 2;
    }
    else if(0 == strBevelShape.compare("bsBox", Qt::CaseInsensitive))
    {
        rc = aNextUiDomElement.setUiPropertyFrameShape(QString("QFrame::Panel"));
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShape'.");
            return(-1);
        }
    }
    else if(0 == strBevelShape.compare("bsFrame", Qt::CaseInsensitive) || 0 == strBevelShape.compare("bsSpacer", Qt::CaseInsensitive))
    {
        rc = aNextUiDomElement.setUiPropertyFrameShape(QString("QFrame::Box"));
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShape'.");
            return(-1);
        }
    }
    else
    {
        logging("Error: Unknown bevel shape " + strBevelShape + " '.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(QString::number(guiTreeLeft), QString::number(guiTreeTop), QString::number(guiTreeWidth), QString::number(guiTreeHeight));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyFrameShadow(0 == aGuiTreeElm.getDomProperty("Style").compare("bsRaised", Qt::CaseInsensitive)? QString("QFrame::Raised"): QString("QFrame::Sunken"));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'frameShadow'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTTabControl(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    QString guitreeTabPos;
    QString uiTabPos;
    QStringList tabCaption;

    aNextUiDomElement = aUiElm.createUiWidget("QTabWidget", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyCurrentIndex(aGuiTreeElm.getDomProperty( "TabIndex", "0"));
    if(rc != 0)
    {
        logging("Warning: Failed to set currentIndex");
        return(-1);
    }

    guitreeTabPos = aGuiTreeElm.getDomProperty( "TabPosition", "tbLeft");
    if(0 == guitreeTabPos.compare("tbLeft"))
        uiTabPos = "QTabWidget::West";
    else if(0 == guitreeTabPos.compare("tbTop"))
        uiTabPos = "QTabWidget::North";
    else if(0 == guitreeTabPos.compare("tpRight"))
        uiTabPos = "QTabWidget::East";
    else
        uiTabPos = "QTabWidget::South";

    rc = aNextUiDomElement.setUiPropertyTabPosition(uiTabPos);
    if(rc != 0)
    {
        logging("Warning: Failed to set currentIndex to \"" + aGuiTreeElm.getDomProperty( "ActivePage") + "\"");
        return(-1);
    }

    tabCaption = aGuiTreeElm.getTabStrings();
    while(tabCaption.size() > 0)
    {
        CUiDomElement uiTabElm;
        uiTabElm = aNextUiDomElement.createUiWidget("QWidget", aNextUiDomElement.getRandName("tab_", ""));
        if(aNextUiDomElement.isNull())
        {
            logging("Error: Failed to create ui widget.");
            return(-1);
        }

        rc = uiTabElm.setUiAttributeTitle(aGuiTreeElm.translateToNonQuotedUnicode(tabCaption[0]));
        if(rc != 0)
        {
            logging("Error: Failed to setup attribute 'title'.");
            return(-1);
        }

        // delete used entry
        tabCaption.removeAt(0);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTPageControl(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    QString guitreeTabPos;
    QString uiTabPos;
    aNextUiDomElement = aUiElm.createUiWidget("QTabWidget", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyCurrentIndex(QString::number(aGuiTreeElm.getNumberOfGuiObject(aGuiTreeElm.getDomProperty( "ActivePage", ""))) );
    if(rc != 0)
    {
        logging("Warning: Failed to set currentIndex to \"" + aGuiTreeElm.getDomProperty( "ActivePage") + "\"");
        return(-1);
    }

    guitreeTabPos = aGuiTreeElm.getDomProperty( "TabPosition", "tbTop");
    if(0 == guitreeTabPos.compare("tbLeft"))
        uiTabPos = "QTabWidget::West";
    else if(0 == guitreeTabPos.compare("tbTop"))
        uiTabPos = "QTabWidget::North";
    else if(0 == guitreeTabPos.compare("tpRight"))
        uiTabPos = "QTabWidget::East";
    else
        uiTabPos = "QTabWidget::South";

    rc = aNextUiDomElement.setUiPropertyTabPosition(uiTabPos);
    if(rc != 0)
    {
        logging("Warning: Failed to set currentIndex to \"" + aGuiTreeElm.getDomProperty( "ActivePage") + "\"");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTTabSheet(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QWidget", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiAttributeTitle(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Caption","")));
    if(rc != 0)
    {
        logging("Error: Failed to setup attribute 'title'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTButton(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QColor color;

    aNextUiDomElement = aUiElm.createUiWidget("QPushButton", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyText(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Caption","" /* no default*/ )));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'text'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyFlat(aGuiTreeElm.getDomProperty( "Flat", "false").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'flat'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    rc = aGuiTreeElm.getColor(color, "Font.Color");  // set only if an supported entry exist, instead use default color
    if(rc == 0)
    {
        rc = aNextUiDomElement.setUiPropertyPalette("WindowText", color);
        if(rc != 0)
        {
            logging("Error: Failed to create property 'Palette'.");
            return(-1);
        }
    }

    return(0);
}

/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTLabel(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QString dfmBorderStyle;
    QColor color;

    aNextUiDomElement = aUiElm.createUiWidget("QLabel", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyText(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Caption","" /* no default*/ )));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'text'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    // this is only used for TStaticText
    dfmBorderStyle = aGuiTreeElm.getDomProperty("BorderStyle");
    if(0 == dfmBorderStyle.compare("sbsSingle"))
    {
        rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::Panel");
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShape'.");
            return(-1);
        }

        rc = aNextUiDomElement.setUiPropertyFrameShadow("QFrame::Plain");
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShadow'.");
            return(-1);
        }
    }
    else if(0 == dfmBorderStyle.compare("sbsSunken"))
    {
        rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::Panel");
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShape'.");
            return(-1);
        }

        rc = aNextUiDomElement.setUiPropertyFrameShadow("QFrame::Sunken");
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShadow'.");
            return(-1);
        }
    }

    rc = aGuiTreeElm.getColor(color, "Font.Color");  // set only if an supported entry exist, instead use default color
    if(rc == 0)
    {
        rc = aNextUiDomElement.setUiPropertyPalette("WindowText", color);
        if(rc != 0)
        {
            logging("Error: Failed to create property 'Palette'.");
            return(-1);
        }
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTEdit(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QLineEdit", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyText(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Text","" /* no default*/ )));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'text'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTPanel(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QString bevelInner, bevelOuter;
    QColor color;

    aNextUiDomElement = aUiElm.createUiWidget("QFrame", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyText(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Caption","" /* no default*/ )));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'text'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyAlignment(QString("Qt::AlignCenter"));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Alignment'.");
        return(-1);
    }


    rc = aNextUiDomElement.setUiPropertyLayoutDirection(QString("Qt::LeftToRight"));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'layoutDirection'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    rc = aGuiTreeElm.getColor(color, "Font.Color");  // set only if an supported entry exist, instead use default color
    if(rc == 0)
    {
        rc = aNextUiDomElement.setUiPropertyPalette("WindowText", color);
        if(rc != 0)
        {
            logging("Error: Failed to create property 'Palette'.");
            return(-1);
        }
    }



    bevelInner = aGuiTreeElm.getDomProperty("BevelInner", "bvNone");
    bevelOuter = aGuiTreeElm.getDomProperty("BevelOuter", "bvRaised");
    if(0 == bevelInner.compare("bvLowered"))
    {
        if(0 == bevelOuter.compare("bvLowered"))
        {
            rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::WinPanel");
            if(rc != 0)
            {
                logging("Error: Failed to set property 'frameShadow'.");
                return(-1);
            }

            rc = aNextUiDomElement.setUiPropertyFrameShadow(QString("QFrame::Sunken"));
            if(rc != 0)
            {
                logging("Error: Failed to create property 'frameShadow'.");
                return(-1);
            }

        }
        else if(0 == bevelOuter.compare("bvNone"))
        {
            rc = aNextUiDomElement.setUiPropertyFrameShape(QString("QFrame::Panel"));
            if(rc != 0)
            {
                logging("Error: Failed to create property 'frameShape'.");
                return(-1);
            }

            rc = aNextUiDomElement.setUiPropertyFrameShadow(QString("QFrame::Sunken"));
            if(rc != 0)
            {
                logging("Error: Failed to create property 'frameShadow'.");
                return(-1);
            }
        }
        else
        {
            rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::Box");
            if(rc != 0)
            {
                logging("Error: Failed to set property 'frameShadow'.");
                return(-1);
            }

            rc = aNextUiDomElement.setUiPropertyFrameShadow(QString("QFrame::Raised"));
            if(rc != 0)
            {
                logging("Error: Failed to create property 'frameShadow'.");
                return(-1);
            }
        }
    }
    else if(0 == bevelInner.compare("bvNone"))
    {
        if(0 == bevelOuter.compare("bvLowered"))
        {
            rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::Panel");
            if(rc != 0)
            {
                logging("Error: Failed to set property 'frameShadow'.");
                return(-1);
            }

            rc = aNextUiDomElement.setUiPropertyFrameShadow(QString("QFrame::Sunken"));
            if(rc != 0)
            {
                logging("Error: Failed to create property 'frameShadow'.");
                return(-1);
            }

        }
        else if(0 == bevelOuter.compare("bvNone"))
        {
            rc = aNextUiDomElement.setUiPropertyFrameShape(QString("QFrame::NoFrame"));
            if(rc != 0)
            {
                logging("Error: Failed to create property 'frameShape'.");
                return(-1);
            }
        }
        else
        {
            rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::Panel");
            if(rc != 0)
            {
                logging("Error: Failed to set property 'frameShadow'.");
                return(-1);
            }

            rc = aNextUiDomElement.setUiPropertyFrameShadow(QString("QFrame::Raised"));
            if(rc != 0)
            {
                logging("Error: Failed to create property 'frameShadow'.");
                return(-1);
            }
        }
    }
    else
    {
        if(0 == bevelOuter.compare("bvLowered"))
        {
            rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::Box");
            if(rc != 0)
            {
                logging("Error: Failed to set property 'frameShadow'.");
                return(-1);
            }

            rc = aNextUiDomElement.setUiPropertyFrameShadow(QString("QFrame::Sunken"));
            if(rc != 0)
            {
                logging("Error: Failed to create property 'frameShadow'.");
                return(-1);
            }

        }
        else if(0 == bevelOuter.compare("bvNone"))
        {
            rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::Panel");
            if(rc != 0)
            {
                logging("Error: Failed to set property 'frameShadow'.");
                return(-1);
            }

            rc = aNextUiDomElement.setUiPropertyFrameShadow(QString("QFrame::Raised"));
            if(rc != 0)
            {
                logging("Error: Failed to create property 'frameShadow'.");
                return(-1);
            }
        }
        else
        {
            rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::WinPanel");
            if(rc != 0)
            {
                logging("Error: Failed to set property 'frameShadow'.");
                return(-1);
            }

            rc = aNextUiDomElement.setUiPropertyFrameShadow(QString("QFrame::Raised"));
            if(rc != 0)
            {
                logging("Error: Failed to create property 'frameShadow'.");
                return(-1);
            }
        }
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTGroupBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QGroupBox", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyTitle(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Caption","" /* no default*/ )));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'title'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTCheckBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QCheckBox", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyText(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Caption","" /* no default*/ )));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'text'.");
        return(-1);
    }

    if(0 == aGuiTreeElm.getDomProperty( "State", "cbUnchecked").compare("cbGrayed"))
    {
        rc = aNextUiDomElement.setUiPropertyCheckable("false");
        if(rc != 0)
        {
            logging("Error: Failed to setup property 'checkable'.");
            return(-1);
        }
    }
    else
    {
        rc = aNextUiDomElement.setUiPropertyChecked((0 == aGuiTreeElm.getDomProperty( "State", "cbUnchecked").compare("cbChecked"))? "true": "false");
        if(rc != 0)
        {
            logging("Error: Failed to setup property 'checked'.");
            return(-1);
        }
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTRadioButton(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QRadioButton", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyText(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Caption","" /* no default*/ )));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'text'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyChecked(aGuiTreeElm.getDomProperty( "Checked", "False").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'checked'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyLayoutDirection((0 == aGuiTreeElm.getDomProperty( "Alignment", "taRightJustify").compare("taRightJustify")? "Qt::LeftToRight": "Qt::RightToLeft"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'layoutDirection'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTImage(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QGraphicsView", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTStatusBar(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QStatusBar", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTScrollBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QScrollArea", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget QScrollArea.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    aNextUiDomElement = aNextUiDomElement.createUiWidget("QWidget", aGuiTreeValueName+"Contents");
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget QWidget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTCSpinEdit(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QSpinBox", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMinimum(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "MinValue", "-9999")));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'minimum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMaximum(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "MaxValue", "9999")));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'maximum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertySingleStep(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Increment", "1")));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'singleStep'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyValue(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Value", "0")));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'value'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTMainMenu(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QRect formGeometry;
    CGuiTreeDomElement rootElement;

    aNextUiDomElement = aUiElm.createUiWidget("QMenuBar", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }


    rootElement = aGuiTreeElm.getRootGuiObject();
    formGeometry = rootElement.getPropertyGeometry();


    rc = aNextUiDomElement.setUiPropertyGeometry("0","0",
                                                 QString::number(formGeometry.width()),
                                                 aGuiTreeElm.getDomProperty( "Height", "21"));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}

/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTToolBar(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QRect formGeometry;
    CGuiTreeDomElement rootElement;

    aNextUiDomElement = aUiElm.createUiWidget("QToolBar", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry("0","22",
                                                 QString::number(formGeometry.width()),
                                                 aGuiTreeElm.getDomProperty( "Height", "21"));
    rootElement = aGuiTreeElm.getRootGuiObject();
    formGeometry = rootElement.getPropertyGeometry();

    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}

/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTMenuItem(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    /* TMenuItem might be a QMenu or a QAction.
     * 1. if parent is TMainMenu it has to be a QMenu-widget
     * 2. or if TMenuItem has sub-TMenuItem's it has to be a QMenu-widget
     * 3. else if the DFM Caption property is "'-'" it has to be an "addaction"-element with name-attr "separator"
     * 3. else it has to be just an "addaction"-element and also an QAction widget by root widget
     */

    if(aGuiTreeElm.parentNode().getDomProperty("class") == "TMainMenu" ||
       aGuiTreeElm.hasGuiObjectChildNodes())
    {
        aNextUiDomElement = aUiElm.createUiWidget("QMenu", aGuiTreeValueName);
        if(aNextUiDomElement.isNull())
        {
            logging("Error: Failed to create ui widget.");
            return(-1);
        }

        // add element to parent node
        if(aGuiTreeElm.getDomProperty("Action","") == "")
            rc = aUiElm.addUiAddAction(aGuiTreeValueName);
        else
            rc = aUiElm.addUiAddAction(aGuiTreeElm.getDomProperty("Action"));
        if(rc != 0)
        {
            logging("Error: Failed to add an 'addaction' entry.");
            return(-1);
        }

        rc = aNextUiDomElement.setUiPropertyTitle(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Caption")));
        if(rc != 0)
        {
            logging("Error: Failed to setup property 'title'.");
            return(-1);
        }

        rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
        if(rc != 0)
        {
            logging("Error: Failed to setup property 'enabled'.");
            return(-1);
        }

        rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
        if(rc != 0)
        {
            logging("Error: Failed to create property 'toolTip'.");
            return(-1);
        }
    }
    else if(0 == aGuiTreeElm.getDomProperty("Caption").compare("'-'"))
    {
        rc = aUiElm.addUiAddAction("separator");
        if(rc != 0)
        {
            logging("Error: Failed to add an 'addaction' entry.");
            return(-1);
        }

        // The global action has not to be added!
    }
    else
    {
        if(aGuiTreeElm.getDomProperty("Action","") == "")
            rc = aUiElm.addUiAddAction(aGuiTreeValueName);
        else
            rc = aUiElm.addUiAddAction(aGuiTreeElm.getDomProperty("Action"));
        if(rc != 0)
        {
            logging("Error: Failed to add an 'addaction' entry.");
            return(-1);
        }

        if(aGuiTreeElm.getDomProperty("Action","") == "")
            rc = aUiElm.addUiGlobalAction(aGuiTreeValueName, aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Caption")));
        if(rc != 0)
        {
            logging("Error: Failed to add an global 'action' entry.");
            return(-1);
        }
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTActionList(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{

}

/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTProgressBar(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QProgressBar", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyValue(aGuiTreeElm.getDomProperty("Position", "0"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'value'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyOrientation(aGuiTreeElm.getDomProperty( "Orientation", "pbHorizontal") == "pbVertical"? "Qt::Vertical" : "Qt::Horizontal");
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'orinentation'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMinimum(aGuiTreeElm.getDomProperty( "Min", "0"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'minimum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMaximum(aGuiTreeElm.getDomProperty( "Max", "100"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'maximum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTCGauge(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QProgressBar", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyValue(aGuiTreeElm.getDomProperty("Progress", "0"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'value'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyTextVisible(aGuiTreeElm.getDomProperty("ShowText", "True").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'value'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyOrientation(aGuiTreeElm.getDomProperty( "Kind", "gkHorizontalBar") == "gkHorizontalBar"? "Qt::Horizontal" : "Qt::Vertical");
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'orinentation'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMinimum(aGuiTreeElm.getDomProperty( "MinValue", "0"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'minimum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMaximum(aGuiTreeElm.getDomProperty( "MaxValue", "100"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'maximum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyAlignment("Qt::AlignCenter");
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'maximum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTCSpinBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QSpinBox", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMinimum(aGuiTreeElm.getDomProperty( "MinValue", "0"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'minimum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMaximum(aGuiTreeElm.getDomProperty( "MaxValue", "100"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'maximum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyValue(aGuiTreeElm.getDomProperty("Value", "0"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'value'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertySingleStep(aGuiTreeElm.getDomProperty("Increment", "1"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'value'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTScrollBar(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QScrollBar", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyOrientation(aGuiTreeElm.getDomProperty( "Kind", "gkHorizontalBar") == "gkHorizontalBar"? "Qt::Horizontal" : "Qt::Vertical");
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'orinentation'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMinimum(aGuiTreeElm.getDomProperty( "Min", "0"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'minimum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMaximum(aGuiTreeElm.getDomProperty( "Max", "100"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'maximum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyValue(aGuiTreeElm.getDomProperty("Position", "0"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'value'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertySingleStep(aGuiTreeElm.getDomProperty("Increment", "1"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'value'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTMemo(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QString dfmBorderStyle;
    QString dfmLines;

    aNextUiDomElement = aUiElm.createUiWidget("QPlainTextEdit", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    dfmBorderStyle = aGuiTreeElm.getDomProperty("BorderStyle", "bsSingle");
    if(0 == dfmBorderStyle.compare("bsSingle"))
    {
        rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::Panel");
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShape'.");
            return(-1);
        }

        rc = aNextUiDomElement.setUiPropertyFrameShadow("QFrame::Sunken");
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShadow'.");
            return(-1);
        }
    }
    else // bsNone
    {
        rc = aNextUiDomElement.setUiPropertyFrameShape("QFrame::NoFrame");
        if(rc != 0)
        {
            logging("Error: Failed to create property 'frameShape'.");
            return(-1);
        }
    }

    dfmLines = aGuiTreeElm.getItemsOf("Lines.Strings");
    rc = aNextUiDomElement.setUiPropertyPlainText(dfmLines);
    if(rc != 0)
    {
        logging("Error: Failed to create property 'plainText'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTTrackBar(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QString tickStyle;

    aNextUiDomElement = aUiElm.createUiWidget("QSlider", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMinimum(aGuiTreeElm.getDomProperty( "Min", "0"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'minimum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyMaximum(aGuiTreeElm.getDomProperty( "Max", "100"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'maximum'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyValue(aGuiTreeElm.getDomProperty("Position", "0"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'value'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyOrientation(aGuiTreeElm.getDomProperty( "Orientation", "trHorizontal") == "trHorizontal"? "Qt::Horizontal" : "Qt::Vertical");
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'orinentation'.");
        return(-1);
    }

    tickStyle = aGuiTreeElm.getDomProperty("TickMarks");
    if(tickStyle == "tmTopLeft")
        rc = aNextUiDomElement.setUiPropertyTickPosition("QSlider::TicksAbove");
    else if(tickStyle == "tmBottomRight")
        rc = aNextUiDomElement.setUiPropertyTickPosition("QSlider::TicksBelow");
    else
        rc = aNextUiDomElement.setUiPropertyTickPosition("QSlider::TicksBothSides");
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'tickPosition'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTTreeView(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QTreeWidget", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTCheckListBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QStringList items;

    aNextUiDomElement = aUiElm.createUiWidget("QListWidget", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    items = aGuiTreeElm.getItemListOf("Items.Strings");
    rc = aNextUiDomElement.addUiItemList(items, "Unchecked" /* there are no other info in DFM */);
    if(rc != 0)
    {
        logging("Error: Failed to add items.");
        return(-1);
    }

    return(0);
}


/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTComboBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QStringList items;

    aNextUiDomElement = aUiElm.createUiWidget("QComboBox", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    items = aGuiTreeElm.getItemListOf("Items.Strings");
    rc = aNextUiDomElement.addUiItemList(items);
    if(rc != 0)
    {
        logging("Error: Failed to add items.");
        return(-1);
    }

    return(0);
}

/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTRadioGroup(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QStringList items;

    aNextUiDomElement = aUiElm.createUiWidget("QGroupBox", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }
    rc = aNextUiDomElement.setUiPropertyTitle(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Caption","" /* no default*/ )));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'title'.");
        return(-1);
    }
    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    items = aGuiTreeElm.getItemListOf("Items.Strings");
    aNextUiDomElement.setUiPropertyLayoutDirection("Vertical");
    foreach(QString item, items)
    {
        CUiDomElement radiobuttonChild = aUiElm.createUiWidget("QRadioButton",item.replace(" ","_"));
        radiobuttonChild.setUiPropertyText(item);
        aNextUiDomElement.appendChild(radiobuttonChild);
    }

    //rc = aNextUiDomElement.addUiItemList(items);
    //if(rc != 0)
    //{
//        logging("Error: Failed to add items.");
//        return(-1);
//    }

    return(0);
}

/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTListBox(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QStringList items;

    aNextUiDomElement = aUiElm.createUiWidget("QListWidget", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    items = aGuiTreeElm.getItemListOf("Items.Strings");
    rc = aNextUiDomElement.addUiItemList(items);
    if(rc != 0)
    {
        logging("Error: Failed to add items.");
        return(-1);
    }

    return(0);
}

/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTDBGrid(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;
    QStringList items;

    aNextUiDomElement = aUiElm.createUiWidget("QTableWidget", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true").toLower());
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }

    items = aGuiTreeElm.getItemListOf("Columns");
//    rc = aNextUiDomElement.addUiItemList(items);
//    if(rc != 0)
//    {
//        logging("Error: Failed to add items.");
//        return(-1);
//    }

    return(0);
}


/**
 * Convert givin guiObject and all sub ordered guiObjects to QT4 UI Objects.
 *
 * @param uiDoc DOM document of ui structure.
 * @param uiElm Current used DOM element of ui structure.
 * @param domDocGuiTree DOM document of guitree structure.
 * @param guiTreeElmIsFrist This is true if the givin DOM element of guiTree structure is the first one.
 * @param guiTreeElm Current used DOM element of sourced guiTree structure.
 * @return 0 is OK else anything failed.
 *
 *
 */
int CGuiTree2Ui::convertGuiObject(CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, bool aGuiTreeElmIsFirst)
{
    QString guiTreeValueClass;
    QString guiTreeValueName;
    CGuiTreeDomElement guiTreeChildElm;
    CUiDomElement aNextUiDomElement;
    QDomNodeList guiTreeList;
    bool aNextUiDomElementHasUnknownType = false;
    int rc;

    // Contition - abort recursive loop?
    if(aGuiTreeElm.isNull())
        return(0);

    // Init
    QDomImplementation impl;
    impl.setInvalidDataPolicy(QDomImplementation::ReturnNullNode);

    // read in name of instanceof gui element
    rc = aGuiTreeElm.getDomPropertySecure(guiTreeValueName, "name");
    if(rc != 0)
    {
        logging("Error: No name of instance");
        return(-1);
    }

    // read in class name of gui element
    rc = aGuiTreeElm.getDomPropertySecure(guiTreeValueClass, "class");
    if(rc != 0)
    {
        logging("Error: No class type");
        return(-1);
    }
    // logging("Class = " + guiTreeValue);

    // First one have to be the form
    if(aGuiTreeElmIsFirst)
    {
        rc = convertForm(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TBevel")  // look for classes
    {
        rc = this->convertTBevel(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TTabControl")
    {
        rc = this->convertTTabControl(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TPageControl")
    {
        rc = this->convertTPageControl(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TTabSheet")
    {
        rc = this->convertTTabSheet(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TButton" ||
            guiTreeValueClass == "TBitBtn" ||
            guiTreeValueClass == "TSpeedButton")
    {
        rc = this->convertTButton(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TLabel" ||
            guiTreeValueClass == "TStaticText")
    {
        rc = this->convertTLabel(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TMaskEdit")
    {
        rc = this->convertTEdit(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
        logging("Info: Please adjust inputMask of " + guiTreeValueName + ":QEdit converted from TMaskEdit manually.");
    }
    else if(guiTreeValueClass == "TEdit" ||
            guiTreeValueClass == "TOvcNumericField" ||
            guiTreeValueClass == "TOvcEdit")
    {
        rc = this->convertTEdit(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TPanel")
    {
        rc = this->convertTPanel(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TGroupBox")
    {
        rc = this->convertTGroupBox(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TCheckBox")
    {
        rc = this->convertTCheckBox(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TRadioButton")
    {
        rc = this->convertTRadioButton(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TImage")
    {
        rc = this->convertTImage(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TStatusBar")
    {
        rc = this->convertTStatusBar(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TScrollBox")
    {
        rc = this->convertTScrollBox(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TCSpinEdit")
    {
        rc = this->convertTCSpinEdit(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TLMDSpinEdit")
    {
        rc = this->convertTCSpinEdit(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TSpinEdit")
    {
        rc = this->convertTCSpinEdit(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TOvcDateEdit")
    {
        rc = this->convertTOvcDateEdit(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TOvcAttachedLabel")
    {
        rc = this->convertTLabel(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TOvcPictureField")
    {
        rc = this->convertTEdit(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
        logging("Info: Please adjust inputMask of " + guiTreeValueName + ":QEdit converted from TMaskEdit manually.");
    }
    else if(guiTreeValueClass == "TDBGrid")
    {
        rc = this->convertTDBGrid(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TMainMenu")
    {
        rc = this->convertTMainMenu(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TMenuItem")
    {
        rc = this->convertTMenuItem(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TToolBar")
    {
        rc = this->convertTToolBar(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TPopupMenu")
    {
        logging("Warning: QT4-Lib has no equivalent to TPopupMenu");
    }
    else if(guiTreeValueClass == "TProgressBar")
    {
        rc = this->convertTProgressBar(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TCGauge")
    {
        rc = this->convertTCGauge(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TCSpinBox")
    {
        rc = this->convertTCSpinBox(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TCSpinButton")
    {
        logging("Warning: QT4-Lib has no equivalent to TCSpinButton");
    }
    else if(guiTreeValueClass == "TScrollBar")
    {
        rc = this->convertTScrollBar(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TMemo")
    {
        rc = this->convertTMemo(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TTrackBar")
    {
        rc = this->convertTTrackBar(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TTreeView")
    {
        rc = this->convertTTreeView(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TRadioGroup")
    {
        //logging("Warning: QT4-Lib has no equivalent to TRadioGroup");
        this->convertTRadioGroup(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TCheckListBox")
    {
        rc = this->convertTCheckListBox(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TComboBox" ||
            guiTreeValueClass == "TOvcComboBox")
    {
        rc = this->convertTComboBox(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else if(guiTreeValueClass == "TListBox" ||
            guiTreeValueClass == "TOvcListBox" ||
            guiTreeValueClass == "TLMDListBox")
    {
        rc = this->convertTListBox(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }
    else
    {
        aNextUiDomElementHasUnknownType = true;
        logging(QString("Warning: Unknown class '") + guiTreeValueName + ":" + guiTreeValueClass + QString("'."));

        rc = this->convertUnknownWidget(aNextUiDomElement, aUiElm, aGuiTreeElm, guiTreeValueName);
        if(rc != 0)
            return(-1);
    }

    // convert all child gui objects
    guiTreeList = aGuiTreeElm.childNodes();
    for(int i = 0; i < guiTreeList.count(); i++)
    {
        if("guiObject" == guiTreeList.item(i).nodeName())
        {
            guiTreeChildElm = (CGuiTreeDomElement)guiTreeList.item(i).toElement();

            if(aNextUiDomElementHasUnknownType)
            {
                guiTreeValueName = guiTreeChildElm.getDomProperty("name");
                guiTreeValueClass = guiTreeChildElm.getDomProperty("class");
                logging("    Skip child element (" + guiTreeValueName + ":" + guiTreeValueClass + ")");
            }
            else
            {
                if(aNextUiDomElement.isNull())
                    aNextUiDomElement = aUiElm;
                rc = convertGuiObject(aNextUiDomElement, guiTreeChildElm, false /* not the first element, it doesn't matter if one was added */);
                if(rc != 0)
                    return(-1);
            }
        }
    }

    return(0);
}


/**
 * @brief Convert givin guiTree to QT4 UI format.
 *
 * @return 0 = OK
 * @return -1 = Failed
 */
int CGuiTree2Ui::convert(CUiDomDocument *domDocUi, CGuiTreeDomDocument *domDocGuiTree)
{

    CUiDomElement domElmUi;
    CGuiTreeDomElement domElmGuiTree;
    int rc;

    // get guiTree element node
    domElmGuiTree = domDocGuiTree->getFirstGuiObjectElement();
    if(domElmGuiTree.isNull())
    {
        logging("Empty guiTree");
        return(-1);
    }

    // get ui element node
    domElmUi = domDocUi->init();

    rc = convertGuiObject(domElmUi, domElmGuiTree, true /* this is the first DOM element after root element */);

    return(rc);
}

/**
 * Convert specified widget
 *
 * @param[out] aNextUiDomElement Element which should be use for next convertion.
 * @param[in] aUiElm UI element
 * @param[in] aGuiTreeElm GuiTree element
 * @param[in] aGuiTreeValueName This is the name of Object.
 * @return 0 on success.
 */
int CGuiTree2Ui::convertTOvcDateEdit(CUiDomElement &aNextUiDomElement, CUiDomElement &aUiElm, CGuiTreeDomElement &aGuiTreeElm, const QString &aGuiTreeValueName)
{
    int rc;

    aNextUiDomElement = aUiElm.createUiWidget("QDateEdit", aGuiTreeValueName);
    if(aNextUiDomElement.isNull())
    {
        logging("Error: Failed to create ui widget.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyGeometry(aGuiTreeElm.getPropertyGeometry());
    if(rc != 0)
    {
        logging("Error: Failed to create property 'Geometry'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyText(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty( "Text","" /* no default*/ )));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'text'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyEnabled(aGuiTreeElm.getDomProperty( "Enabled", "true"));
    if(rc != 0)
    {
        logging("Error: Failed to setup property 'enabled'.");
        return(-1);
    }

    rc = aNextUiDomElement.setUiPropertyToolTip(aGuiTreeElm.translateToNonQuotedUnicode(aGuiTreeElm.getDomProperty("Hint")));
    if(rc != 0)
    {
        logging("Error: Failed to create property 'toolTip'.");
        return(-1);
    }
    return(0);
}
