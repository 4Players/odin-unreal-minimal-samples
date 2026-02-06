![Screenshot from the Minimal Samples Project](https://docs.4players.io/img/odin/unreal/minimal-samples/odin_unreal_minimal-samples_teaser.webp)

# Odin Integration Sample for Unreal Engine

This project offers a simple, yet comprehensive example showcasing the integration of the [ODIN Unreal SDK](https://github.com/4Players/odin-sdk) by [4Players](https://www.4players.io/company/about_us/) for Unreal Engine. For more in-depth information on replication and how to make Proximity Voice Chat work in Unreal, take a look at our [Odin Unreal Tutorial series](https://www.youtube.com/watch?v=MfZsbYhtUlU&list=PLAe4Im8mFTAuFFrFKnnl_MMJi8de7dYHs&index=2).

## Key Topics Showcased

This sample demonstrates the minimal amount of blueprints needed to use ODIN for both Global or Proximity Voice Chat on different platforms:

- Setting up Odin by creating a new room token, constructing a room handle and joining an Odin room
- Setting up Audio Capture for the local client and linking it to the Odin room.
- Connecting remote media streams to local playback with the Odin Synth Component and destroying local playback when a media stream gets disconnected.
- **Proximity Voice Chat:** Using RepNotify to synchronize Odin Peers with Unreal Characters. This makes Proximity Voice Chat possible in Multiplayer games.
- Handling Android Permissions.

## Getting Started

You'll need the latest Unreal Engine 5 release to start the minimal sample project. The Odin Unreal Plugin in general is compatible with Unreal versions starting with 4.26 - Odin 2 supports Unreal Engine 5.3 and onwards.

This repository uses [LFS](https://git-lfs.github.com) (large file storage) to manage pre-compiled binaries. Note that a standard clone of the repository might only retrieve the metadata about these files managed with LFS. In order to retrieve the actual data with LFS, please follow these steps:

1. Clone the repository:  
   ```
   git clone https://github.com/4Players/odin-unreal-minimal-samples.git
   ```

2. Cache the actual LFS data on your local machine and replace the metadata in the binary files with their actual contents: 
   ```
   git lfs fetch
   git lfs checkout
   ```
   ... or if you have a recent LFS version:
   ```
   git lfs pull
   ```

## Starting the Minimal Multiplayer Sample:

- Open the Map `Content > Odin > Maps > Odin_Multiplayer`
- In the Play-In-Editor options, change the `Number of Players` to more than 1.
- Set the `Net Mode` to `Play As Listen Server` or `Play As Client`

This will start the Editor in Multiplayer Mode and spawn the given number of Players. By walking to another Player Character, you will be able to hear yourself speaking from the Character's position, showcasing Proximity Voice Chat in Unreal. You can change the Proximity Voice behavior by adjusting the values in the Asset `Content > Odin > Blueprints > Multiplayer_Odin_Attenuation`.

## Starting the Minimal Android Sample:

To test the project on Android, you'll need to adjust the Game Default Map, before packaging and copying a build to your device. Go to `Project Settings > Maps & Modes` and select the `Odin_AndroidMinimal` map as the Game Default Map. You're now ready to package and launch the project on your mobile device!

You'll find the utilized Blueprints in the `Content > Odin > AndroidMinimal` directory. The Game Mode `GM_OdinAndroid` simply references the Player Controller. The `PC_OdinAndroid` Player Controller Blueprint derives from the `PC_OdinMultiplayer` class and contains the important setup logic. There you'll see the blueprint setup for requesting microphone permissions and basic Odin-Setup without attenuation (i.e. proximity voice chat). For more information on the microphone permissions, please take a look at our [in-depth guide for setting up Android permissions in Unreal](https://docs.4players.io/voice/unreal/guides/android-permissions/).

## Basic Multiplayer Synchronization Principles for Proximity Voice Chat

You can find all the relevant Blueprints regarding Multiplayer Synchronization in the `Content > Odin > Multiplayer` directory. The Game Mode BP `GM_OdinMultiplayer` will simply reference the Player Controller and Player Character. The Player Character BP `PC_OdinMultiplayer` will set up Odin by binding to the relevant events and then joining the Odin room.

Multiplayer specific code is called first in the `On Room Joined` event handler. The "Room Joined" callback will provide us with the local player's peer id in the current room. We'll call `Set Pawn Peer Id` Event on our Player Character, which sets the `Peer Id` value on the server. The server will then replicate the value to all connected clients. Because we changed the `Replication` setting of the `Peer Id` value to `RepNotify`, any change to the `Peer Id` value will call the `OnRep_PeerId` function on all clients. The `OnRep_PeerId` function was automatically created by Unreal. 

![Calling replicate Peer Id in the On Success callback](https://docs.4players.io/img/odin/unreal/minimal-samples/odin_unreal_minimal-samples_OnSuccess.webp)

In the `OnRep_PeerId` implementation we'll handle the spawning of an `OdinSynthComponent` on remotely controlled Player Characters. We don't want to create the component on the locally controlled Player Character, because we don't want to hear any Voice from there. We also only want to create the Odin Synth Component, if a media stream was already registered for the `Peer Id` value. If this component was not yet created, it means that Unreal was faster than Odin regarding replication and we need to wait for the Odin media stream to connect.

![The OnRep_PeerId implementation in the Player Character Blueprint](https://docs.4players.io/img/odin/unreal/minimal-samples/odin_unreal_minimal-samples_OnRepPeerId.webp)

In the `OnRoomPeerJoined` event we finally handle the creation of an Odin Synth Component for the remote peer. If a Player Character object was registered for the `Peer Id` we got from the event, we know that Unreal replication has already happened and we can securely create the Odin Synth Component for Playback. Otherwise we'll wait and rely on the `OnRep_PeerId` implementation on the Player Character the current Decoder belongs to.

### More information

For more in-depth information on replication and how to make Proximity Voice Chat work in Unreal, take a look at our [Odin Unreal Tutorial series, specifically the Spatial Audio video](https://www.youtube.com/watch?v=MfZsbYhtUlU&list=PLAe4Im8mFTAuFFrFKnnl_MMJi8de7dYHs&index=2). 

## Basic Setup without Multiplayer Synchronization

The Blueprint `PC_OdinAndroid` will show how to setup a simple connection to an Odin room, without using Spatializaiton for Proximity Voice Chat. The setup in this case is less complicated, because we don't need to replicate the Odin Peer Id from the dedicated server to the clients' Player Characters. Instead we simply create an `OdinSynthComponent` for each Media that connects to our Odin room on the local Player Controller.

The implementation shown in `PC_OdinAndroid` will work on every platform. If the nodes are called on another platform, the Android Permission steps will simply be skipped.
