// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "BaseCharacter.h"
#include "PlatformerGameState.h"
#include "EndLevelActor.h"


// Sets default values
AEndLevelActor::AEndLevelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Visual Mesh"));
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	RootComponent = VisualMesh;

	OverlapMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Overlap Mesh"));
	OverlapMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	OverlapMesh->OnComponentBeginOverlap.AddDynamic(this, &AEndLevelActor::OnComponentOverlap);
	OverlapMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEndLevelActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEndLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndLevelActor::OnComponentOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Role == ROLE_Authority) {
		ABaseCharacter *OverlappedActor = Cast<ABaseCharacter>(OtherActor);

		if (OverlappedActor) {
			//Get the match state
			APlatformerGameState *GameState = Cast<APlatformerGameState>(GetWorld()->GetGameState());

			APlayerState *PlayerState = OtherActor->GetNetOwningPlayer()->PlayerController->PlayerState;

			if (GameState) {

				GameState->SetWinner(PlayerState);

				//change to game complete
				GameState->ChangeState(EMatchState::EGameComplete);
			}

		}
	}
}

