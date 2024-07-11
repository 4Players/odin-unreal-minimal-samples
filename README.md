# Odin Integration Sample for Unreal Engine

This project offers a simple, yet comprehensive example showcasing the integration of 4Players' ODIN technology with Unreal Engine 5.4.

## Key Topics Showcased

- Setting up Odin by creating a new room token, constructing a room handle and joining an Odin room
- Setting up Local Client Audio capture and linking it to the Odin room.
- Connecting remote media streams to local playback with the Odin Synth Component and destroying local playback when a media stream gets disconnected.
- **Proximity Voice Chat:** Using RepNotify to synchronize Odin Peers with Unreal Characters. This makes Proximity Voice Chat possible in Multiplayer games.
- Handling Android Permissions

## To Test Multiplayer:

- Open the Map `Content > Odin > Maps > Odin_Multiplayer`
- In the Play-In-Editor options, change the `Number of Players` to more than 1.
- Set the `Net Mode` to `Play As Listen Server` or `Play As Client`

This will start the Editor in Multiplayer Mode and spawn the given number of Players. By walking to another Player Character, you will be able to hear yourself speaking from the Character's position, showcasing Proximity Voice Chat in Unreal. You can change the Proximity Voice behavior by adjusting the values in the Asset `Content > Odin > Blueprints > Multiplayer_Odin_Attenuation`.

## To Test Android Minimal Sample:

To test the project on Android, you'll need to adjust the Game Default Map, before packaging and copying a build to your device. Go to `Project Settings > Maps & Modes` and select the `Odin_AndroidMinimal` map as the Game Default Map. You're now ready to package and launch the project on your mobile device!

You'll find the utilized Blueprints in the `Content > Odin > AndroidMinimal` directory. The Game Mode `GM_OdinAndroid` simply references the Player Controller. The `PC_OdinAndroid` PlayerController Blueprint contains the important setup logic. There you'll see the blueprint setup for requesting microphone permissions and the default Odin setup afterwards. For more information on the microphone permissions, please take a look at our [in-depth guide for setting up Android permissions in Unreal](https://www.4players.io/odin/guides/unreal/android-permissions/).

## Basic Synchronization Principles

You can find all the relevant Blueprints regarding Multiplayer Synchronization in the `Content > Odin > Multiplayer` directory. The Game Mode BP `GM_OdinMultiplayer` will simply reference the Player Controller and Player Character. The Player Character BP `PC_OdinMultiplayer` will set up Odin by binding to the relevant events and then joining the Odin room.

Multiplayer specific code is called first in the `On Success` callback of the `Join Room` node. The success callback will provide us with the local player's peer id in the current room. We'll call `Replicate Peer Id` Event on our Player Character, which sets the `Peer Id` value on the server. The server will then replicate the value to all connected clients. Because we changed the `Replication` setting of the `Peer Id` value to `RepNotify`, any change to the `Peer Id` value will call the `OnRep_PeerId` function on all clients. The `OnRep_PeerId` function was automatically created by Unreal.

In the `OnRep_PeerId` implementation we'll handle the spawning of an `OdinSynthComponent` on remote controlled Player Characters. We don't want to create the component on the locally controller Player Character, because we don't want to playback any Voice from there. We also only want to create the Odin Synth Component, if a media stream was already registered for the `Peer Id` value. If this component was not yet created, it means that Unreal was faster than Odin regarding replication and we need to wait for the Odin media stream to connect.

The `OnMediaAdded` event on the PlayerController will be called, once this is the case. If a Player Character object was registered for the `Peer Id` we got from the event, we know that Unreal replication has already happened and we can savely Create the Odin Synth Component for Voice Playback. Otherwise we'll wait and rely on the `OnRep_PeerId` implementation on the Player Character the current Media Stream belongs to.

For more in-depth information on replication and how to make Proximity Voice Chat work in Unreal, take a look at our [Odin Unreal Tutorial series, specifically the Spatial Audio video](https://www.youtube.com/watch?v=MfZsbYhtUlU&list=PLAe4Im8mFTAuFFrFKnnl_MMJi8de7dYHs&index=2).