// code/level.h
// Module: in-game action
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Module states                                                   //
/////////////////////////////////////////////////////////////////////

#define MSLV_READY      0
#define MSLV_IDLE       1
#define MSLV_GAMEOVER   2


/////////////////////////////////////////////////////////////////////
// Sprite definitions                                              //
/////////////////////////////////////////////////////////////////////

#define SPR_NONE        0
#define SPR_PLAYER      1
#define SPR_BULLET      2
#define SPR_WALLGEN     3
#define SPR_WALL        4
#define SPR_BOSS        5
#define SPR_BOSSBULLET  6
#define SPR_LASERFOCUS  7
#define SPR_LASER       8
#define SPR_POWERUP     9
#define SPR_POWBULLET  10
#define SPR_WHITEPART  11
#define SPR_REDPART    12
#define SPR_BULLETPART 13

#define MAX_SPRITES   128
#define SPRITE_DIV    256


/////////////////////////////////////////////////////////////////////
// Collision types                                                 //
/////////////////////////////////////////////////////////////////////

#define COL_NONE        0
#define COL_PLAYER      1
#define COL_GOODSHOT    2
#define COL_ENEMY       3
#define COL_ENEMYSHOT   4
#define COL_POWERUP     5
#define COL_PLAYERHIT   6


/////////////////////////////////////////////////////////////////////
// Powerup types                                                   //
/////////////////////////////////////////////////////////////////////

#define PUP_NONE        0
#define PUP_RAPIDFIRE   1
#define PUP_TRIPLESHOT  2
#define PUP_POWERSHOT   3
#define PUP_SWITCH      4


/////////////////////////////////////////////////////////////////////
// Other definitions                                               //
/////////////////////////////////////////////////////////////////////

#define MAX_TIMEDIFF   25 //Maximum amount of time difference


/////////////////////////////////////////////////////////////////////
// Structures                                                      //
/////////////////////////////////////////////////////////////////////

typedef struct
{
	RECT rc;
	long x;
	long y;
	long state1;
	long state2;
	long state3;
	long state4;
	long state5;
	unsigned long anitimer;
	unsigned long anidelay;
	unsigned long filter;
	unsigned long filterparam;
	unsigned char aniframes;
	unsigned char type;
	unsigned char action;
	unsigned char health;
	unsigned char coltype;
}SPRITE;


/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

class ClassLevel : public ClassModule
{
	private:
		SPRITE sprite[MAX_SPRITES];
		ClassEVTimer spritetimer;
		bool colmask[256];
		unsigned char lives;
		unsigned char powerup;
		unsigned char curlevel;
		unsigned long score;
		unsigned long msgtimer;
		unsigned long timerspeed;
		unsigned long puptimer;
		unsigned long CreateSprite(long x, long y, unsigned char type, unsigned char property);
		void UpdateSprites(unsigned long timedelta);
		void GetAnimRect(RECT *dst, RECT *src, long frame);
		void OnCollision(unsigned long nr, unsigned char withtype);
		void CheckCollision(void);
		void CreateLevel(void);
		void RenderSprites(E2DSurface *buffer);
		void RenderStatus(E2DSurface *buffer);
		void RenderScreen(E2DSurface *buffer);
	public:
		ClassLevel(void);
		~ClassLevel(void);
		void OnMinimize(void);
		void OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta);
};


/////////////////////////////////////////////////////////////////////
// ClassLevel::Private                                             //
/////////////////////////////////////////////////////////////////////

