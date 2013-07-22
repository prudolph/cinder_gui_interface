//
//  MovableObject.cpp
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/13.
//
//
#include "MovableObject.h"

MovableObject:: MovableObject(){

    console()<<"CREATED MOVEABLE OBJECT"<<endl;
    //Set background color
    gui_ContainerColor = ColorA(0.0f,1.0f,0.0f,1.0f);
    
}

void MovableObject::touchesBeganHandler(){
    console()<<"Moveable Touch BEGAN Handler "<<endl;
   if(gui_ObjectTouches.size()>0) setSelected(true);
}

void MovableObject::touchesMovedHandler(){
    
    //Get object's current touches and find the center point, then move the object to that point
    Vec2f centroid;
    int pntCnt=0;

    for(TouchEvent::Touch touch : gui_ObjectTouches ){
        centroid += touch.getPos();
        pntCnt++;
    }
    
     if(gui_CanMove){
    
         centroid/=pntCnt;//Get the center/average point of the touches
         if (!isNaN(centroid) ){
        //check that the object is not going out of the window
             setPosition(centroid);
   
         }
     }
}


void MovableObject::touchesEndedHandler(){
    if(gui_ObjectTouches.size()==0) setSelected(false);
}