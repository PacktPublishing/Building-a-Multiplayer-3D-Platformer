// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class MASTERINGUNREAL_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	/* COMPONENTS */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		UStaticMeshComponent *Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		USphereComponent *CollisionSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		float SpinsPerSecond;

	//called when pickup overlaps with another actor
	//used to collect pickup
	UFUNCTION()
		void OnComponentOverlap(UPrimitiveComponent *OverlappedComp, AActor* OtherActor, UPrimitiveComponent *OtherComp 
			,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Server function called to actually collect the pickup
	UFUNCTION(Server, WithValidation, Reliable, Category = "Pickup")
		virtual void CollectPickup(ABaseCharacter *Character);

	virtual void CollectPickup_Implementation(ABaseCharacter *Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Pickup")
		bool bIsPickedUp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	
	
	
};