//Create a new sprite
unsigned long ClassLevel::CreateSprite(long x, long y, unsigned char type, unsigned char property)
{
	unsigned long ctr, found = MAX_SPRITES;
	for (ctr = 0; ctr < MAX_SPRITES; ctr++)
		if (sprite[ctr].type == SPR_NONE)
		{
			found = ctr;
			break;
		}
	if (found < MAX_SPRITES)
	{
		SetRect(&sprite[found].rc, 0, 0, 0, 0);
		sprite[found].type = type;
		sprite[found].anitimer = 0;
		sprite[found].anidelay = 1;
		sprite[found].aniframes = 1;
		sprite[found].filter = EFX_COLORKEY;
		sprite[found].filterparam = 0;
		sprite[found].health = 0;
		sprite[found].coltype = COL_NONE;
		sprite[found].state1 = 0;
		sprite[found].state2 = 0;
		sprite[found].state3 = 0;
		sprite[found].state4 = 0;
		sprite[found].state5 = 0;
		switch(type)
		{
			case SPR_PLAYER:
				SetRect(&sprite[found].rc, 12, 0, 18, 6);
				sprite[found].coltype = COL_PLAYER;
				sprite[found].state3 = 500;
				break;
			case SPR_BULLET:
				if (property == 0)
					SetRect(&sprite[found].rc, 30, 0, 32, 1);
				else if (property == 1)
					SetRect(&sprite[found].rc, 30, 2, 32, 4);
				else if (property == 2)
					SetRect(&sprite[found].rc, 30, 4, 32, 6);
				sprite[found].coltype = COL_GOODSHOT;
				sprite[found].state1 = property;
				break;
			case SPR_WALLGEN:
				sprite[found].state1 = property;
				sprite[found].state2 = 15;
				if (curlevel < 5)
					sprite[found].state3 = 1200;
				else if (curlevel < 9)
					sprite[found].state3 = 600;
				else
					sprite[found].state3 = 300;
				break;
			case SPR_WALL:
				if (shared->ecd->math->Rand() % 100 < 15)
				{
					SetRect(&sprite[found].rc, 0, 10, 4, 14);
					sprite[found].state4 = 1;
				}
				else
					SetRect(&sprite[found].rc, 0, 6, 4, 10);
				sprite[found].coltype = COL_ENEMY;
				sprite[found].aniframes = 8;
				sprite[found].anidelay = 64;
				sprite[found].health = 1 + curlevel / 2;
				if (sprite[found].health > 5)
					sprite[found].health = 5;
				if (property == 0)
					sprite[found].state1 = -1;
				else
					sprite[found].state1 = 1;
				if (curlevel < 5)
					sprite[found].state1 *= 1;
				else if (curlevel < 9)
					sprite[found].state1 *= 2;
				else
					sprite[found].state1 *= 4;
				break;
			case SPR_BOSS:
				SetRect(&sprite[found].rc, 12, 18, 18, 24);
				sprite[found].coltype = COL_ENEMY;
				sprite[found].health = 4;
				break;
			case SPR_BOSSBULLET:
				SetRect(&sprite[found].rc, 24, 16, 28, 18);
				sprite[found].coltype = COL_ENEMYSHOT;
				break;
			case SPR_LASERFOCUS:
				SetRect(&sprite[found].rc, 24, 14, 26, 16);
				sprite[found].aniframes = 4;
				sprite[found].anidelay = 64;
				break;
			case SPR_LASER:
				SetRect(&sprite[found].rc, 28, 17, 32, 18);
				sprite[found].coltype = COL_ENEMYSHOT;
				break;
			case SPR_POWERUP:
				SetRect(&sprite[found].rc, property * 4, 14, 4 + property * 4, 18);
				sprite[found].coltype = COL_POWERUP;
				sprite[found].state1 = property;
				break;
			case SPR_POWBULLET:
				SetRect(&sprite[found].rc, 30, 1, 32, 2);
				sprite[found].coltype = COL_GOODSHOT;
				break;
			case SPR_WHITEPART:
				SetRect(&sprite[found].rc, 30, 18, 31, 19);
				sprite[found].filter = EFX_COLORKEY | EFX_ALPHA;
				sprite[found].state1 = property * 16;
				break;
			case SPR_REDPART:
				SetRect(&sprite[found].rc, 31, 18, 32, 19);
				sprite[found].filter = EFX_COLORKEY | EFX_ALPHA;
				sprite[found].state1 = property * 16;
				break;
			case SPR_BULLETPART:
				SetRect(&sprite[found].rc, 28, 16, 30, 17);
				break;
		}
		SetRect(&sprite[found].rc, sprite[found].rc.left * DIM_SPRITE, sprite[found].rc.top * DIM_SPRITE, sprite[found].rc.right * DIM_SPRITE, sprite[found].rc.bottom * DIM_SPRITE);
		sprite[found].x = (x - (sprite[found].rc.right - sprite[found].rc.left) / 2) * SPRITE_DIV;
		sprite[found].y = (y - (sprite[found].rc.bottom - sprite[found].rc.top) / 2) * SPRITE_DIV;
	}
	return(found);
}

