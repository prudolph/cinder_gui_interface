//
//  NavigationObject.cpp
//  guiObjects
//
//  Created by Paul Rudolph on 7/3/13.
//
//

#include "NavigationObject.h"

using namespace std;

NavigationObject::NavigationObject():GuiObject(){

    gui_CanMove=false;
    gui_DefaultDraw=false;
}


void NavigationObject::setup(Vec2f size,ColorA containerColor,string text,int tag){
    
    setSize(size);
    setContainerColor(containerColor);
    setText(text);
    setObjectTag(tag);
}

