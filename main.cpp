#include "Planet.h"
#include "GravityHandler.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

const int SCREEN_BPP	= 32;

const int FPS_LIMIT     = 30;

SDL_Surface *earth_pic	= NULL;
SDL_Surface *moon_pic	= NULL;
SDL_Surface *sun_pic    = NULL;
SDL_Surface *bg_pic    	= NULL;
SDL_Surface *screen 	= NULL;
SDL_Surface *pause_pic  = NULL;

SDL_Event event;

SDL_Surface *load_image( std::string filename )
{
  SDL_Surface* loaded_image = NULL;
  SDL_Surface* optimized_image = NULL;
	
  loaded_image = IMG_Load( filename.c_str() );
	
  if( loaded_image != NULL )
    {
      optimized_image = SDL_DisplayFormat( loaded_image );
      SDL_FreeSurface( loaded_image );
    }
	
  if( optimized_image != NULL )
    {
      Uint32 color_key = SDL_MapRGB( optimized_image->format, 0xFF, 0, 0xFF ); 
      SDL_SetColorKey( optimized_image, SDL_SRCCOLORKEY, color_key );
    }
	
  return optimized_image;
	
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* dest )
{
  SDL_Rect offset;
	
  offset.x = x;
  offset.y = y;
	
  SDL_BlitSurface( source, NULL, dest, &offset);
	
}

bool init()
{
  if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    return false;
		
  screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
  
  if( screen == NULL )
    return false;
		
  SDL_WM_SetCaption( "Gravity", NULL );
	
  return true;
}

int main( int argc, char* args[] )
{
 	
  bool quit = false;
	
  init();

  moon_pic  = load_image("img/moon_pic.png");
  bg_pic    = load_image("img/bg_pic.png");
  sun_pic   = load_image("img/sun_pic.png");
  earth_pic = load_image("img/earth_pic.png");
  pause_pic = load_image("img/pause_pic.png");
  
  srand(time(NULL));

  Planet earth = Planet(5.972 * pow(10, 24), rand() % (SCREEN_WIDTH - 64) + 32, rand() % (SCREEN_HEIGHT - 64) + 32, 0, 0, *earth_pic);
  Planet moon = Planet(7.348 * pow(10, 22), rand() % (SCREEN_WIDTH - 32) + 16, rand() % (SCREEN_HEIGHT - 32) + 16, 0, 0, *moon_pic);
  Planet moon2 = Planet(7.348 * pow(10, 22), rand() % (SCREEN_WIDTH - 32) + 16, rand() % (SCREEN_HEIGHT - 32) + 16, 0, 0, *moon_pic);

  std::vector<Planet> planets;
  planets.push_back(earth);
  planets.push_back(moon);
  planets.push_back(moon2);

  GravityHandler grav;

  apply_surface( 0, 0, bg_pic, screen );

  for(unsigned int i = 0; i < planets.size(); i++)
      apply_surface(planets.at(i).get_x_pos(), planets.at(i).get_y_pos(), planets.at(i).get_pic(), screen);
	
  int fps_init = 0;
  bool paused = false;
  while( !quit )
    {
      fps_init = SDL_GetTicks();

      //EVENT HANDLING
      while( SDL_PollEvent( &event ) )
	{
	  for(unsigned int i = 0; i < planets.size(); i++)
	    planets.at(i).handle_events(event);
	  if(event.type == SDL_KEYDOWN)
	    {
	      switch(event.key.keysym.sym)
	      case SDLK_SPACE:
		{
		  if(paused)
		    SDL_WM_SetCaption("Gravity", NULL);
		  else
		    SDL_WM_SetCaption("Gravity -PAUSED-", NULL);
		  paused = !paused;
		}
	      break;
	    }
	  if(event.type == SDL_QUIT)
	    quit = true;
	}

      //LOGIC
      if(!paused)
	{
	  std::vector<Vector2d> forces;
      
	  for(unsigned int i = 0; i < planets.size(); i++)
	    {
	      for(unsigned int j = 0; j < planets.size(); j++)
		{
		  if( i != j)
		    forces.push_back(grav.calculate_force(planets.at(i).get_mass(), planets.at(j).get_mass(), planets.at(i).get_x_pos(), planets.at(i).get_y_pos(), planets.at(j).get_x_pos(), planets.at(j).get_y_pos()));
		  else
		    {
		      Vector2d zed;
		      zed.magnitude = 0;
		      zed.direction = 0;
		      forces.push_back(zed);
		    }
		}
	    }

	  //zero the acceleration every time so the forces can be added correctly
	  for(unsigned int i = 0; i < planets.size(); i++)
	    planets.at(i).zero_acc();

	  for(unsigned int i = 0; i < planets.size(); i++)
	    for(unsigned int j = 0; j < planets.size(); j++)
	      planets.at(i).set_force(forces.at((planets.size() * i) + j));
	  
	  for(unsigned int i = 0; i < planets.size(); i++)
	    planets.at(i).handle_logic();
	}

      //run the time or else the time gaps causes the planets to apache
      else
	{
	  for(unsigned int i = 0; i < planets.size(); i++)
	    planets.at(i).run_time();
	}

      //RENDERING
      apply_surface( 0, 0, bg_pic, screen );
      for(unsigned int i = 0; i < planets.size(); i++)
	apply_surface(planets.at(i).get_x_pos(), planets.at(i).get_y_pos(), planets.at(i).get_pic(), screen);
      if(paused)
	apply_surface(0, 0, pause_pic, screen);
      
      
      if( SDL_Flip( screen ) == -1 )
	return 1;
      
      if((SDL_GetTicks() - fps_init) < 1000 / FPS_LIMIT )
	SDL_Delay( ( 1000 / FPS_LIMIT ) - (SDL_GetTicks() - fps_init) );
    }

  SDL_FreeSurface( earth_pic );
  SDL_FreeSurface( moon_pic );
  SDL_FreeSurface( bg_pic );

  SDL_Quit();
  return 0;  
   
}
