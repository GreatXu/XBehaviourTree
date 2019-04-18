#pragma once

#include <map>
#include <string>
#include "BehaviourTree/public/BehaviourTreeNode.h"
#include "BehaviourTree/public/BehaviourTreeManager.h"

using namespace std;
/**/
namespace NativeClassNameMapFactory
{
	template<typename T>
	T* CreateNewInstance()
	{
		return new T();
	}

	inline BehaviourTreeNode* CreateBTCompositeSequence(){ return CreateNewInstance<BTCompositeSequence>();}
	inline BehaviourTreeNode* CreateBTCompositeSelector(){ return CreateNewInstance<BTCompositeSelector>();}
	inline BehaviourTreeNode* CreateBTCompositeRandom(){ return CreateNewInstance<BTCompositeRandom>();}

	inline BehaviourTreeNode* CreateBTConditionCooldown(){ return CreateNewInstance<BTConditionCooldown>();}
	inline BehaviourTreeNode* CreateBTActionDebugLog(){ return CreateNewInstance<BTActionDebugLog>();}

	inline static map<string , BehaviourTreeNode*(*)(void)> CreatNativeClassMap()
	{
		map<string , BehaviourTreeNode*(*)(void)> tempMap;

		//tempMap.insert(make_pair(string("Parallel"), &CreateBTCompositeParallel));
		tempMap.insert(make_pair(string("Sequence"), &CreateBTCompositeSequence));
		tempMap.insert(make_pair(string("Selector"), &CreateBTCompositeSelector ));
		tempMap.insert(make_pair(string("Random"), &CreateBTCompositeRandom));

		tempMap.insert(make_pair(string("Cooldown"), &CreateBTConditionCooldown));

		tempMap.insert(make_pair(string( "DebugLog"), &CreateBTActionDebugLog));
		return tempMap;
	}

	static map<string, BehaviourTreeNode*(*)(void)> nativeClassNameMap = CreatNativeClassMap();

	inline BehaviourTreeNode* BTCreateNewInstanceByName(string inClassName)
	{
		
		map<std::string , BehaviourTreeNode*(*)(void)>::iterator it;
		it = nativeClassNameMap.find(inClassName);
		if (it != nativeClassNameMap.end())
		{
			return it->second();
		}
		else
		{
			return NULL;
		}
	}
}
