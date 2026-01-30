#include "Character/GWOLPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Vehicle/VehicleBase.h"

AGWOLPlayerCharacter::AGWOLPlayerCharacter()
{
    PrimaryActorTick.TickInterval = 0.0f;
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 400.0f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bIsInVehicle = false;
    CurrentVehicle = nullptr;
    CurrentSpeed = WalkSpeed;
    bIsSprinting = false;
}

void AGWOLPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AGWOLPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsInVehicle)
    {
        FindNearbyVehicles();
    }
}

void AGWOLPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent =
        Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGWOLPlayerCharacter::Move);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGWOLPlayerCharacter::Look);

        // Vehicle Interaction
        EnhancedInputComponent->BindAction(EnterVehicleAction, ETriggerEvent::Triggered, this, &AGWOLPlayerCharacter::TryEnterVehicle);
    }
}

void AGWOLPlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Find forward direction
        const FRotation Rotation = Controller->GetControlRotation();
        const FRotation YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotator(0, YawRotation.Yaw, 0).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotator(0, YawRotation.Yaw, 0).GetUnitAxis(EAxis::Y);

        // Add movement
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AGWOLPlayerCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AGWOLPlayerCharacter::EnterVehicle(AVehicleBase* Vehicle)
{
    if (Vehicle && !bIsInVehicle)
    {
        bIsInVehicle = true;
        CurrentVehicle = Vehicle;
        Vehicle->SetOwner(this);
        GetCharacterMovement()->DisableMovement();
    }
}

void AGWOLPlayerCharacter::ExitVehicle()
{
    if (bIsInVehicle && CurrentVehicle)
    {
        bIsInVehicle = false;
        CurrentVehicle->SetOwner(nullptr);
        CurrentVehicle = nullptr;
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }
}

void AGWOLPlayerCharacter::TryEnterVehicle()
{
    if (!bIsInVehicle && NearbyVehicles.Num() > 0)
    {
        EnterVehicle(NearbyVehicles[0]);
    }
}

void AGWOLPlayerCharacter::FindNearbyVehicles()
{
    NearbyVehicles.Empty();

    FVector PlayerLocation = GetActorLocation();
    TArray<FOverlapResult> OverlapResults;

    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(InteractionRange);
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    if (GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        PlayerLocation,
        FQuat::Identity,
        ECC_WorldDynamic,
        CollisionShape,
        QueryParams))
    {
        for (FOverlapResult& Result : OverlapResults)
        {
            AVehicleBase* Vehicle = Cast<AVehicleBase>(Result.GetActor());
            if (Vehicle)
            {
                NearbyVehicles.Add(Vehicle);
            }
        }
    }
}