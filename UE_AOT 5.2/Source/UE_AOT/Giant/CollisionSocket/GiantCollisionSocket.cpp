#include "Giant/CollisionSocket/GiantCollisionSocket.h"
#include "Components/SphereComponent.h"
#include "Giant/GiantCharacter.h"

AGiantCollisionSocket::AGiantCollisionSocket()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsValid = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereComponent->InitSphereRadius(50.f);
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));

	RootComponent = SphereComponent;

	SphereComponent->SetGenerateOverlapEvents(true);
}

AGiantCollisionSocket::~AGiantCollisionSocket()
{
}

void AGiantCollisionSocket::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = GetActorLocation();
	float Radius = SphereComponent->GetUnscaledSphereRadius();
	FColor DrawColor = FColor::Purple;
	DrawDebugSphere(GetWorld(), CapsuleOrigin, Radius, 16, DrawColor);

#endif
}

void AGiantCollisionSocket::OnHit()
{
	if(!bIsValid)
	{
		return;
	}
	
	bIsValid = false;
	OwnerGiant->OnDamage(SocketType);
}

