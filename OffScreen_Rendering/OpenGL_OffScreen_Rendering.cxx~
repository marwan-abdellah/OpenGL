//-----------------------------------------------------------------------------
//           Name: ogl_offscreen_rendering_2.cpp
//         Author: Kevin Harris
//  Last Modified: 02/15/05
//    Description: This sample demonstrates how to create dynamic textures 
//                 through off-screen rendering. The off-screen rendering step 
//                 is accomplished using a p-buffer, which is created using 
//                 GLX's glXCreatePbuffer method. The step of dynamic texture 
//                 creation from a p-buffer involves sharing the window's 
//                 GLXContext with the p-buffer's GLXContext so we can load 
//                 the texture with pixel data with a simple call to 
//                 glCopyTexSubImage2D.
//                 
//                 As a demonstration, a spinning textured cube is rendered 
//                 to a p-buffer, which is in turn, used to create a dynamic 
//                 texture. The dynamic texture is then used to texture a 
//                 second spinning cube, which will be rendered to the 
//                 application's window.
//
//   Control Keys: Left Mouse Button  - Spin the large, black cube.
//                 Right Mouse Button - Spin the textured cube being rendered 
//                                      into the p-buffer.
//-----------------------------------------------------------------------------

#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <math.h>
#include <sys/timeb.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------
#define TEXTURE_WIDTH  256
#define TEXTURE_HEIGHT 256
#define PBUFFER_WIDTH  256
#define PBUFFER_HEIGHT 256

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
Display   *g_pDisplay = NULL;
Window     g_window;
GLXContext g_windowContext;
GLXContext g_pbufferContext = NULL;
GLXPbuffer g_pbuffer = 0;
bool       g_bDoubleBuffered = GL_TRUE;

GLuint g_dynamicTextureID = 0;
GLuint g_testTextureID    = 0;

float g_fSpinX_L = 0.0f;
float g_fSpinY_L = 0.0f;
float g_fSpinX_R = 0.0f;
float g_fSpinY_R = 0.0f;

struct Vertex
{
    float tu, tv;
    float x, y, z;
};

Vertex g_cubeVertices[] =
{
    { 0.0f,0.0f, -1.0f,-1.0f, 1.0f },
    { 1.0f,0.0f,  1.0f,-1.0f, 1.0f },
    { 1.0f,1.0f,  1.0f, 1.0f, 1.0f },
    { 0.0f,1.0f, -1.0f, 1.0f, 1.0f },
   
    { 1.0f,0.0f, -1.0f,-1.0f,-1.0f },
    { 1.0f,1.0f, -1.0f, 1.0f,-1.0f },
    { 0.0f,1.0f,  1.0f, 1.0f,-1.0f },
    { 0.0f,0.0f,  1.0f,-1.0f,-1.0f },
   
    { 0.0f,1.0f, -1.0f, 1.0f,-1.0f },
    { 0.0f,0.0f, -1.0f, 1.0f, 1.0f },
    { 1.0f,0.0f,  1.0f, 1.0f, 1.0f },
    { 1.0f,1.0f,  1.0f, 1.0f,-1.0f },
   
    { 1.0f,1.0f, -1.0f,-1.0f,-1.0f },
    { 0.0f,1.0f,  1.0f,-1.0f,-1.0f },
    { 0.0f,0.0f,  1.0f,-1.0f, 1.0f },
    { 1.0f,0.0f, -1.0f,-1.0f, 1.0f },
   
    { 1.0f,0.0f,  1.0f,-1.0f,-1.0f },
    { 1.0f,1.0f,  1.0f, 1.0f,-1.0f },
    { 0.0f,1.0f,  1.0f, 1.0f, 1.0f },
    { 0.0f,0.0f,  1.0f,-1.0f, 1.0f },
   
    { 0.0f,0.0f, -1.0f,-1.0f,-1.0f },
    { 1.0f,0.0f, -1.0f,-1.0f, 1.0f },
    { 1.0f,1.0f, -1.0f, 1.0f, 1.0f },
    { 0.0f,1.0f, -1.0f, 1.0f,-1.0f }
};

