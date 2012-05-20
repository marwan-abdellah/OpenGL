//-----------------------------------------------------------------------------
//           Name: ogl_sprite_overlay.cpp
//         Author: Kevin Harris
//  Last Modified: 02/10/05
//    Description: This sample demonstrates how to create animated sprites, 
//                 which are useful for rendering 2D overlays to the screen. 
//                 The sprites manipulate texture coordinates to create 
//                 animations on a simple quad instead using the traditional 
//                 method of bit-blitting straight to video memory.
//
//   Control Keys: Left Mouse Button - Spin the teapot
//-----------------------------------------------------------------------------

#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <math.h>
#include <sys/timeb.h>
#include <iostream>
using namespace std;
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "sprite.h"
#include "geometry.h"
#include "tga.h"

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
Display   *g_pDisplay = NULL;
Window     g_window;
GLXContext g_glxContext;
bool       g_bDoubleBuffered = GL_TRUE;

int g_nWindowWidth  = 640;
int g_nWindowHeight = 480;

float g_fElapsedTime = 0.0f;
timeb g_curTime;
timeb g_lastTime;

float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;

GLuint g_spriteTextureID = 0;
sprite g_donutSprite;
sprite g_numberSprite;

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int main(int argc, char **argv);
void render(void);
void init(void);
void shutDown(void);
void loadTexture(void);
void setupViewForSpriteRendering(void);

