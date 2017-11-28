/*
 * widgets.h
 *
 *  Created on: Jan 8, 2013
 *      Author: sushil
 */

#ifndef WIDGETS_H_
#define WIDGETS_H_

#include <OgreVector2.h>
#include <OgreColourValue.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OISMouse.h>


#include <inputHandlers.h>
#include "commandFactory.h"
#include "GraphicsInteractionManager.h"




namespace OgreGFX
{

static std::string DEFAULT_PANEL_MATERIAL = "ECSLENT/navy/material/shipInfo/overlay";
static Ogre::Vector2 DEFAULT_LABEL_SIZE = Ogre::Vector2(100, 20);

enum Placement
{
    Below = 0,
    Right = 1
};

class GraphicsInteractionManager;

/*! The base class for all GUI interface elements. This class mostly provides virtual interfaces for
 * subclasses to implement later. However, the Widget class does implement a few basic functions related to positioning.
 */
class Widget
{
public:
    Ogre::Vector2 pos; /*! Upper-right corner position of the widget, in absolute coordinates if this is a parent node, or relative to the parent node if this is a child node */
    Ogre::Vector2 size; /*! length and height of the widget */
    Widget *parent; /*! Parent node, if any*/
    GraphicsInteractionManager *gfx; /*! Graphics interaction manager singleton */
    Ogre::GuiMetricsMode mode;
    std::string desc; /*! Description of the widget to display to the user */

    /*!
     * Constructor for basic widget requirements.
     * @param gim a pointer to the global Graphics Interaction Manager, to display widget.
     * @param aParent a pointer to the parent node, if any (null if this is a root element, or pointer to another widget if this is a child of that widget). Defaults to NULL.
     * @param defaultPos Upper-right corner position of the widget, in absolute coordinates if this is a parent node, or relative to the parent node if this is a child node.
     * @param defaultSize length and height of the widget.
     * @param defaultMode Mode for how size/position are interpreted (relative/absolute)
     * @see ~Widget()
     * @return void
     */
    Widget(GraphicsInteractionManager *gim, Widget *aParent = 0, Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X, Ogre::Vector2 defaultSize = Ogre::Vector2::NEGATIVE_UNIT_X, Ogre::GuiMetricsMode
    defaultMode = Ogre::GMM_RELATIVE);
    ~Widget();

    /*!
     * Function to return the widget's upper-right corner position. Position returned is always in absolute screen coordinates, regardless of widget being a parent or child node.
     * @see pos
     * @return Vector2 of the absolute screen coordinates of the widget's upper-right corner.
     */
    Ogre::Vector2 screenPos();

    /*!
     * Function to check if the mouse is currently inside the widget's area.
     * @param pos Current absolute screen position of the mouse cursor.
     * @see screenPos()
     * @return True if the mouse is inside the widget at the time the function is called, false otherwise.
     */
    bool cursonInMe(const Ogre::Vector2 &pos);

    /*!
     * Updates the relative position of all sub-elements/widgets
     * @see screenPos()
     */
    virtual void posChanged()
    {
    };

    /*!
     * Draws and displays the widget and all sub-elements.
     * @see hide()
     */
    virtual void show()
    {
    };

    /*!
     * Hides the widget and all sub-elements.
     * @see show()
     */
    virtual void hide()
    {
    };

    /*!
     * Returns the overlay element used to display the widgets appearance.
     * @return Pointer to the resource that displays the widgets appearance.
     */
    virtual Ogre::OverlayElement* getOverlayElementToAdd()
    {
        return 0;
    };

};

class UIOutline : public Widget
{
public:

    Ogre::OverlayManager *overlayManagerPtr;
    Ogre::Overlay* overlay;
    virtual void show();
    virtual void hide();

    UIOutline(GraphicsInteractionManager *gim);
    ~UIOutline();

};

class FEProgressBar : public Widget
{
private:

public:
    FEProgressBar(GraphicsInteractionManager *gim, std::string name = "FEProgressBar", Widget* aParent = 0,
            Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X, Ogre::Vector2 defaultSize = Ogre::Vector2::NEGATIVE_UNIT_X,
            std::string barMaterial = DEFAULT_PANEL_MATERIAL, std::string fillMaterial = DEFAULT_PANEL_MATERIAL, Ogre::GuiMetricsMode defaultMode = Ogre::GMM_RELATIVE); //: Widget(gim) {};
    ~FEProgressBar();