struct BMPImage 
{
    int   width;
    int   height;
    char *data;
};

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int main(int argc, char **argv);
void render(void);
void init(void);
void shutDown(void);
void getBitmapImageData(char *pFileName, BMPImage *pImage);
void loadTexture(void);

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
    g_windowContext = glXCreateContext( g_pDisplay, visualInfo, 
                                        NULL,      // No sharing of display lists
                                        GL_TRUE ); // Direct rendering if possible
                           
    if( g_windowContext == NULL )
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

    XSetStandardProperties( g_pDisplay, g_window, 
                            "OpenGL - Off-Screen Rendering Using P-Buffers",
                            "ogl_offscreen_rendering_2", None, argv, argc, NULL );

    glXMakeCurrent( g_pDisplay, g_window, g_windowContext );
    XMapWindow( g_pDisplay, g_window );

    init();
 
    //
    // Enter the render loop and don't forget to dispatch X events as
    // they occur.
    //

    XEvent event;
	bool bRunning = true;
	
	bool bMousing_L = false;
	bool bMousing_R = false;
	int  nLastMousePositX_L = 0;
	int  nLastMousePositY_L = 0;
	int  nLastMousePositX_R = 0;
	int  nLastMousePositY_R = 0;
	
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
						nLastMousePositX_L = event.xmotion.x;
				        nLastMousePositY_L= event.xmotion.y;
						bMousing_L = true;
					}
					
					if( event.xbutton.button == 3 )
            		{
						nLastMousePositX_R = event.xmotion.x;
				        nLastMousePositY_R= event.xmotion.y;
						bMousing_R = true;
					}
                }
                break;

                case ButtonRelease:
                {
                	if( event.xbutton.button == 1 )
                		bMousing_L = false;
                		
                	if( event.xbutton.button == 3 )
                		bMousing_R = false;
                }
                break;

                case MotionNotify:
                {
                    if( bMousing_L )
                    {
	                    g_fSpinX_L -= (event.xmotion.x - nLastMousePositX_L);
						g_fSpinY_L -= (event.xmotion.y - nLastMousePositY_L);
					
						nLastMousePositX_L = event.xmotion.x;
					    nLastMousePositY_L = event.xmotion.y;
                    }
                    
                    if( bMousing_R )
                    {
	                    g_fSpinX_R -= (event.xmotion.x - nLastMousePositX_R);
						g_fSpinY_R -= (event.xmotion.y - nLastMousePositY_R);
					
						nLastMousePositX_R = event.xmotion.x;
					    nLastMousePositY_R = event.xmotion.y;
                    }
                }
                break;

                case ConfigureNotify:
                {
					glXMakeCurrent( g_pDisplay, g_window, g_windowContext );
                	    
                	int nWidth  = event.xconfigure.width; 
					int nHeight = event.xconfigure.height;
					glViewport(0, 0, nWidth, nHeight);
		
					glMatrixMode( GL_PROJECTION );
					glLoadIdentity();
					gluPerspective( 45.0, (GLdouble)nWidth / (GLdouble)nHeight, 0.1, 100.0);
                }
                break;

                case DestroyNotify:
                {
					bRunning = false;
                }
				break;
            }
        }

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
	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
  
    glXMakeCurrent( g_pDisplay, g_window, g_windowContext );
    
	//
	// This is our dynamic texture, which will be loaded with new pixel data
	// after we're finshed rendering to the p-buffer.
	//
    // Create a white RGBA image to initialize the dynamic texture.
    //
    
    void *whiteTextureImage = malloc( TEXTURE_WIDTH * TEXTURE_HEIGHT * 4 );

    if( NULL == whiteTextureImage )
    {
        cout << "Error: init - Call to malloc failed! Not enough memory!" << endl;
        exit( 1 );
    }

    memset( whiteTextureImage, 255, TEXTURE_WIDTH * TEXTURE_HEIGHT * 4 );
       
    glGenTextures( 1, &g_dynamicTextureID );
    glBindTexture( GL_TEXTURE_2D, g_dynamicTextureID );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT,
                  0, GL_RGBA, GL_UNSIGNED_BYTE, whiteTextureImage );
    free( whiteTextureImage );
    
	//
	// Create the p-buffer...
	//
    
    int scrnum;
    GLXFBConfig *fbconfig;
    XVisualInfo *visinfo;
    int nitems;

    int attrib[] = 
    {
        GLX_DOUBLEBUFFER,  False,
        GLX_RED_SIZE,      1,
        GLX_GREEN_SIZE,    1,
        GLX_BLUE_SIZE,     1,
        GLX_DEPTH_SIZE,    1,
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT | GLX_WINDOW_BIT,
        None
    };

    int pbufAttrib[] = 
    {
        GLX_PBUFFER_WIDTH,   PBUFFER_WIDTH,
        GLX_PBUFFER_HEIGHT,  PBUFFER_HEIGHT,
        GLX_LARGEST_PBUFFER, False,
        None
    };

    scrnum = DefaultScreen( g_pDisplay );

    fbconfig = glXChooseFBConfig( g_pDisplay,
                                  scrnum,
                                  attrib,
                                  &nitems );
    if( NULL == fbconfig )
    {
        cout << "Error: init - Couldn't get fbconfig" << endl;
        exit( 1 );
    }

    g_pbuffer = glXCreatePbuffer( g_pDisplay, fbconfig[0], pbufAttrib );

    visinfo = glXGetVisualFromFBConfig( g_pDisplay, fbconfig[0] );

    if( !visinfo )
    {
        cout << "Error: init - Couldn't get an RGBA, double-buffered visual" << endl;
        exit( 1 );
    }

	//
	// In this sample, we're going to speed things up by sharing display lists 
	// and textures between rendering contexts.
	//
	// With sharing turned on, we can simple create our dynamic texture by 
	// binding it to the p-buffer and calling glCopyTexSubImage2D, but this
	// will only work if the window's rendering context is sharing its display 
	// lists and texture space with the p-buffer's context.
	//
	
    g_pbufferContext = glXCreateContext( g_pDisplay,
                                         visinfo,
                                         g_windowContext, // Share display lists and textures with the regular window
                                         GL_TRUE );

    if( !g_pbufferContext )
    {
        cout << "ERROR: init - Call to glXCreateContext failed!" << endl;
        exit( 1 );
     }

    XFree( fbconfig );
    XFree( visinfo );
    
    //
	// We were successful in creating a p-buffer. We can now make its context 
	// current and set it up just like we would a regular context 
	// attached to a window.
	//
	
    glXMakeCurrent( g_pDisplay, g_pbuffer, g_pbufferContext );

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, PBUFFER_WIDTH / PBUFFER_HEIGHT, 0.1f, 10.0f );

	loadTexture();
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )	
{
    // Destroy pbuffer
    glXDestroyContext( g_pDisplay, g_pbufferContext );
    glXDestroyPbuffer( g_pDisplay, g_pbuffer );

    glDeleteTextures( 1, &g_testTextureID );
    glDeleteTextures( 1, &g_dynamicTextureID );

    if( g_windowContext != NULL )
    {
        // Release the context
        glXMakeCurrent( g_pDisplay, None, NULL );

        // Delete the context
        glXDestroyContext( g_pDisplay, g_windowContext );
        g_windowContext = NULL;
    }
}

