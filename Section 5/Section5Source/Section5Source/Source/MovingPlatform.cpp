// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "MovingPlatform.h"


// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CollisionBox
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = CollisionBox;

	//Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Platform Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);

	//default property values
	EndPosition = FVector(0, 0, 0);
	TimeOneDirection = 1.f;
	TimeTravellingInDirection = 0.f;
	bForward = true;
	DistanceToTravel = FVector(0, 0, 0);

	//replication properties
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	FVector StartPosition = GetActorLocation();

	DistanceToTravel = EndPosition - StartPosition;
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (Role == ROLE_Authority) {
		//ensure the platform should be moving
		if (DistanceToTravel != FVector(0, 0, 0)) {
			FVector DistanceThisFrame = DistanceToTravel * (DeltaTime / TimeOneDirection);

			//get the current Location
			FVector CurrentLocation = GetActorLocation();

			//if we're moving forward, add the distance
			if (bForward) {
				CurrentLocation += DistanceThisFrame;
			}
			else { //else subtract it

				CurrentLocation -= DistanceThisFrame;
			}

			//Add the time this frame to total time travelling this direction
			TimeTravellingInDirection += DeltaTime;

			//if we've gon past our total travel time, change direction and reset timer
			if (TimeTravellingInDirection >= TimeOneDirection) {
				bForward = !bForward;

				TimeTravellingInDirection = 0.0f;
			}

			//set the new location
			SetActorLocation(CurrentLocation);
		}
	}
}