    //Ogre::Overlay* overlayPtr;
    Ogre::OverlayManager* overlayManagerPtr;
    Ogre::OverlayContainer *progressBar;
    //Ogre::OverlayContainer *progressFill;
    std::string barMaterial; /*! The name of the texture to display */
    std::string fillMaterial; /*! The name of the texture to display */
    std::string name; /*! The name given to this progress bar */
    double percentFull;
    
    void UpdateFillPercent();
    virtual void show();
    virtual void hide();
    virtual void posChanged();
    virtual Ogre::OverlayElement* getOverlayElementToAdd();

};

/*! A basic type of widget used for defining an area where multiple widgets will be placed.
 * Also displays a background texture on which the widgets display over.
 */
class FEPanel : public Widget
{
private:

public:
    /*!
     * Constructor for a panel widget.
     * @param gim a pointer to the global Graphics Interaction Manager, to display widget.
     * @param name the name given to the panel object
     * @param aParent a pointer to the parent node, if any (null if this is a root element, or pointer to another widget if this is a child of that widget). Defaults to NULL.
     * @param defaultPos Upper-right corner position of the widget, in absolute coordinates if this is a parent node, or relative to the parent node if this is a child node.
     * @param defaultSize length and height of the widget.
     * @param material  The name of the texture to display in the background.
     * @param defaultMode Mode for how size/position are interpreted (relative/absolute)
     * @see ~FEPanel()
     */
    FEPanel(GraphicsInteractionManager *gim, std::string name = "FEPanel", Widget* aParent = 0,
            Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X, Ogre::Vector2 defaultSize = Ogre::Vector2::NEGATIVE_UNIT_X,
            std::string material = DEFAULT_PANEL_MATERIAL, Ogre::GuiMetricsMode defaultMode = Ogre::GMM_RELATIVE); //: Widget(gim) {};
    ~FEPanel();

    Ogre::Overlay* overlayPtr;
    Ogre::OverlayManager* overlayManagerPtr;
    Ogre::OverlayContainer *panel;
    //Ogre::TextAreaOverlayElement *textArea; /*! The visual element used to display text */
    std::string material; /*! The name of the texture to display in the background*/
    std::string name; /*! The name given to this panel */
    Ogre::Vector2 belowPos; /*! Next valid position for a widget below the previous row that had a widget added */
    Ogre::Vector2 rightPos; /*! Next valid position for a widget to the right of the previously added widget */
    Ogre::Vector2 gap; /*! the size of the gap between widgets on the x and y axis */
    std::list<Widget*> items; /*! A list of pointers to all widgets contained in this panel */
    std::list<Ogre::OverlayElement*> separators; /*! an element that visually separates each item in the item list */
    int separatorHeight; /*! the height of each separator element */


    virtual void show();
    virtual void hide();
    virtual void posChanged();

    /*!
     * Adds a new widget to the panel
     * @param item A pointer to the widget to add to the panel.
     * @param placement Where to place the new widget in the panel. Either to the right of the previously added widget, or at the start of the next row below the previously added widget.
     * @return void
     */
    void addItem(Widget *item, Placement placement = Below);

    //		void getItems();
    //		void getOverlayElementToAdd();

    void addSep(int separation);

    /*!
     * Adjust the location of the separators. Used if the size of the panel changes since the separators were added.
     */
    void adjustSeparators();
    /*!
     * Creates a new separator that fits the panel object.
     * @return OverlayElement pointer used to display the new separator.
     */
    Ogre::OverlayElement* makeSep();