//-----------------------------------------------------------------------------
// Name: getBitmapImageData()
// Desc: Simply image loader for 24 bit BMP files.
//-----------------------------------------------------------------------------
void getBitmapImageData( char *pFileName, BMPImage *pImage )
{
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
	int i;

    if( (pFile = fopen(pFileName, "rb") ) == NULL )
		cout << "ERROR: getBitmapImageData - %s not found " << pFileName << "." << endl;

    // Seek forward to width and height info
    fseek( pFile, 18, SEEK_CUR );

    if( (i = fread(&pImage->width, 4, 1, pFile) ) != 1 )
		cout << "ERROR: getBitmapImageData - Couldn't read width from " << pFileName << "." << endl; 

    if( (i = fread(&pImage->height, 4, 1, pFile) ) != 1 )
		cout << "ERROR: getBitmapImageData - Couldn't read height from " << pFileName << "." << endl; 

    if( (fread(&nNumPlanes, 2, 1, pFile) ) != 1 )
		cout << "ERROR: getBitmapImageData - Couldn't read plane count from " << pFileName << "." << endl; 
	
    if( nNumPlanes != 1 )
		cout << "ERROR: getBitmapImageData - Plane count from " << pFileName << " is not 1: " << nNumPlanes << "." << endl;

    if( (i = fread(&nNumBPP, 2, 1, pFile)) != 1 )
		cout << "ERROR: getBitmapImageData - Couldn't read BPP from " << pFileName << endl; 
	
    if( nNumBPP != 24 )
		cout << "ERROR: getBitmapImageData - BPP from " << pFileName << " is not 24: " << nNumBPP << "." << endl;

    // Seek forward to image data
    fseek( pFile, 24, SEEK_CUR );

	// Calculate the image's total size in bytes. Note how we multiply the 
	// result of (width * height) by 3. This is becuase a 24 bit color BMP 
	// file will give you 3 bytes per pixel.
    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (char*) malloc( nTotalImagesize );
	
    if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
		cout << "ERROR: getBitmapImageData - Couldn't read image data from " << pFileName << "." << endl;

    //
	// Finally, rearrange BGR to RGB
	//
	
	char charTemp;
    for( i = 0; i < nTotalImagesize; i += 3 )
	{ 
		charTemp = pImage->data[i];
		pImage->data[i] = pImage->data[i+2];
		pImage->data[i+2] = charTemp;
    }
}

