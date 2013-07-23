//
//  NavigationController.h
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/19/13.
//
//
#pragma once
#include "MovableObject.h"
#include "Navigation.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class NavigationController{
    
public:
    
    NavigationController(){};
    
    void setup(App *app);
    void draw();
    void navigationCallback(GuiObject* object);
    
    MovableObject       movableObject;
    Navigation          navigationBar;
    
    app::WindowRef      *window;
    
    Rectf               navigationContent;
    ColorA              navigationContentColor;
    gl::Texture         navTextTexture;
    TextBox             navTextBox;
    string              navText;
    
};