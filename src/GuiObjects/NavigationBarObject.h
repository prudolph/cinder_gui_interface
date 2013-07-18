//
//  NavigationBarObject.h
//  guiObjects
//
//  Created by Paul Rudolph on 7/10/13.
//
//



#pragma once
#include "NavigationObject.h"
#include <map>
using namespace std;

class NavigationBarObject:public GuiObject{
    
public:
    NavigationBarObject();
    void setup(Rectf containerRect, boost::function<void(GuiObject*)> fn);
    void objectDraw();
    void addChild(GuiObject* o);
    
    void navButtonSelected(GuiObject *selectedObject);
    void organizeChildren();

    float nbBufferSz;
    ColorA nbContainerColor;
    
    virtual void touchesBeganHandler();
    virtual void touchesMovedHandler();
    virtual void touchesEndedHandler();
    
protected:
    vector<GuiObject*> nbChildren;
    map<GuiObject*,int> touchCountMap;

};