#include "cinder/app/AppNative.h"

#include "NavigationController.h"
#include "GuiObject.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class InterfaceApp : public AppBasic {
public:
    void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();
    
  void buttonCallback(GuiObject *object );
   
    NavigationController navigationController;
    
    Rectf  sampleContentArea;
    ColorA sampleContentAreaColor;
};


void InterfaceApp::prepareSettings( Settings *settings )
{
    //This will enable multitouch events from the operating system. Only supported on Windows 7 and Mac OS X trackpad.
	//settings->enableMultiTouch(); // It is not currently needed since touch events are recived through a TUIO Client.
    settings->setWindowSize( 800, 800 );

}

void InterfaceApp::setup()
{
   
    navigationController.setup(this);
    navigationController.guiObject.addCallBack( bind(&InterfaceApp::buttonCallback,this,std::__1::placeholders::_1));
    sampleContentArea =      Rectf(100,100,getWindowWidth()-50,getWindowHeight()-100);
    sampleContentAreaColor = ColorA(1.0f, 1.0f, 1.0f, 0.5f);

}

void InterfaceApp::update()
{
}

void InterfaceApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    
    //draw the content area
    gl::color( sampleContentAreaColor );
    gl::drawSolidRect( sampleContentArea);
    
    //This will draw all Guiobject subclases if that object is visible
    GuiObject::draw();
    
}


void InterfaceApp::buttonCallback(GuiObject *object){
    console()<<"BUTTON  "<< object->getText()<<"Has been selected"<<endl;
    sampleContentAreaColor =object->getContainerColor()/2;
}
CINDER_APP_NATIVE( InterfaceApp, RendererGl )
