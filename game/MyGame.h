#pragma once

class CMyGame : public CGame
{
public:
	// Game variables
	int score; // score
	int lives; // hitpoints
	int level; // what level you are on
	int increaseX; // for scaling
	bool gamewon; // for the win condition 
	int normalcooldown; // the cool down for the normal shot
	int superlasercooldown; // the cooldown for the supershot
	int mode; // for changing between endless and level based gameplay
	// Declare sprites
	CSprite player;	// player spaceship

	CSprite startScreen; 
	CSprite background;
	CSpriteList shotList;  // list of shots
	CSpriteList enemyList; // first list of enemies  
	CSpriteList rocketlist; // second list of enemies 
	CSpriteList ufolist; // third list of enemies 
	CSpriteList debrislist; // list of debris 
	CSpriteList bombs; // list of enemy shots
	
	CSpriteList explosions; // list of explosion animations

	// sounds
	CSoundPlayer shotSound; // plays when the shot is shot
	CSoundPlayer explosionSound; // plays an explosion
	CSoundPlayer win;// if a player wins
	CSoundPlayer lose;// if a player lose
	CSoundPlayer playerdamage;// if a player is hit
	CSoundPlayer music;// the menu and game musis
	CSoundPlayer levelchange; // if the level changes
	CSoundPlayer supershotcharge;// if the supershot isn't charged this plays
	// -- my own member functions
	void EnemyControl();
	virtual void Onlevelchange();// changes the level and spawns more enemies 
	virtual void Wincon();// the win condition if played on level mode
	
public:
	CMyGame(void);
	~CMyGame(void);

	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);
	
	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnGameOver();
	virtual void OnTerminate();
	
	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
