// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "BaseCharacter.h"
#include "CanTakeDamage.h"
#include "KillBox.h"


// Sets default values
AKillBox::AKillBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	RootComponent = CollisionBox;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetupAttachment(RootComponent);

	//attach our OnHitActor to CollisionBox's collision responses
	CollisionBox->OnComponentHit.AddDynamic(this, &AKillBox::OnHitActor);
}

// Called when the game starts or when spawned
void AKillBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKillBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillBox::OnHitActor(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ICanTakeDamage *TheOtherCharacter = Cast<ICanTakeDamage>(OtherActor);

	if (TheOtherCharacter) {
		TheOtherCharacter->Execute_ReceiveDamage(OtherActor,1000);
	}
}

