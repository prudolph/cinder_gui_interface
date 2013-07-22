//
//  NavigationController.cpp
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/19/13.
//
//

#include "NavigationController.h"
#include <boost/lexical_cast.hpp>

NavigationController::NavigationController(){
    
    
}
/*
 Create navigation objects here
 */
void NavigationController::setup(App *app){

    movableObject.setContainer(Vec2f(400,400), 100, 100);
    movableObject.registerForInput(app);
    movableObject.setContainerColor(ColorA(1.0f,0.0f,0.0f,1.0f));
    movableObject.setText("Mooovable GuiObject 0");
    
#warning Figure out how to manage call backs
   // guiObject.addCallBack(bind(buttonCallback,this,std::__1::placeholders::_1));
   

    
    
    navigationBarObject.setup(app,Vec2f(0,0),app->getWindowWidth(),50);

    //add 3 objects to the nav bar
    for(int i =0;i<4;i++){
        
        NavigationObject *n = new NavigationObject();
        //n->setSize(Vec2f(200,50));
        n->setSize(200, 50);
        n->setContainerColor(ColorA(0.0f,1.0f,1.0f,1.0f));//choose random color for now
      //n->getSelectedSignal().connect(  bind(&cinder_gui_interfaceApp::buttonCallback,this,std::__1::placeholders::_1));// connect the button's function
        n->setText("Navigation Button "+  boost::lexical_cast<string>( i ) + "  ");
        
        navigationBarObject.addChild(n);
    }
    

    
}
void NavigationController::update(){
}

void NavigationController::draw(){
  
}