//
//  NavigationController.cpp
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/19/13.
//
//

#include "NavigationController.h"
#include <boost/lexical_cast.hpp>

/*
 Create navigation objects here
 */
void NavigationController::setup(App *app){
   
    //Get a reference to the app's window to pass to the objects on creation
    app::WindowRef window= app->getWindow();
    
    
    
    /*Create a MOVABLE OBJECT
     This object has defined input handler functions that reposition the object based on the touch positions.
     
     Pass in a reference to the window so the object can register for input events, The origin point and size of the object, and finally the text to be drawn on the object(optional).
     */
    movableObject.setup(window, Vec2f(400,400), Vec2f(100,100),"MOOOvable Object");
 
    /*Create a NAVIGATION BAR
        This object has the ability to contain child objects. All touch events are passed through this object down to the children objects. Only the touchDown handler is defined in this object since we do not want the nav bar or its children to move with user's touches.
     
        We can attach a callback function to the Nav Bar's selected signal which will pass back a reference to the child object that was selected.
     
        Child NavigationObjects are added to the Nav Bar. The Navigation class will handle the callback and positioning of the child object.
     */
    navigationBar.setup(window,Vec2f(0,0),Vec2f(app->getWindowWidth(),50));
    navigationBar.addCallBack(bind(& NavigationController::navigationCallback,this,std::__1::placeholders::_1));

    //Add 3 objects to the nav bar
    for(int i =0;i<=3;i++){
        NavigationObject *n = new NavigationObject();
        
        string objectTitle = "Navigation Button "+  boost::lexical_cast<string>( i ) + "  ";
        n->setup(Vec2f(175,50), ColorA(0.0f,1.0f,1.0f,1.0f),objectTitle, i);
        navigationBar.addChild(n);
    }
    
    
    
    
    //Setup the sample Content container with a text box
    navigationContent =      Rectf(100,100,getWindowWidth()-50,getWindowHeight()-100);
    navigationContentColor = ColorA(1.0f, 1.0f, 1.0f, 0.5f);
    
    //Setup the  navigationContent textbox
    navTextBox = TextBox().alignment( TextBox::CENTER )
                          .font( Font("Arial",25) )
                          .size( Vec2f(navigationContent.getWidth(), TextBox::GROW))
                          .text( "__" )
                          .color(ColorA( 0.0f, 0.0f, 0.0f, 1.0f ))
                          .backgroundColor(ColorA( 0, 0, 0, 0.0f ));

	navTextTexture = gl::Texture( navTextBox.render() );
}


//Sample Call back that is listining to 
void NavigationController::navigationCallback(GuiObject* object){
    
    //update Navigation content  color and text based on the selected object
    navigationContentColor=object->getContainerColor();
    navTextBox.text("View : "+ boost::lexical_cast<string>(object->getObjectTag()) );
    navTextTexture = gl::Texture( navTextBox.render());
}


void NavigationController::draw(){
    
    //draw the content area
    gl::color( navigationContentColor );
    gl::drawSolidRect( navigationContent);
    
    //draw Text
    gl::enableAlphaBlending();
        gl::draw(navTextTexture,Vec2f(navigationContent.getUpperLeft().x,navigationContent.getUpperLeft().y+50));
    gl::disableAlphaBlending();
}