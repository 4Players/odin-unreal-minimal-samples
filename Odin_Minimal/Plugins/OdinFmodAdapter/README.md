# Unreal Plugin: Odin FMOD Adapter

![FMOD and ODIN](/img/fmod_unreal_odin_header.jpg)

This is an additional Unreal Engine plugin, providing you with a Scene Component integrating the [ODIN Voice Chat Plugin](https://odin.4players.io/voice-chat/) with
the [FMOD Audio Solution](https://www.fmod.com/docs/2.02/unreal/welcome.html) in Unreal.

## Guide

Following is a guide on how to integrate the Plugin in your Unreal Engine project. Alternatively you can look it up in our [Developer Documentation](https://docs.4players.io/voice/unreal/guides/odin-fmod-adapter-unreal#usage).

## Getting Started

To use the Component we recommend the following:

- Basic knowledge of Unreal as well as FMOD
- The FMOD Plugin for Unreal, which you can get [here](https://www.fmod.com/download#fmodforunreal)
- The ODIN Voice Chat Plugin, available [here](https://github.com/4Players/odin-sdk-unreal/releases)

To set up FMOD in your project, please follow FMOD's in-depth integration-tutorial. You can find the
tutorial [here](https://www.fmod.com/docs/2.03/unreal/welcome.html).

To set up the ODIN Voice Chat Plugin, please take a look at our Getting-Started guide, which you can find [here](https://docs.4players.io/voice/unreal).

This readme will show you the changes to make, when integrating Odin with FMOD based on the outcome of the Getting Started Guide - as most steps are identical to the usual integration of Odin in your Unreal Project and we only need to exchange a few nodes and scene components in the end.

A sophisticated example can be found in a branch of our [Demo Project Repository](https://github.com/4Players/odin-unreal-demo/tree/fmod-adapter-sample).

## Plugin Structure

The plugin's structure is fairly straight forward - it consists of two classes basically:
- The `UOdinFmodAdapterComponent` working as a substitute for the [UOdinSynthComponent](/voice/unreal/blueprint-reference/odin-synth-component/). This component passes the Odin Media Stream to the FMOD Audio Engine instead of the Unreal Audio Engine.
- The `UFmodOdinSubsystem` starts Odin with the correct sample rate, defined in FMOD. This class does its work without the need to adjust anything in the Unreal Project.

## Usage

### Installation

You can install the plugin similarly to the Odin plugin:
1. Download the Code of this repository.
2. Unpack and copy to your Unreal Project's `Plugins` folder. If not present, create the folder manually.
3. Follow the steps in below to implement the Adapter Component in your game!

### FMOD Studio

The `UOdinFmodAdapterComponent` takes an FMOD Event to playback the Odin Media Stream in your Unreal Project. This is mainly used to define additional properties and effects in FMOD Studio that should be applied to the voice chat audio. For instance, this enables the FMOD audio engineer to apply 3D Attenuation to the voice chat.

These are the steps to take in FMOD Studio:

1. Define an FMOD Audio Event

![Create an FMOD Audio Event](/img/fmod-studio-create-event.png)

2. Set the Event to **Persistent** playback

![Create an FMOD Audio Event](/img/fmod-studio-event-set-persistent.png)

3. Apply all needed effects specific to your project

![Apply all needed effects](/img/fmod-studio-event-effects.png)

4. Add the event to a sound bank

![Add event to sound bank](/img/ofmod-studio-add-to-bank.png)

That is all that is needed to setup the event in FMOD Studio, the other steps are taken in the Unreal Project. Build the FMOD Project as usual - exporting to your Unreal Project and switch to the Unreal Editor.

### Integration the Adapter in your Unreal Project

In the next step we will now use the Adapter Component to play back the incoming Odin Media Stream.

First replace the creation of an `OdinSynthComponent` that you have placed in
the [Odin Unreal Guide](/voice/unreal/manual)
in your project with the plugin's `UOdinFmodAdapterComponent`.

In the `OnMediaAdded` event of your Odin implementation in your project you can then call the `Play Odin Voice` afterwards. This function takes the FMOD Event defined earlier and the Odin Media Stream as Inputs. If both are valid, it will assign the Media Stream and then play the FMOD Event. This should look something like this:

![Replacing Blueprint Nodes](/img/unreal-blueprint-replace-nodes.png)

Like with the `OdinSynthComponent`, you can also choose to place the `UOdinFmodAdapter` directly on the Player Character
as a component and then reference it in your `OnMediaAdded` event handler. This way you do not have to create it in the
Blueprint and it is easier to change its properties - e.g. its FMOD-specific (attenuation) settings.

## Conclusion

This simple implementation of an Odin to FMOD adapter for Unreal is a good starting point to give you the control over
the audio playback that you need for your Odin Integration in your project. It is flexible enough to enable your Audio Engineers and Unreal Programmers to make changes to the behavior and acoustics of your Voice Chat.

This is only a starting point of your Odin Integration with FMOD and the Unreal Engine. Feel free to check out any other
learning resources and adapt the material like needed, e.g. create realistic or out of this world dynamic immersive
experiences with [FMOD Spatial Audio](https://www.fmod.com/docs/2.02/studio/advanced-topics.html#spatialization-options)
aka "_proximity chat_" or "_positional audio_":

- [FMOD Spatial Audio](https://www.fmod.com/docs/2.02/studio/advanced-topics.html#spatialization-options)
- [Video Tutorial Series on Odin and Unreal Engine](https://www.youtube.com/playlist?list=PLAe4Im8mFTAuFFrFKnnl_MMJi8de7dYHs)
- [Odin General Introduction](/voice/introduction/)
- [Odin Unreal SDK Blueprint Reference](/voice/unreal/blueprint-reference/)
