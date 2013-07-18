//
//  GuiObject.h
//  guiObjects
//
//  Created by Paul Rudolph on 7/2/13.
//
//

#pragma once

//#include <boost/signal.hpp>
//#include <boost/bind.hpp>

//#include "cinder/Cinder.h"
//#include "cinder/App/App.h"
//#include "cinder/Vector.h"
//#include "cinder/app/TouchEvent.h"
#include "TuioClient.h"
//s#include <iostream>
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include <forward_list>

using namespace std;
using namespace ci;
using namespace cinder::app;
using namespace boost::signals2;
    
class GuiObject {
public:
    typedef enum InputStyle { NORMAL , TOGGLE, DROPDOWN } InputStyle;
    
    
    GuiObject();
  
    static void draw();
    static vector<TouchEvent::Touch> getMouseTouches();
    
    //Callback Functions
    
    void registerForInput(ci::app::WindowRef window, tuio::Client *tuio=NULL);
    
    signal<void(GuiObject *)>&	getSelectedSignal() { return oOnSelectSignal; }
    void addCallBack(boost::function<void(GuiObject*)> fn){oOnSelectSignal.connect(fn);};
    virtual void onMouseBegan(MouseEvent &e);
    virtual void onMouseMoved(MouseEvent &e);
    virtual void onMouseEnded(MouseEvent &e);

    virtual void touchesBegan(app::TouchEvent event);
    virtual void touchesMoved(app::TouchEvent event);
    virtual void touchesEnded(app::TouchEvent event);

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
    tuio::Client                    *oTuioClient;
    Vec2f oCenter;
    Rectf oContainer;

 

    //Mouse Event Callbacks
    ci::signals::scoped_connection	oCbMouseDown, oCbMouseDrag,oCBMouseEnd;

    //Touch Event Callbacks
    ci::signals::scoped_connection	oCbTouchesBegan, oCbTouchesMoved,oCBTouchesEnd;

   std::vector<TouchEvent::Touch> currentObjTouches;

private:

    signal<void(GuiObject *)> oOnSelectSignal,oOnHitSignal;
    static   std::vector<TouchEvent::Touch> mouseTouches;

    


    
};


