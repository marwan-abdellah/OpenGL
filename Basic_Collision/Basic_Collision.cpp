//-----------------------------------------------------------------------------
//           Name: ogl_basic_collision.cpp
//         Author: Kevin Harris
//  Last Modified: 02/10/05
//    Description: As you may or may not know, truly accurate collision
//                 detection between two 3D objects can only be done by
//                 performing an intersection test on every triangle in the
//                 first object against every triangle of the other object.
//                 Obviously, it's too wasteful to just go around blindly
//                 performing this costly procedure on every object in the
//                 scene against every other object. To save us considerable
//                 CPU cycles, we'll calculate and assign a bounding sphere
//                 for each of our 3D objects so we can perform a quick check
//                 to see whether or not the two objects are even close to one
//                 another and require further testing. In short, if the two
//                 bounding spheres of our objects overlap, then there exists
//                 a possible collision and we need investigate further by
//                 checking triangles. If the two spheres don't overlap, a
//                 collision is not possible and we can move on.
//
// Note: Most of the math used in this sample is based on a small paper
// written by Gary Simmons, which is one of the instructors at
// www.gameinstitute.com. I've included the paper with the sample for reference.
// Please see it for further details.
//
//   Control Keys: F1 - Toggle bounding sphere visibility
//                 F2 - Toggle triangle motion
//
//                 Up         - View moves forward
//                 Down       - View moves backward
//                 Left       - View strafes left
//                 Right      - View strafes Right
//                 Left Mouse - Perform looking
//                 Mouse      - Look about the scene
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
#include "geometry.h"
#include "matrix4x4f.h"
#include "vector3f.h"

//-----------------------------------------------------------------------------
// SYMBOLIC CONSTANTS
//-----------------------------------------------------------------------------
enum Collision
{
    COLLISION_NO,
    COLLISION_YES,
    COLLISION_NOT_CHECKED
};

//-----------------------------------------------------------------------------
// STRUCTS
//-----------------------------------------------------------------------------
struct triangle
{
    vector3f v0;
    vector3f v1;
    vector3f v2;
    vector3f vNormal;

    // Bounding sphere
    vector3f vCenter;
    float    fRadius;
};

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
Display   *g_pDisplay = NULL;
Window     g_window;
GLXContext g_glxContext;
bool       g_bDoubleBuffered = GL_TRUE;

float g_fSpinX           = 0.0f;
float g_fSpinY           = 0.0f;
int   g_nLastMousePositX = 0;
int   g_nLastMousePositY = 0;
bool  g_bMousing = false;
float g_fMoveSpeed = 25.0f;

float g_fElapsedTime = 0.0f;
timeb g_curTime;
timeb g_lastTime;

vector3f g_vEye(8.0f, 8.0f, 8.0f);     // Eye Position
vector3f g_vLook(-0.5f, -0.5f, -0.5f); // Look Vector
vector3f g_vUp(0.0f, 1.0f, 0.0f);      // Up Vector
vector3f g_vRight(1.0f, 0.0f, 0.0f);   // Right Vector

triangle g_tri1;
triangle g_tri2;
bool     g_bDrawBoundingSpheres = true;
bool     g_bMoveSpheres = true;

struct Vertex
{
    // GL_C4UB_V3F
    unsigned char r, g, b, a;
    float x, y, z;
};