//Sprite movement
void ClassLevel::UpdateSprites(unsigned long timedelta)
{
	unsigned long ctr;
	for (ctr = 0; ctr < MAX_SPRITES; ctr++)
	{
		sprite[ctr].anitimer += timedelta;
		switch(sprite[ctr].type)
		{
			case SPR_PLAYER:
				if (modulestate == MSLV_IDLE)
				{
					long tomove = 0, wheeldir = shared->ecd->input->GetWheelScroll();
					if (wheeldir < 0)
						sprite[ctr].state2 = timedelta * 10;
					else if (wheeldir > 0)
						sprite[ctr].state2 = timedelta * -10;
					if (shared->ecd->input->KeyHold(DIAM_MENUUP))
						tomove = timedelta * MOVE_SPEED * -6;
					else if (shared->ecd->input->KeyHold(DIAM_MENUDOWN))
						tomove = timedelta * MOVE_SPEED * 6;
					else if (shared->ecd->input->ClickHold())
					{
						long clickx, clicky;
						shared->ecd->input->GetClickPosition(clickx, clicky);
						if (clickx < DIM_SPRITE * 12)
						{
							long sprite_y = sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2;
							if (clicky < sprite_y)
							{
								tomove = timedelta * MOVE_SPEED * -6;
								if (sprite_y + tomove / SPRITE_DIV < clicky)
									tomove = (clicky - sprite_y) * SPRITE_DIV;
							}
							else
							{
								tomove = timedelta * MOVE_SPEED * 6;
								if (sprite_y + tomove / SPRITE_DIV > clicky)
									tomove = (clicky - sprite_y) * SPRITE_DIV;
							}
						}
					}
					if (sprite[ctr].state2 != 0)
					{
						if (ClassEMath::Abs(sprite[ctr].state2) < (long)timedelta)
						{
							if (tomove == 0)
								tomove = sprite[ctr].state2;
							sprite[ctr].state2 = 0;
						}
						else
						{
							if (sprite[ctr].state2 < 0)
							{
								if (tomove == 0)
									tomove = timedelta * MOVE_SPEED * -6;
								sprite[ctr].state2 += timedelta;
							}
							else
							{
								if (tomove == 0)
									tomove = timedelta * MOVE_SPEED * 6;
								sprite[ctr].state2 -= timedelta;
							}
						}
					}
					if (powerup == PUP_SWITCH)
						tomove = -tomove;
					sprite[ctr].y += tomove;
					sprite[ctr].state1 += tomove * 8 / MOVE_SPEED / 5;
					if (sprite[ctr].state1 < 0)
					{
						if (ClassEMath::Abs(sprite[ctr].state1) < (long)timedelta * 5)
							sprite[ctr].state1 = 0;
						else
							sprite[ctr].state1 += timedelta * 5;
						if (sprite[ctr].state1 < -3000)
							sprite[ctr].state1 = -3000;
					}
					else
					{
						if (ClassEMath::Abs(sprite[ctr].state1) < (long)timedelta * 5)
							sprite[ctr].state1 = 0;
						else
							sprite[ctr].state1 -= timedelta * 5;
						if (sprite[ctr].state1 > 3000)
							sprite[ctr].state1 = 3000;
					}
					if (sprite[ctr].y / SPRITE_DIV < (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / -2)
						sprite[ctr].y = (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / -2 * SPRITE_DIV;
					if (sprite[ctr].y / SPRITE_DIV > YRES - (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2)
						sprite[ctr].y = (YRES - (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2) * SPRITE_DIV;
					if (shared->ecd->input->KeyHold(DIAM_MENUCONFIRM_SP) || shared->cfg.autoshot)
					{
						sprite[ctr].state3 += timedelta;
						while (sprite[ctr].state3 >= 500)
						{
							if (powerup == PUP_POWERSHOT)
								CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_POWBULLET, 0);
							else
								CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BULLET, 0);
							if (powerup == PUP_TRIPLESHOT)
							{
								CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BULLET, 1);
								CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BULLET, 2);
							}
							if (powerup == PUP_RAPIDFIRE)
								sprite[ctr].state3 -= 200;
							else
								sprite[ctr].state3 -= 500;
						}
					}
					else if (sprite[ctr].state3 < 500)
						sprite[ctr].state3 += timedelta;
					if (sprite[ctr].state4 > (long)timedelta)
						sprite[ctr].state4 -= timedelta;
					else
						sprite[ctr].state4 = 0;
					if (sprite[ctr].state4 > 0)
						sprite[ctr].coltype = COL_PLAYERHIT;
					else
						sprite[ctr].coltype = COL_PLAYER;
				}
				break;
			case SPR_BULLET:
				if (sprite[ctr].state1 == 1 || sprite[ctr].state1 == 2)
				{
					if (sprite[ctr].state1 == 1)
						sprite[ctr].y -= timedelta * MOVE_SPEED * 6;
					else
						sprite[ctr].y += timedelta * MOVE_SPEED * 6;
					sprite[ctr].x += timedelta * MOVE_SPEED * 10;
				}
				else
					sprite[ctr].x += timedelta * MOVE_SPEED * 12;
				if (sprite[ctr].x / SPRITE_DIV >= XRES)
					sprite[ctr].type = SPR_NONE;
				break;
			case SPR_WALLGEN:
				sprite[ctr].state4 += timedelta;
				while(sprite[ctr].state4 >= sprite[ctr].state3 && sprite[ctr].state2 > 0)
				{
					sprite[ctr].state4 -= sprite[ctr].state3;
					sprite[ctr].state2--;
					CreateSprite(sprite[ctr].x / SPRITE_DIV, sprite[ctr].y / SPRITE_DIV, SPR_WALL, (unsigned char)sprite[ctr].state1);
				}
				if (sprite[ctr].state2 == 0)
					sprite[ctr].type = SPR_NONE;
				break;
			case SPR_WALL:
				if (sprite[ctr].state3)
				{
					if (sprite[ctr].state3 <= (long)timedelta)
					{
						sprite[ctr].filter = EFX_COLORKEY;
						sprite[ctr].state3 = 0;
					}
					else
					{
						sprite[ctr].filter = EFX_COLORKEY | EFX_COLORIZE;
						sprite[ctr].filterparam = ECOLOR_WHITE;
						sprite[ctr].state3 -= timedelta;
					}
				}
				sprite[ctr].y += timedelta * MOVE_SPEED * sprite[ctr].state1;
				sprite[ctr].state2 += ClassEMath::Abs(sprite[ctr].state1) * timedelta;
				while(sprite[ctr].state2 >= 18000)
				{
					if (sprite[ctr].state1 < 0)
						sprite[ctr].y += 18000 * MOVE_SPEED;
					else
						sprite[ctr].y -= 18000 * MOVE_SPEED;
					sprite[ctr].state2 -= 18000;
				}
				break;
			case SPR_BOSS:
				if (sprite[ctr].state5)
				{
					if (sprite[ctr].state5 <= (long)timedelta)
					{
						sprite[ctr].filter = EFX_COLORKEY;
						sprite[ctr].state5 = 0;
					}
					else
					{
						sprite[ctr].filter = EFX_COLORKEY | EFX_COLORIZE;
						sprite[ctr].filterparam = ECOLOR_WHITE;
						sprite[ctr].state5 -= timedelta;
					}
				}
				if (modulestate == MSLV_IDLE)
				{
					bool newroutine = false;
					long tomove = 0;
					if (sprite[ctr].y / SPRITE_DIV < (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / -2)
					{
						sprite[ctr].y = (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / -2 * SPRITE_DIV;
						sprite[ctr].state2 = 1;
						newroutine = true;
					}
					if (sprite[ctr].y / SPRITE_DIV > YRES - (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2)
					{
						sprite[ctr].y = (YRES - (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2) * SPRITE_DIV;
						sprite[ctr].state2 = 0;
						newroutine = true;
					}
					if (sprite[ctr].state2 == 0)
						tomove = timedelta * MOVE_SPEED * -6;
					else
						tomove = timedelta * MOVE_SPEED * 6;
					sprite[ctr].y += tomove;
					sprite[ctr].state1 += tomove * 8 / MOVE_SPEED / 5;
					if (sprite[ctr].state1 < 0)
					{
						if (ClassEMath::Abs(sprite[ctr].state1) < (long)timedelta * 5)
							sprite[ctr].state1 = 0;
						else
							sprite[ctr].state1 += timedelta * 5;
						if (sprite[ctr].state1 < -3000)
							sprite[ctr].state1 = -3000;
					}
					else
					{
						if (ClassEMath::Abs(sprite[ctr].state1) < (long)timedelta * 5)
							sprite[ctr].state1 = 0;
						else
							sprite[ctr].state1 -= timedelta * 5;
						if (sprite[ctr].state1 > 3000)
							sprite[ctr].state1 = 3000;
					}
					switch(sprite[ctr].state3)
					{
						case 3:
						case 4:
							if (sprite[ctr].state4 == 0)
							{
								CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 2, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BOSSBULLET, 0);
								sprite[ctr].state4 = 500 + shared->ecd->math->Rand() % 500;
							}
							else
							{
								if (sprite[ctr].state4 > (long)timedelta)
									sprite[ctr].state4 -= timedelta;
								else
									sprite[ctr].state4 = 0;
							}
							if (newroutine)
								sprite[ctr].state3--;
							if (sprite[ctr].state3 == 2)
								sprite[ctr].state3 = 0;
							break;
						case 5:
							if (sprite[ctr].state4 == 0)
							{
								CreateSprite(sprite[ctr].x / SPRITE_DIV, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_LASERFOCUS, 0);
								sprite[ctr].state4 = 500;
							}
							else
							{
								if (sprite[ctr].state4 > (long)timedelta)
									sprite[ctr].state4 -= timedelta;
								else
									sprite[ctr].state4 = 0;
							}
							if (newroutine)
							{
								unsigned long sctr;
								for (sctr = 0; sctr < MAX_SPRITES; sctr++)
									if (sprite[sctr].type == SPR_LASERFOCUS)
									{
										sprite[sctr].type = SPR_NONE;
										CreateSprite(sprite[sctr].x / SPRITE_DIV + (sprite[sctr].rc.right - sprite[sctr].rc.left) / 2, sprite[sctr].y / SPRITE_DIV + (sprite[sctr].rc.bottom - sprite[sctr].rc.top) / 2, SPR_LASER, 0);
									}
								sprite[ctr].state3 = 1;
							}
							break;
						default:
							if (newroutine)
							{
								if (sprite[ctr].state3 == 0)
								{
									sprite[ctr].state3 = 4 + shared->ecd->math->Rand() % 2;
									sprite[ctr].state4 = 0;
									if (sprite[ctr].state3 == 5)
										sprite[ctr].state4 = shared->ecd->math->Rand() % 2 * 250;
								}
								else if (sprite[ctr].state3 < 5)
									sprite[ctr].state3--;
							}
					}
				}
				break;
			case SPR_BOSSBULLET:
				sprite[ctr].x -= timedelta * MOVE_SPEED * 12;
				if (sprite[ctr].state1 < (long)timedelta)
				{
					CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE, sprite[ctr].y / SPRITE_DIV + DIM_SPRITE, SPR_BULLETPART, 0);
					sprite[ctr].state1 = 384 + shared->ecd->math->Rand() % 128;
				}
				else
					sprite[ctr].state1 -= timedelta;
				if (sprite[ctr].x / SPRITE_DIV + (sprite[ctr].rc.right - sprite[ctr].rc.left) < 0)
					sprite[ctr].type = SPR_NONE;
				break;
			case SPR_LASER:
				sprite[ctr].x -= timedelta * MOVE_SPEED * 16;
				if (sprite[ctr].x / SPRITE_DIV + (sprite[ctr].rc.right - sprite[ctr].rc.left) < 0)
					sprite[ctr].type = SPR_NONE;
				break;
			case SPR_POWERUP:
				sprite[ctr].x -= timedelta * MOVE_SPEED * 2;
				if (sprite[ctr].x / SPRITE_DIV + (sprite[ctr].rc.right - sprite[ctr].rc.left) < 0)
					sprite[ctr].type = SPR_NONE;
				break;
			case SPR_POWBULLET:
				sprite[ctr].x += timedelta * MOVE_SPEED * 10;
				if (sprite[ctr].x / SPRITE_DIV >= XRES)
					sprite[ctr].type = SPR_NONE;
				break;
			case SPR_WHITEPART:
			case SPR_REDPART:
				sprite[ctr].x += (long)timedelta * MOVE_SPEED * shared->ecd->math->Sin(sprite[ctr].state1) / 10000;
				sprite[ctr].y -= (long)timedelta * MOVE_SPEED * shared->ecd->math->Cos(sprite[ctr].state1) / 10000;
				sprite[ctr].state2 += timedelta;
				sprite[ctr].rc.top = (18 + sprite[ctr].state2 * 6 / 512) * DIM_SPRITE;
				sprite[ctr].rc.bottom = sprite[ctr].rc.top + DIM_SPRITE;
				if (sprite[ctr].state2 >= 512)
					sprite[ctr].type = SPR_NONE;
				break;
			case SPR_BULLETPART:
				sprite[ctr].x += timedelta * MOVE_SPEED * 5;
				sprite[ctr].state1 += timedelta;
				sprite[ctr].rc.left = (28 + sprite[ctr].state1 * 2 / 512 * 2) * DIM_SPRITE;
				sprite[ctr].rc.right = sprite[ctr].rc.left + DIM_SPRITE * 2;
				if (sprite[ctr].state1 >= 512)
					sprite[ctr].type = SPR_NONE;
				break;
		}
	}
}

//Calculate a rectangle based on animation
void ClassLevel::GetAnimRect(RECT *dst, RECT *src, long frame)
{
	SetRect(dst, src->left + frame * (src->right - src->left), src->top, src->right + frame * (src->right - src->left), src->bottom);
}

//On collision callback
void ClassLevel::OnCollision(unsigned long nr, unsigned char withtype)
{
	if (sprite[nr].type == SPR_NONE || withtype == SPR_NONE)
		return;
	if (sprite[nr].type == SPR_BULLET || sprite[nr].type == SPR_POWBULLET)
	{
		unsigned char direction = (unsigned char)(shared->ecd->math->Rand() % 32);
		CreateSprite(sprite[nr].x / SPRITE_DIV + DIM_SPRITE, sprite[nr].y / SPRITE_DIV, SPR_WHITEPART, 160 + direction);
		CreateSprite(sprite[nr].x / SPRITE_DIV + DIM_SPRITE, sprite[nr].y / SPRITE_DIV, SPR_WHITEPART, 192 + direction);
		sprite[nr].type = SPR_NONE;
	}
	else if (sprite[nr].type == SPR_WALL && (withtype == SPR_BULLET || withtype == SPR_POWBULLET))
	{
		sprite[nr].state3 = 150;
		if (sprite[nr].health > 0 && withtype == SPR_BULLET)
		{
			sprite[nr].health--;
		}
		else
		{
			unsigned char ctr, direction = (unsigned char)(shared->ecd->math->Rand() % 32);
			sprite[nr].type = SPR_NONE;
			score += 10 * curlevel;
			if (sprite[nr].state4)
				CreateSprite(sprite[nr].x / SPRITE_DIV + DIM_SPRITE * 2, sprite[nr].y / SPRITE_DIV + DIM_SPRITE * 2, SPR_POWERUP, (unsigned char)(shared->ecd->math->Rand() % 4));
			for (ctr = 0; ctr < 8; ctr++)
				CreateSprite(sprite[nr].x / SPRITE_DIV + DIM_SPRITE * 2, sprite[nr].y / SPRITE_DIV + DIM_SPRITE * 2, SPR_REDPART, ctr * 32 + direction);
		}
	}
	else if (sprite[nr].type == SPR_PLAYER && sprite[nr].state4 == 0)
	{
		if (withtype == SPR_BOSSBULLET || withtype == SPR_LASER)
		{
			if (lives > 0)
			{
				sprite[nr].state4 = 2000;
				lives--;
			}
			else
			{
				sprite[nr].type = SPR_NONE;
				modulestate = MSLV_GAMEOVER;
				msgtimer = 0;
			}
		}
	}
	else if (sprite[nr].type == SPR_BOSS)
	{
		if (sprite[nr].health > 0)
		{
			sprite[nr].health--;
			sprite[nr].state5 = 150;
		}
		else
		{
			unsigned long ctr;
			for (ctr = 0; ctr < MAX_SPRITES; ctr++)
				if (sprite[ctr].type != SPR_PLAYER)
					sprite[ctr].type = SPR_NONE;
			curlevel++;
			score += 1000;
			CreateLevel();
			modulestate = MSLV_READY;
			msgtimer = 0;
		}
	}
	else if (sprite[nr].type == SPR_POWERUP)
	{
		sprite[nr].type = SPR_NONE;
		powerup = (unsigned char)sprite[nr].state1 + 1;
		puptimer = 5000;
	}
}

//Check for collision between sprites
void ClassLevel::CheckCollision(void)
{
	unsigned long ctr1, ctr2, numsprites = 0;
	unsigned char type1, type2;
	for (ctr1 = 0; ctr1 < MAX_SPRITES; ctr1++)
		if (sprite[ctr1].type != SPR_NONE)
			if (sprite[ctr1].coltype != COL_NONE)
				numsprites = ctr1 + 1;
	for (ctr1 = 0; ctr1 < numsprites; ctr1++)
		for (ctr2 = ctr1 + 1; ctr2 < numsprites; ctr2++)
			if (colmask[sprite[ctr1].coltype * 16 + sprite[ctr2].coltype])
				if (shared->sheet.Intersect(sprite[ctr1].x / SPRITE_DIV, sprite[ctr1].y / SPRITE_DIV, &sprite[ctr1].rc, sprite[ctr2].x / SPRITE_DIV, sprite[ctr2].y / SPRITE_DIV, &sprite[ctr2].rc, &shared->sheet))
				{
					type1 = sprite[ctr1].type;
					type2 = sprite[ctr2].type;
					OnCollision(ctr1, type2);
					OnCollision(ctr2, type1);
				}
}

//Create a new level
void ClassLevel::CreateLevel(void)
{
	unsigned char ctr, num_generators;
	num_generators = 2 + (curlevel + 1) / 2;
	if (num_generators > 6)
		num_generators = 6;
	for (ctr = 0; ctr < num_generators; ctr++)
	{
		if (ctr % 2 == 0)
			CreateSprite(XRES / 2 - num_generators * DIM_SPRITE * 3 + ctr * DIM_SPRITE * 6, YRES / 2 + DIM_SPRITE * 32, SPR_WALLGEN, 0);
		else
			CreateSprite(XRES / 2 - num_generators * DIM_SPRITE * 3 + ctr * DIM_SPRITE * 6, YRES / 2 - DIM_SPRITE * 32, SPR_WALLGEN, 1);
	}
	CreateSprite(XRES - DIM_SPRITE * 8, YRES / 2, SPR_BOSS, 0);
	if (curlevel < 5)
		timerspeed = 17000;
	else if (curlevel < 9)
		timerspeed = 9000;
	else
		timerspeed = 5000;
	spritetimer.Reset(0);
	spritetimer.SetSpeed(timerspeed);
	powerup = PUP_NONE;
	puptimer = 0;
}

//Render all sprites
void ClassLevel::RenderSprites(E2DSurface *buffer)
{
	RECT rc;
	unsigned long ctr;
	for (ctr = 0; ctr < MAX_SPRITES; ctr++)
		if (sprite[ctr].type != SPR_NONE)
		{
			if (sprite[ctr].type == SPR_PLAYER || sprite[ctr].type == SPR_BOSS)
				GetAnimRect(&rc, &sprite[ctr].rc, sprite[ctr].state1 / 1024);
			else
				GetAnimRect(&rc, &sprite[ctr].rc, sprite[ctr].anitimer / sprite[ctr].anidelay % sprite[ctr].aniframes);
			if (!(sprite[ctr].type == SPR_PLAYER && sprite[ctr].state4 % 300 >= 150))
				buffer->BltFast(sprite[ctr].x / SPRITE_DIV, sprite[ctr].y / SPRITE_DIV, &shared->sheet, &rc, sprite[ctr].filter, sprite[ctr].filterparam);
		}
}

//Render the player status on the screen
void ClassLevel::RenderStatus(E2DSurface *buffer)
{
	char showstr[32];
	unsigned long tmp_score;
	unsigned char ctr;
	ClassEStd::StrCpy(showstr, "Lives: ");
	ClassEStd::StrCat(showstr, lives);
	buffer->DrawFont(DIM_SPRITE / 2, DIM_SPRITE / 2, &shared->font, showstr);
	ClassEStd::StrCpy(showstr, "Score: 0000000");
	tmp_score = score;
	for (ctr = 0; ctr < 7; ctr++)
	{
		showstr[ClassEStd::StrLen(showstr) - 1 - ctr] = (unsigned char)(tmp_score % 10 + '0');
		tmp_score /= 10;
	}
	buffer->DrawFont(XRES - DIM_SPRITE / 2, DIM_SPRITE / 2, &shared->font, showstr, EFX_COLORKEY | EFO_HRIGHT);
}

//Render the graphics on the screen
void ClassLevel::RenderScreen(E2DSurface *buffer)
{
	RECT rc;
	buffer->BltFast(0, 0, &shared->gamebg, NULL);
	RenderSprites(buffer);
	#if !defined(DEVICE_GP2X) && !defined(DEVICE_IPHONE)
		SetRect(&rc, DIM_SPRITE * 4, YRES - DIM_SPRITE * 3 - shared->font.GetHeight(), XRES / 2 - DIM_SPRITE * 4, YRES - DIM_SPRITE);
		buffer->FillRect(&rc, E_RGBX(16, 48, 32), EFX_OPACITY, 128);
		buffer->DrawRect(&rc, E_RGBX(16, 48, 32));
		if (modulestate == MSLV_IDLE)
			buffer->DrawFont(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, &shared->font, "Minimize", EFX_COLORKEY | EFO_HCENTER | EFO_VCENTER);
		else
			buffer->DrawFont(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, &shared->font, "Minimize", EFX_COLORKEY | EFX_OPACITY | EFO_HCENTER | EFO_VCENTER, 64);
	#endif
	SetRect(&rc, XRES / 2 + DIM_SPRITE * 4, YRES - DIM_SPRITE * 3 - shared->font.GetHeight(), XRES - DIM_SPRITE * 4, YRES - DIM_SPRITE);
	buffer->FillRect(&rc, E_RGBX(16, 48, 32), EFX_OPACITY, 128);
	buffer->DrawRect(&rc, E_RGBX(16, 48, 32));
	if (modulestate == MSLV_IDLE)
		buffer->DrawFont(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, &shared->font, "Pause", EFX_COLORKEY | EFO_HCENTER | EFO_VCENTER);
	else
		buffer->DrawFont(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, &shared->font, "Pause", EFX_COLORKEY | EFX_OPACITY | EFO_HCENTER | EFO_VCENTER, 64);
	if (modulestate == MSLV_READY)
		if (msgtimer % 384 < 192)
			buffer->DrawFont(XRES / 2, YRES / 2, &shared->font, "Ready", EFX_COLORKEY | EFO_HCENTER | EFO_VCENTER);
	if (modulestate == MSLV_GAMEOVER)
		buffer->DrawFont(XRES / 2, YRES / 2, &shared->font, "Game Over", EFX_COLORKEY | EFO_HCENTER | EFO_VCENTER);
	RenderStatus(buffer);
}


/////////////////////////////////////////////////////////////////////
// ClassLevel::Public                                              //
/////////////////////////////////////////////////////////////////////

//ClassLevel: Constructor
ClassLevel::ClassLevel(void)
{
	unsigned short ctr;
	for (ctr = 0; ctr < 256; ctr++)
		colmask[ctr] = false;
	colmask[COL_PLAYER    * 16 + COL_ENEMYSHOT] = true;
	colmask[COL_PLAYER    * 16 + COL_POWERUP]   = true;
	colmask[COL_GOODSHOT  * 16 + COL_ENEMY]     = true;
	colmask[COL_ENEMY     * 16 + COL_GOODSHOT]  = true;
	colmask[COL_ENEMYSHOT * 16 + COL_PLAYER]    = true;
	colmask[COL_POWERUP   * 16 + COL_PLAYER]    = true;
	colmask[COL_POWERUP   * 16 + COL_PLAYERHIT] = true;
	colmask[COL_PLAYERHIT * 16 + COL_POWERUP]   = true;
}

//ClassLevel: Destructor
ClassLevel::~ClassLevel(void)
{
}

//Module minimize callback event
void ClassLevel::OnMinimize(void)
{
	if (modulestate != MSLV_GAMEOVER)
	{
		shared->module = MODULE_MENU;
		shared->modulemsg = MM_FROMMODULE + MODULE_LEVEL;
	}
}

//Module game loop
void ClassLevel::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta)
{
	if (*modulemsg == MM_INIT)
	{
		unsigned long ctr;
		for (ctr = 0; ctr < MAX_SPRITES; ctr++)
			sprite[ctr].type = SPR_NONE;
		CreateSprite(DIM_SPRITE * 8, YRES / 2, SPR_PLAYER, 0);
		lives = 2;
		score = 0;
		curlevel = shared->cfg.startlevel;
		CreateLevel();
		modulestate = MSLV_READY;
		msgtimer = 0;
		*modulemsg = MM_NONE;
	}
	else if (*modulemsg == MM_FROMMODULE + MODULE_MENU)
	{
		if (modulestate != MSLV_GAMEOVER)
			modulestate = MSLV_READY;
		msgtimer = 0;
		*modulemsg = MM_NONE;
	}
	else if (*modulemsg == MM_NONE)
	{
		unsigned long timediff;
		if (modulestate == MSLV_READY || modulestate == MSLV_GAMEOVER)
		{
			msgtimer += timedelta;
			if ((modulestate == MSLV_READY && msgtimer >= 1000) || (modulestate == MSLV_GAMEOVER && msgtimer >= 3000))
			{
				if (modulestate == MSLV_GAMEOVER)
				{
					*module = MODULE_MENU;
					*modulemsg = MM_INIT;
				}
				else if (modulestate == MSLV_READY)
					modulestate = MSLV_IDLE;
			}
		}
		if (modulestate == MSLV_IDLE)
		{
			E_INPUTEVENT inputevent;
			while(shared->ecd->input->GetClickEvent(&inputevent))
			{
				if (inputevent.type == EIE_PRESSED)
				{
					if (inputevent.clicky >= (long)(YRES - shared->font.GetHeight() - DIM_SPRITE * 3))
					{
						if (inputevent.clickx < XRES / 2)
						{
							shared->forceminimize = true;
						}
						else
						{
							*module = MODULE_MENU;
							*modulemsg = MM_FROMMODULE + MODULE_LEVEL;
						}
					}
				}
			}
			if (shared->ecd->input->GetKeyEvent(DIAM_MENUSOFT1) == EIE_PRESSED)
			{
				shared->forceminimize = true;
			}
			else if (shared->ecd->input->GetKeyEvent(DIAM_MENUSOFT2) == EIE_PRESSED)
			{
				*module = MODULE_MENU;
				*modulemsg = MM_FROMMODULE + MODULE_LEVEL;
			}
		}
		if (puptimer <= timedelta)
		{
			puptimer = 0;
			powerup = PUP_NONE;
		}
		else
			puptimer -= timedelta;
		if (timerspeed > 1000)
		{
			if (curlevel < 5)
				timerspeed -= timedelta * 16;
			else if (curlevel < 9)
				timerspeed -= timedelta * 8;
			else
				timerspeed -= timedelta * 4;
			if (timerspeed < 1000)
				timerspeed = 1000;
		}
		spritetimer.SetSpeed(timerspeed);
		spritetimer.Increase(timedelta);
		timediff = spritetimer.RetrieveDelta();
		while(timediff > 0)
		{
			if (timediff >= MAX_TIMEDIFF)
			{
				UpdateSprites(MAX_TIMEDIFF);
				timediff -= MAX_TIMEDIFF;
			}
			else
			{
				UpdateSprites(timediff);
				timediff = 0;
			}
		}
		CheckCollision();
		RenderScreen(buffer);
	}
}
