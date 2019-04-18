#ifndef BehaviourTreeNode_h
#define BehaviourTreeNode_h

#include <vector>
#include <string>
#include "ThirdParty/rapidjson/document.h"
#include "Common/public/qc_types.h"

//using namespace std;
using namespace rapidjson;

enum EBTNodeState
{
	EBTNS_Successed,
	EBTNS_Failed,
	EBTNS_None
};

class BehaviourTreeBlackboard;

class BehaviourTreeNode
{
public:
	BehaviourTreeNode();
	virtual ~BehaviourTreeNode();
	virtual EBTNodeState GetNodeState();
	void SetNodeState(EBTNodeState inNodeState);
	virtual void ReadJson(Value& inJsonValue);
	virtual void SetBlackboard(BehaviourTreeBlackboard* inBlackboard);
	void SetParentNode(BehaviourTreeNode* inParentNode);
	virtual void AddChild(BehaviourTreeNode* inChildNode);

	virtual void Tick(double inDelay);
	virtual void OnEnter();
	virtual void OnSuccessed();
	virtual void OnFailed();

protected:
	bool reverseResult;
	EBTNodeState nodeState;
	BehaviourTreeNode* parentNode;
	BehaviourTreeBlackboard* blackboard;
};

/*******  Composite Begin  ******/

class BehaviourTreeCompositeBase :public BehaviourTreeNode
{
public:
	virtual ~BehaviourTreeCompositeBase();

	virtual void ReadJson(Value& inJsonValue);
	virtual void AddChild(BehaviourTreeNode* inChildNode);
	virtual void SetBlackboard(BehaviourTreeBlackboard* inBlackboard);

protected:
	std::vector<BehaviourTreeNode*> childrenNodes;
};

class BTCompositeSequence : public BehaviourTreeCompositeBase
{
public:
	virtual void Tick(double inDelay);
};

class BTCompositeSelector : public BehaviourTreeCompositeBase
{
public:
	virtual void Tick(double inDelay);
};

class BTCompositeRandom : public BehaviourTreeCompositeBase
{
public:
	virtual void Tick(double inDelay);
};

/***
****	We don't need Parallel node ,becasue all actions are instant , Sequence and Selector can take place.
***

class BTCompositeParallel : public BehaviourTreeCompositeBase
{
public:
	BTCompositeParallel();
	~BTCompositeParallel();

	virtual void ReadJson( Value& inJsonValue );
	void SetParallelType( bool inOneSuccessed );
	void SetOtherChildrenSuccess( qint32 inIndex );
	virtual void OnEnter();
	virtual void Tick( double inDelay );

private:
	bool successedWhenOneSuccessed;
};
*/

/*******  Composite End  ******/

/*******  Condition Begin  ******/

class BehaviourTreeConditionBase : public BehaviourTreeNode
{
};

class BTConditionCooldown : public BehaviourTreeConditionBase
{
public:
	virtual void ReadJson(Value& inJsonValue);
	virtual void Tick(double inDelay);
private:
	double coolDownTime;
	double curDelayTime;
};

/*******  Condition End  ******/

/*******  Action Begin  ******/

class BehaviourTreeActionBase : public BehaviourTreeNode
{
};

class BTActionDebugLog : public BehaviourTreeActionBase
{
public:
	virtual void ReadJson(Value& inJsonValue);
	virtual void Tick(double inDelay);

private:
	std::string logStr;
};
/*******  Action End  ******/
#endif