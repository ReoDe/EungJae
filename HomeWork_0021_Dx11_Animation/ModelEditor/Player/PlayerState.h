#pragma once

class PlayerState
{

public:
	PlayerState() {};
	virtual ~PlayerState() {};

	virtual void handleInput();
	virtual void StateUpdate();

	void setplayerAddressLink(Player* pl)
	{
		player = pl;
	}

protected:
	static stateIdle* _stateIdle;
	static stateRun *_stateRun;


	static Player* player;


};

class stateIdle : public PlayerState
{
public:
	stateIdle() {};
	virtual ~stateIdle() {};
private:
	virtual void handleInput();
	virtual void StateUpdate();
};

class stateRun : public PlayerState
{
public:
	stateRun() {}
	virtual ~stateRun() {};
private:
	virtual void handleInput();
	virtual void StateUpdate();
};