    /*!
     * Gets the OverlayElement used to display the panel. Added this to see if it overwrite widget desc.
     * @return Pointer the OverlayElement used to display the panel.
     */
    virtual Ogre::OverlayElement* getOverlayElementToAdd();

};

/*! A basic type of widget used for displaying text.
 */
class FELabel : public Widget
{
public:
    /*!
     * Constructor for a Label widget, which displays some text.
     * @param gim a pointer to the global Graphics Interaction Manager, to display widget.
     * @param caption The text to display in the panel
     * @param aParent A pointer to the parent node, if any (null if this is a root element, or pointer to another widget if this is a child of that widget). Defaults to NULL.
     * @param color The color of the displayed text. Defaults to red
     * @param defaultPos Upper-right corner position of the widget, in absolute coordinates if this is a parent node, or relative to the parent node if this is a child node.
     * @param defaultSize length and height of the widget.
     * @param defaultMode Mode for how size/position are interpreted (relative/absolute)
     * @see ~FELabel()
     */
    FELabel(GraphicsInteractionManager *gim, std::string caption = "FELabel",
            Widget *aParent = 0, Ogre::ColourValue color = Ogre::ColourValue::Red,
            Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X, Ogre::Vector2 defaultSize = DEFAULT_LABEL_SIZE, Ogre::GuiMetricsMode defaultMode = Ogre::GMM_RELATIVE);
    ~FELabel();

    Ogre::OverlayManager* overlayManagerPtr;
    std::string caption; /*! The text displayed by the label  */
    Ogre::ColourValue color; /*! The color of the text displayed  */
    Ogre::Vector2 offset; /*! Distance the text should stay away from the edges of the widget.  */
    Ogre::TextAreaOverlayElement* textArea; /*! Size of the text area within the widget, defined by widget size - offset  */

    /*!
     * Sets the text displayed by the label.
     * @param val The string to display
     * @return void
     */
    virtual void setCaption(std::string val);

    virtual void show();
    virtual void hide();
    virtual void posChanged();
    virtual Ogre::OverlayElement* getOverlayElementToAdd();

};

class FELineWrapLabel : public FELabel
{
public:
    FELineWrapLabel(GraphicsInteractionManager *gim, std::string caption = "FELineWrapLabel",
            Widget *aParent = 0, Ogre::ColourValue color = Ogre::ColourValue::Red,
            Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X, Ogre::Vector2 defaultSize = DEFAULT_LABEL_SIZE, Ogre::GuiMetricsMode defaultMode = Ogre::GMM_RELATIVE, double lines = 1);
            
    ~FELineWrapLabel();
    virtual void setCaption(std::string val);
    
    double lines;
    
};

/*! A widget which can display text or an image, and detects when
 * a mouse click occurs inside its area. If a mouse event does occur in its area,
 * a function associated with that button is called. Buttons consume the mouse click,
 * so the mouse click doesn't get passed on to any units that might exist beneath the overlay. 
 */

class FEButton : public FEPanel, public MouseHandler, public KeyHandler
{
public:
    /*!
     * Constructor for a Button widget, which displays some text. Clicking on the text causes a function to be called to perform some task.
     * @param gim a pointer to the global Graphics Interaction Manager, to display widget.
     * @param modifierKeys Pointer to a configuration of keyboard keys that must all be pressed for the button handler to be called.
     * @param oisMouseButtonId The ID of the mouse button that must be pressed for the button handler to be called.
     * @param name Name of the button
     * @param command CommandFactory that produces a command when the button is clicked
     * @param aParent A pointer to the parent node, if any (null if this is a root element, or pointer to another widget if this is a child of that widget). Defaults to NULL.
     * @param defaultPos Upper-right corner position of the widget, in absolute coordinates if this is a parent node, or relative to the parent node if this is a child node.
     * @param defaultSize length and height of the widget.
     * @param material Texture name to use for button image
     * @param desc Description for button displayed in button help window
     * @param defaultMode Mode for how size/position are interpreted (relative/absolute)
     * @see ~FEButton()
     */
    FEButton(GraphicsInteractionManager *gim, const OIS::KeyCode keyCode,
             const std::set<OIS::KeyCode> *modifierKeys, const OIS::MouseButtonID oisMouseButtonId,
             std::string name = "FEButton", FastEcslent::CommandFactory* command = NULL,
             Widget *aParent = 0,
             Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X,
             Ogre::Vector2 defaultSize = DEFAULT_LABEL_SIZE,std::string material = DEFAULT_PANEL_MATERIAL, std::string desc = "BLANK", Ogre::GuiMetricsMode defaultMode = Ogre::GMM_RELATIVE);
    ~FEButton();

