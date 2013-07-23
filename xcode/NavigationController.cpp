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
    //Testing Objects
    navigationContent =      Rectf(100,100,getWindowWidth()-50,getWindowHeight()-100);
    navigationContentColor = ColorA(1.0f, 1.0f, 1.0f, 0.5f);

    navTextBox = TextBox()
                        .alignment( TextBox::CENTER )
                        .font( Font("Arial",25) )
                        .size( Vec2f(navigationContent.getWidth(), TextBox::GROW) )
                        .text( "__" );
    
    navTextBox.setColor( ColorA( 0.0f, 0.0f, 0.0f, 1.0f ) );
	navTextBox.setBackgroundColor( ColorA( 0, 0, 0, 0.0f ) );
    
	navTextTexture = gl::Texture( navTextBox.render() );

    ////////
    
    
    
    movableObject.setContainer(Vec2f(400,400), 100, 100);
    movableObject.registerForInput(app);
    movableObject.setContainerColor(ColorA(1.0f,0.0f,0.0f,1.0f));
    movableObject.setText("Mooovable GuiObject 0");
   

    
    
    navigationBarObject.setup(app,Vec2f(0,0),app->getWindowWidth(),50);
    //add 3 objects to the nav bar
    for(int i =0;i<4;i++){
        NavigationObject *n = new NavigationObject();
        n->setSize(200, 50);
        n->setContainerColor(ColorA(0.0f,1.0f,1.0f,1.0f));//choose random color for now
        n->setText("Navigation Button "+  boost::lexical_cast<string>( i ) + "  ");
        n->setObjectTag(i);
        navigationBarObject.addChild(n);
    }
    
    navigationBarObject.addCallBack(bind(& NavigationController::navigationCallback,this,std::__1::placeholders::_1));

    
}
void NavigationController::navigationCallback(GuiObject* object){
    
    
    //update Navigation content text
    navigationContentColor=object->getContainerColor();
    navTextBox.text("View : "+ boost::lexical_cast<string>(object->getObjectTag()) );
    navTextTexture = gl::Texture( navTextBox.render() );

}

void NavigationController::update(){
}

void NavigationController::draw(){
    
    //draw the content area
    gl::color( navigationContentColor );
    gl::drawSolidRect( navigationContent);
    
    gl::enableAlphaBlending();
        gl::draw(navTextTexture,Vec2f(navigationContent.getUpperLeft().x,navigationContent.getUpperLeft().y+50));
    gl::disableAlphaBlending();
}