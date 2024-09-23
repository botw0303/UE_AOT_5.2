#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GiantCollisionSocket.generated.h"

class AGiantCharacter;
class USphereComponent;

UENUM()
enum class EGiantSocketType : uint8
{
	HeadSocket			UMETA(DisplayName = "HeadSocket"),
	Forearm_LSocket		UMETA(DisplayName = "Forearm_LSocket"),
	Forearm_RSocket		UMETA(DisplayName = "Forearm_RSocket"),
	Shin_LSocket		UMETA(DisplayName = "Shin_LSocket"),
	Shin_RSocket		UMETA(DisplayName = "Shin_RSocket"),
	Cnt			
};

enum
{
	EGiantSocketTypeCnt = static_cast<int>(EGiantSocketType::Cnt) + 1,
};

UCLASS()
class UE_AOT_API AGiantCollisionSocket : public AActor
{
	GENERATED_BODY()
	
public:
	AGiantCollisionSocket();
	virtual ~AGiantCollisionSocket() override;

public:
	virtual void Tick(float DeltaSeconds) override;

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
