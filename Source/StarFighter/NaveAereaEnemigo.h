// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NaveAerea.h"
#include "NaveAereaEnemigo.generated.h"

/**
 *
 */
UCLASS()
class STARFIGHTER_API ANaveAereaEnemigo : public ANaveAerea
{
	GENERATED_BODY()
private:
	uint32 bCanFire : 1;
	FTimerHandle TimerHandle_ShotTimerExpired;

public:

	ANaveAereaEnemigo();

	FTimerHandle MemberTimerHandle;

	virtual void BeginPlay() override;


	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly)
		FVector GunOffset;//es como la distancia que aparecera la proyectil.
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly)
		float FireRate; //propiedad que hara cuanto tardara en desaparecer el proyectil.
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadOnly)
		float Speed;

	void Tick(float DeltaSeconds);

	float MovingTX;
	float MovingTY;

	void FireEnemigo();
	void FireShotEnemigo(FVector FireDirectionEnemigo);
	void ShotTimerExpired();


};