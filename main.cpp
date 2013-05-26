/*
TODO: 
   Allow the user to edit the mass of the planets
   Allow screen resizing
   Optimize it bish
*/

#include "Planet.h"
#include "GravityHandler.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#endif

const int SCREEN_BPP	= 32;
const int FPS_LIMIT     = 60;

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

  //Load in all the pictures here
  moon_pic  = load_image("img/moon_pic.png");
  bg_pic    = load_image("img/bg_pic.png");
  earth_pic = load_image("img/earth_pic.png");
  pause_pic = load_image("img/pause_pic.png");
  
  //Placing some number of planets in random positions
  srand(time(NULL));

  std::vector<Planet> planets;

  GravityHandler grav;

  apply_surface( 0, 0, bg_pic, screen );

  for(unsigned int i = 0; i < planets.size(); i++)
      apply_surface(planets.at(i).get_x_pos(), planets.at(i).get_y_pos(), planets.at(i).get_pic(), screen);
	
  int fps_init = 0;

  bool paused = false;
  bool usr_paused = false;
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
	      if(event.key.keysym.sym == SDLK_SPACE)
		{
		  if(usr_paused)
		    SDL_WM_SetCaption("Gravity", NULL);
		  else
		    SDL_WM_SetCaption("Gravity -PAUSED-", NULL);
		  usr_paused = !usr_paused;
		}
	      else if(event.key.keysym.sym == SDLK_c)
		planets.clear();

	    }
	  
	  if(event.type == SDL_MOUSEBUTTONDOWN)
	    {
	      Planet p;
	      paused = true;
	      if(event.button.button == SDL_BUTTON_RIGHT)
		{
		  p.set_mass(7.348 * pow(10, 22));
		  p.set_pic(*moon_pic);
		  p.set_x_pos(event.button.x - 16);
		  p.set_y_pos(event.button.y - 16);
		}

	      else if(event.button.button == SDL_BUTTON_LEFT)
		{		
		  p.set_mass(5.972 * pow(10, 24));
		  p.set_pic(*earth_pic);
		  p.set_x_pos(event.button.x - 32);
		  p.set_y_pos(event.button.y - 32);
		}
	      
	      planets.push_back(p);
	    }

	  else if(event.type == SDL_MOUSEBUTTONUP)
	    {
	      if(event.button.button == SDL_BUTTON_LEFT)
		{
		  planets.at(planets.size() - 1).set_y_vel((double)(planets.at(planets.size() - 1).get_y_pos() + 32 - event.button.y) / (1000 * PI));
		  planets.at(planets.size() - 1).set_x_vel(-(double)(planets.at(planets.size() - 1).get_x_pos() + 32 - event.button.x) / (1000 * PI));
		}
	      else if(event.button.button == SDL_BUTTON_RIGHT)
		{
		  planets.at(planets.size() - 1).set_y_vel((double)(planets.at(planets.size() - 1).get_y_pos() + 16 - event.button.y) / (1000 * PI));
		  planets.at(planets.size() - 1).set_x_vel(-(double)(planets.at(planets.size() - 1).get_x_pos() + 16 - event.button.x) / (1000 * PI));
		}
	      paused = false;
	    }

	  else if(event.type == SDL_QUIT)
	    quit = true;
	  
	}	

      //LOGIC
      if(!paused && !usr_paused)
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

      //run the time when paused or else the time gaps causes the planets to apache
      else
	{
	  for(unsigned int i = 0; i < planets.size(); i++)
	    planets.at(i).run_time();
	}

      //RENDERING
      apply_surface( 0, 0, bg_pic, screen );
      for(unsigned int i = 0; i < planets.size(); i++)
	apply_surface(planets.at(i).get_x_pos(), planets.at(i).get_y_pos(), planets.at(i).get_pic(), screen);
      if(usr_paused)
	apply_surface(0, 0, pause_pic, screen);
      
      
      if( SDL_Flip( screen ) == -1 )
	return 1;

      if((SDL_GetTicks() - fps_init) < 1000 / FPS_LIMIT )
	SDL_Delay( ( 1000 / FPS_LIMIT ) - (SDL_GetTicks() - fps_init) );
      
    }

  SDL_FreeSurface(earth_pic);
  SDL_FreeSurface(moon_pic);
  SDL_FreeSurface(sun_pic);
  SDL_FreeSurface(bg_pic);

  SDL_Quit();
  return 0;  
   
}
