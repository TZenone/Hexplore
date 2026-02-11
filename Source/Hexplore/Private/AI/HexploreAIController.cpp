// Copyright Tristan ZENONE.


#include "AI/HexploreAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AHexploreAIController::AHexploreAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	
	check(Blackboard)
	check(BehaviorTreeComponent)
}