//-----------------------------------------------------------------------------
// Name: main()
// Desc:
//-----------------------------------------------------------------------------
int main( int argc, char **argv )
{
    // Open a connection to the X server
    g_pDisplay = XOpenDisplay( NULL );

    if( g_pDisplay == NULL )
        exit(1);

    int errorBase;
	int eventBase;
	
    // Make sure OpenGL's GLX extension supported
    if( !glXQueryExtension( g_pDisplay, &errorBase, &eventBase ) )
        exit(1);

    // Find an appropriate visual

    int doubleBufferVisual[]  =
    {
        GLX_RGBA,           // Needs to support OpenGL
        GLX_DEPTH_SIZE, 16, // Needs to support a 16 bit depth buffer
        GLX_DOUBLEBUFFER,   // Needs to support double-buffering
        None                // end of list
    };

    int singleBufferVisual[] =
    {
        GLX_RGBA,           // Needs to support OpenGL
        GLX_DEPTH_SIZE, 16, // Needs to support a 16 bit depth buffer
        None                // end of list
    };

    // Try for the double-bufferd visual first
    XVisualInfo *visualInfo = NULL;
    visualInfo = glXChooseVisual( g_pDisplay, DefaultScreen(g_pDisplay), doubleBufferVisual );

    if( visualInfo == NULL )
    {
    	// If we can't find a double-bufferd visual, try for a single-buffered visual...
        visualInfo = glXChooseVisual( g_pDisplay, DefaultScreen(g_pDisplay), singleBufferVisual );

        if( visualInfo == NULL )
            exit(1);

        g_bDoubleBuffered = false;
    }

    // Create an OpenGL rendering context
    g_glxContext = glXCreateContext( g_pDisplay, visualInfo, 
                                     NULL,      // No sharing of display lists
                                     GL_TRUE ); // Direct rendering if possible
                           
    if( g_glxContext == NULL )
        exit(1);

    // Create an X colormap since we're probably not using the default visual
    Colormap colorMap;
    colorMap = XCreateColormap( g_pDisplay, 
                                RootWindow(g_pDisplay, visualInfo->screen), 
                                visualInfo->visual, 
                                AllocNone );

    XSetWindowAttributes winAttr;
    winAttr.colormap     = colorMap;
    winAttr.border_pixel = 0;
    winAttr.event_mask   = ExposureMask           |
                           VisibilityChangeMask   |
                           KeyPressMask           |
                           KeyReleaseMask         |
                           ButtonPressMask        |
                           ButtonReleaseMask      |
                           PointerMotionMask      |
                           StructureNotifyMask    |
                           SubstructureNotifyMask |
                           FocusChangeMask;

    // Create an X window with the selected visual
    g_window = XCreateWindow( g_pDisplay, RootWindow(g_pDisplay, visualInfo->screen), 
                              0, 0, 640, 480, 0, visualInfo->depth, InputOutput, 
						      visualInfo->visual, CWBorderPixel | CWColormap | CWEventMask,
                              &winAttr );

    XSetStandardProperties( g_pDisplay, g_window, "OpenGL - Animated Sprite Overlays",
                            "ogl_sprite_overlay", None, argv, argc, NULL );

    glXMakeCurrent( g_pDisplay, g_window, g_glxContext );
    XMapWindow( g_pDisplay, g_window );

    init();
 
    //
    // Enter the render loop and don't forget to dispatch X events as
    // they occur.
    //

    XEvent event;
	bool bRunning = true;
	
	int  nLastMousePositX = 0;
	int  nLastMousePositY = 0;
	bool bMousing         = false;

	ftime( &g_lastTime );
    g_curTime = g_lastTime;
    	
    while( bRunning )
    {
        while( XPending(g_pDisplay) )
        {
            XNextEvent( g_pDisplay, &event );

            switch( event.type )
            {
            	case KeyPress:
                {
					switch( XKeycodeToKeysym( g_pDisplay, event.xkey.keycode, 0 ) )
					{
						case XK_Escape:
							bRunning = false;
							break;
					}
                }
                break;
                
                case ButtonPress:
                {
            	    if( event.xbutton.button == 1 )
            		{
						nLastMousePositX = event.xmotion.x;
				        nLastMousePositY = event.xmotion.y;
						bMousing = true;
					}
                }
                break;

                case ButtonRelease:
                {
                	if( event.xbutton.button == 1 )
                		bMousing = false;
                }
                break;

                case MotionNotify:
                {
                    if( bMousing )
                    {
	                    g_fSpinX -= (event.xmotion.x - nLastMousePositX);
						g_fSpinY -= (event.xmotion.y - nLastMousePositY);
					
						nLastMousePositX = event.xmotion.x;
					    nLastMousePositY = event.xmotion.y;
                    }
                }
                break;

                case ConfigureNotify:
                {
                	int g_nWindowWidth  = event.xconfigure.width; 
					int g_nWindowHeight = event.xconfigure.height;
					glViewport(0, 0, g_nWindowWidth, g_nWindowHeight);
		
					glMatrixMode( GL_PROJECTION );
					glLoadIdentity();
					gluPerspective( 45.0, (GLdouble)g_nWindowWidth / (GLdouble)g_nWindowHeight, 0.1, 100.0);
                }
                break;

                case DestroyNotify:
                {
					bRunning = false;
                }
				break;
            }
        }

		ftime( &g_curTime );

        // This is accurate to one second
        g_fElapsedTime  = (float)(g_curTime.time - g_lastTime.time);
        // This gets it down to one millisecond
        g_fElapsedTime += (float)((g_curTime.millitm - g_lastTime.millitm) / 1000.0);
		g_lastTime = g_curTime;

        render();
    }

    shutDown();
    
	XDestroyWindow( g_pDisplay, g_window );
	XCloseDisplay( g_pDisplay );
}

