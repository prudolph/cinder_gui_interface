//
//  Navigation.h
//  guiObjects
//
//  Created by Paul Rudolph on 7/10/13.

#pragma once
#include "NavigationObject.h"

class Navigation:public GuiObject{
    
public:
    Navigation();
    
    void setup(app::WindowRef window,Vec2f pos, Vec2f size);
    void addChild(GuiObject* o);
    
    void    setButtonSpacing(float space){buttonSpacing= space;}
    float getButtonSpacing(){return buttonSpacing;}
 	virtual void		setPosition( float x, float y );
	virtual void		setPosition( Vec2f pt );
    
protected:
    void drawObject();
    void touchesBeganHandler();
    void touchesMovedHandler(){};
    void touchesEndedHandler(){};
    void setChildObjectPositions();
    void onObjectSelect(GuiObject* object);
   
    vector<GuiObject*> childObjects;
    Vec2f nextObjectPosition;
    float buttonSpacing;   
};




