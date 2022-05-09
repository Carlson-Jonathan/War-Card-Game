#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <iostream>
#include <vector>
using namespace std;

class SpriteManager {
public:

    SpriteManager() {}
    SpriteManager(const TextureManager & textures);

    const TextureManager* textures;

};

#endif // SPRITEMANAGER_H

// =================================================================================================

// SpriteManager::SpriteManager(const TextureManager & textures) {
//     this->textures = &textures;
// }