//
//  GuiObject.cpp
//  guiObjects
//
//  Created by Paul Rudolph on 7/2/13.
//
//



#include "GuiObject.h"
#include "cinder/gl/gl.h"
#include "boost/lexical_cast.hpp"

using namespace std;
using namespace ci::app;



//Static Variables, all classes have access to these shared objects.(Only one instance is created)
vector<GuiObject*>GuiObject::objectOrderList = vector<GuiObject*>();
vector <TouchEvent::Touch> GuiObject::mouseTouches = vector<TouchEvent::Touch>();
Client GuiObject::mTuio;

GuiObject::GuiObject(){
  
    
    oCenter=Vec2f(0,0);
    oContainerColor = ColorA(1.0f,0.0f,0.0f,1.0f);
    oContainer= Rectf( Vec2f( 0, 0 ), Vec2f( 10, 10 ) );

    oIsAcceptingTouches=oCanMove=oCanRotate=oCanResize=oIsEnabled=oIsVisible=true;
    oHit=oSelected=false;
    oTextFont="Arial";
    oStyle = NORMAL;

    objectOrderList.push_back(this);
}



void GuiObject::draw(){
    ////DRAW EACH OBJECT back to front
    for (std::vector<GuiObject*>::reverse_iterator it = objectOrderList.rbegin() ; it != objectOrderList.rend(); ++it){
        GuiObject *object = (GuiObject*)*it;
        if(object->oIsVisible)  object->drawObject();
    }
    
    //DRAW TOUCHES
    gl::enableAlphaBlending();
        //DRAW TOUCH POINTS
        for(int i=0;i< mTuio.getActiveTouches().size();i++){
            gl::color(ColorA(1.25f,0.25f,0.25f,0.25f));
            gl::drawSolidCircle( mTuio.getActiveTouches().at(i).getPos(), 25.0f);
        }
        //DRAW MOUSE POINTS
        for(int i=0;i<mouseTouches.size();i++){
            gl::color(ColorA(1.25f,0.25f,0.25f,0.25f));
            gl::drawSolidCircle(mouseTouches.at(i).getPos(), 25.0f);
        }
    gl::disableAlphaBlending();
}


//Non-static default draw function
//THis can be overridden for each subclass object
void GuiObject::drawObject(){
    
    if(isSelected()){
        gl::color(1.0f,0.0f,0.0f);
        gl::lineWidth(8.0f);
        gl::drawStrokedRect(oContainer);
    }
    
    if(isHit()){
        gl::color(0.0f,0.0f,1.0f);
        gl::lineWidth(4.0f);
        gl::drawStrokedRect(oContainer);
    }
    
    gl::enableAlphaBlending();
    gl::color(oContainerColor);
    gl::drawSolidRect(oContainer);
    gl::disableAlphaBlending();
    
    //draw gui object text
    updateTextBox();
    gl::enableAlphaBlending();
    Vec2f textboxPos= Vec2f(oContainer.getCenter().x-oTextTexture.getWidth()/2,
                            oContainer.getCenter().y-oTextTexture.getHeight()/2);
    gl::draw(oTextTexture,textboxPos);
    gl::disableAlphaBlending();

    
}

void GuiObject::registerForInput(App* app){
    if(!mTuio.isConnected()){
        mTuio.connect();
    }


    oCbMouseDown = app->getWindow()->getSignalMouseDown().connect( std::bind( &GuiObject::onMouseBegan, this, std::_1 ) );
	oCbMouseDrag = app->getWindow()->getSignalMouseDrag().connect( std::bind( &GuiObject::onMouseMoved, this, std::_1 ) );
    oCBMouseEnd  = app->getWindow()->getSignalMouseUp()  .connect( std::bind( &GuiObject::onMouseEnded, this, std::_1 ) );
    
          mTuio.registerTouchesBegan(this,  &GuiObject::touchesBegan);
          mTuio.registerTouchesMoved(this,  &GuiObject::touchesMoved);
          mTuio.registerTouchesEnded(this,  &GuiObject::touchesEnded);
    
    
#ifdef INPUT_DEBUG
    console()<<"Registered Input callbacks"<<endl;
#endif

}

