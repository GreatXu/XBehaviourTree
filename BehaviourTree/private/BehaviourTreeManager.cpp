#include <string>
#include <iostream>
#include <fstream>
#include "BehaviourTree/public/BehaviourTreeManager.h"
#include "BehaviourTree/public/BehaviourTreeNativeClassFactory.h"
#include "ThirdParty/rapidjson/rapidjson.h"
#include "ThirdParty/rapidjson/document.h"
#include "ThirdParty/rapidjson/filereadstream.h"
#include "Common/public/qc_util.h"
#include "BehaviourTree/public/BehaviourTreeTree.h"

using namespace std;
using namespace rapidjson;
using namespace NativeClassNameMapFactory;

BehaviourTreeManager::BehaviourTreeManager()
{
	currentDelay = 0;
	nownNanosecond = 0;
	debugCallBack = NULL;
}

BehaviourTreeManager::~BehaviourTreeManager()
{
	for (treesIterator = trees.begin(); treesIterator != trees.end(); )
	{
		delete (*treesIterator);
		treesIterator = trees.erase(treesIterator);
	}
}

//BehaviourTreeManager* BehaviourTreeManager::_Instance = NULL;
BehaviourTreeManager* BehaviourTreeManager::GetInstance()
{
	/*
	if (_Instance == nullptr)
	{
	_Instance = new BehaviourTreeManager();
	}
	return BehaviourTreeManager::_Instance;
	*/
	static BehaviourTreeManager _Instance;
	return &_Instance;
}

void BehaviourTreeManager::DebugUnityLog(string inStr)
{
	if (debugCallBack != NULL)
	{
		debugCallBack(inStr.c_str());
	}
}

BehaviourTreeTree* BehaviourTreeManager::CreateNewTree(const char * inFilePath, const char * inTreeName, Player* inOwnerPlayer, Scene* inScene)
{
	BehaviourTreeTree* newTree = ReadJson(inFilePath, inTreeName);
	if (newTree != NULL)
	{
		newTree->SetBlackBoardInfo(inOwnerPlayer, inScene);
	}
	return newTree;
}

void BehaviourTreeManager::RemoveTree(BehaviourTreeTree* inTree)
{
	//inTree->deleteMe = true;

	for (list< BehaviourTreeTree* >::iterator tempIt = trees.begin(); tempIt != trees.end(); ++tempIt)
	{
		if (*tempIt == inTree)
		{
			delete (*tempIt);
			trees.erase(tempIt);
			return;
		}
	}
}

BehaviourTreeTree* BehaviourTreeManager::ReadJson(const char * inFilePath, const char * inTreeName)
{
	ifstream tempifsteam;
	tempifsteam.open(inFilePath);
	string str, tempStr = "";
	while (getline(tempifsteam, str))
	{
		tempStr = tempStr + str + "\n";
	}
	tempifsteam.close();

	if (tempStr == "")
	{
		string logStr = "Can't open file or file is empty : ";
		logStr += inFilePath;
		DebugUnityLog(logStr);
		return NULL;
	}

	Document tempDocument;

	tempDocument.Parse<0>(tempStr.c_str());

	if (tempDocument.HasParseError())
	{
		string logStr = "Parse json error : ";
		logStr += tempDocument.GetParseError();
		DebugUnityLog(logStr);
		return NULL;
	}
	Value treesJsonValue;
	treesJsonValue = tempDocument["trees"];
	if (treesJsonValue.IsArray() && !treesJsonValue.Empty())
	{
		for (quint32 i = 0; i < treesJsonValue.Size(); ++i)
		{
			//find tree
			if (strcmp(treesJsonValue[i]["name"].GetString(), inTreeName) == 0)
			{
				BehaviourTreeTree* newTree = new BehaviourTreeTree();
				newTree->ReadJson(treesJsonValue[i]);
				trees.push_back(newTree);
				return newTree;
			}
		}
	}
	string logStr = "Can't find tree ";
	logStr += inTreeName;
	logStr += " in file ";
	logStr += inFilePath;
	DebugUnityLog(logStr);
	return NULL;
}

void BehaviourTreeManager::Tick(int64_t inNewNanosecond)
{
	if (trees.size() <= 0)
		return;
	if (nownNanosecond == 0)
		nownNanosecond = inNewNanosecond;

	currentDelay += (double)(inNewNanosecond - nownNanosecond) / 1000000000;
	if (currentDelay >= 0.1)
	{
		for (treesIterator = trees.begin(); treesIterator != trees.end(); ++treesIterator)
		{
			(*treesIterator)->Tick(currentDelay);
		}
		currentDelay = 0;
	}
	nownNanosecond = inNewNanosecond;
}

void BehaviourTreeManager::Reset()
{
	for (treesIterator = trees.begin(); treesIterator != trees.end(); )
	{
		delete (*treesIterator);
		treesIterator = trees.erase(treesIterator);
	}
}

EXPORT_API int InitBehaviourTreeCallBack(DebugCallBack inCallBack)
{
	if (inCallBack)
	{
		BehaviourTreeManager::GetInstance()->debugCallBack = inCallBack;
		return 1;
	}
	return 0;
}