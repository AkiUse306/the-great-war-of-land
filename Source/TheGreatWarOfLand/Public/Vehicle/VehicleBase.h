#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "VehicleBase.generated.h"

/**
 * Base class for vehicles in The Great War of Land.
 */
UCLASS()
class THEGREATWAROFLAND_API AVehicleBase : public AWheeledVehicle
{
    GENERATED_BODY()

public:
    // Default constructor
    AVehicleBase();

protected:
    // Vehicle properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Properties")
    FColor VehicleColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Properties")
    float Horsepower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Properties")
    float TopSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Properties")
    FString VehicleType; // "Car", "Truck", "Motorcycle", etc.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Properties")
    float CurrentFuel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Properties")
    float MaxFuel;

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Getters
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    float GetCurrentFuel() const { return CurrentFuel; }

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    float GetFuelPercentage() const { return (CurrentFuel / MaxFuel) * 100.0f; }

    // Setters
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void RefuelVehicle(float FuelAmount);

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void CustomizeVehicle(FColor NewColor, float NewHorsepower);
};