#pragma mark Input Functions -
#pragma mark Mouse Functions

//Converts mouse events into Touch Events
void GuiObject::onMouseBegan(MouseEvent &e){

    TouchEvent te= mouse2Touch(e);
    touchesBegan(te);
    mouseTouches.insert(mouseTouches.end(), te.getTouches().begin(),   te.getTouches().end());
    
#ifdef INPUT_DEBUG
    /*
    if(oSelected)console()<<"Object : "<< this <<" Selected @ "<< e.getX()<<","<< e.getY() <<endl;
    else          console()<<"Mouse Began @ "<< e.getX()<<","<< e.getY() <<endl;
     */
#endif

}

void GuiObject::onMouseMoved(MouseEvent &e){

    TouchEvent te= mouse2Touch(e);
    touchesMoved(te);
    
    mouseTouches.clear();
    mouseTouches.insert(mouseTouches.end(), te.getTouches().begin(),   te.getTouches().end());
  
    
#ifdef INPUT_DEBUG
    /*
    if(oSelected) console()<<"Object : "<< this <<" Selected @ "<< e.getX()<<","<< e.getY() <<endl;
    else          console()<<"Mouse Moved to "<< e.getX()<<","<< e.getY() <<endl;
     */
#endif
}

void GuiObject::onMouseEnded(MouseEvent &e){
  
    TouchEvent te= mouse2Touch(e);
    touchesEnded(mouse2Touch(e));
    mouseTouches.clear();
    
#ifdef INPUT_DEBUG
          //  console()<<"Mouse Ended "<<endl;
#endif

}

vector<TouchEvent::Touch> GuiObject::getMouseTouches(){
    return mouseTouches;
}
TouchEvent GuiObject::mouse2Touch(MouseEvent e){
    app::TouchEvent::Touch t = app::TouchEvent::Touch( e.getPos(), Vec2f(), -1, app::getElapsedSeconds() , nullptr );
    vector<app::TouchEvent::Touch> tVector;
    tVector.push_back(t);
    app::TouchEvent te= TouchEvent(e.getWindow(),tVector );
    
    return te;
}




#pragma mark Touch Functions
void GuiObject::touchesBegan(app::TouchEvent event){

        //Go Through each touch object and check if this object contains the point
        for( vector<TouchEvent::Touch>::iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
            
            //if this object contains the point and this is the object is not sitting below any other object;
            if( oContainer.contains( touchIt->getPos()) &&// does this object contain the touch point
                objectOrderList.at(getTopMostObject(touchIt->getPos())) == this //is the topmost object and the current object the same
                ){
                
                    // if this object is accepting touches add the touch to the obj's touch list
                    if(oIsAcceptingTouches){
                        currentObjTouches.push_back(*touchIt);
                         event.setHandled( true );
                    }
                }
        }

    if(currentObjTouches.size()>0)        touchesBeganHandler();

}
void GuiObject::touchesMoved(app::TouchEvent event){
    
    //Vec2f centroid;
    //int pntCnt=0;
    
        //Go Through all the touches in the event if the current touch is in the objects touch list then call the objects touches moved handler
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
            //If the touch object has current touches, check if they are in this event. if so move/reposition object based on the updated touches.
            auto found=std::find_if (currentObjTouches.begin(),currentObjTouches.end(), FindTouch(*touchIt));
            if(found!=currentObjTouches.end() ){//this touch id is touching this object
                touchesMovedHandler();
                event.setHandled();

                // centroid += touchIt->getPos();
               // pntCnt++;
            }
          
        }
    /*
    if(this->oCanMove){
        centroid/=pntCnt;//Get the center/average point of the touches
        if(currentObjTouches.size()>0 && !isNaN(centroid) ){
            oContainer.offsetCenterTo(centroid);
        }
    }
    if(currentObjTouches.size()>0 && !oSelected)  setSelected(true);
*/


}
void GuiObject::touchesEnded(app::TouchEvent event){
    
    
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
            
            //if current touches vector contains the iterators touch id, remove it from the list, since it is no longer touching the object. If there are no more touches in the current object touches set then the object is no longer selected

            auto found=std::find_if (currentObjTouches.begin(),currentObjTouches.end(), FindTouch(*touchIt));
            
            //this touch id is touching this object
            if(found!=currentObjTouches.end()){
                currentObjTouches.erase(found);
                touchesEndedHandler();
            }
        }
        
   
    
}

