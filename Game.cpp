#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}



void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	Bricks.clear();

	Box brick1;
	brick1.width = 10;
	brick1.height = 2;
	brick1.x_position = 0;
	brick1.y_position = 5;
	brick1.doubleThick = true;
	brick1.color = ConsoleColor::DarkCyan;
	Bricks.push_back(brick1);

	Box brick2;
	brick2.width = 10;
	brick2.height = 2;
	brick2.x_position = 12;
	brick2.y_position = 5;
	brick2.doubleThick = true;
	brick2.color = ConsoleColor::DarkCyan;
	Bricks.push_back(brick2);

	Box brick3;
	brick3.width = 10;
	brick3.height = 2;
	brick3.x_position = 24;
	brick3.y_position = 5;
	brick3.doubleThick = true;
	brick3.color = ConsoleColor::DarkCyan;
	Bricks.push_back(brick3);

	Box brick4;
	brick4.width = 10;
	brick4.height = 2;
	brick4.x_position = 36;
	brick4.y_position = 5;
	brick4.doubleThick = true;
	brick4.color = ConsoleColor::DarkCyan;
	Bricks.push_back(brick4);

	Box brick5;
	brick5.width = 10;
	brick5.height = 2;
	brick5.x_position = 48;
	brick5.y_position = 5;
	brick5.doubleThick = true;
	brick5.color = ConsoleColor::DarkCyan;
	Bricks.push_back(brick5);

}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering should be done between the locks in this function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw(); 

	for (auto& Brick : Bricks)
	{
		Brick.Draw();
	}

	if (Bricks.empty()) {
		Console::WordWrap(10, 10, 40, "You win! Please press R to restart.");
	}

	if (ball.y_position + ball.y_velocity >= Console::WindowHeight()) {
		Console::WordWrap(10, 10, 40, "You lose! Please press R to restart.");
		return;
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	for (int BrickIter = 0; BrickIter != Bricks.size(); ++BrickIter)
	{
		if (Bricks[BrickIter].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			Bricks[BrickIter].color = ConsoleColor(Bricks[BrickIter].color - 1);
			ball.y_velocity *= -1;

			if (Bricks[BrickIter].color == ConsoleColor::Black) {
				Bricks.erase(Bricks.begin() + BrickIter);
				--BrickIter;
				if (Bricks.empty()) {
					ball.moving = false;
					return;
				}
			}
			
		}  
	}




	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display defeat text with R to reset
	if (ball.y_position + ball.y_velocity >= Console::WindowHeight()) {
		ball.moving = false;
		return;
	}
}
