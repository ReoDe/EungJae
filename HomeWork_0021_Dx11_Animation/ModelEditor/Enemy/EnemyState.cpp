#include "stdafx.h"
#include "Enemy.h"
#include "EnemyState.h"

EstateIdle* EnemyState::_stateIdle = new EstateIdle();
EstateRun* EnemyState::_stateRun = new EstateRun();
EstateHitted* EnemyState::_stateHitted = new EstateHitted();

Enemy* EnemyState::enemy = nullptr;

// 부모 클래스

void EnemyState::handleInput()
{
	enemy->_enemyState = _stateIdle;
	enemy->_preEnemyState = _stateIdle;
}

void EnemyState::StateUpdate()
{
	
}

///////////
// Idle
///////////

void EstateIdle::handleInput()
{
}

void EstateIdle::StateUpdate()
{
	if (enemy->bRun == true) enemy->_enemyState = _stateRun;

	if (enemy->bHitted == true) enemy->_enemyState = _stateHitted;

	if (enemy->bKicking == false)
	{
		enemy->Moving();
		enemy->Kicking();
	}
	else
	{
		enemy->NoKicing();
	}

	if (enemy->bHitted == true)
	{

	}
	
}


/////////
// Run
/////////


void EstateRun::handleInput()
{
}

void EstateRun::StateUpdate()
{
	if (enemy->bRun == false) enemy->_enemyState = _stateIdle;

	if (enemy->bHitted == true) enemy->_enemyState = _stateHitted;

	if (enemy->bKicking == false)
	{
		enemy->Moving();
		enemy->Kicking();
	}
	else
	{
		enemy->NoKicing();
	}
}

//////////
// hitted
//////////

void EstateHitted::handleInput()
{
}

void EstateHitted::StateUpdate()
{
	if (enemy->bHitted == false) enemy->_enemyState = _stateIdle;

	enemy->ToIdel();
}
