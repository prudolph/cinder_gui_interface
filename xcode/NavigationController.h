//
//  NavigationController.h
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/19/13.
//
//
#pragma once
#include "GuiObject.h"
#include "NavigationBarObject.h"




using namespace ci;
using namespace ci::app;
using namespace std;

class NavigationController{
    
public:
    NavigationController();
    void setup(App *app);
    void update();
    void draw();

protected:
    GuiObject       guiObject;
    NavigationBarObject navigationBarObject;
private:
 
};