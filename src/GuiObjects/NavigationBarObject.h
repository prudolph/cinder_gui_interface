//
//  NavigationBarObject.h
//  guiObjects
//
//  Created by Paul Rudolph on 7/10/13.

#pragma once
#include "NavigationObject.h"

class NavigationBarObject:public GuiObject{
    
public:
    
    NavigationBarObject();
    
    void setup(App *app,Vec2f pos, float width, float height);
    void addChild(GuiObject* o);
    
protected:
    void drawObject();
    void touchesBeganHandler();
    void touchesMovedHandler(){};
    void touchesEndedHandler(){};
    void onObjectSelect(GuiObject* object);
    vector<GuiObject*> childObjects;
    Vec2f nextObjectPosition;
    float buttonSpacing;   
};




