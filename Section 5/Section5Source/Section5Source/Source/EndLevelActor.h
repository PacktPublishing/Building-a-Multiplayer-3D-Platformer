// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EndLevelActor.generated.h"

UCLASS()
class MASTERINGUNREAL_API AEndLevelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndLevelActor();

	UPROPERTY(EditDefaultsOnly, Category = "End Level Actor")
		UStaticMeshComponent *VisualMesh;

	UPROPERTY(EditDefaultsOnly, Category = "End Level Actor")
		UStaticMeshComponent *OverlapMesh;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnComponentOverlap(UPrimitiveComponent *OverlappedComp, AActor* OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
