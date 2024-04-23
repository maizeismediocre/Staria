#include "stdafx.h"
#include "MyGame.h"

CMyGame::CMyGame(void)
	// to initialise more sprites here use a comma-separated list
{
	// TODO: add initialisation here
	score = 0;
	level = 0;
	
	increaseX = 0;
	gamewon = false;
	normalcooldown = 0;
	superlasercooldown = 0;
	mode = 0;
	lives = 0;
}


CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	if (IsMenuMode() || IsGameOver()) return;


	long t = GetTime();
	// general cooldowns for the two shots
	if (normalcooldown > 0) normalcooldown--;
	if (superlasercooldown > 0) superlasercooldown--;
	
	// update sprites
	player.Update(t);

	// update sprite lists
	for (CSprite* pShot : shotList) pShot->Update(t);//update shots
	for (CSprite* pEnemy : enemyList) pEnemy->Update(t);//update enemies
	for (CSprite* ufo : ufolist) ufo->Update(t);//  update bombs
	for (CSprite* rocket : rocketlist) rocket->Update(t);
	for (CSprite* pExplosion : explosions) pExplosion->Update(t); //  update explosions
	for (CSprite* bomb : bombs) bomb->Update(t); // update bombs
	for (CSprite* debris : debrislist) debris->Update(t);// update debris

	// ---- game rules -----
	// delete shot if reached the top of the screen
	for (CSprite* shot : shotList)
	{

		if (shot->GetY() > 600) shot->Delete();
	}
	// remove deleted shotList from list
	shotList.delete_if(deleted);

	for (CSprite* bomb : bombs)
	{

		if (bomb->GetY() < 0) bomb->Delete();
	}
	// remove deleted shotList from list
	bombs.delete_if(deleted);
	Onlevelchange();
	Wincon();

	// remove an enemy once it has left the game world 
	// repeat 2 times for the other 2 enemy lists
	for (CSprite* pEnemy : enemyList)
	{
		if (pEnemy->GetY() < 0)
		{
			GameOver();
			pEnemy->Delete();
			lose.Play("gameover.wav");

		}
	}
	enemyList.delete_if(deleted);

	for (CSprite* ufo : ufolist)
	{
		if (ufo->GetY() < 0)
		{
			GameOver();
			ufo->Delete();
			lose.Play("gameover.wav");
		}
	}
	ufolist.delete_if(deleted);
	for (CSprite* rocket : rocketlist)
	{
		if (rocket->GetY() < 0)
		{
			GameOver();
			rocket->Delete();
			lose.Play("gameover.wav");
		}
	}
	rocketlist.delete_if(deleted);
	// creating bombs dynamically at random enemy position 
	// repeat 2 times for the other 2 enemy lists
	for (CSprite* pEnemy : enemyList)
	{
		if (rand() % 5000 == 0)
		{
			CSprite* bomb = new CSprite(pEnemy->GetX(), pEnemy->GetY(), 0, 0, GetTime());
			bomb->AddImage("shot2.png", "enemyshot", 10, 1, 0, 0, 9, 0);
			bomb->SetAnimation("enemyshot");
			bomb->SetMotion(0, -200);

			bombs.push_back(bomb);
		}

	}
	for (CSprite* ufo : ufolist)
	{
		if (rand() % 1000 == 0)
		{
			CSprite* bomb = new CSprite(ufo->GetX(), ufo->GetY(), 0, 0, GetTime());
			bomb->AddImage("shot2.png", "enemyshot", 10, 1, 0, 0, 9, 0);
			bomb->SetAnimation("enemyshot");
			bomb->SetMotion(0, -200);

			bombs.push_back(bomb);
		}
	}
	for (CSprite* rocket : rocketlist)
	{

		if (rand() % 2000 == 0)
		{
			CSprite* bomb = new CSprite(rocket->GetX(), rocket->GetY(), 0, 0, GetTime());
			bomb->AddImage("shot2.png", "enemyshot", 10, 1, 0, 0, 9, 0);
			bomb->SetAnimation("enemyshot");
			bomb->SetMotion(0, -200);

			bombs.push_back(bomb);
		}
	}


	


	

	//  collision detection between player and bombs
	
	for (CSprite* bomb : bombs)
	{
		if (player.HitTest(bomb))
		{
			lives--;
			playerdamage.Play("playerhit.wav");
			bomb->Delete();
			CSprite* pExplosion = new CSprite(player.GetX(), player.GetY(), 0, 0, GetTime());
			// setup explosion animation sequence
			pExplosion->AddImage("Explosion.png", "explode", 12, 1, 0, 0, 11, 0);
			// play explosion animation
			pExplosion->SetAnimation("explode", 12);
			// set explosion animation to die in 500ms
			pExplosion->Die(500); // remove in 500ms
			// add explosion animation sprite to explosions sprite lists
			explosions.push_back(pExplosion);
			if (lives == 0)
			{
				GameOver();
				lose.Play("gameover.wav");
			}

		}

	}
	explosions.delete_if(deleted);
	bombs.delete_if(deleted);
	// player collision detection for debris
	for (CSprite* debris : debrislist)
	{
		if (player.HitTest(debris))
		{
			lives--;
			playerdamage.Play("playerhit.wav");
			debris->Delete();
			CSprite* pExplosion = new CSprite(player.GetX(), player.GetY(), 0, 0, GetTime());
			// setup explosion animation sequence
			pExplosion->AddImage("Explosion.png", "explode", 12, 1, 0, 0, 11, 0);
			// play explosion animation
			pExplosion->SetAnimation("explode", 12);
			// set explosion animation to die in 500ms
			pExplosion->Die(500); // remove in 500ms
			// add explosion animation sprite to explosions sprite list
			explosions.push_back(pExplosion);
			if (lives == 0)
			{
				GameOver();
				lose.Play("gameover.wav");
			}
		}
		explosions.delete_if(deleted);
	}
	// hit detection for all enemy types
	for (CSprite* pEnemy : enemyList)
	{
		if (player.HitTest(pEnemy))
		{
			GameOver();
			pEnemy->Delete();
			lose.Play("gameover.wav");

		}
	}
	for (CSprite* ufo : ufolist)
	{
		if (player.HitTest(ufo))
		{
			GameOver();
			ufo->Delete();
			lose.Play("gameover.wav");

		}
	}
	for (CSprite* rocket : rocketlist)
	{
		if (player.HitTest(rocket))
		{
			GameOver();
			rocket->Delete();
			lose.Play("gameover.wav");

		}
	}

	// game over rule
	if (score < 0) GameOver();

	// --- player control ----------

	// if left key pressed move left, if right key pressed move right, otherwise don't move
	if (IsKeyDown(SDLK_a)) player.SetMotion(-400, 0);
	else if (IsKeyDown(SDLK_d)) player.SetMotion(400, 0);
	else player.SetMotion(0, 0);
	if (player.GetX() > 800)
	{
		player.SetX(0);
	}
	if (player.GetX() < 0)
	{
		player.SetX(800);
	}

	// ---- enemy control ----------
	
	EnemyControl();

	// test every shot against every enemy
	// repeat two times for the other 2 enemies
	for (CSprite* pShot : shotList)
	{
		for (CSprite* pEnemy : enemyList)
		{
			if (pShot->HitTest(pEnemy, 1))
				if (pShot->GetHealth() == 25)
			{
				// dynamically create explosion sprite
				CSprite* pExplosion = new CSprite(pEnemy->GetX(), pEnemy->GetY(), 0, 0, GetTime());
				// setup explosion animation sequence
				pExplosion->AddImage("Explosion.png", "explode", 12, 1, 0, 0, 11, 0);
				// play explosion animation
				pExplosion->SetAnimation("explode", 12);
				// set explosion animation to die in 500ms
				pExplosion->Die(500); // remove in 500ms
				// add explosion animation sprite to explosions sprite list
				explosions.push_back(pExplosion);
				// mark shot and enemy for deletion
				pEnemy->Delete();
				score = score +10;
				explosionSound.Play("explosion.wav");
				
			}
				else {
					CSprite* pExplosion = new CSprite(pEnemy->GetX(), pEnemy->GetY(), 0, 0, GetTime());
					// setup explosion animation sequence
					pExplosion->AddImage("Explosion.png", "explode", 12, 1, 0, 0, 11, 0);
					// play explosion animation
					pExplosion->SetAnimation("explode", 12);
					// set explosion animation to die in 500ms
					pExplosion->Die(500); // remove in 500ms
					// add explosion animation sprite to explosions sprite list
					explosions.push_back(pExplosion);
					// mark shot and enemy for deletion
					pShot->Delete();
					pEnemy->Delete();
					score = score + 10;
					
					explosionSound.Play("explosion.wav");
				}
	    }
	}
	// remove deleted sprites
	shotList.delete_if(deleted);
	enemyList.delete_if(deleted);
	explosions.delete_if(deleted);
	// hit detection for the scientist ship
	// spawn the escape pods only if they are not destroyed by the super shot
	for (CSprite* pShot : shotList)
	{
		for (CSprite* ufo : ufolist)
		{
			if (pShot->HitTest(ufo, 1))
			{
				if (pShot->GetHealth() == 25)
				{
					
					// dynamically create explosion sprite
					CSprite* pExplosion = new CSprite(ufo->GetX(), ufo->GetY(), 0, 0, GetTime());
					// setup explosion animation sequence
					pExplosion->AddImage("Explosion.png", "explode", 12, 1, 0, 0, 11, 0);
					// play explosion animation
					pExplosion->SetAnimation("explode", 12);
					// set explosion animation to die in 500ms
					pExplosion->Die(500); // remove in 500ms
					// add explosion animation sprite to explosions sprite list
					explosions.push_back(pExplosion);
					// mark shot and enemy for deletio
					ufo->Delete();
					score = score + 30;
					
					explosionSound.Play("explosion.wav");

				}
				else {
					if (ufo->GetHealth() == 75)
					{
						for (int n = 0; n < 2; n++)
						{
							CSprite* escapepod = new CSprite(float(rand() % 30) + 400, ufo->GetY() + 100, 0, 0, GetTime());
							escapepod->AddImage("Escapepod.png", "fly4", 3, 1, 0, 0, 2, 0);
							escapepod->SetAnimation("fly4", 12);
							escapepod->SetMotion(0, (-50));
							ufolist.push_back(escapepod);
						}
					}
					// dynamically create explosion sprite
					CSprite* pExplosion = new CSprite(ufo->GetX(), ufo->GetY(), 0, 0, GetTime());
					// setup explosion animation sequence
					pExplosion->AddImage("Explosion.png", "explode", 12, 1, 0, 0, 11, 0);
					// play explosion animation
					pExplosion->SetAnimation("explode", 12);
					// set explosion animation to die in 500ms
					pExplosion->Die(500); // remove in 500ms
					// add explosion animation sprite to explosions sprite list
					explosions.push_back(pExplosion);
					// mark shot and enemy for deletion
					pShot->Delete();
					ufo->Delete();
					score = score + 30;
					
					explosionSound.Play("explosion.wav");
				}
			}
		}
	}
	// remove deleted sprites
	shotList.delete_if(deleted);
	ufolist.delete_if(deleted);
	explosions.delete_if(deleted);
	// hit detection between the shots and the exploders 
	// if it by the supershot don't explode into debris 
	for (CSprite* pShot : shotList)
	{
		for (CSprite* rocket : rocketlist)
		{
			if (pShot->HitTest(rocket, 1))
			{
				if (pShot->GetHealth() == 25)
				{



					// dynamically create explosion sprite
					CSprite* pExplosion = new CSprite(rocket->GetX(), rocket->GetY(), 0, 0, GetTime());
					// setup explosion animation sequence
					pExplosion->AddImage("Explosion.png", "explode", 12, 1, 0, 0, 11, 0);
					// play explosion animation
					pExplosion->SetAnimation("explode", 12);
					// set explosion animation to die in 500ms
					pExplosion->Die(500); // remove in 500ms
					// add explosion animation sprite to explosions sprite list
					explosions.push_back(pExplosion);
					// mark shot and enemy for deletion
					
					rocket->Delete();
					score = score + 20;
				
					explosionSound.Play("explosion.wav");

				}
				else
				{
					for (int n = 0; n < 10; n++)
					{
						CSprite* debris = new CSprite(rocket->GetX(), rocket->GetY(), "debris.png", GetTime());
						debris->SetDirection(float(rand() % 180) + 100);
						debris->SetSpeed(500);
						debris->Die(2000);
						debrislist.push_back(debris);
					}
					// dynamically create explosion sprite
					CSprite* pExplosion = new CSprite(rocket->GetX(), rocket->GetY(), 0, 0, GetTime());
					// setup explosion animation sequence
					pExplosion->AddImage("Explosion.png", "explode", 12, 1, 0, 0, 11, 0);
					// play explosion animation
					pExplosion->SetAnimation("explode", 12);
					// set explosion animation to die in 500ms
					pExplosion->Die(500); // remove in 500ms
					// add explosion animation sprite to explosions sprite list
					explosions.push_back(pExplosion);
					// mark shot and enemy for deletion
					pShot->Delete();
					rocket->Delete();
					score = score + 20;
					
					explosionSound.Play("explosion.wav");
					
				}
			}
		}
	}
	// remove deleted sprites
	shotList.delete_if(deleted);
	rocketlist.delete_if(deleted);
	explosions.delete_if(deleted);
	debrislist.delete_if(deleted);
}


