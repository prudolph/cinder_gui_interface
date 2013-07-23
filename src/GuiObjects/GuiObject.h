//
//  GuiObject.h
//  guiObjects
//
//  Created by Paul Rudolph on 7/2/13.
//
//

#pragma once
#include "TuioClient.h"

#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include <forward_list>


using namespace std;
using namespace ci;
using namespace cinder::app;
using namespace boost::signals2;
using namespace tuio;
    
class GuiObject {
   
public://anyone can gete access to this stuff with the "." accessor
    
    GuiObject();

   
    static  void draw(); //This can be called from anywhere and draws all the ojects
    virtual void drawObject();
    
   
    
    //Callback Functions
    signal<void(GuiObject *)>&	getSelectedSignal()         { return gui_OnSelectSignal; }
    void addCallBack(std::function <void(GuiObject*)> fn)   { gui_OnSelectSignal.connect(fn);};


    //Position
	virtual void		setPosition( float x, float y )     { gui_Position = Vec2f(x, y);}
	virtual void		setPosition( Vec2f pt )             { gui_Position = pt; }
	virtual Vec2f       getPosition()                       { return gui_Position; }
    
    //Size
    virtual void		setSize( float w, float h)          { gui_Width=w; gui_Height=h; }
	virtual void		setSize( Vec2f size )               { gui_Width=size.x; gui_Height=size.y; }
    virtual Vec2f       getSize()                           { return ci::Vec2f(gui_Width, gui_Height); }
    
    //Width
    virtual void		setWidth( float w)                  { gui_Width=w; }
    virtual float		getWidth()                          { return gui_Width; }
    
    //Height
	virtual void		setHeight( float h)                 { gui_Height=h; }
	virtual float		getHeight()                         { return gui_Height; }

    //Get position and size as a rectangle
    virtual Rectf       getRect()                           { return  Rectf(gui_Position.x,gui_Position.y,gui_Position.x+gui_Width,gui_Position.y+gui_Height); }
    
    //Color
    virtual void        setContainerColor(ColorA clr)       { gui_ContainerColor = clr;};
    ColorA              getContainerColor()                 { return gui_ContainerColor;};

    //Text
    void                setText(string s);
    void                updateTextBox();
    string              getText()                           { return gui_Text;};

    //Selected
    virtual void        setSelected(bool state);
    bool                isSelected()                        {return gui_Selected;};
    
    //Visible
    void                setVisible( bool v = true)          { gui_IsVisible=v;}
    bool                isVisible()                         { return gui_IsVisible;}
    
    void                setObjectTag(int tag)               { gui_ObjectTag=tag;}
    int                 getObjectTag()                      { return gui_ObjectTag;}
    
    int                 getTopMostObject(Vec2f pos);
    void                registerForInput(app::WindowRef window);

    Vec2f               convertToObjectSpace( Vec2f pos)    { return Vec2f(getPosition().x+pos.x,getPosition().y+pos.y);};
       
    bool                hasTouchPoint(Vec2f pnt)            { return getRect().contains(pnt);};
   



protected://Only children of this class have access to these variables, to allow access use "->" acessor(i.e make an accessor method)

    //This is a functor used to compare Touch objects based on id
    //Touch Comparator based on id of touch
    struct FindTouch {
        uint32_t id;
        FindTouch( TouchEvent::Touch t1 ) : id(t1.getId()){}
        bool operator() (const TouchEvent::Touch &t2){
            return(t2.getId() == id );
        }
    };
    
     
    
    Vec2f       gui_Position;
    float		gui_Width, gui_Height;
    int         gui_ObjectTag;
    
    //Text Objects
    std::string gui_Text,
                gui_TextFont;
    
    TextBox     gui_TextBox;
    
    gl::Texture	gui_TextTexture;
    
    Rectf       gui_TextRect;
    
    
    bool        gui_CanMove,
                gui_IsVisible,
                gui_AcceptTouch,
                gui_Selected,
                gui_DefaultDraw;//Enable this if the object should not be drawn by the static drawFunction;
 
    ColorA      gui_ContainerColor;
    
    
    //Mouse Event Callbacks
    scoped_connection
                gui_CbMouseDown,
                gui_CbMouseDrag,
                gui_CbMouseEnd;

    //Touch Event Callbacks
    CallbackId  gui_CbTouchBegan,
                gui_CbTouchMoved,
                gui_CbTouchEnd;

    
    ci::signals::scoped_connection	gui_BeganSignal, gui_HitEndSignal;

    //These functions will be overridden to decide if the object is selected, should move, etc..
    virtual void touchesBeganHandler();
    virtual void touchesMovedHandler();
    virtual void touchesEndedHandler();
    
    
    std::vector<TouchEvent::Touch> gui_ObjectTouches;

private://No one other than this class can access theses variables
  TouchEvent mouse2Touch(MouseEvent e);
    
    void onMouseBegan(MouseEvent &e);
    void onMouseMoved(MouseEvent &e);
    void onMouseEnded(MouseEvent &e);
    
    void touchesBegan(app::TouchEvent event);
    void touchesMoved(app::TouchEvent event);
    void touchesEnded(app::TouchEvent event);
    static vector<TouchEvent::Touch> getMouseTouches(){return gui_MouseTouches;};
    
    static Client gui_Tuio;
    signal<void(GuiObject *)>gui_OnSelectSignal,
                             gui_OnHitSignal;
    
    static std::vector<TouchEvent::Touch> gui_MouseTouches;
    static vector<GuiObject*>             gui_Objects;
};


