// Fill out your copyright notice in the Description page of Project Settings.


#include "FmodOdinSubsystem.h"
#include "OdinFmodAdapter.h"
#include "OdinInitializationSubsystem.h"
#include "fmod_studio.hpp"
#include "FMODStudioModule.h"
#include "odin_sdk.h"

void UFmodOdinSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Wait for OdinInitializationSubsystem to initialize first
	UOdinInitializationSubsystem* odinSystem = Collection.InitializeDependency<UOdinInitializationSubsystem>();

	if (odinSystem->IsOdinInitialized())
	{
		// Get FMOD Studio Core System
		FMOD::Studio::System* System = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
		FMOD::System* CoreSystem = nullptr;
		System->getCoreSystem(&CoreSystem);

		if (!CoreSystem)
		{
			UE_LOG(OdinFmodAdapter, Error, TEXT("Cannot find FMOD Core System. Make sure it is properly loaded."));
			return;
		}

		// Get the currently used settings from Odin

		int sampleRate = odinSystem->GetSampleRate();
		int channelCount = odinSystem->GetChannelCount();



		/*FMOD_SPEAKERMODE speakermode;
		int numrawspeakers;*/
		odin_shutdown();
		FMOD_RESULT result = CoreSystem->getSoftwareFormat(&sampleRate, nullptr, nullptr);

		if (result != FMOD_OK)
		{
			UE_LOG(OdinFmodAdapter, Error, TEXT("Cannot retrieve sample rate of FMOD. Make sure it is properly initialized."));
			return;
		}

		OdinAudioStreamConfig config;
		config.channel_count = channelCount;
		config.sample_rate = sampleRate;

		odin_startup_ex(ODIN_VERSION, config);

		UE_LOG(OdinFmodAdapter, Log, TEXT("Reinitialized Odin with proper Sample Rate: %d"), sampleRate);
	}
}
