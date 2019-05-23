// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class MASTERINGUNREAL_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

	UPROPERTY(EditDefaultsOnly, Category = "Moving Platform")
		UStaticMeshComponent *Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Moving Platform")
		UBoxComponent *CollisionBox;

	//Position to which the platform will travel
	UPROPERTY(EditAnywhere, Category = "Moving Platform")
		FVector EndPosition;

	//Time it takes to travel on direction
	UPROPERTY(EditAnywhere, Category = "Moving Platform")
		float TimeOneDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//total time it has been travelling in the current direction
	float TimeTravellingInDirection;

	//Whether or not the platform is moving to our EndPosition
	bool bForward;

	//Distance to travel
	FVector DistanceToTravel;
	
};
