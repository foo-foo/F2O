//
//  Character.hpp
//  F2O
//
//  Created by Anatole Menichetti on 01/11/2015.
//  Copyright © 2015 Anatole Menichetti. All rights reserved.
//

#ifndef Character_hpp
# define Character_hpp

# include <vector>
# include <stdio.h>
# include <SFML/Graphics.hpp>
# include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

# define GLOBAL_SPEED       2

/*
**  Player
*/
# define CHARACTER_SIZE     50
# define CHARACTER_COLOR    Color::Black

/*
**  Moves
*/
# define CHARACTER_SPEED    1.5 * GLOBAL_SPEED
# define FLATTENING_SPEED1  .01 * GLOBAL_SPEED
# define FLATTENING_LIMIT   .6
# define FLATTENING_SPEED2  .005 * GLOBAL_SPEED
# define INFLATION_SPEED1   .1 * GLOBAL_SPEED
# define INFLATION_LIMIT    0
# define INFLATION_SPEED2   .05 * GLOBAL_SPEED
# define JUMP_MIN_HEIGHT    .5
# define JUMP_SPEED         3 * GLOBAL_SPEED
# define GRAVITY            3 * GLOBAL_SPEED
# define NONE               0
# define LEFT               1
# define RIGHT              2

class Character
{
    typedef void (Character::*moveFptr)(void);
    
private:
    CircleShape             _sprite;
    bool                    _isFlattening;
    float                   _jumpHeight;
    moveFptr                _movesFptrs[3];
    unsigned short          _moving;
    vector<RectangleShape>  &_walls;
    vector<CircleShape>     &_coins;
    Sound                   &_bounceSound;
    const double            &_backgroundSpeed;
    Uint32                  _score;
    
    void                    _moveLeft();
    void                    _moveRight();
    void                    _setFlattening();
    void                    _releaseJump();
    void                    _jump();
    void                    _inflation(const float width, const float height);
    void                    _flattening(const float width, const float height);
    void                    _fall();
    void                    _handleCollision();
    int                     _checkCollision();
    
public:
    Character(const double &, vector<RectangleShape> &, vector<CircleShape> &, Sound &);
    
    CircleShape         &getSprite();
    void                handleMoves();
    void                handleMoves(Event);
    void                reinit();
    
    Uint32 const        getScore() const;
};

#endif /* Character_hpp */
