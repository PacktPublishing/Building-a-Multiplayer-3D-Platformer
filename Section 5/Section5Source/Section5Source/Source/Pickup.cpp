// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "UnrealNetwork.h"
#include "BaseCharacter.h"
#include "Pickup.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set up the collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	//Enable collision for both physics and query engine
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//will trigger overlap events when other objects pass within its radius
	CollisionSphere->bGenerateOverlapEvents = true;
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//replicates
	CollisionSphere->SetIsReplicated(true);
	//Add our OnComponentOverlap function to our OnComponentBeginOverlap delegate
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnComponentOverlap);
	//Set as root component
	RootComponent = CollisionSphere;

	//set up the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Pickup mesh"));
	//remove all collision on the mesh
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//attach collision to sphere
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetIsReplicated(true);

	//Set default spin speed
	SpinsPerSecond = 0.5f;

	//not picked up by default
	bIsPickedUp = false;

	//Set up replication
	bReplicates = true;
	bAlwaysRelevant = true;
}

void APickup::GetLifetimeReplicatedProps(TArray <FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickup, bIsPickedUp);
}

void APickup::OnComponentOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//check to see if the actor is a character
	ABaseCharacter *Character = Cast<ABaseCharacter>(OtherActor);

	if (Character) {
		//if it is a character, Collect pickup
		CollectPickup(Character);
	}
}

void APickup::CollectPickup_Implementation(ABaseCharacter * Character)
{
	//ensure that we are indeed on the server
	if (Role != ROLE_Authority) return;

	bIsPickedUp = true;
}

bool APickup::CollectPickup_Validate(ABaseCharacter * Character)
{
	return true;
}
// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if it hasn't been picked up, spin constantly
	if (!bIsPickedUp) {
		float SpinThisFrame = (DeltaTime * SpinsPerSecond) * 360;

		FVector rotation = RootComponent->GetComponentRotation().Euler();
		rotation += FVector(0, 0, SpinThisFrame);
		RootComponent->SetWorldRotation(FQuat::MakeFromEuler(rotation));
	}
}

