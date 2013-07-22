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
   public:
    
    //Touch Comparator based on id of touch
    struct FindTouch {
        uint32_t id;
        FindTouch( TouchEvent::Touch t1 ) : id(t1.getId()){}
        bool operator() (const TouchEvent::Touch &t2){
            return(t2.getId() == id );
        }
    };
    

    
    typedef enum InputStyle { NORMAL , TOGGLE, DROPDOWN } InputStyle;
    
    GuiObject();
   
 
    static void draw();
    static vector<TouchEvent::Touch> getMouseTouches();
    
    //Callback Functions
    
     
    signal<void(GuiObject *)>&	getSelectedSignal() { return oOnSelectSignal; }
    void addCallBack(boost::function<void(GuiObject*)> fn){oOnSelectSignal.connect(fn);};
    virtual void onMouseBegan(MouseEvent &e);
    virtual void onMouseMoved(MouseEvent &e);
    virtual void onMouseEnded(MouseEvent &e);

    virtual void touchesBegan(app::TouchEvent event);
    virtual void touchesMoved(app::TouchEvent event);
    virtual void touchesEnded(app::TouchEvent event);

    //These functions will be overridden to decide if the object is selected, should move, etc..
    virtual void touchesBeganHandler();
    virtual void touchesMovedHandler();
    virtual void touchesEndedHandler();
   
    virtual void drawObject();
    
    TouchEvent mouse2Touch(MouseEvent e);
    virtual void setSelected(bool t, bool force =false);
    bool isSelected(){return oSelected;};

    virtual void setHit(bool t);
    bool isHit(){return oHit;}
    
    void setInputStyle(InputStyle inStyle){oStyle=inStyle;};
    
    
    void setPositon(Vec2f pos);
    void rePosition(Vec2f pos);
    void setSize(Vec2f size);
    Vec2f getSize(){return oContainer.getSize();};
    void setContainerColor(ColorA clr){ oContainerColor = clr;};
    ColorA getContainerColor(){return oContainerColor;};
    void setVisible(bool v = true){oIsVisible=v;}

    
    //Text FUnctions
    void setText(string s);
    void updateTextBox();
    string getText(){return oText;};
    

    static vector<GuiObject*>objectOrderList;
    int getTopMostObject(Vec2f pos);
    void registerForInput(App* app);
    
    bool containerHasPoint(Vec2f pnt){    return oContainer.contains(pnt);};
       std::vector<TouchEvent::Touch> currentObjTouches;
protected:

     InputStyle oStyle;
    
    
    //Text Objects
    TextBox oTextBox;
    std::string oText,oTextFont;
    gl::Texture	oTextTexture;
    Rectf oTextRect;
    
    
    bool oCanMove,oCanRotate,oCanResize,oIsEnabled,oIsVisible,oIsAcceptingTouches,oSelected,oHit;
    ColorA oContainerColor;
    
    ci::app::WindowRef				oWindow;
    
    Vec2f oCenter;
    Rectf oContainer;

 
    
    //Mouse Event Callbacks
    ci::signals::scoped_connection	oCbMouseDown, oCbMouseDrag,oCBMouseEnd;

    //Touch Event Callbacks
    ci::signals::scoped_connection	oCbTouchesBegan, oCbTouchesMoved,oCBTouchesEnd;
    ci::signals::scoped_connection	oBeganSignal, oHitEndSignal;


private:
     static Client mTuio;
    signal<void(GuiObject *)> oOnTouchSignal,oOnSelectSignal,oOnHitSignal;
    static   std::vector<TouchEvent::Touch> mouseTouches;

    


    
};


