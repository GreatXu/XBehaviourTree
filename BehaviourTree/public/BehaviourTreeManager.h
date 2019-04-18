#pragma once
#ifndef BehaviourTreeManager_h
#define BehaviourTreeManager_h

#include "BehaviourTree/public/BehaviourTreeDefine.h"
#include "BehaviourTree/public/BehaviourTreeTree.h"
#include <stdio.h>
#include <list>

using namespace std;

class BehaviourTreeTree;
class Player;
class Scene;

class BehaviourTreeManager
{
public:
	BehaviourTreeManager();
	~BehaviourTreeManager();

	static BehaviourTreeManager* GetInstance();
	DebugCallBack debugCallBack;
	void DebugUnityLog(std::string inStr);

	BehaviourTreeTree* CreateNewTree(const char* inFilePath, const char* inTreeName, Player* inOwnerPlayer, Scene* inScene);
	void RemoveTree(BehaviourTreeTree* inTree);

	void Tick(int64_t inDelay);
	void Reset();

private:
	list<BehaviourTreeTree*> trees;
	list<BehaviourTreeTree*>::iterator treesIterator;

	int64_t nownNanosecond;
	double currentDelay;

	BehaviourTreeTree* ReadJson(const char* inFilePath, const char* inTreeName);
};

#endif