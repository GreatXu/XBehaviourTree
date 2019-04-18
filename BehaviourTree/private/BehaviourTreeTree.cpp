#include "BehaviourTree/public/BehaviourTreeTree.h"
#include "BehaviourTree/public/BehaviourTreeManager.h"
#include "BehaviourTree/public/BehaviourTreeNativeClassFactory.h"


BehaviourTreeTree::BehaviourTreeTree()
{
	TickRate = 0.5;
	curDelayTime = 0;
	rootNode = NULL;
	blackboard = NULL;
}

BehaviourTreeTree::~BehaviourTreeTree()
{
	if (blackboard != NULL)
	{
		delete blackboard;
		blackboard = NULL;
	}
	if (rootNode != NULL)
	{
		delete rootNode;
		rootNode = NULL;
	}
}

void BehaviourTreeTree::SetRootNode(BehaviourTreeNode* inNode)
{
	rootNode = inNode;
}

BehaviourTreeNode* BehaviourTreeTree::GetRootNode()
{
	return rootNode;
}

void BehaviourTreeTree::ReadJson(Value& inJsonValue)
{
	treeName = inJsonValue["name"].GetString();
	if (inJsonValue.HasMember("tickrate"))
	{
		TickRate = atof(inJsonValue["tickrate"].GetString());
	}
	if (inJsonValue.HasMember("node"))
	{
		Value& newJsonvalue = inJsonValue["node"];
		BehaviourTreeNode* newNode = NativeClassNameMapFactory::BTCreateNewInstanceByName(newJsonvalue["name"].GetString());
		if (newNode != NULL)
		{
			SetRootNode(newNode);
			newNode->ReadJson(newJsonvalue);
			BehaviourTreeBlackboard* newBlackboard = new BehaviourTreeBlackboard();
			if (newBlackboard != NULL)
			{
				blackboard = newBlackboard;
				newNode->SetBlackboard(newBlackboard);
			}
		}
	}
}

void BehaviourTreeTree::SetBlackBoardInfo(Player * inOwnerPlayer, Scene * inScene)
{
	if (blackboard != NULL)
	{
		blackboard->SetOwnerPlayer(inOwnerPlayer);
		blackboard->SetSceneInfo(inScene);
	}
}

void BehaviourTreeTree::Tick(double inDelay)
{
	if (rootNode == NULL)
		return;
	curDelayTime += inDelay;
	if (curDelayTime >= TickRate)
	{
		rootNode->Tick(curDelayTime);
		curDelayTime = 0;
	}
}