//-----------------------------------------------------------------------------
// Name: init()
// Desc: Init OpenGL context for rendering
//-----------------------------------------------------------------------------
void init( void )
{
	glClearColor( 0.35f, 0.53f, 0.7f, 1.0f );
    glEnable( GL_DEPTH_TEST );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    gluPerspective( 45.0, (GLdouble)g_nWindowWidth / (GLdouble)g_nWindowHeight, 0.1, 100.0);

    // Set up a material
    glEnable( GL_COLOR_MATERIAL );
    GLfloat ambient_mtrl[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat diffuse_mtrl[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv( GL_FRONT, GL_AMBIENT, ambient_mtrl ); 
    glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse_mtrl );
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

    // Set light 0 to be a pure white directional light
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    GLfloat diffuse_light0[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specular_light0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position_light0[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light0 );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular_light0 );
    glLightfv( GL_LIGHT0, GL_POSITION, position_light0 );

    // Enable some dim, grey ambient lighting so objects that are not lit 
    // by the other lights are not completely black.
    GLfloat ambient_lightModel[] = { 0.2f, 0.2f, 0.2f, 0.2f };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );

    //
    // Load up the sprite texture...
    //

    loadTexture();

    g_donutSprite.setTextureID( g_spriteTextureID );
    g_donutSprite.setPositionX( 0.0f );
    g_donutSprite.setPositionY( 0.0f );
    g_donutSprite.setAlpha( 1.0f );
    g_donutSprite.setWidth( 1.0f );
    g_donutSprite.setHeight( 1.0f );
    g_donutSprite.setFrameDelay( 0.01f );
    g_donutSprite.setTextureAnimeInfo( 512, 512, 64, 64, 5, 6, 30 );

    //-------------------------------------------------------------------------
    //
    // -- Argument explantion for setTextureAnimeInfo() call above -- 
    //
    // From a 512 x 512 texture map we're going to pull a series of frames 
    // that are 64 x 64.
    // 
    // The frames have been laid out in a column/row fashion starting in the 
    // upper left corner with 5 frames in each of the 6 rows.
    //
    // The total frame count is 30 because all rows are fully complete and 
    // contain 5 frames with none of them being short any frames.
    //
    //-------------------------------------------------------------------------

    //
    // Load up the number sprite...
    //

    g_numberSprite.setTextureID( g_spriteTextureID );
    g_numberSprite.setPositionX( -5.0f );
    g_numberSprite.setPositionY( -3.5f );
    g_numberSprite.setAlpha( 1.0f );
    g_numberSprite.setWidth( 0.5f );
    g_numberSprite.setHeight( 0.5f );
    g_numberSprite.setFrameDelay( 1.0f );
    g_numberSprite.setTextureAnimeInfo( 512, 512, 15, 20, 5, 2, 10, 328, 4 );

    //-------------------------------------------------------------------------
    //
    // -- Argument explantion for setTextureAnimeInfo() call above -- 
    //
    // From the same 512 x 512 texture map we're going to pull a series of 
    // frames that are 15 x 20.
    // 
    // The frames have been laid out in a column/row fashion starting in the 
    // upper left corner with 5 frames in each of the 2 rows.
    //
    // The total frame count is 10 because all rows are fully complete and 
    // contain 5 frames with none of them being short any frames.
    //
    // Finally, unlike the donut sprites, the first frame for our number 
    // sprites is not located in the upper left hand corner. This means we 
    // need to provide an x/y offset measured in pixels from the upper left
    // corner so the code that generates the correct texture coordinates for 
    // the sprite can find the first frame.
    //
    //-------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )	
{
    glDeleteTextures( 1, &g_spriteTextureID );

    if( g_glxContext != NULL )
    {
        // Release the context
        glXMakeCurrent( g_pDisplay, None, NULL );

        // Delete the context
        glXDestroyContext( g_pDisplay, g_glxContext );
        g_glxContext = NULL;
    }
}

//-----------------------------------------------------------------------------
// Name: loadTexture()
// Desc: 
//-----------------------------------------------------------------------------
void loadTexture(void)	
{
    //
    // The animations for both the spinning donut and the numbers are stored 
    // together in a .tga file which has an alpha channel.
    //

    tgaImageFile tgaImage;
    tgaImage.load( "sprites.tga" );

    glGenTextures( 1, &g_spriteTextureID );

    glBindTexture( GL_TEXTURE_2D, g_spriteTextureID );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexImage2D( GL_TEXTURE_2D, 0, tgaImage.m_texFormat, 
                  tgaImage.m_nImageWidth, tgaImage.m_nImageHeight, 
                  0, tgaImage.m_texFormat, GL_UNSIGNED_BYTE, 
                  tgaImage.m_nImageData );
}

//-----------------------------------------------------------------------------
// Name: setupViewForSpriteRendering()
// Desc: 
//-----------------------------------------------------------------------------
void setupViewForSpriteRendering( void )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0f, (GLdouble)g_nWindowWidth / (GLdouble)g_nWindowHeight, 0.1f, 100.0f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Set up current camera
    gluLookAt( 0.0, 0.0, 10,  // Camera Position
               0.0, 0.0, 0.0, // Look At Point
               0.0, 1.0, 0.0);// Up Vector
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: Called when the GLX window is ready to render
//-----------------------------------------------------------------------------
void render( void )
{
	//
    // Place the donut sprite in motion so we can test the methods of our 
    // sprite class.
    //

	static float velocityX  = 5.0f;
	static float velocityY  = 5.0f;
	static float widthRate  = 2.0f;
	static float heightRate = 2.0f;
	static float alphaRate  = 1.0f;

	g_donutSprite.setPositionX( g_donutSprite.getPositionX() + (velocityX*g_fElapsedTime) );
	g_donutSprite.setPositionY( g_donutSprite.getPositionY() + (velocityY*g_fElapsedTime) );
	g_donutSprite.setWidth( g_donutSprite.getWidth() + (widthRate*g_fElapsedTime) );
	g_donutSprite.setHeight( g_donutSprite.getHeight() + (heightRate*g_fElapsedTime) );
	g_donutSprite.setAlpha( g_donutSprite.getAlpha() + (alphaRate*g_fElapsedTime));

    if( g_donutSprite.getPositionX() > 5.0f || g_donutSprite.getPositionX() < -5.0f )
        velocityX *= -1.0f; // invert direction along the x axis

    if( g_donutSprite.getPositionY() > 3.0f || g_donutSprite.getPositionY() < -3.0f )
        velocityY *= -1.0f; // invert direction along the y axis

    if( g_donutSprite.getWidth() > 5.0f || g_donutSprite.getWidth() < 1.0f )
        widthRate *= -1.0f; // invert change in width

    if( g_donutSprite.getHeight() > 5.0f || g_donutSprite.getHeight() < 1.0f )
        heightRate *= -1.0f; // invert change in height

    if( g_donutSprite.getAlpha() > 1.0f || g_donutSprite.getAlpha() < 0.0f )
        alphaRate *= -1.0f; // invert change in alpha

    // Perform a boundary check and make sure our sprite hasn't jumped from 
    // the screen duing an update or faded out too much. 
    if( g_donutSprite.getPositionX() > 5.0f )
        g_donutSprite.setPositionX( 5.0f );
        
    if( g_donutSprite.getPositionX() < -5.0f )
        g_donutSprite.setPositionX( -5.0f );

    if( g_donutSprite.getPositionY() > 3.0f )
        g_donutSprite.setPositionY( 3.0f );

    if( g_donutSprite.getPositionY() < -3.0f )
        g_donutSprite.setPositionY( -3.0f );

    if( g_donutSprite.getWidth() > 5.0f )
        g_donutSprite.setWidth( 5.0f );

    if( g_donutSprite.getWidth() < 1.0f )
        g_donutSprite.setWidth( 1.0f );

    if( g_donutSprite.getHeight() > 5.0f )
        g_donutSprite.setHeight( 5.0f );

    if( g_donutSprite.getHeight() < 1.0f )
        g_donutSprite.setHeight( 1.0f );
        
    if( g_donutSprite.getAlpha() > 1.0f )
        g_donutSprite.setAlpha( 1.0f );

    if( g_donutSprite.getAlpha() < 0.0f )
        g_donutSprite.setAlpha( 0.0f );

    //
    // Render...
    //

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0.0f, 0.0f, -5.0f );
    glRotatef( -g_fSpinY, 1.0f, 0.0f, 0.0f );
    glRotatef( -g_fSpinX, 0.0f, 1.0f, 0.0f );

    //
    // First, we'll render a teapot so our scene will have at least something 
    // 3D in it.
    //

    renderSolidTeapot( 1.0f );
    //renderSolidTorus( 0.2, 1.0, 20, 20 );

    //
    // Finally, we render our two animated sprites as overlays...
    //

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    // Set up a perspective projection matrix suitable for rendering sprite
    // overlays
    setupViewForSpriteRendering();

    g_numberSprite.render();
    g_donutSprite.render();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    if( g_bDoubleBuffered )
        glXSwapBuffers( g_pDisplay, g_window ); // Buffer swap does implicit glFlush
    else
        glFlush(); // Explicit flush for single buffered case 
}
