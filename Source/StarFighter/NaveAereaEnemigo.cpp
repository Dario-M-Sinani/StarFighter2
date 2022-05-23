// Fill out your copyright notice in the Description page of Project Settings.



#include "NaveAereaEnemigo.h"
#include "NaveAereaJugador.h"
#include "Kismet/GameplayStatics.h"
#include "Proyectil.h"

ANaveAereaEnemigo::ANaveAereaEnemigo()
{
	GunOffset = FVector(90.f, 0.f, 0.f); //para la distancia de aparicion de la bala
	FireRate = 1.0f;
	bCanFire = true;
}

void ANaveAereaEnemigo::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ANaveAereaEnemigo::FireEnemigo, 2.0f, true, 4.0f);

}

void ANaveAereaEnemigo::Tick(float DeltaSeconds)
{
	srand(time(NULL));

	MovingTX = rand() % 20 - 10;
	MovingTY = rand() % 20 - 10;

	MoveSpeed = 20;


	const FVector MoveDirection = FVector(MovingTX, MovingTY, 0.0f);
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();

		FHitResult Hit(1.0f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
	}

	// basic intel: move the monster towards the player
	ANaveAereaJugador* avatar = Cast<ANaveAereaJugador>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!avatar)
	{
		return;

	}
	FVector toPlayer = avatar->GetActorLocation() - GetActorLocation();
	toPlayer.Normalize();


	FRotator toPlayerRotation = toPlayer.Rotation();
	toPlayerRotation.Pitch = 0; // 0 off the pitch
	RootComponent->SetWorldRotation(toPlayerRotation);

}

void ANaveAereaEnemigo::FireEnemigo()
{
	//UE_LOG(LogTemp, Warning, TEXT("Presionaste una tecla Rayo"));//solo muestra en el registro de salida

	bCanFire = true;
	const FVector FireDirectionEnemigo = FVector(MovingTX, MovingTY, 0.0f).GetClampedToMaxSize(1.0f);

	FireShotEnemigo(FireDirectionEnemigo);
}

void ANaveAereaEnemigo::FireShotEnemigo(FVector FireDirectionEnemigo)
{

	if (bCanFire == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Presionaste una tecla Rayo"));

		if (FireDirectionEnemigo.SizeSquared() > 0.0f)
		{
			ANaveAereaJugador* avatar = Cast<ANaveAereaJugador>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			if (!avatar)
			{
				return;

			}
			FVector toPlayer = avatar->GetActorLocation() - GetActorLocation();

			const FRotator FireRotation = toPlayer.Rotation();
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				World->SpawnActor<AProyectil>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ANaveAereaEnemigo::ShotTimerExpired, FireRate);


		}
	}

}

void ANaveAereaEnemigo::ShotTimerExpired()
{
	bCanFire = true;
}