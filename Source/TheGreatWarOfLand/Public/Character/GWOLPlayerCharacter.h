// GWOLPlayerCharacter.h
// Main player character class supporting on-foot movement and vehicle possession

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GWOLPlayerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class THEGREATWAROFLAND_API AGWOLPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AGWOLPlayerCharacter();

protected:
    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* FollowCamera;

    // Enhanced Input System
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* EnterVehicleAction;

    // Movement
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed = 1200.0f;

    // Vehicle System
    UPROPERTY(BlueprintReadWrite, Category = "Vehicle")
    bool bIsInVehicle;

    UPROPERTY(BlueprintReadWrite, Category = "Vehicle")
    class AVehicleBase* CurrentVehicle;

    // Interaction System
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractionRange = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TArray<class AVehicleBase*> NearbyVehicles;

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Vehicle Functions
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void EnterVehicle(AVehicleBase* Vehicle);

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void ExitVehicle();

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    bool GetIsInVehicle() const { return bIsInVehicle; }

    // Input Callbacks
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void TryEnterVehicle();

    // Interaction
    void FindNearbyVehicles();

private:
    float CurrentSpeed;
    bool bIsSprinting;