void CMyGame::EnemyControl()
{
	// this code controls the movement of the enemy swarm
	// as well as the other enemy types

	for (CSprite* pEnemy : enemyList)
	{

		if (pEnemy->GetX() <= 0)
		{

			if (enemyList.size() <= 36 && enemyList.size() >= 19)
			{
				for (CSprite* pEnemy : enemyList)
				{

					pEnemy->SetMotion(float(50 + increaseX), 0);
					pEnemy->SetY(pEnemy->GetY() - 50);

				}
				break;

			}
			if (enemyList.size() <= 18 && enemyList.size() >= 11)
			{
				for (CSprite* pEnemy : enemyList)
				{
					pEnemy->SetMotion(float(60 + increaseX), 0);
					pEnemy->SetY(pEnemy->GetY() - 50);


				}
				break;
			}
			if (enemyList.size() <= 10 && enemyList.size() >= 0)
			{
				for (CSprite* pEnemy : enemyList)
				{
					pEnemy->SetMotion(float(70 + increaseX), 0);
					pEnemy->SetY(pEnemy->GetY() - 50);

				}
				break;
			}

		}

		if (pEnemy->GetX() >= 800)
		{
			if (enemyList.size() <= 36 && enemyList.size() >= 19)
			{
				for (CSprite* pEnemy : enemyList)
				{
					pEnemy->SetMotion(float(-50 - increaseX), 0);
					pEnemy->SetY(pEnemy->GetY() - 50);


				}
				break;
			}



			if (enemyList.size() <= 18 && enemyList.size() >= 11)
			{
				for (CSprite* pEnemy : enemyList)
				{
					pEnemy->SetMotion(float(-60 - increaseX), 0);
					pEnemy->SetY(pEnemy->GetY() - 50);


				}
				break;
			}
			if (enemyList.size() <= 10 && enemyList.size() >= 0)
			{
				for (CSprite* pEnemy : enemyList)
				{
					pEnemy->SetMotion(float(-70 - increaseX), 0);
					pEnemy->SetY(pEnemy->GetY() - 50);


				}
				break;
			}
		}
	}
	for (CSprite* rocket : rocketlist)
	{

		if (rocket->GetX() <= 0)
		{



			for (CSprite* rocket : rocketlist)
			{

				rocket->SetMotion(float(50 + increaseX), 0);
				rocket->SetY(rocket->GetY() - 25);

			}
			break;



		}

		if (rocket->GetX() >= 800)
		{


			for (CSprite* rocket : rocketlist)
			{
				rocket->SetMotion(float(-50 - increaseX), 0);
				rocket->SetY(rocket->GetY() - 25);


			}
			break;

		}

	}

	for (CSprite* ufo : ufolist)
	{

		if (ufo->GetX() <= 0)
		{



			for (CSprite* ufo : ufolist)
			{

				ufo->SetMotion(float(50 + increaseX), 0);
				ufo->SetY(ufo->GetY() - 25);

			}
			break;
		}


		if (ufo->GetX() >= 800)
		{

			for (CSprite* ufo : ufolist)
			{
				ufo->SetMotion(float(-50 - increaseX), 0);
				ufo->SetY(ufo->GetY() - 25);


			}
			break;




		}
	}
}
		
	
	
	

	




	void CMyGame::OnDraw(CGraphics * g)
	{
		if (IsMenuMode())
		{
			startScreen.Draw(g);
			return;
		}

		// draw sprites
		
		background.Draw(g);
		player.Draw(g);



		// draw sprite lists
		for (CSprite* pSprite : shotList) pSprite->Draw(g);
		for (CSprite* pSprite : enemyList) pSprite->Draw(g); // draw enemyList
		for (CSprite* pExplosion : explosions) pExplosion->Draw(g); //  draw explosions
		for (CSprite* bomb : bombs) bomb->Draw(g); // draw bombs
		for (CSprite* ufo : ufolist) ufo->Draw(g);
		for (CSprite* rocket : rocketlist) rocket->Draw(g);
		for (CSprite* debris : debrislist) debris->Draw(g);
		// print all different game variables
		*g << font(28) << color(CColor::Red()) << xy(10, 540) << "score: " << score;

		*g << font(28) << color(CColor::Green()) << xy(700, 570) << "lives: " << lives;
		if (mode == 1)
		{
			*g << font(28) << color(CColor::Blue()) << xy(10, 570) << "level: " << level;
		}
		if (mode == 2)
		{
			*g << font(28) << color(CColor::Blue()) << xy(10, 570) << "wave: " << level;
		}
		if (superlasercooldown == 0)
		{
			*g << font(28) << color(CColor::Yellow()) << xy(250, 570) << "supershot ready" << endl;
		}
		if (superlasercooldown != 0)
		{
			*g << font(28) << color(CColor::Yellow()) << xy(250, 570) << "supershot recharging";
		}
		*g << font(28) << color(CColor::Red()) << xy(10, 10) << "f2 = reset";
		*g << font(28) << color(CColor::Red()) << xy(10, 40) << "space = pause";

		
		
		// print game over at at the centre of the screen
		if (IsGameOverMode())
		{
			if (gamewon)
			{
				*g << font(46) << color(CColor::Green()) << vcenter << center << "You won";
				*g << font(46) << color(CColor::Green()) << xy(400, 240) << "Your score is " << score;
			}
			else
			{
				*g << font(46) << color(CColor::Red()) << vcenter << center << " GAME OVER ";
				*g << font(46) << color(CColor::Red()) << xy(400, 240) << "Your score is " << score;
			}
		}
	}

	/////////////////////////////////////////////////////
	// Game Life Cycle

	// one time initialisation
	void CMyGame::OnInitialize()
	{


		startScreen.LoadImage("startScreen.bmp");
		startScreen.SetImage("startScreen.bmp");
		background.LoadImage("background.bmp");
		background.SetImage("background.bmp");
		//player sprite animation
		player.AddImage("SpaceshipA.png", "move", 5, 1, 0, 0, 4, 0);
		player.SetAnimation("move", 10);
	}

	// called when a new game is requested (e.g. when F2 pressed)
	// use this function to prepare a menu or a welcome screen
	void CMyGame::OnDisplayMenu()
	{
		//show the start screen and play menu music
		startScreen.SetPosition(400, 300);
		score = 0;
		lives = 2;
		music.Play("menumusic.wav", -1);
		mode = 0;



	}

	// called when a new game is started
	// as a second phase after a menu or a welcome screen
	void CMyGame::OnStartGame()
	{
		//stop the menu music from playing and play the game music
		if (!IsMenuMode())
		{
			music.Stop();
			music.Play("gamemusic.wav", -1);
		}
		// clear sprite lists
		shotList.delete_all();
		enemyList.delete_all();
		explosions.delete_all();
		bombs.delete_all();
		ufolist.delete_all();
		rocketlist.delete_all();
		debrislist.delete_all();
		// set initial player position and size
		background.SetPosition(400, 300);
		player.SetPosition(300, 30); player.SetSize(32, 40);




		increaseX = 0;
		level = 0;
		gamewon = false;
		superlasercooldown = 0;
	}
	
		


	// called when the game is over
	void CMyGame::OnGameOver()
	{
	}

	// one time termination code
	void CMyGame::OnTerminate()
	{
	}

	void CMyGame::
    Wincon()
{// the win condition for the level mode
	if (level > 3 && mode == 1)
	{
		gamewon = true;
		GameOver();
		win.Play("wincon.wav");
	}
}







	void CMyGame::Onlevelchange()
	{
		// allows the game to be infinitely scaling when in endless mode as well as level mode 
		if (enemyList.size() == 0 && rocketlist.size() == 0 && ufolist.size() == 0)
		{
			shotList.delete_all();

			level = level + 1;
			for (int x = 100; x <= 500; x = x + 50)
			{
				for (int y = 400; y <= 550; y = y + 50)
				{
					// dynamically create a new enemy sprite 
					CSprite* pEnemy = new CSprite(float(x), float(y), 0, 0, GetTime());
					//  add fly animation frames from enemy3A.bmp sheet to sprite
					pEnemy->AddImage("Enemy1.png", "fly", 5, 1, 0, 0, 4, 0);
					// play fly animation with 10 fps
				    pEnemy->SetAnimation("fly", 10);
					// set enemy sprxite into motion
					pEnemy->SetMotion(float(-50 - increaseX), 0);
					// add enemy sprite to enemyList list
					enemyList.push_back(pEnemy);

				}

			}
			// increases scaling
			if (level > 1)
			{
				increaseX = increaseX + 15;
				levelchange.Play("nextlevel.wav");
			}
			if (level > 1) // spawn atleast 2 copies on the next round of the second enemy 
			{
				for (int i = 0; i < 2; i++)
				{
					//make it so the enemy only spawns between 300 and 600
					CSprite* rocket = new CSprite(float(rand() % 300) + 300, 550, 0, 0, GetTime());

					rocket->AddImage("enemy2.png", "fly2", 4, 1, 0, 0, 3, 0);
					rocket->SetAnimation("fly2", 12);
					rocket->SetMotion(float(-50  - increaseX), 0);

					rocketlist.push_back(rocket);
				}
			}
			// spawn atleast two copies of the third enemy 
			if (level > 2)
			{
				for (int i = 0; i < 2; i++)
				{
					
					
						// make it so the enemy only spawns between 300 and 600 horizontally 
						CSprite* ufo = new CSprite(float(rand() % 300) + 300, 550, 0, 0, GetTime());
						ufo->AddImage("enemy3.png", "fly3", 3, 1, 0, 0, 2, 0);
						ufo->SetAnimation("fly3", 12);
						ufo->SetMotion(float(50 + increaseX), 0);
						ufo->SetHealth(75);
						ufolist.push_back(ufo);



					
				}
			}
			// reward system for staying over 3 lives
			if (lives >= 3)
			{
				lives++;
			}
			if (lives < 3)
			{
				lives = 3;
			}
			
			
		}
		//spawn the second enemy continously 
		if (enemyList.size() > 5)
		{
			if (rocketlist.size() < 10)
			{ 
				if (level > 1)
				{


					if (rand() % 200 == 0)
					{
						//make it so the enemy only spawns between 300 and 600
						CSprite* rocket = new CSprite(float(rand() % 300) + 300, 550, 0, 0, GetTime());

						rocket->AddImage("enemy2.png", "fly2", 4, 1, 0, 0, 3, 0);
						rocket->SetAnimation("fly2", 12);
						rocket->SetMotion(float(-50 - increaseX), 0);

						rocketlist.push_back(rocket);
					}
				}
			}
		}
		//spawn the third enemy continously 
		if (enemyList.size() > 5)
		{
			if (ufolist.size() < 10)
			{
				if (level > 2)

				{
					if (rand() % 200 == 0)
					{
						// make it so the enemy only spawns between 300 and 600 horizontally 
						CSprite* ufo = new CSprite(float(rand() % 300) + 300, 550, 0, 0, GetTime());
						ufo->AddImage("enemy3.png", "fly3", 3, 1, 0, 0, 2, 0);
						ufo->SetAnimation("fly3", 12);
						ufo->SetMotion(float(50 + increaseX), (0));
						ufo->SetHealth(75);
						ufolist.push_back(ufo);



					}
				}

			}
		}
	}
		

	



