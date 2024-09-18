// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AOTCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AAOTCharacterPlayer::AAOTCharacterPlayer()
{
	//// Camera
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 400.0f; // 4m
	//CameraBoom->bUsePawnControlRotation = true;

	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//FollowCamera->bUsePawnControlRotation = false;

	// Input
	//static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/Player/Input/IMC_Default.IMC_Default"));
	//if (nullptr != InputMappingContextRef.Object)
	//{
	//	DefaultMappingContext = InputMappingContextRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRef(TEXT("/Game/Player/Input/Actions/IA_Attack.IA_Attack"));


}

void AAOTCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	//APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//{
	//	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//}
}

void AAOTCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AAOTCharacterPlayer::Attack);
}