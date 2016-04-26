#ifndef CORE_HPP_
# define CORE_HPP_

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <Box2D/Box2D.h>


class		Core
{
protected:
  int		wid;
  int		hgt;
  float		len;
  float		div;
  b2World*	world;
  SDL_Surface*	screen;

public:
  Core();
  void		Loop();
  void		putPoint(SDL_Surface*, int,int,int,int,int);
  void		mySwap(int&, int&);
  void		putLine(SDL_Surface*, int,int,int,int);
  b2Body*	putRect(int,int,int,int,bool);
  void		putSquare(b2Vec2*, b2Vec2, float);
  void		Update();
  void		setAngle(b2Vec2&, const b2Vec2&, float);
  void		Quit();

};

#endif /* CORE_HPP_ */
