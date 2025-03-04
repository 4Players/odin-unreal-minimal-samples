// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODAudioComponent.h"
#include "fmod_studio.hpp"
#include "Components/ActorComponent.h"
#include "OdinFmodAdapterComponent.generated.h"

class OdinMediaSoundGenerator;
class UOdinPlaybackMedia;

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class ODINFMODADAPTER_API UOdinFmodAdapterComponent : public UFMODAudioComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOdinFmodAdapterComponent();

	void EndPlay(EEndPlayReason::Type reason) override;

	/** Start a sound playing on an audio component. */
	UFUNCTION(BlueprintCallable, Category = "Audio|Odin|FMOD")
	void PlayOdinVoice(UFMODEvent* OdinVoiceEvent, UPARAM(ref)UOdinPlaybackMedia*& Media);

	FMOD_RESULT dspreadcallback(FMOD_DSP_STATE* dsp_state, float* data, unsigned int datalen, int inchannels, int outchannels);

	static FMOD_RESULT OdinDSPReadCallback(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer, unsigned int length, int inchannels, int* outchannels);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Odin|Sound")
	UOdinPlaybackMedia* PlaybackMedia = nullptr;
	TSharedPtr<OdinMediaSoundGenerator, ESPMode::ThreadSafe> SoundGenerator;

	float* Buffer = nullptr;
	unsigned int BufferSize = 0;


	FMOD::Sound* Sound = nullptr;
	FMOD::DSP* mOdinDSP = nullptr;
	FMOD::ChannelGroup* group;

	void PlayOdinVoiceInternal();
	void AssignOdinMediaInternal(UOdinPlaybackMedia*& Media);
};
