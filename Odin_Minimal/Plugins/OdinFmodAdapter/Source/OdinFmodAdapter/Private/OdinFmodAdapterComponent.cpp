// Fill out your copyright notice in the Description page of Project Settings.


#include "OdinFmodAdapterComponent.h"
#include "OdinFmodAdapter.h"
#include "odin.h"
#include "fmod_errors.h"
#include "OdinFunctionLibrary.h"
#include "OdinMediaSoundGenerator.h"
#include "OdinPlaybackMedia.h"
#include "OdinInitializationSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UOdinFmodAdapterComponent::UOdinFmodAdapterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UOdinFmodAdapterComponent::AssignOdinMediaInternal(UPARAM(ref)UOdinPlaybackMedia*& Media)
{
	if (nullptr == Media)
		return;

	this->SoundGenerator = MakeShared<OdinMediaSoundGenerator, ESPMode::ThreadSafe>();
	this->PlaybackMedia = Media;

	SoundGenerator->SetOdinStream(Media->GetMediaHandle());
}

void UOdinFmodAdapterComponent::EndPlay(EEndPlayReason::Type reason)
{
	auto result4 = group->removeDSP(mOdinDSP);
	mOdinDSP->release();

	Super::EndPlay(reason);
}

FMOD_RESULT UOdinFmodAdapterComponent::OdinDSPReadCallback(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer, unsigned int length, int inchannels, int* outchannels)
{
	void* userdata;

	dsp_state->functions->getuserdata(dsp_state, &userdata);

	*outchannels = 1;

	UOdinFmodAdapterComponent* instance = reinterpret_cast<UOdinFmodAdapterComponent*>(userdata);

	if (instance->GetWorld() && instance->GetWorld()->GetGameInstance()) {
		const UOdinInitializationSubsystem* OdinInitSubsystem = instance->GetWorld()->GetGameInstance()->GetSubsystem<UOdinInitializationSubsystem>();
		if (OdinInitSubsystem)
		{
			*outchannels = OdinInitSubsystem->GetChannelCount();
		}
	}

	return instance->dspreadcallback(dsp_state, outbuffer, length, inchannels, *outchannels);
}

void UOdinFmodAdapterComponent::PlayOdinVoice(UFMODEvent* OdinVoiceEvent, UPARAM(ref)UOdinPlaybackMedia*& Media)
{
	AssignOdinMediaInternal(Media);
	SetEvent(OdinVoiceEvent);

	Activate();
	Play();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UOdinFmodAdapterComponent::PlayOdinVoiceInternal, 0.2f, false);
}

void UOdinFmodAdapterComponent::PlayOdinVoiceInternal()
{

	if (!StudioInstance->isValid())
	{
		UE_LOG(OdinFmodAdapter, Error, TEXT("No Event Instance found to attach to! Please define an event in FMOD Studio and pass it to this function as an event."))
			return;
	}

	FMOD::Studio::System* System = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
	FMOD::System* CoreSystem = nullptr;
	System->getCoreSystem(&CoreSystem);

	FMOD_DSP_READ_CALLBACK mReadCallback = OdinDSPReadCallback;

	FMOD_DSP_DESCRIPTION desc = { 0 };
	desc.read = mReadCallback;
	desc.userdata = this;
	desc.numoutputbuffers = 1;

	FMOD_RESULT res = CoreSystem->createDSP(&desc, &mOdinDSP);

	if (res == FMOD_RESULT::FMOD_OK)
	{
		FMOD_STUDIO_PLAYBACK_STATE playbackState;
		StudioInstance->getPlaybackState(&playbackState);
		if (playbackState != FMOD_STUDIO_PLAYBACK_PLAYING)
		{
			UE_LOG(OdinFmodAdapter, Error, TEXT("FMOD Event Instance is not playing."));
			return;
		}

		auto channelGroupResult = StudioInstance->getChannelGroup(&group);

		if (channelGroupResult != FMOD_OK)
		{
			UE_LOG(OdinFmodAdapter, Error, TEXT("Error getting Channel Group: %hs"), FMOD_ErrorString(channelGroupResult));
		}

		if (group->addDSP(1, mOdinDSP) == FMOD_RESULT::FMOD_OK)
		{
			UE_LOG(OdinFmodAdapter, Log, TEXT("Added Odin DSP to channel group."));
		}
	}
}

FMOD_RESULT UOdinFmodAdapterComponent::dspreadcallback(FMOD_DSP_STATE* dsp_state, float* data, unsigned int datalen, int inchannels, int outchannels)
{
	if (!data)
		return FMOD_ERR_INVALID_PARAM;

	if (!SoundGenerator || !PlaybackMedia)
		return FMOD_OK;

	unsigned int requestedDataArrayLength = datalen * outchannels;

	const uint32 Result = SoundGenerator->OnGenerateAudio(data, (int32)requestedDataArrayLength);

	if (odin_is_error(Result))
	{
		FString ErrorString = UOdinFunctionLibrary::FormatError(Result, true);
		UE_LOG(LogTemp, Error, TEXT("UOdinFmodAdapter: Error during FillSamplesBuffer: %s"), *ErrorString);
		return FMOD_OK;
	}

	return FMOD_OK;
}

