#include "BehaviourTree/public/BehaviourTreeBlackboard.h"
#include "BehaviourTree/public/BehaviourTreeManager.h"

BehaviourTreeBlackboard::BehaviourTreeBlackboard()
{
}

void BehaviourTreeBlackboard::SetOwnerPlayer(Player * inOwnerPlayer)
{
	ownerPlayer = inOwnerPlayer;
}

void BehaviourTreeBlackboard::SetSceneInfo(Scene * inScene)
{
	scene = inScene;
}