Vertex g_lineVertices[] =
{
    { 255,   0,   0, 255,  0.0f, 0.0f, 0.0f }, // red   = +x Axis
    { 255,   0,   0, 255,  5.0f, 0.0f, 0.0f },
    {   0, 255,   0, 255,  0.0f, 0.0f, 0.0f }, // green = +y Axis
    {   0, 255,   0, 255,  0.0f, 5.0f, 0.0f },
    {   0,   0, 255, 255,  0.0f, 0.0f, 0.0f }, // blue  = +z Axis
    {   0,   0, 255, 255,  0.0f, 0.0f, 5.0f }
};

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int main(int argc, char **argv);
void render(void);
void init(void);
void shutDown(void);
void updateViewMatrix(void);
void createBoundingSphere(triangle *tri);
bool doSpheresIntersect(triangle *tri1, triangle *tri2);
bool doTrianglesIntersect(triangle tri1, triangle tri2);
bool getLinePlaneIntersectionPoint( vector3f *vLineStart, vector3f *vLineEnd,
                                   vector3f *vPointInPlane, vector3f *vPlaneNormal,
                                   vector3f *vIntersection );
bool isPointInsideTriangle( vector3f *vIntersectionPoint, triangle *tri );

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

    XSetStandardProperties( g_pDisplay, g_window, "OpenGL - Basic 3D Collision Detection",
                            "ogl_basic_collision", None, argv, argc, NULL );

    glXMakeCurrent( g_pDisplay, g_window, g_glxContext );
    XMapWindow( g_pDisplay, g_window );

    init();
 
    //
    // Enter the render loop and don't forget to dispatch X events as
    // they occur.
    //

    XEvent event;
	bool bRunning = true;

	ftime( &g_lastTime );
    g_curTime = g_lastTime;;
	
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
									
						case XK_F1:
		                    g_bDrawBoundingSpheres = !g_bDrawBoundingSpheres;
		                    break;
		
		                case XK_F2:
		                    g_bMoveSpheres = !g_bMoveSpheres;
		                    break;
		                    
						case XK_Up:
							// View moves forward
							//vector3f tmpLook  = g_vLook;
							g_vEye -= g_vLook*-g_fMoveSpeed*g_fElapsedTime;
							break;
				
						case XK_Down:
							// View moves backward
							//vector3f tmpLook  = g_vLook;
							g_vEye += (g_vLook*-g_fMoveSpeed)*g_fElapsedTime;
							break;
				
						case XK_Left:
							// View side-steps or strafes to the left
							//vector3f tmpRight = g_vRight;
							g_vEye -= (g_vRight*g_fMoveSpeed)*g_fElapsedTime;
							break;
				
						case XK_Right:
							// View side-steps or strafes to the right
	                        //vector3f tmpRight = g_vRight;
							g_vEye += (g_vRight*g_fMoveSpeed)*g_fElapsedTime;
							break;
				
						case XK_Home:
							// View elevates up
							g_vEye.y += g_fMoveSpeed*g_fElapsedTime;
							break;
				
						case XK_End:
							// View elevates down
							g_vEye.y -= g_fMoveSpeed*g_fElapsedTime;
							break;
					}
                }
                break;
                
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

                case MotionNotify:
                {				
                	matrix4x4f matRot;

					if( g_bMousing )
					{
						int nXDiff = (event.xmotion.x - g_nLastMousePositX);
						int nYDiff = (event.xmotion.y - g_nLastMousePositY);
				
						if( nXDiff != 0 )
						{
				            vector3f vAxisOfRotation = vector3f(0.0f, 1.0f, 0.0f);
				            matRot.rotate( -(float)nXDiff / 3.0f, vAxisOfRotation );
							matRot.transformVector( &g_vLook );
							matRot.transformVector( &g_vUp );
						}
				
						if( nYDiff != 0 )
						{
							matRot.rotate( -(float)nYDiff / 3.0f, g_vRight );
							matRot.transformVector( &g_vLook );
							matRot.transformVector( &g_vUp );
						}
					}
				
					g_nLastMousePositX = event.xmotion.x;
				    g_nLastMousePositY = event.xmotion.y;
                }
                break;

                case ConfigureNotify:
                {
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
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glEnable( GL_DEPTH_TEST );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);

    //
    // Triangle #1 (small/blue)
    //

    g_tri1.v0.x =  1.0f;
    g_tri1.v0.y = -1.0f;
    g_tri1.v0.z =  1.0f;

    g_tri1.v1.x = -1.0f;
    g_tri1.v1.y =  1.0f;
    g_tri1.v1.z =  1.0f;

    g_tri1.v2.x = -3.0f;
    g_tri1.v2.y = -1.0f;
    g_tri1.v2.z =  1.0f;

    g_tri1.vNormal = vector3f( 0.0f, 0.0f, 0.0f );

    createBoundingSphere( &g_tri1 );

    //
    // Triangle #2 (large/green)
    //

    g_tri2.v0.x = 0.0f;
    g_tri2.v0.y = 2.0f;
    g_tri2.v0.z = 0.0f;

    g_tri2.v1.x =  0.0f;
    g_tri2.v1.y = -2.0f;
    g_tri2.v1.z =  2.0f;

    g_tri2.v2.x =  0.0f;
    g_tri2.v2.y = -2.0f;
    g_tri2.v2.z = -2.0f;

    g_tri2.vNormal = vector3f( 0.0f, 0.0f, 0.0f );

    createBoundingSphere( &g_tri2 );
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )	
{
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
// Name : updateViewMatrix() 
// Desc : Builds a view matrix suitable for OpenGL.
//
// Here's what the final view matrix should look like:
//
//  |  rx   ry   rz  -(r.e) |
//  |  ux   uy   uz  -(u.e) |
//  | -lx  -ly  -lz   (l.e) |
//  |   0    0    0     1   |
//
// Where r = Right vector
//       u = Up vector
//       l = Look vector
//       e = Eye position in world space
//       . = Dot-product operation
//
//-----------------------------------------------------------------------------
void updateViewMatrix( void )
{
	matrix4x4f view;
	view.identity();

	g_vLook.normalize();

	g_vRight = crossProduct(g_vLook, g_vUp);
	g_vRight.normalize();

	g_vUp = crossProduct(g_vRight, g_vLook);
	g_vUp.normalize();

	view.m[0] =  g_vRight.x;
	view.m[1] =  g_vUp.x;
	view.m[2] = -g_vLook.x;
	view.m[3] =  0.0f;

	view.m[4] =  g_vRight.y;
	view.m[5] =  g_vUp.y;
	view.m[6] = -g_vLook.y;
	view.m[7] =  0.0f;

	view.m[8]  =  g_vRight.z;
	view.m[9]  =  g_vUp.z;
	view.m[10] = -g_vLook.z;
	view.m[11] =  0.0f;

	view.m[12] = -dotProduct(g_vRight, g_vEye);
	view.m[13] = -dotProduct(g_vUp, g_vEye);
	view.m[14] =  dotProduct(g_vLook, g_vEye);
	view.m[15] =  1.0f;

	glMultMatrixf( view.m );
}

//-----------------------------------------------------------------------------
// Name: createBoundingSphere()
// Desc:
//-----------------------------------------------------------------------------
void createBoundingSphere( triangle *tri )
{
    float fMinX;
    float fMinY;
    float fMinZ;

    float fMaxX;
    float fMaxY;
    float fMaxZ;

    float fRadius1;
    float fRadius2;

    fMinX = fMaxX = tri->v0.x;
    fMinY = fMaxY = tri->v0.y;
    fMinZ = fMaxZ = tri->v0.z;

    if( tri->v1.x < fMinX ) fMinX = tri->v1.x;
    if( tri->v2.x < fMinX ) fMinX = tri->v2.x;
    if( tri->v1.y < fMinY ) fMinY = tri->v1.y;
    if( tri->v2.y < fMinY ) fMinY = tri->v2.y;
    if( tri->v1.z < fMinZ ) fMinZ = tri->v1.z;
    if( tri->v2.z < fMinZ ) fMinZ = tri->v2.z;

    if( tri->v1.x > fMaxX ) fMaxX = tri->v1.x;
    if( tri->v2.x > fMaxX ) fMaxX = tri->v2.x;
    if( tri->v1.y > fMaxY ) fMaxY = tri->v1.y;
    if( tri->v2.y > fMaxY ) fMaxY = tri->v2.y;
    if( tri->v1.z > fMaxZ ) fMaxZ = tri->v1.z;
    if( tri->v2.z > fMaxZ ) fMaxZ = tri->v2.z;

    tri->vCenter.x = (fMinX + fMaxX) / 2;
    tri->vCenter.y = (fMinY + fMaxY) / 2;
    tri->vCenter.z = (fMinZ + fMaxZ) / 2;

    fRadius1 = sqrt( ((tri->vCenter.x - tri->v0.x) * (tri->vCenter.x - tri->v0.x)) +
                     ((tri->vCenter.y - tri->v0.y) * (tri->vCenter.y - tri->v0.y)) +
                     ((tri->vCenter.z - tri->v0.z) * (tri->vCenter.z - tri->v0.z)) );

    fRadius2 = sqrt( ((tri->vCenter.x - tri->v1.x) * (tri->vCenter.x - tri->v1.x)) +
                     ((tri->vCenter.y - tri->v1.y) * (tri->vCenter.y - tri->v1.y)) +
                     ((tri->vCenter.z - tri->v1.z) * (tri->vCenter.z - tri->v1.z)) );

    if( fRadius1 < fRadius2 )
        fRadius1 = fRadius2;

    fRadius2 = sqrt( ((tri->vCenter.x - tri->v2.x) * (tri->vCenter.x - tri->v2.x)) +
                     ((tri->vCenter.y - tri->v2.y) * (tri->vCenter.y - tri->v2.y)) +
                     ((tri->vCenter.z - tri->v2.z) * (tri->vCenter.z - tri->v2.z)) );

    if( fRadius1 < fRadius2 )
		fRadius1 = fRadius2;

    tri->fRadius = fRadius1;

    return;
}

//-----------------------------------------------------------------------------
// Name: doSpheresIntersect()
// Desc: Determine whether two bounding spheres of "tri1" and "tr2" intersect.
//-----------------------------------------------------------------------------
bool doSpheresIntersect( triangle *tri1, triangle *tri2 )
{
    float fDistance = tri1->fRadius + tri2->fRadius;
	float fRadius;

    fRadius = sqrt( ((tri2->vCenter.x - tri1->vCenter.x) * (tri2->vCenter.x - tri1->vCenter.x)) +
                    ((tri2->vCenter.y - tri1->vCenter.y) * (tri2->vCenter.y - tri1->vCenter.y)) +
                    ((tri2->vCenter.z - tri1->vCenter.z) * (tri2->vCenter.z - tri1->vCenter.z)) );

    if( fRadius < fDistance )
        return true;

    else
        return false;
}

//-----------------------------------------------------------------------------
// Name: doTrianglesIntersect()
// Desc: Determine whether triangle "tri1" intersects "tri2".
//-----------------------------------------------------------------------------
bool doTrianglesIntersect( triangle tri1, triangle tri2 )
{
	bool bIntersect = false;
	vector3f vPoint;

	//
	// Create a normal for 'tri1'
	//

	vector3f vEdgeVec1 = tri1.v1 - tri1.v0;
	vector3f vEdgeVec2 = tri1.v2 - tri1.v0;
	tri1.vNormal = crossProduct( vEdgeVec1, vEdgeVec2 );
	//tri1.vNormal.normalize(); // Some people feel compelled to normalize this, but it's not really necessary.

	//
	// Check the first line segment of triangle #2 against triangle #1
    //
    // The first line segment is defined by vertices v0 and v1.
	//

	bIntersect = getLinePlaneIntersectionPoint( &tri2.v0,      // Line start
		                                        &tri2.v1,      // Line end
				                                &tri1.v0,      // A point in the plane
							                    &tri1.vNormal, // The plane's normal
				                                &vPoint );     // Holds the intersection point, if the function returns true

	if( bIntersect == true )
	{
		//
		// The line segment intersects the plane, but does it actually go 
		// through the triangle?
		//

		if( isPointInsideTriangle( &vPoint, &tri1 ) == true )
			return true;
	}

    //
	// Check the second line segment of triangle #2 against triangle #1
    //
    // The second line segment is defined by vertices v1 and v2.
	//

	bIntersect = getLinePlaneIntersectionPoint( &tri2.v1,      // Line start
		                                        &tri2.v2,      // Line end
				                                &tri1.v0,      // A point in the plane
							                    &tri1.vNormal, // The plane's normal
				                                &vPoint );     // Holds the intersection point, if the function returns true

	if( bIntersect == true )
	{
		//
		// The line segment intersects the plane, but does it actually go 
		// through the triangle?
		//

		if( isPointInsideTriangle( &vPoint, &tri1 ) == true )
			return true;
	}

    //
	// Check the third line segment of triangle #2 against triangle #1
    //
    // The third line segment is defined by vertices v2 and v0.
	//

	bIntersect = getLinePlaneIntersectionPoint( &tri2.v2,      // Line start
		                                        &tri2.v0,      // Line end
				                                &tri1.v0,      // A point in the plane
							                    &tri1.vNormal, // The plane's normal
				                                &vPoint );     // Holds the intersection point, if the function returns true

	if( bIntersect == true )
	{
		//
		// The line segment intersects the plane, but does it actually go 
		// through the triangle?
		//
		
		if( isPointInsideTriangle( &vPoint, &tri1 ) == true )
			return true;
	}

    return false;
}

//-----------------------------------------------------------------------------
// Name : getLinePlaneIntersectionPoint
// Desc : Determine whether a line or ray defined by "vLineStart" and "vLineEnd",
//        intersects with a plane which is defined by "vPlaneNormal" and
//        "vPointInPlane". If it doesn't, return false, otherwise, return true
//        and set "vIntersection" to the intersection point in 3D space.
//-----------------------------------------------------------------------------
bool getLinePlaneIntersectionPoint( vector3f *vLineStart, vector3f *vLineEnd,
				                    vector3f *vPointInPlane, vector3f *vPlaneNormal,
				                    vector3f *vIntersection )
{
	vector3f vDirection;
	vector3f L1;
	float	 fLineLength;
    float    fDistanceFromPlane;
	float    fPercentage;

	vDirection.x = vLineEnd->x - vLineStart->x;
	vDirection.y = vLineEnd->y - vLineStart->y;
	vDirection.z = vLineEnd->z - vLineStart->z;

	fLineLength = dotProduct( vDirection, *vPlaneNormal );

	// Check the line's length allowing for some tolerance for floating point
	// rounding errors. If it's 0 or really close to 0, the line is parallel to
	// the plane and can not intersect it.
	if( fabsf( fLineLength ) < 0.001f )
        return false;

	L1.x = vPointInPlane->x - vLineStart->x;
	L1.y = vPointInPlane->y - vLineStart->y;
	L1.z = vPointInPlane->z - vLineStart->z;

	fDistanceFromPlane = dotProduct( L1, *vPlaneNormal );

	// How far from Linestart , intersection is as a percentage of 0 to 1
	fPercentage	= fDistanceFromPlane / fLineLength;

	if( fPercentage < 0.0f || // The plane is behind the start of the line
		fPercentage > 1.0f )  // The line segment does not reach the plane
        return false;

	// Add the percentage of the line to line start
	vIntersection->x = vLineStart->x + vDirection.x * fPercentage;
	vIntersection->y = vLineStart->y + vDirection.y * fPercentage;
	vIntersection->z = vLineStart->z + vDirection.z * fPercentage;

	return true;
}

//-----------------------------------------------------------------------------
// Name : isPointInsideTriangle
// Desc : Determine wether a point in 3D space, "vIntersectionPoint", can be
//        considered to be inside of the three vertices of a triangle as
//        defined by "tri".
//-----------------------------------------------------------------------------
bool isPointInsideTriangle( vector3f *vIntersectionPoint, triangle *tri )
{
	vector3f vVectors[3];
	float fTotalAngle = 0.0f; // As radians

	//
	// Create and normalize three vectors that radiate out from the
	// intersection point towards the triangle's three vertices.
	//

	vVectors[0] = *vIntersectionPoint - tri->v0;
	vVectors[0].normalize();

	vVectors[1] = *vIntersectionPoint - tri->v1;
	vVectors[1].normalize();

	vVectors[2] = *vIntersectionPoint - tri->v2;
	vVectors[2].normalize();

	//
	// We then sum together the angles that exist between each of the vectors.
	//
	// Here's how:
	//
	// 1. Use dotProduct() to get cosine of the angle between the two vectors.
	// 2. Use acos() to convert cosine back into an angle.
	// 3. Add angle to fTotalAngle to keep track of running sum.
	//

	fTotalAngle  = acos( dotProduct( vVectors[0], vVectors[1] ) );
	fTotalAngle += acos( dotProduct( vVectors[1], vVectors[2] ) );
	fTotalAngle += acos( dotProduct( vVectors[2], vVectors[0] ) );

	//
	// If we are able to sum together all three angles and get 360.0, the
	// intersection point is inside the triangle.
	//
	// We can check this by taking away 6.28 radians (360 degrees) away from
	// fTotalAngle and if we're left with 0 (allowing for some tolerance) the
	// intersection point is definitely inside the triangle.
	//

	if( fabsf( fTotalAngle - 6.28f ) < 0.01f )
		return true;

	return false;
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
	updateViewMatrix();

    //
	// Place one of the triangles in motion to demonstrate collision detection.
	//

	static bool bMoveBack = true;

	if( g_bMoveSpheres == true )
	{
        float fMoveAmount = 2.0f * g_fElapsedTime;

		if( bMoveBack == true )
		{
			g_tri1.v0.x -= fMoveAmount;
			g_tri1.v1.x -= fMoveAmount;
			g_tri1.v2.x -= fMoveAmount;
			g_tri1.vCenter.x -= fMoveAmount;

			if( g_tri1.vCenter.x < -7.0f )
				bMoveBack = false;
		}
		else
		{
			g_tri1.v0.x += fMoveAmount;
			g_tri1.v1.x += fMoveAmount;
			g_tri1.v2.x += fMoveAmount;
			g_tri1.vCenter.x += fMoveAmount;

			if( g_tri1.vCenter.x > 7.0f )
				bMoveBack = true;
		}
	}

	//
    // Check for collisions...
	//

	Collision nCollisionStateOfSpheres = COLLISION_NO;
	Collision nCollisionStateOfTris    = COLLISION_NO;

    if( doSpheresIntersect( &g_tri1, &g_tri2 ) == true )
    {
		// Hmmm... the spheres are colliding, so it's possible that the triangles are colliding as well.
		nCollisionStateOfSpheres = COLLISION_YES;

		// We'll start off by checking tri1 against tri2
        if( doTrianglesIntersect( g_tri1, g_tri2 ) == true )
			nCollisionStateOfTris = COLLISION_YES;
        else
        {
			// Ok, tr1 doesn't seem to intersect tri2, but maybe tri2 will intersect tr1.
            if( doTrianglesIntersect( g_tri2, g_tri1 ) == true )
				nCollisionStateOfTris = COLLISION_YES;
        }
    }
	else
	{
		// The spheres aren't colliding, so the triangles couldn't
		// possibly be colliding either.
		nCollisionStateOfTris = COLLISION_NOT_CHECKED;
	}

	//
	// Print out collision states for both spheres and triangles...
	//
    
	if( nCollisionStateOfSpheres == COLLISION_NO )
		cout << "Spheres = COLLISION_NO   |  ";
	else if( nCollisionStateOfSpheres == COLLISION_YES )
		cout << "Spheres = COLLISION_YES  |  ";

	if( nCollisionStateOfTris == COLLISION_NO )
		cout << "Triangles = COLLISION_NO" << endl;
	else if( nCollisionStateOfTris == COLLISION_YES )
		cout << "Triangles = COLLISION_YES" << endl;
	else if( nCollisionStateOfTris == COLLISION_NOT_CHECKED )
		cout << "Triangles = COLLISION_NOT_CHECKED" << endl;

	//
    // Draw triangle 1...
	//

    if( nCollisionStateOfTris == COLLISION_NO ||
		nCollisionStateOfTris == COLLISION_NOT_CHECKED )
        glColor3f( 0.0f, 0.0f, 1.0f );
    else if( nCollisionStateOfTris == COLLISION_YES )
        glColor3f( 1.0f, 0.0f, 0.0f );

    glBegin(GL_POLYGON);
	{
		glVertex3f( g_tri1.v0.x, g_tri1.v0.y, g_tri1.v0.z );
		glVertex3f( g_tri1.v1.x, g_tri1.v1.y, g_tri1.v1.z );
		glVertex3f( g_tri1.v2.x, g_tri1.v2.y, g_tri1.v2.z );
	}
    glEnd();

    if( g_bDrawBoundingSpheres == true )
    {
        if( nCollisionStateOfSpheres == COLLISION_NO )
            glColor3f( 0.0f, 0.0f, 1.0f );
        else if( nCollisionStateOfSpheres == COLLISION_YES )
            glColor3f( 1.0f, 0.0f, 0.0f );

        glPushMatrix();
        glTranslatef( g_tri1.vCenter.x, g_tri1.vCenter.y, g_tri1.vCenter.z );
        renderWireSphere( g_tri1.fRadius, 16, 16 );
        glPopMatrix();
    }

	//
    // Draw triangle 2...
	//

    if( nCollisionStateOfTris == COLLISION_NO ||
		nCollisionStateOfTris == COLLISION_NOT_CHECKED )
        glColor3f( 0.0f, 1.0f, 0.0f );
    else if( nCollisionStateOfTris == COLLISION_YES )
        glColor3f( 1.0f, 1.0f, 0.0f );

    glBegin(GL_POLYGON);
	{
		glVertex3f( g_tri2.v0.x, g_tri2.v0.y, g_tri2.v0.z );
		glVertex3f( g_tri2.v1.x, g_tri2.v1.y, g_tri2.v1.z );
		glVertex3f( g_tri2.v2.x, g_tri2.v2.y, g_tri2.v2.z );
	}
    glEnd();

	//
    // Draw bounding spheres...
	//

    if( g_bDrawBoundingSpheres == true )
    {
        if( nCollisionStateOfSpheres == COLLISION_NO )
            glColor3f( 0.0f, 1.0f, 0.0f );
        else if( nCollisionStateOfSpheres == COLLISION_YES )
            glColor3f( 1.0f, 1.0f, 0.0f );

        glPushMatrix();
        glTranslatef( g_tri2.vCenter.x, g_tri2.vCenter.y, g_tri2.vCenter.z );
        renderWireSphere( g_tri2.fRadius, 16, 16 );
        glPopMatrix();
    }

    //
    // Draw the X, Y, and Z axis...
    //

    glInterleavedArrays( GL_C4UB_V3F, 0, g_lineVertices );
    glDrawArrays( GL_LINES, 0, 6 );


    if( g_bDoubleBuffered )
        glXSwapBuffers( g_pDisplay, g_window ); // Buffer swap does implicit glFlush
    else
        glFlush(); // Explicit flush for single buffered case 
}

