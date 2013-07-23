//
//  NavigationObject.h
//  guiObjects
//
//  Created by Paul Rudolph on 7/3/13.
//
//

#pragma once
#include "GuiObject.h"
#include "Navigation.h"

class NavigationObject: public GuiObject{

public:
    NavigationObject();
    void setup(Vec2f size,ColorA containerColor,string text,int tag);
   
protected:
    int order;
};