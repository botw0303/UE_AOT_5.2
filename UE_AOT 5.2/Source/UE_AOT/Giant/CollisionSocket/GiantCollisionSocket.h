#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GiantCollisionSocket.generated.h"

class AGiantCharacter;
class USphereComponent;

UENUM()
enum class EGiantSocketType : uint8
{
	Head		UMETA(DisplayName = "Head"),
	Forearm_L	UMETA(DisplayName = "Forearm_L"),
	Forearm_R	UMETA(DisplayName = "Forearm_R"),
	Shin_L		UMETA(DisplayName = "Shin_L"),
	Shin_R		UMETA(DisplayName = "Shin_R"),
};

UCLASS()
class UE_AOT_API AGiantCollisionSocket : public AActor
{
	GENERATED_BODY()
	
public:
	AGiantCollisionSocket();
	virtual ~AGiantCollisionSocket() override;

public:
	void OnHit();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SocketCollision")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SocketCollision")
	AGiantCharacter* OwnerGiant;

	UPROPERTY(EditAnywhere, Category = "SocketCollision")
	EGiantSocketType SocketType;
	
};
