# What is it
 
This is an unfinished system which allows you to have water splashes across the ship/boat hull causing by its movement across the water. The system requires certain components from the Ocean Community Project: https://github.com/UE4-OceanProject (see 'Dependencies')

Video how it looks:

https://gfycat.com/ru/unconsciousbetterimperatorangel

https://gfycat.com/ru/speedysickakitainu

# Dependencies

- UE4 4.25 (tested with source build but should work with binary version as well

- BP_Ocean placed in the level

- BuoyancyForce Component

- Ocean Manager class

# Getting Started

- Compile and run

- The Content/Levels/SplashExample map should be loaded

- Press play and swim around

- If you don't see any splashes for some reason go to the Splash Component attached to the boat and set up particle components (see section 'How it works' below)

# How it works

- The system consists of three classes: SplashComponent, ParticleHandler and DistributionVectorUniformParam.

- You add the SplashComponent to the boat/ship (assuming that you're starting fresh in some another project or with another boat/ship)

- SplashComponent allows you to set up a grid which represents where the traces should come from. For better results check 'use complex collision' for traces and set up this type of collision in the hull mesh settings. Traces are needed to get set of locations close to the hull regardless of the hull mesh. The system curently works only with simmetrical hulls (meaning that there should be the same amount of hit locations on both left and right sides of the hull)

- When pressing 'Create Particle Handlers' button in the SplashComponent detailes muliple ParticleHandlers are being created across the hull

- You can also 'Remove Particle Handlers' and start from the beginning

- Once you satisfied with the result you can press Play

- The reason we are creating this joint ParticleHandler components is because apparently, UParticleComponent is not intended to inherit from (I have tried multiple times but the particles won't emit no matter what). That's why particle components are being created inside the ParticleHandler class as instances (see 'PSC' pointer in the constructor)

- The actual particle system asset is in Content/Assets/Particles/Splashes/P_Splash. The reason you can't see anything useful when opening it up is because most of its values are driven though the code using Unreal 'particle parameters' feature

# Questions

- My name is Alexander Alikin (Saint-Petersburg, Russia), if you have any questions you can find me in the OceanCommunityProject discord channel: https://discord.gg/TgzMRT (nickname '</script>') or on Facebook: https://www.facebook.com/aaalikin