void GuiObject::touchesBeganHandler(){
    console()<<"THIS IS AN ABSTRACT FUNCTIONS- Must be OVERRIDDEN in subclass"<<endl;
    console()<<"Touch began Handler"<<endl;
    
}
void GuiObject::touchesMovedHandler(){

    console()<<"THIS IS AN ABSTRACT FUNCTIONS- Must be OVERRIDDEN in subclass"<<endl;
    console()<<"Touch Moved Handler"<<endl;
}
void GuiObject::touchesEndedHandler(){
    console()<<"THIS IS AN ABSTRACT FUNCTIONS- Must be OVERRIDDEN in subclass"<<endl;
    console()<<"Touch Ended Handler"<<endl;

}






int GuiObject::getTopMostObject(Vec2f pos){
    //check if other buttons also get this touch
    for(int i =0;i<objectOrderList.size();i++){
        if( objectOrderList.at(i)->oContainer.contains(pos)){
            return i;
        }
    }
    return -1;
}


 void GuiObject::setSelected(bool t,bool force){
   /*
     //update object order - put the clicked button at the top of the list
     if(t){

         objectOrderList.erase( find(objectOrderList.begin(),objectOrderList.end(),this));
         objectOrderList.insert(objectOrderList.begin(),(this));
     }

     if(oStyle == NORMAL){
         //console()<<"Style is NORMAL"<<endl;
         oSelected=t;
     }
     if(oStyle == TOGGLE){
         //console()<<"Style is TOGGLE"<<endl;
         if(t)oSelected=!oSelected;
     }
     
     if(oStyle == DROPDOWN){
         console()<<"Style is DROPDOWN"<<endl;
         oSelected=t;
     }
     
     if(force)oSelected = t;
    */
     
     oSelected  =t;
     if(oSelected) oOnSelectSignal(this);
    }


void GuiObject::setHit(bool t){
    string state = t?"HIT":"NOT HIT" ;
        console()<<getText() <<"  "<<state <<endl;
    oHit = t;
}



#pragma mark - Object Positioning functions
void GuiObject::setPositon(Vec2f pos){
    oContainer.set(oContainer.x1+pos.x , oContainer.y1+pos.y, oContainer.x2+pos.x, oContainer.y2+pos.y);
}
void GuiObject::rePosition(Vec2f pos){
    if(oCanMove)oContainer.offsetCenterTo(pos);

}
void GuiObject::setSize(Vec2f size){
    oContainer.set(oContainer.x1 , oContainer.y1, oContainer.x1+size.x, oContainer.y1+size.y);
}

#pragma mark - Text functions
void GuiObject::setText(string s){

    oText = s;
    string countLabel = "\n " + boost::lexical_cast<string>(currentObjTouches.size());
    oTextBox = TextBox().alignment( TextBox::CENTER ).font( Font(oTextFont,18) ).size( Vec2f(oContainer.getWidth(), TextBox::GROW) ).text( oText+ countLabel );
    oTextBox.setColor( ColorA( 0.0f, 0.0f, 0.0f, 1.0f ) );
	oTextBox.setBackgroundColor( ColorA( 0, 0, 0, 0.0f ) );
	oTextTexture = gl::Texture( oTextBox.render() );
}

void GuiObject::updateTextBox(){
    string countLabel = "\n Tchs:" + boost::lexical_cast<string>(currentObjTouches.size());
        oTextBox.setText(oText+ countLabel );

    oTextTexture = gl::Texture( oTextBox.render() );
}
