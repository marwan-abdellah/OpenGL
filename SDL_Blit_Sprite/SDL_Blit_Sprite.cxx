//-----------------------------------------------------------------------------
//           Name: sdl_blit_sprite.cpp
//         Author: Kevin Harris
//  Last Modified: 08/09/04
//    Description: This sample demonstrates how to blit an animated sprite to 
//                 an SDL_Surface.
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
SDL_Surface *g_screenSurface = NULL;
SDL_Surface *g_donutSurface  = NULL;

const int UPDATE_INTERVAL = 30;

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int main(int argc, char *argv[]);
void init(void);
void shutDown(void);
void loadBMP(void);
Uint32 timeLeft(void);
void render(void);

//-----------------------------------------------------------------------------
// Name: main()
// Desc: 
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    init();

    loadBMP();

    bool bDone = false;

    while( bDone == false )
    {
        SDL_Event event;

        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )  
                bDone = true;

            if( event.type == SDL_KEYDOWN )
            {
                if( event.key.keysym.sym == SDLK_ESCAPE ) 
                    bDone = true;
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

    atexit( SDL_Quit );

    g_screenSurface = SDL_SetVideoMode( 640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );

    if( g_screenSurface == NULL )
    {
        printf( "Unable to set 640x480 video: %s\n", SDL_GetError() );
        exit(1);
    }
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )
{
    SDL_FreeSurface( g_donutSurface );
    SDL_FreeSurface( g_screenSurface );
}

//-----------------------------------------------------------------------------
// Name: loadBMP()
// Desc: 
//-----------------------------------------------------------------------------
void loadBMP( void )
{
    // Load the BMP file into a surface
    g_donutSurface = SDL_LoadBMP( "donut.bmp" );

    if( g_donutSurface == NULL ) 
    {
        //fprintf(stderr, "Couldn't load \"donut.bmp\"", SDL_GetError());
        return;
    }

    SDL_SetColorKey( g_donutSurface, SDL_SRCCOLORKEY, 
                     SDL_MapRGB( g_donutSurface->format, 0, 0, 0) );
}

//-----------------------------------------------------------------------------
// Name: timeLeft()
// Desc: 
//-----------------------------------------------------------------------------
Uint32 timeLeft( void )
{
    static Uint32 timeToNextUpdate = 0;
    Uint32 currentTime;

    currentTime = SDL_GetTicks();

    if( timeToNextUpdate <= currentTime ) 
    {
        timeToNextUpdate = currentTime + UPDATE_INTERVAL;
        return 0;
    }

    return( timeToNextUpdate - currentTime );
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: 
//-----------------------------------------------------------------------------
void render( void )
{
    SDL_Delay( timeLeft() );

    SDL_FillRect( g_screenSurface, NULL, SDL_MapRGB( g_screenSurface->format, 0, 0, 255));
    
    //
    // Blit the bitmap surface to the main surface
    //

    static int nPosition = 0;
	static int nFrame    = 0;

    SDL_Rect srcRect;
    SDL_Rect destRect;

    // Build a source SDL_Rect which will copy only a small portion of the texture.
    srcRect.x = (( nFrame % 5 ) * 64);
    srcRect.y = (( nFrame / 5 ) * 64);
	srcRect.w = 64;
    srcRect.h = 64;

    destRect.x = nPosition;
    destRect.y = 200;
    destRect.w = 64;
    destRect.h = 64;

    SDL_BlitSurface( g_donutSurface, &srcRect, g_screenSurface, &destRect );

    // Update the changed portion of the screen
    SDL_UpdateRects( g_screenSurface, 1, &destRect );

    //
	// Increment the sprite's frame number. Our sprite's animation sequence 
    // consists of 30 frames (0-29).
	//

	++nFrame;
    if( nFrame > 29 )
		nFrame = 0;

    //
    // Slowly move our sprite across the screen. This demonstrates how to use
    // a SDL_Rect destination rect with the SDL_BlitSurface method to place a 
    // sprite on the main screen surface.
    //

	++nPosition;
    if( nPosition > 640 )
		nPosition = 0;

    SDL_Flip( g_screenSurface );
}
