// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FmodOdinSubsystem.generated.h"

/**
 * This class waits until the OdinInitializationSubsystem is 
 * loaded and then re-initializes it with FMODs actual Sample Rate. 
 * This way we do not need a resampler that creates additional delays in audio processing.
 */
UCLASS()
class UFmodOdinSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
