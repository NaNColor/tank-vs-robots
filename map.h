#ifndef __map__
#define __map__

#include "Constants.h"
#include <SFML/Graphics.hpp>
/*
0 - границы
b - препятствие
p - растение)
' ' - земля/пол
*/
sf::String TileMapMy[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0  p                                   0",
	"0   bp      p    bb             p      0",
	"0   bbbb         b                     0",
	"0   bb    p                pp          0",
	"0                    p             b   0",
	"0         s                            0",
	"0                                      0",
	"0    p      p             bbb          0",
	"0                                  s   0",
	"0         b                            0",
	"0         b         p          p       0",
	"0      p    bbb                        0",
	"0                         b            0",
	"0                                      0",
	"0    b          p                 h    0",
	"0   bbb                   p            0",
	"0    b    p                            0",
	"0                   bbb           p    0",
	"0                   bbb     s          0",
	"0  p           p    bbb                0",
	"0         s          p       p         0",
	"0                                 b    0",
	"0     p                                0",
	"0000000000000000000000000000000000000000",
};

#endif