    FastEcslent::CommandFactory* commandFactory; /*! CommandFactory that generates a command to give to selected entity on button press */
    bool pressed; /*! Stores if the button has been clicked since last update */
    bool enabled; /*! Stores if the button is available to be clicked or not. If false, clicking the button does not result in the associated function being called. */

    /*!
     * Mouse handler called when a mouse event occurs.
     * @param args Information about the mouse state (mouse button, cursor position, etc).
     * @return void
     */
    virtual void handleMouseEvent(const OIS::MouseEvent &args);
    virtual void handleKeyEvent();
    virtual void show();
    virtual void hide();
    bool processButtonFirstEvent(bool buttonActivated);
    bool processButtonSecondEvent(bool buttonActivated);
};

///*! A widget which can display text or an image, and detects when
// * a mouse click occurs inside its area. If a mouse event does occur in its area,
// * a function associated with that button is called. Buttons consume the mouse click,
// * so the mouse click doesn't get passed on to any units that might exist beneath the overlay. 
// */
//
//class FEButton : public FEPanel, public MouseHandler
//{
//public:
//    /*!
//     * Constructor for a Button widget, which displays some text. Clicking on the text causes a function to be called to perform some task.
//     * @param gim a pointer to the global Graphics Interaction Manager, to display widget.
//     * @param modifierKeys Pointer to a configuration of keyboard keys that must all be pressed for the button handler to be called.
//     * @param oisMouseButtonId The ID of the mouse button that must be pressed for the button handler to be called.
//     * @param caption The text to display in the panel
//     * @param aParent A pointer to the parent node, if any (null if this is a root element, or pointer to another widget if this is a child of that widget). Defaults to NULL.
//     * @param color The color of the displayed text. Defaults to red
//     * @param defaultPos Upper-right corner position of the widget, in absolute coordinates if this is a parent node, or relative to the parent node if this is a child node.
//     * @param defaultSize length and height of the widget.
//     * @param pClickFunction Pointer to a function, which is called when the button handler is called.
//     * @see ~FEButton()
//     */
//    FEButton(GraphicsInteractionManager *gim,
//             const std::set<OIS::KeyCode> *modifierKeys, const OIS::MouseButtonID oisMouseButtonId,
//             std::string name = "FEButton",
//             Widget *aParent = 0, Ogre::ColourValue color = Ogre::ColourValue::Red,
//             Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X,
//             Ogre::Vector2 defaultSize = DEFAULT_LABEL_SIZE, bool (*pClickFunction)() = NULL, std::string material = DEFAULT_PANEL_MATERIAL, std::string = "BLANK", Ogre::GuiMetricsMode defaultMode = Ogre::GMM_RELATIVE);
//    ~FEButton();
//
//    bool (*pClickFunction)();
//    bool pressed; /*! Stores if the button has been clicked since last update */
//    bool enabled; /*! Stores if the button is available to be clicked or not. If false, clicking the button does not result in the associated function being called. */
//    /*!
//     * Mouse handler called when a mouse event occurs.
//     * @param args Information about the mouse state (mouse button, cursor position, etc).
//     * @return void
//     */
//    virtual void handleMouseEvent(const OIS::MouseEvent &args);
//    virtual void show();
//    virtual void hide();
//};
//
///*! A Button widget that creates an SCV near the currently selected unit.
// */
//class FESCVButton : public FEButton
//{
//public:
//    /*!
//     * Constructor for a SCV button, which builds an SCV near the selected structure.
//     * @param gim a pointer to the global Graphics Interaction Manager, to display widget.
//     * @param modifierKeys Pointer to a configuration of keyboard keys that must all be pressed for the button handler to be called.
//     * @param oisMouseButtonId The ID of the mouse button that must be pressed for the button handler to be called.
//     * @param caption The text to display in the panel
//     * @param aParent A pointer to the parent node, if any (null if this is a root element, or pointer to another widget if this is a child of that widget). Defaults to NULL.
//     * @param color The color of the displayed text. Defaults to red
//     * @param defaultPos Upper-right corner position of the widget, in absolute coordinates if this is a parent node, or relative to the parent node if this is a child node.
//     * @param defaultSize length and height of the widget.
//     * @see ~FESCVButton()
//     */
//    FESCVButton(GraphicsInteractionManager *gim,
//                const std::set<OIS::KeyCode> *modifierKeys, const OIS::MouseButtonID oisMouseButtonId,
//                std::string caption = "FEButton",
//                Widget *aParent = 0, Ogre::ColourValue color = Ogre::ColourValue::Red,
//                Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X,
//                Ogre::Vector2 defaultSize = DEFAULT_LABEL_SIZE, Ogre::GuiMetricsMode defaultMode = Ogre::GMM_RELATIVE);
//    ~FESCVButton();
//
//    virtual void handleMouseEvent(const OIS::MouseEvent &args);
//};
//
///*! A Button widget that creates a Marine near the currently selected unit.
// */
//class FEMarineButton : public FEButton
//{
//public:
//    /*!
//     * Constructor for a Marine button, which builds an Marine near the selected structure.
//     * @param gim a pointer to the global Graphics Interaction Manager, to display widget.
//     * @param modifierKeys Pointer to a configuration of keyboard keys that must all be pressed for the button handler to be called.
//     * @param oisMouseButtonId The ID of the mouse button that must be pressed for the button handler to be called.
//     * @param caption The text to display in the panel
//     * @param aParent A pointer to the parent node, if any (null if this is a root element, or pointer to another widget if this is a child of that widget). Defaults to NULL.
//     * @param color The color of the displayed text. Defaults to red
//     * @param defaultPos Upper-right corner position of the widget, in absolute coordinates if this is a parent node, or relative to the parent node if this is a child node.
//     * @param defaultSize length and height of the widget.
//     * @see ~FEMarineButton()
//     */
//    FEMarineButton(GraphicsInteractionManager *gim,
//                   const std::set<OIS::KeyCode> *modifierKeys, const OIS::MouseButtonID oisMouseButtonId,
//                   std::string caption = "FEButton",
//                   Widget *aParent = 0, Ogre::ColourValue color = Ogre::ColourValue::Red,
//                   Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X,
//                   Ogre::Vector2 defaultSize = DEFAULT_LABEL_SIZE, Ogre::GuiMetricsMode defaultMode = Ogre::GMM_RELATIVE);
//    ~FEMarineButton();
//
//    virtual void handleMouseEvent(const OIS::MouseEvent &args);
//};

/*! A Panel widget specialized to display two labels horizontally adjacent.
 * Made up to two label widgets.
 */
class FELabelPair : public FEPanel
{
public:
    /*!
     * Constructor for a pair of labels, displayed side by side
     * @param gim a pointer to the global Graphics Interaction Manager, to display widget.
     * @param caption1 The left-most text to display.
     * @param caption2 The right-most text to display.
     * @param aParent A pointer to the parent node, if any (null if this is a root element, or pointer to another widget if this is a child of that widget). Defaults to NULL.
     * @param color The color of the displayed text. Defaults to green.
     * @param defaultPos Upper-right corner position of the widget, in absolute coordinates if this is a parent node, or relative to the parent node if this is a child node.
     * @param defaultSize length and height of the widget.
     * @param defaultColumnWidths default width of the panel containing both labels
     * @param columnHeight default height of the panel containing both labels
     * @see ~FEMarineButton()
     */
    FELabelPair(GraphicsInteractionManager *gim, std::string caption1 = "Key", std::string caption2 = "Value",
                Widget *aParent = 0, Ogre::ColourValue color = Ogre::ColourValue::Green,
                Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X,
                Ogre::Vector2 defaultColumnWidths = Ogre::Vector2(100, 200), int columnHeight = DEFAULT_LABEL_SIZE.y,
                std::string materialName = DEFAULT_PANEL_MATERIAL, Ogre::GuiMetricsMode defaultMode = Ogre::GMM_RELATIVE);

    /*dimensions/positions are in pixels */
    ~FELabelPair()
    {
    };

    FELabel *left, *right;

    virtual void show();
    virtual void hide();

    /*!
     * Sets the string to display for the left-most label.
     * @param val The string to display
     * @return void
     */
    void setLeft(std::string val);

    /*!
     * Sets the string to display for the right-most label.
     * @param val The string to display
     * @return void
     */
    void setRight(std::string val);

};


}


#endif /* WIDGETS_H_ */
