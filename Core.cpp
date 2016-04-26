#include "Core.hpp"


Core::Core(){
  SDL_Init(SDL_INIT_EVERYTHING);
  this->screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  this->world = new b2World(b2Vec2(0.0, 9.81), true);


  this->wid = 640;
  this->hgt = 480;
  this->len = 20.0;
  this->div = 1 / this->len;
}

void			Core::putPoint(SDL_Surface* dest, int x, int y, int r, int g, int b){
  if(x >= 0 && x < dest->w && y >= 0 && y < dest->h)
    ((Uint32*)dest->pixels)[y*dest->pitch / 4 + x] = SDL_MapRGB(dest->format, r, g, b);
}

void			Core::mySwap(int& a, int& b)
{
  int			val;
  
  val = a;
  a = b;
  b = val;
}

void			Core::putLine(SDL_Surface* dest, int x0, int y0, int x1, int y1)
{
  bool			step;
  float			error;
  float			y;
  float			roundError;
  int			ystep;
     
  step = abs(y1 - y0) > abs(x1 - x0);
  if (step)
    {
      mySwap(x0, y0);
      mySwap(x1, y1);
    }
       
  if (x0 > x1)
    {
      mySwap(x1, x0);
      mySwap(y1, y0);
    }
  
  error = 0.0;
  y = y0;
  roundError = (float)abs(y1 - y0) / (x1 - x0);
  ystep = (y1 > y0 ? 1 : -1);
  
  for (int i = x0; i < x1; i++)
    {
      if (step)
	this->putPoint(dest, y, i, 255, 255, 255);
      else
	this->putPoint(dest, i, y, 255, 255, 255);
      error += roundError;
      if (error >= 0.5)
	{
	  y += ystep;
	  error -= 1;
	}
    }
}

b2Body*			Core::putRect(int x, int y, int w, int h, bool dyn)
{
  b2BodyDef		bodydef;
  b2Body*		body;
  b2PolygonShape	shape;
  b2FixtureDef		fixturedef;

  bodydef.position.Set(x * this->div, y * this->div);
  if (dyn)
    bodydef.type = b2_dynamicBody;
  body = this->world->CreateBody(&bodydef);
  shape.SetAsBox(this->div * w / 2, this->div * h /2);
  fixturedef.shape = &shape;
  fixturedef.density = 1.0;
  body->CreateFixture(&fixturedef);     
}


void			Core::putSquare(b2Vec2* points, b2Vec2 center, float angle)
{
  for(int i = 0; i < 4; i++)
    this->putLine(this->screen, points[i].x * this->len, points[i].y * this->len, points[(i + 1) > 3 ? 0 : (i + 1)].x * this->len, points[(i + 1) > 3 ? 0 : (i + 1)].y * this->len);

}

void			Core::Update()
{
  b2Body*		tmp;
  b2Vec2		points[4];

  SDL_FillRect(this->screen, NULL, 0);
  tmp = this->world->GetBodyList();
  while (tmp)
    {
      for(int i = 0; i < 4; i++)
	{
	  points[i] = ((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);
	  this->setAngle(points[i], tmp->GetWorldCenter(), tmp->GetAngle());
	}
      this->putSquare(points, tmp->GetWorldCenter(), tmp->GetAngle());
      tmp = tmp->GetNext();
    }
}

void			Core::setAngle(b2Vec2& vector, const b2Vec2& center, float angle)
{
  b2Vec2		tmp;

  tmp.x = vector.x * cos(angle) - vector.y * sin(angle);
  tmp.y = vector.x * sin(angle) + vector.y * cos(angle);
  vector = tmp + center;
}

void			Core::Loop(){
  Uint32		beg;
  SDL_Event		event;
  bool			loop;
  
  loop = true;
  this->putRect(this->wid / 2, this->hgt - 50, this->wid, 30, false);
  while (loop)
    {
      beg = SDL_GetTicks();
      while (SDL_PollEvent(&event))
	{
	  switch(event.type)
	    {
	    case SDL_QUIT:
	      loop = false;
	      break;
	    case SDL_KEYDOWN:
	      switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
		  loop = false;
		  break;
		}
	      break;
	    case SDL_MOUSEBUTTONDOWN:
	      this->putRect(event.button.x, event.button.y, 20, 20, true);
	      break;
	    }
	}
      this->Update();
      this->world->Step(1.0 / 30.0, 8, 3);
      SDL_Flip(this->screen);
      if (1000.0 / 30 > SDL_GetTicks() - beg)
	SDL_Delay(1000.0 / 30 - (SDL_GetTicks() - beg));
    }
  this->Quit();
}

void			Core::Quit(){
  SDL_Quit();
}