/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();

	
	
	if (IsMenuMode())
	{
		if (sym == SDLK_1)
		{
			mode = 1;
			StartGame();
		}
		else if (sym == SDLK_2)
		{
			mode = 2;
			StartGame();
		}
	}
}


void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x, Uint16 y)
{
	if (!IsMenuMode() && !IsGameOver())
		if (normalcooldown == 0)// cooldown between shots
		{
			{
				CSprite* newShot = new CSprite(player.GetX(), player.GetTop(), 0, 0, GetTime());
				// set the motion of the new shot sprite
				newShot->AddImage("shot.png", "shot1", 10, 1, 0, 0, 9, 0);
				newShot->SetAnimation("shot1", 12);
				newShot->SetMotion(0, 400);
				// add the shot sprite to the list
				shotList.push_back(newShot);
				// play shot sound

				
				shotSound.Play("shot.wav");
				
				normalcooldown = 10; // just under half a second
			}
		}
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
	if (!IsMenuMode() && !IsGameOver())
		if (superlasercooldown != 0)
		{
			supershotcharge.Play("shotcharging.wav");// play this when the supershot is charging
		}
	if (!IsMenuMode() && !IsGameOver())
	{
		if (superlasercooldown == 0)
		{

			{
				CSprite* supershot = new CSprite(player.GetX(), player.GetTop(), 0, 0, GetTime());
				// set the motion of the new shot sprite
				supershot->AddImage("supershot.png", "shot3", 10, 1, 0, 0, 9, 0);
				supershot->SetAnimation("shot3", 12);
				supershot->SetMotion(0, 2000);
				supershot->SetHealth(25);
				// add the shot sprite to the list
				shotList.push_back(supershot);
				// play shot sound

				superlasercooldown = 300; // 10 second cooldown
				shotSound.Play("supershot.wav");

			}

		}
	}
	
	
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
