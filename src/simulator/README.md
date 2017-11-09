## Dependencies

### Ogre3D

This project uses Ogre3D v1.10. Grab it from the github:
https://github.com/OGRECave/ogre

You'll need to tell it to not build its own dependencies, or it'll try to use
an old version of Freetype.

```cmake .. -DOGRE_BUILD_DEPENDENCIES=OFF```
