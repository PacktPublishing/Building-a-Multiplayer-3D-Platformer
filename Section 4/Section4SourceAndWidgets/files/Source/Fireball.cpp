// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "BaseCharacter.h"
#include "Fireball.h"


// Sets default values
AFireball::AFireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set up the collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	//Set the collision response
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//Ensure that it notifies things of when it collides
	CollisionSphere->SetNotifyRigidBodyCollision(true);
	//Call our OnHitActor function when it collides with an object
	CollisionSphere->OnComponentHit.AddDynamic(this, &AFireball::OnHitActor);
	//Ensure that it is replicated
	CollisionSphere->SetIsReplicated(true);
	//set the radius
	CollisionSphere->SetSphereRadius(16.f);
	//enable collision
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//enable physics
	CollisionSphere->SetSimulatePhysics(true);
	//Make root component
	RootComponent = CollisionSphere;

	//set up the mesh
	Mesh = CreateDefaultSubobject < UStaticMeshComponent>(FName("Mesh"));
	//set the mesh to the Sphere shape from StarterContent
	Mesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'")).Object);
	//rescale it to fit the collision sphere
	Mesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	//disable physics
	Mesh->SetSimulatePhysics(false);
	//disable collision
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Attach to CollisionSphere
	Mesh->SetupAttachment(CollisionSphere);
	//reposition to fit inside the collision sphere
	Mesh->SetRelativeLocation(FVector(0, 0, -16.f));

	//import the material we want on our mesh
	theMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/M_Fire.M_Fire'")).Object;
	//set the material on our mesh
	Mesh->SetMaterial(0, theMaterial);

	//Set replicated properties
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	//Set up the physical material property
	physMaterial = CreateDefaultSubobject<UPhysicalMaterial>(FName("Physical Material"));

	//Import the particle system we want to use for our fireball
	FireParticles = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/Assets/Particles/P_Fire.P_Fire'")).Object;

	//Set up our particle system component
	FireParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("Particle System"));
	//Set our particle system template to the system we imported
	FireParticlesComponent->SetTemplate(FireParticles);
	//auto activate on creation
	FireParticlesComponent->bAutoActivate = true;
	//Attach to RootComponent
	FireParticlesComponent->SetupAttachment(RootComponent);

	//Set our default total lifetime to 2 seconds
	TotalLifeTime = 2.f;

	//Set our owner to nullptr
	TheOwner = nullptr;

}

// Called when the game starts or when spawned
void AFireball::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFireball::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Set up our physics properties for proper bounce and stuff
	CollisionSphere->SetMassOverrideInKg(NAME_None, 0.1f, true);

	//Will always bounce at same height
	physMaterial->bOverrideRestitutionCombineMode = true;
	physMaterial->Restitution = 1.0f;
	physMaterial->RestitutionCombineMode = EFrictionCombineMode::Max;

	//Will never slow down
	physMaterial->bOverrideFrictionCombineMode = true;
	physMaterial->Friction = 0;
	physMaterial->FrictionCombineMode = EFrictionCombineMode::Max;

	//set the physical material to our collision shere
	CollisionSphere->SetPhysMaterialOverride(physMaterial);
}

// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeAlive += DeltaTime;

	//Destroy fireball if it has been alive for over TotalLifetime
	if (TimeAlive >= TotalLifeTime) {
		this->Destroy();
	}
}

void AFireball::OnHitActor(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ABaseCharacter *Character = Cast<ABaseCharacter>(OtherActor);

	//Ensure that the fireball has an owner and that the owner is not the hit character
	if (TheOwner && Character && TheOwner != Character) {
		Character->ReceiveDamage(1);
		this->Destroy();
	}
}

