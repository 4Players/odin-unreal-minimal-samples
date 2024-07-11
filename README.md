# Odin Integration Sample for Unreal Engine

This project offers a simple, yet comprehensive example showcasing the integration of 4Players' ODIN technology with Unreal Engine 5.4.

## Key Topics Showcased

- Setting up Odin by creating a new room token and constructing a room handle.
- Joining an Odin Room.
- Setting up Local Client Audio captur and linkig it with the Odin room.
- Connecting remote media streams to local playback by utilizing Odin Synth Components and destroying local playback when a media stream gets disconnected.
- **Multiplayer Synchronization:** Leveraging RepNotify to synchronize Odin Peers with Unreal Characters, enabling Directional Audio for a more immersive multiplayer experience.

## To Test:

- Open the Map `Content > Odin > Maps > Odin_Multiplayer`
- Change the `Number of Players` to more than 1.
- Set the `Net Mode` to `Play As Listen Server` or `Play As Client`

This will start the Editor in Multiplayer Mode and spawn the given number of Players. By walking to another Player Character, you will be able to hear yourself speaking from the Character's position, showcasing Proximity Voice Chat in Unreal. You can change the Proximity Voice behavior by adjusting the values in the Asset `Content > Odin > Blueprints > Multiplayer_Odin_Attenuation`.
