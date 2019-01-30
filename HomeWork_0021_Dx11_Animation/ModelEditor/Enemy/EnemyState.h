#pragma once

class EnemyState
{
public:
	EnemyState() {};
	virtual ~EnemyState() {};

	virtual void handleInput();
	virtual void StateUpdate();

	void SetEnemyAddressLink(Enemy * en)
	{
		enemy = en;
	}

protected:
	static EstateIdle* _stateIdle;
	static EstateRun* _stateRun;
	static EstateHitted* _stateHitted;

	static Enemy* enemy;
};


class EstateIdle : public EnemyState
{
public:
	EstateIdle() {};
	virtual ~EstateIdle() {};

private:
	virtual void handleInput();
	virtual void StateUpdate();
};

class EstateRun : public EnemyState
{
public:
	EstateRun() {};
	virtual~EstateRun() {};
private:
	virtual void handleInput();
	virtual void StateUpdate();
};

class EstateHitted : public EnemyState
{
public:
	EstateHitted() {};
	virtual~EstateHitted() {};
private:
	virtual void handleInput();
	virtual void StateUpdate();
};

