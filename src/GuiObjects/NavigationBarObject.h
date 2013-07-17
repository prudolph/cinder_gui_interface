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

class NavigationBarObject{
    
    public:
        NavigationBarObject();
    void setup(ci::app::WindowRef window,tuio::Client *tuio, int numItems,Rectf containerRect, boost::function<void(GuiObject*)> fn);
    void draw();
    void navButtonSelected(GuiObject *selectedObject);
    
    int nbNumObjects;
    float nbBufferSz;
    Rectf nbContainer;
    ColorA nbContainerColor;
    vector<GuiObject*> navigationBarObjects;
};