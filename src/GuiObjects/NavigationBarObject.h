//
//  NavigationBarObject.h
//  guiObjects
//
//  Created by Paul Rudolph on 7/10/13.
//
//



#pragma once
#include "NavigationObject.h"

using namespace std;

class NavigationBarObject:GuiObject{
    
public:
    NavigationBarObject();
    void setup(ci::app::WindowRef window,tuio::Client *tuio,Rectf containerRect, boost::function<void(GuiObject*)> fn);
    void draw();
    void addChild(GuiObject* o);
    
    void navButtonSelected(GuiObject *selectedObject);
    void organizeChildren();

    float nbBufferSz;
    Rectf nbContainer;
    ColorA nbContainerColor;
    
protected:
    vector<GuiObject*> nbChildren;

};