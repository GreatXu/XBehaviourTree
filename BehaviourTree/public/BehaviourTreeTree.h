#ifndef BehaviourTreeTree_h
#define BehaviourTreeTree_h

#include "BehaviourTree/public/BehaviourTreeNode.h"
#include "BehaviourTree/public/BehaviourTreeBlackboard.h"
#include "ThirdParty/rapidjson/document.h"


using namespace std;
using namespace rapidjson;

class Player;
class Scene;

class BehaviourTreeTree
{
public:

	//bool deleteMe;

	BehaviourTreeTree();
	~BehaviourTreeTree();
	void SetRootNode(BehaviourTreeNode* inNode);
	BehaviourTreeNode* GetRootNode();
	void ReadJson(Value& inJsonValue);
	virtual void SetBlackBoardInfo(Player* inOwnerPlayer, Scene* inScene);
	void Tick(double inDelay);

private:
	double curDelayTime;
	double TickRate;
	BehaviourTreeNode* rootNode;
	BehaviourTreeBlackboard* blackboard;
	string treeName;
};

#endif