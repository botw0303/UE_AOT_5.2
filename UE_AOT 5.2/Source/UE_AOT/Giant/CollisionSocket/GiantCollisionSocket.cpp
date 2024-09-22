#include "Giant/CollisionSocket/GiantCollisionSocket.h"
#include "Components/SphereComponent.h"
#include "Giant/GiantCharacter.h"

AGiantCollisionSocket::AGiantCollisionSocket()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereComponent->InitSphereRadius(50.f);
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));

	RootComponent = SphereComponent;

	SphereComponent->SetGenerateOverlapEvents(true);
}

AGiantCollisionSocket::~AGiantCollisionSocket()
{
}

void AGiantCollisionSocket::OnHit()
{
	OwnerGiant->OnDamage(SocketType);
}

