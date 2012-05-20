//-----------------------------------------------------------------------------
//           Name: ogl_glx_sample.cpp
//         Author: Kevin Harris
//  Last Modified: 08/06/04
//    Description:  This sample is a basic demonstration of how to use GLX to 
//                  create windowed OpenGL samples via XFree86, which is a a 
//                  freely redistributable open-source implementation of the 
//                  X Window System.
//
//   Control Keys: 
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <GL/glx.h> // This includes the necessary X headers
#include <GL/gl.h>
#include <GL/glu.h>

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
Display *g_pDisplay = NULL;
Window   g_window;
bool     g_bDoubleBuffered = GL_TRUE;
GLuint   g_textureID = 0;

float g_fSpinX           = 0.0f;
float g_fSpinY           = 0.0f;
int   g_nLastMousePositX = 0;
int   g_nLastMousePositY = 0;
bool  g_bMousing         = false;

struct Vertex
{
    float tu, tv;
    float x, y, z;
};

Vertex g_quadVertices[] =
{
    { 0.0f,0.0f, -1.0f,-1.0f, 0.0f },
    { 1.0f,0.0f,  1.0f,-1.0f, 0.0f },
    { 1.0f,1.0f,  1.0f, 1.0f, 0.0f },
    { 0.0f,1.0f, -1.0f, 1.0f, 0.0f }
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
void getBitmapImageData(char *pFileName, BMPImage *pImage);
void loadTexture(void);

//-----------------------------------------------------------------------------
// Name: main()
// Desc:
//-----------------------------------------------------------------------------
int main( int argc, char **argv )
{
    XSetWindowAttributes windowAttributes;
    XVisualInfo *visualInfo = NULL;
    XEvent event;
    Colormap colorMap;
    GLXContext glxContext;
    int errorBase;
	int eventBase;

    // Open a connection to the X server
    g_pDisplay = XOpenDisplay( NULL );

    if( g_pDisplay == NULL )
    {
        fprintf(stderr, "glxsimple: %s\n", "could not open display");
        exit(1);
    }

    // Make sure OpenGL's GLX extension supported
    if( !glXQueryExtension( g_pDisplay, &errorBase, &eventBase ) )
    {
        fprintf(stderr, "glxsimple: %s\n", "X server has no OpenGL GLX extension");
        exit(1);
    }

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
    visualInfo = glXChooseVisual( g_pDisplay, DefaultScreen(g_pDisplay), doubleBufferVisual );

    if( visualInfo == NULL )
    {
    	// If we can't find a double-bufferd visual, try for a single-buffered visual...
        visualInfo = glXChooseVisual( g_pDisplay, DefaultScreen(g_pDisplay), singleBufferVisual );

        if( visualInfo == NULL )
        {
            fprintf(stderr, "glxsimple: %s\n", "no RGB visual with depth buffer");
            exit(1);
        }

        g_bDoubleBuffered = false;
    }

    // Create an OpenGL rendering context
    glxContext = glXCreateContext( g_pDisplay, 
                                   visualInfo, 
                                   NULL,      // No sharing of display lists
                                   GL_TRUE ); // Direct rendering if possible
                           
    if( glxContext == NULL )
    {
        fprintf(stderr, "glxsimple: %s\n", "could not create rendering context");
        exit(1);
    }

    // Create an X colormap since we're probably not using the default visual 
    colorMap = XCreateColormap( g_pDisplay, 
                                RootWindow(g_pDisplay, visualInfo->screen), 
                                visualInfo->visual, 
                                AllocNone );

    windowAttributes.colormap     = colorMap;
    windowAttributes.border_pixel = 0;
    windowAttributes.event_mask   = ExposureMask           |
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
    g_window = XCreateWindow( g_pDisplay, 
                              RootWindow(g_pDisplay, visualInfo->screen), 
                              0, 0,     // x/y position of top-left outside corner of the window
                              640, 480, // Width and height of window
                              0,        // Border width
                              visualInfo->depth,
                              InputOutput,
                              visualInfo->visual,
                              CWBorderPixel | CWColormap | CWEventMask,
                              &windowAttributes );

    XSetStandardProperties( g_pDisplay,
                            g_window,
                            "GLX Sample",
                            "GLX Sample",
                            None,
                            argv,
                            argc,
                            NULL );

    // Bind the rendering context to the window
    glXMakeCurrent( g_pDisplay, g_window, glxContext );

    // Request the X window to be displayed on the screen
    XMapWindow( g_pDisplay, g_window );

    // Init OpenGL...
    init();
 
    //
    // Enter the render loop and don't forget to dispatch X events as
    // they occur.
    //

    while(1)
    {
        do
        {
            XNextEvent( g_pDisplay, &event );

            switch( event.type )
            {
                case ButtonPress:
                {
            	    if( event.xbutton.button == 1 )
            		{
						g_nLastMousePositX = event.xmotion.x;
				        g_nLastMousePositY = event.xmotion.y;
						g_bMousing = true;
					}
                }
                break;

                case ButtonRelease:
                {
                	if( event.xbutton.button == 1 )
                		g_bMousing = false;
                }
                break;
                
                case KeyPress:
                {
                    fprintf( stderr, "KeyPress event\n" );
                }
                break;

                case KeyRelease:
                {
                    fprintf( stderr, "KeyRelease event\n" );
                }
                break;

                case MotionNotify:
                {
                    if( g_bMousing )
                    {
	                    g_fSpinX -= (event.xmotion.x - g_nLastMousePositX);
						g_fSpinY -= (event.xmotion.y - g_nLastMousePositY);
					
						g_nLastMousePositX = event.xmotion.x;
					    g_nLastMousePositY = event.xmotion.y;
                    }
                }
                break;

                case Expose:
                {
                    fprintf( stderr, "Expose event\n" );
                }
                break;

                case ConfigureNotify:
                {
                    glViewport( 0, 0, event.xconfigure.width, event.xconfigure.height );
                }
            }
        }
        while( XPending(g_pDisplay) ); // Loop to compress events

        render();
    }
}

//-----------------------------------------------------------------------------
// Name: init()
// Desc: Init OpenGL context for rendering
//-----------------------------------------------------------------------------
void init( void )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glEnable( GL_TEXTURE_2D );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	
	loadTexture();
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
		printf("ERROR: getBitmapImageData - %s not found\n",pFileName);

    // Seek forward to width and height info
    fseek( pFile, 18, SEEK_CUR );

    if( (i = fread(&pImage->width, 4, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read width from %s.\n", pFileName);

    if( (i = fread(&pImage->height, 4, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read height from %s.\n", pFileName);

    if( (fread(&nNumPlanes, 2, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n", pFileName);
	
    if( nNumPlanes != 1 )
		printf( "ERROR: getBitmapImageData - Plane count from %s is not 1: %u\n", pFileName, nNumPlanes );

    if( (i = fread(&nNumBPP, 2, 1, pFile)) != 1 )
		printf( "ERROR: getBitmapImageData - Couldn't read BPP from %s.\n", pFileName );
	
    if( nNumBPP != 24 )
		printf( "ERROR: getBitmapImageData - BPP from %s is not 24: %u\n", pFileName, nNumBPP );

    // Seek forward to image data
    fseek( pFile, 24, SEEK_CUR );

	// Calculate the image's total size in bytes. Note how we multiply the 
	// result of (width * height) by 3. This is becuase a 24 bit color BMP 
	// file will give you 3 bytes per pixel.
    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (char*) malloc( nTotalImagesize );
	
    if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n", pFileName);

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

	glGenTextures( 1, &g_textureID );
	glBindTexture( GL_TEXTURE_2D, g_textureID );

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
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -5.0f );
    glRotatef( -g_fSpinY, 1.0f, 0.0f, 0.0f );
    glRotatef( -g_fSpinX, 0.0f, 1.0f, 0.0f );
    
    glBindTexture( GL_TEXTURE_2D, g_textureID );
    glInterleavedArrays( GL_T2F_V3F, 0, g_quadVertices );
    glDrawArrays( GL_QUADS, 0, 4 );

    if( g_bDoubleBuffered )
        glXSwapBuffers( g_pDisplay, g_window ); // Buffer swap does implicit glFlush
    else
        glFlush(); // Explicit flush for single buffered case 
}