//-----------------------------------------------------------------------------
// Name: loadTexture()
// Desc: 
//-----------------------------------------------------------------------------
void loadTexture( void )	
{
	BMPImage textureImage;
	
    getBitmapImageData( "test.bmp", &textureImage );

	glGenTextures( 1, &g_testTextureID );
	glBindTexture( GL_TEXTURE_2D, g_testTextureID );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexImage2D( GL_TEXTURE_2D, 0, 3, textureImage.width, textureImage.height, 
	               0, GL_RGB, GL_UNSIGNED_BYTE, textureImage.data );
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: Called when the GLX window is ready to render
//-----------------------------------------------------------------------------
void render( void )
{
	//-------------------------------------------------------------------------
	// Make the p-buffer's context current, so we can do an off-screen render 
	// to it...
	//-------------------------------------------------------------------------

	glXMakeCurrent( g_pDisplay, g_pbuffer, g_pbufferContext );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//
	// Let the user spin the cube about with the right mouse button, so our 
	// dynamic texture will show motion.
	//
	
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -5.0f );
    glRotatef( -g_fSpinY_R, 1.0f, 0.0f, 0.0f );
    glRotatef( -g_fSpinX_R, 0.0f, 1.0f, 0.0f );

	glBindTexture( GL_TEXTURE_2D, g_testTextureID );
		
	//
	// Now the render the cube to the p-buffer just like you we would have 
	// done with the regular window.
	//

    glInterleavedArrays( GL_T2F_V3F, 0, g_cubeVertices );
    glDrawArrays( GL_QUADS, 0, 24 );

	//
	// Copy out the pixel data from the p-buffer so we can make a texture 
	// later. We have to do this step now because glCopyTexSubImage2D reads 
	// from the buffer of the current context.
	//
	
	glBindTexture( GL_TEXTURE_2D, g_dynamicTextureID );
	glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, PBUFFER_WIDTH, PBUFFER_HEIGHT );
 
	//-------------------------------------------------------------------------
	// Now, make the window's context current for regular rendering...
	//-------------------------------------------------------------------------

    glXMakeCurrent( g_pDisplay, g_window, g_windowContext );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//
	// Let the user spin the cube about with the left mouse button.
	//

	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -5.0f );
    glRotatef( -g_fSpinY_L, 1.0f, 0.0f, 0.0f );
    glRotatef( -g_fSpinX_L, 0.0f, 1.0f, 0.0f );

	//
	// Finally, we'll use the dynamic texture we created by copying the pixel
	// data from the our p-buffer.
	//

    glBindTexture( GL_TEXTURE_2D, g_dynamicTextureID ); // Use our dynamic texture 

    glInterleavedArrays( GL_T2F_V3F, 0, g_cubeVertices );
    glDrawArrays( GL_QUADS, 0, 24 );

    if( g_bDoubleBuffered )
        glXSwapBuffers( g_pDisplay, g_window ); // Buffer swap does implicit glFlush
    else
        glFlush(); // Explicit flush for single buffered case 
}
