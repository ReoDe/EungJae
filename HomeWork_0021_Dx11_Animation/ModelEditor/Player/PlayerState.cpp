#include "stdafx.h"
#include "Player.h"
#include "PlayerState.h"

stateIdle* PlayerState::_stateIdle = new stateIdle();
stateRun* PlayerState::_stateRun = new stateRun();


Player* PlayerState::player = nullptr;

// �θ� Ŭ����

void PlayerState::handleInput()
{
	player->_playerState = _stateIdle;
	player->_prePlayerState = _stateIdle;

}

void PlayerState::StateUpdate()
{
}

///////////
// Idle
///////////


void stateIdle::handleInput()
{
	if (Keyboard::Get()->Down('A'))
	{
		player->Punching();
		player->ClearEnable();
	}
}

void stateIdle::StateUpdate()
{
	if (player->bRun == true)	player->_playerState = _stateRun;


	if (player->PunchCount == 0)
		player->Moving();
	else
	{
		player->NoPunching();
	}
}

/////////
// Run
/////////


void stateRun::handleInput()
{
	if (Keyboard::Get()->Down('A'))
	{
		player->Punching();
		player->ClearEnable();
	}
}

void stateRun::StateUpdate()
{
	if (player->bRun == false)	player->_playerState = _stateIdle;


	if (player->PunchCount == 0)
		player->Moving();
	else
	{
		player->NoPunching();
	}
}

