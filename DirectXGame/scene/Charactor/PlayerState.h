#pragma once

class Player;
///=======================================================
/// State と Command
///=======================================================
class IPlayerState{
public:
	IPlayerState(Player* host):host_(host){};

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
protected:
	Player* host_;
};

class PlayerNeutralState:
	public IPlayerState{
	PlayerNeutralState(Player* host):IPlayerState(host){};

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Finalize()override;
private:
	float walkSpeed_;
};

class PlayerChargeState:
	public IPlayerState{
	class PlayerChargeState(Player* host):IPlayerState(host){};

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Finalize()override;
private:

};

class PlayerThumpState:
	public IPlayerState{
	PlayerThumpState(Player* host):IPlayerState(host){};

	void Init()    override;
	void Update()  override;
	void Draw()    override;
	void Finalize()override;
private:

};