// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "KillBox.generated.h"

UCLASS()
class MASTERINGUNREAL_API AKillBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKillBox();

	UPROPERTY(EditDefaultsOnly, Category = "Kill Box")
		UStaticMeshComponent *Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Kill Box")
		UBoxComponent *CollisionBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnHitActor(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult & Hit);
	
	
};
