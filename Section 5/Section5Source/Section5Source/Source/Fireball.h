// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Fireball.generated.h"

UCLASS()
class MASTERINGUNREAL_API AFireball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireball();

	/* COMPONENTS */
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		UStaticMeshComponent *Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		USphereComponent *CollisionSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		UPhysicalMaterial *physMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		UMaterial *theMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		UParticleSystem *FireParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		UParticleSystemComponent *FireParticlesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fireball")
		ACharacter *TheOwner;

	float TotalLifeTime;
	float TimeAlive;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//behaviour for when a fireball collides with an actor
	UFUNCTION()
		void OnHitActor(UPrimitiveComponent * HitComponent, AActor *OtherActor, UPrimitiveComponent* OtherComp
			, FVector NormalImpulse, const FHitResult &Hit);
	
	
};
