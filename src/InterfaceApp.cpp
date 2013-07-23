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
    
   
    NavigationController navigationController;

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

}

void InterfaceApp::update()
{
}

void InterfaceApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );

    //Draw the extra components of the navigation controller
    navigationController.draw();
    
    //This will draw all Guiobject subclases if that object is visible
    GuiObject::draw();

    
}

CINDER_APP_NATIVE( InterfaceApp, RendererGl )
