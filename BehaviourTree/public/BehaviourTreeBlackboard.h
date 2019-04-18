#pragma once
#ifndef BehaviourTreeBlackboard_h
#define BehaviourTreeBlackboard_h

#include "BehaviourTree/public/BehaviourTreeDefine.h"
#include <stdint.h>

class Player;
class Scene;

class BehaviourTreeBlackboard
{
public:
	BehaviourTreeBlackboard();

	virtual void SetOwnerPlayer(Player* inOwnerPlayer);
	virtual void SetSceneInfo(Scene* inScene);

private:
	Player* ownerPlayer;
	Scene* scene;
};

#endif