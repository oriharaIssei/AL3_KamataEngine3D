#include <memory>
#include <unordered_map>

class ViewProjection;
struct PartsModel;

struct Vector3;

///=======================================================
/// Player
///=======================================================
class Player{
public:
	Player() = default;
	~Player(){};

	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	// Hand は Body の 子 として扱い， Hand は Body に追従する
	std::unique_ptr<PartsModel> body_;
	std::unique_ptr<PartsModel> hand_;

	Vector3 velocity_;
public:
	PartsModel* GetBody(){ return body_.get(); }
	PartsModel* GetHand(){ return hand_.get(); }
	PartsModel* GetBody()const{ return body_.get(); }
	PartsModel* GetHand()const{ return hand_.get(); }

	const Vector3& GetVelocity()const{ return velocity_; }
	void SetVelocity(const Vector3& velo){ velocity_ = velo; }
};