// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "CanTakeDamage.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class MASTERINGUNREAL_API AEnemyCharacter : public ACharacter, public ICanTakeDamage
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Character")
		int MaxHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Interface")
		void ReceiveDamage(int amount);

	//override interface implementation
	virtual void ReceiveDamage_Implementation(int amount) override;

	UFUNCTION()
		void OnHitActor(UPrimitiveComponent *HitComponent, AActor * OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult & Hit);
	
private:
	int CurrentHealth;
};
