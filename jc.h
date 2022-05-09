#ifndef JC_H
#define JC_H

#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

namespace jc {
class Sprite : public sf::Sprite {
public:
    bool mayDraw = true;
};

class Text : public sf::Text {
public:
    bool mayDraw = true;
};

} // namespace jc

#endif // JC_H