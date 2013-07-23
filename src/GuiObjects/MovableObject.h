//
//  MovableObject.h
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/13.
//
//


#pragma once
#include "GuiObject.h"


class MovableObject: public GuiObject{
public:
    
    MovableObject(){};
    void setup(app::WindowRef window, Vec2f pos,Vec2f size,string text="");

    virtual void touchesBeganHandler();
    virtual void touchesMovedHandler();
    virtual void touchesEndedHandler();
};