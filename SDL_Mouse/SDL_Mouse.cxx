//-----------------------------------------------------------------------------
//           Name: sdl_mouse.cpp
//         Author: Kevin Harris
//  Last Modified: 08/09/04
//    Description: This sample demonstrates how to create a simple paint 
//                 program using mouse input under SDL.
//
//   Control Keys: Left Mouse Button  - Paint a single white pixel
//                 Right Mouse Button - Clear the screen
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
SDL_Surface *g_screenSurface = NULL;

bool g_clearScreen = false;
bool g_leftMouseButtonDown = false;
int  g_currentMouse_X;
int  g_currentMouse_Y;

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int main(int argc, char *argv[]);
void init(void);
void shutDown(void);
void scrollStars(void);
void renderPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
void render(void);

//-----------------------------------------------------------------------------
// Name: main()
// Desc: 
//-----------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    init();

    bool bDone = false;

    while( bDone == false )
    {
        SDL_Event event;

        while( SDL_PollEvent( &event ) )
        {
            switch( event.type ) 
            {
                case SDL_MOUSEMOTION:
                {
                    g_currentMouse_X = event.motion.x;
                    g_currentMouse_Y = event.motion.y;
                }
                break;

                case SDL_MOUSEBUTTONUP:
                {
                    switch( event.button.button )
                    {
		                case SDL_BUTTON_LEFT:
                            g_leftMouseButtonDown = false;
                            break;
                    }
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    switch( event.button.button )
                    {
		                case SDL_BUTTON_LEFT:
                            g_leftMouseButtonDown = true;
                            break;

                        case SDL_BUTTON_RIGHT:
                            g_clearScreen = true;
                            break;
                    }
                }
                break;

                case SDL_QUIT: 
                    bDone = true;
                    break;
            }
        }
        
        render();
    }

    shutDown();

    return 0;
}

//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void init( void )
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        exit(1);
    }

    g_screenSurface = SDL_SetVideoMode( 640, 480, 8, SDL_HWSURFACE | SDL_DOUBLEBUF );

    if( g_screenSurface == NULL )
    {
        printf( "Unable to set video: %s\n", SDL_GetError() );
        exit(1);
    }
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )
{
    SDL_FreeSurface( g_screenSurface );

    SDL_Quit();
}

//-----------------------------------------------------------------------------
// Name: renderPixel()
// Desc: 
//-----------------------------------------------------------------------------
void renderPixel( int x, int y, Uint8 R, Uint8 G, Uint8 B )
{
    Uint32 color = SDL_MapRGB( g_screenSurface->format, R, G, B );

    switch( g_screenSurface->format->BytesPerPixel )
    {
        case 1: // Assuming 8-bpp
        {
            Uint8 *bufp;
            bufp = (Uint8 *)g_screenSurface->pixels + y*g_screenSurface->pitch + x;
            *bufp = color;
        }
        break;

        case 2: // Probably 15-bpp or 16-bpp
        {
            Uint16 *bufp;
            bufp = (Uint16 *)g_screenSurface->pixels + y*g_screenSurface->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3: // Slow 24-bpp mode, usually not used
        {
            Uint8 *bufp;
            bufp = (Uint8 *)g_screenSurface->pixels + y*g_screenSurface->pitch + x * 3;

            if( SDL_BYTEORDER == SDL_LIL_ENDIAN )
            {
                bufp[0] = color;
                bufp[1] = color >> 8;
                bufp[2] = color >> 16;
            }
            else
            {
                bufp[2] = color;
                bufp[1] = color >> 8;
                bufp[0] = color >> 16;
            }
        }
        break;

        case 4: // Probably 32-bpp
        {
            Uint32 *bufp;
            bufp = (Uint32 *)g_screenSurface->pixels + y*g_screenSurface->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: 
//-----------------------------------------------------------------------------
void render( void )
{
    if( g_clearScreen )
    {
        SDL_FillRect( g_screenSurface, NULL, SDL_MapRGB( g_screenSurface->format, 0, 0, 0));
        g_clearScreen = false;
    }

    // Lock the screen's surface...
    if( SDL_MUSTLOCK( g_screenSurface ) )
    {
        if( SDL_LockSurface(g_screenSurface) < 0 )
            return;
    }

    if( g_leftMouseButtonDown )
        renderPixel( g_currentMouse_X, g_currentMouse_Y, 255, 255, 255 );

    // Unlock the screen's surface...
    if( SDL_MUSTLOCK( g_screenSurface ) )
        SDL_UnlockSurface( g_screenSurface );

    SDL_Flip( g_screenSurface );
}
