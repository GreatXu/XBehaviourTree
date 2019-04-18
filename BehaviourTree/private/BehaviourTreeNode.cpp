#include "BehaviourTree/public/BehaviourTreeNode.h"
#include "BehaviourTree/public/BehaviourTreeNativeClassFactory.h"
#include "BehaviourTree/public/BehaviourTreeBlackboard.h"
#include "Common/public/qc_util.h"

BehaviourTreeNode::BehaviourTreeNode()
{
	nodeState = EBTNS_None;
	blackboard = NULL;
}

BehaviourTreeNode::~BehaviourTreeNode()
{
}

EBTNodeState BehaviourTreeNode::GetNodeState()
{
	EBTNodeState tempNodeState = nodeState;
	if (reverseResult)
	{
		tempNodeState = (tempNodeState == EBTNS_Successed) ? EBTNS_Failed : EBTNS_Successed;
	}
	return tempNodeState;
}

void BehaviourTreeNode::SetNodeState(EBTNodeState inNodeState)
{
	nodeState = inNodeState;
}

void BehaviourTreeNode::ReadJson(Value& inJsonValue)
{
	if (inJsonValue.HasMember("reverse"))
	{
		std::string tempStr = inJsonValue["reverse"].GetString();
		if (strcmp(tempStr.c_str(), "True") == 0 || strcmp(tempStr.c_str(), "true") == 0)
		{
			reverseResult = true;
		}
		else
		{
			reverseResult = false;
		}
	}
}

void BehaviourTreeNode::SetBlackboard(BehaviourTreeBlackboard* inBlackboard)
{
	blackboard = inBlackboard;
}

void BehaviourTreeNode::SetParentNode(BehaviourTreeNode* inParentNode)
{
	parentNode = inParentNode;
}

void BehaviourTreeNode::AddChild(BehaviourTreeNode* inChildNode)
{
}

void BehaviourTreeNode::Tick(double inDelay)
{
}

void BehaviourTreeNode::OnEnter()
{
}

void BehaviourTreeNode::OnSuccessed()
{
}

void BehaviourTreeNode::OnFailed()
{
}

/*******  Composite Begin  ******/

BehaviourTreeCompositeBase::~BehaviourTreeCompositeBase()
{
	if (childrenNodes.size() > 0)
	{
		for (vector<BehaviourTreeNode*>::iterator it = childrenNodes.begin(); it != childrenNodes.end(); )
		{
			delete *it;
			it = childrenNodes.erase(it);
		}
	}
}

void BehaviourTreeCompositeBase::ReadJson(Value& inJsonValue)
{
	BehaviourTreeNode::ReadJson(inJsonValue);

	Value& childrenJsonValue = inJsonValue["children"];
	if (childrenJsonValue.IsArray() && !childrenJsonValue.Empty())
	{
		for (quint32 i = 0; i < childrenJsonValue.Size(); ++i)
		{
			Value& newJsonvalue = childrenJsonValue[i];
			BehaviourTreeNode* newNode = NativeClassNameMapFactory::BTCreateNewInstanceByName(newJsonvalue["name"].GetString());
			if (newNode != NULL)
			{
				newNode->ReadJson(newJsonvalue);
				newNode->SetParentNode(this);
				AddChild(newNode);
			}
		}
	}
}

void BehaviourTreeCompositeBase::AddChild(BehaviourTreeNode* inChildNode)
{
	childrenNodes.push_back(inChildNode);
}

void BehaviourTreeCompositeBase::SetBlackboard(BehaviourTreeBlackboard* inBlackboard)
{
	BehaviourTreeNode::SetBlackboard(inBlackboard);
	for (quint i = 0; i < childrenNodes.size(); ++i)
	{
		childrenNodes[i]->SetBlackboard(inBlackboard);
	}
}

void BTCompositeSequence::Tick(double inDelay)
{
	OnEnter();

	if (childrenNodes.size() > 0)
	{
		for (quint32 index = 0; index < childrenNodes.size(); )
		{
			childrenNodes[index]->Tick(inDelay);
			if (childrenNodes[index]->GetNodeState() == EBTNS_Successed)
			{
				++index;
				if (index >= childrenNodes.size())
				{
					SetNodeState(EBTNS_Successed);
					OnSuccessed();
					return;
				}
			}
			else if (childrenNodes[index]->GetNodeState() == EBTNS_Failed)
			{
				SetNodeState(EBTNS_Failed);
				OnFailed();
				return;
			}
		}
	}
	else
	{
		SetNodeState(EBTNS_Failed);
		OnFailed();
	}
}

void BTCompositeSelector::Tick(double inDelay)
{
	OnEnter();

	if (childrenNodes.size() > 0)
	{
		for (quint32 index = 0; index < childrenNodes.size(); )
		{
			childrenNodes[index]->Tick(inDelay);
			if (childrenNodes[index]->GetNodeState() == EBTNS_Successed)
			{
				SetNodeState(EBTNS_Successed);
				OnSuccessed();
				return;
			}
			else if (childrenNodes[index]->GetNodeState() == EBTNS_Failed)
			{
				++index;
				if (index >= childrenNodes.size())
				{
					SetNodeState(EBTNS_Failed);
					OnFailed();
					return;
				}
			}
		}
	}
	else
	{
		SetNodeState(EBTNS_Failed);
		OnFailed();
	}
}

void BTCompositeRandom::Tick(double inDelay)
{
	OnEnter();

	if (childrenNodes.size() > 0)
	{
		quint32 index = rand() % childrenNodes.size();
		for (;;)
		{
			childrenNodes[index]->Tick(inDelay);
			if (childrenNodes[index]->GetNodeState() == EBTNS_Successed)
			{
				SetNodeState(EBTNS_Successed);
				OnSuccessed();
				return;
			}
			else if (childrenNodes[index]->GetNodeState() == EBTNS_Failed)
			{
				SetNodeState(EBTNS_Failed);
				OnFailed();
				return;
			}
		}
	}
	else
	{
		SetNodeState(EBTNS_Failed);
		OnFailed();
	}
}

/*******  Composite End  ******/

/*******  Condition Begin  ******/

void BTConditionCooldown::ReadJson(Value & inJsonValue)
{
	BehaviourTreeNode::ReadJson(inJsonValue);
	Value& propertyJsonValue = inJsonValue["property"];

	if (propertyJsonValue.HasMember("cooldown"))
	{
		coolDownTime = atof(propertyJsonValue["cooldown"].GetString());
	}
}

void BTConditionCooldown::Tick(double inDelay)
{
	OnEnter();
	curDelayTime += inDelay;
	if (curDelayTime >= coolDownTime)
	{
		curDelayTime = 0;
		SetNodeState(EBTNS_Successed);
		OnSuccessed();
	}
	else
	{
		SetNodeState(EBTNS_Failed);
		OnFailed();
	}
}

/*******  Condition End  ******/

/*******  Action Begin  ******/

void BTActionDebugLog::ReadJson(Value& inJsonValue)
{
	BehaviourTreeNode::ReadJson(inJsonValue);

	Value& propertyJsonValue = inJsonValue["property"];

	//logStr = "BTActionDebugLog : ";
	if (propertyJsonValue.HasMember("logStr"))
	{
		logStr += propertyJsonValue["logStr"].GetString();
	}
}

void BTActionDebugLog::Tick(double inDelay)
{
	OnEnter();

	BehaviourTreeManager::GetInstance()->DebugUnityLog(logStr);

	SetNodeState(EBTNS_Successed);
	OnSuccessed();
}

/*******  Action End  ******/