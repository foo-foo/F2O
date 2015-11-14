//
//  Character.cpp
//  F2O
//
//  Created by Anatole Menichetti on 01/11/2015.
//  Copyright © 2015 Anatole Menichetti. All rights reserved.
//

#include "Character.hpp"
#include "GameEngine.hpp"

Character::Character(const double &speed, vector<RectangleShape> &walls,
                    vector<CircleShape> &coins, Sound &bounceSound) :
    _sprite(CHARACTER_SIZE), _isFlattening(false), _jumpHeight(1),
    _moving(0),  _walls(walls), _coins(coins),
    _bounceSound(bounceSound), _backgroundSpeed(speed), _score(0)
{
    _movesFptrs[0] = &Character::_moveLeft;
    _movesFptrs[1] = &Character::_moveRight;
    _movesFptrs[2] = &Character::_setFlattening;
    
    _sprite.setFillColor(CHARACTER_COLOR);
    _sprite.setPosition(Vector2f(75, GROUND_POSITION));
    _sprite.setOrigin(Vector2f(CHARACTER_SIZE, CHARACTER_SIZE * 2));
}

void    Character::reinit()
{
    _sprite.setScale(Vector2f(1, 1));
    _sprite.setFillColor(CHARACTER_COLOR);
    _sprite.setPosition(Vector2f(75, GROUND_POSITION));
    _sprite.setOrigin(Vector2f(CHARACTER_SIZE, CHARACTER_SIZE * 2));
    _moving = 0;
    _isFlattening = false;
    _jumpHeight = 1;
    _score = 0;
}

/*
 **      Set character state to moving left. While the left key haven't been released, it will move left.
 */
void    Character::_moveLeft()
{
    float x = _sprite.getPosition().x;
    float y = _sprite.getPosition().y;
    float s = _sprite.getScale().x * CHARACTER_SIZE;
    
    if (_checkCollision())
        return ;
    if (s < 0)
        s = -s;
    _moving = LEFT;
    if (x - s > 0)
        _sprite.move(Vector2f(-CHARACTER_SPEED, 0));
}

/*
**      Set character state to moving right. While the right key haven't been released, it will move right.
*/
void    Character::_moveRight()
{
    float x = _sprite.getPosition().x;
    float y = _sprite.getPosition().y;
    float s = _sprite.getScale().x * CHARACTER_SIZE;
    
    if (_checkCollision())
        return ;
    if (s < 0)
        s = -s;
    _moving = RIGHT;
    if (x + s < WINDOW_WIDTH)
        _sprite.move(Vector2f(CHARACTER_SPEED, 0));
}

/*
**      Set character state to flattening. While the space key haven't been released, it will be \
**      flattening more and more.
*/
void    Character::_setFlattening()
{
    _isFlattening = true;
}

/*
**      Make the character moving up.
*/
void    Character::_jump()
{
    _sprite.move(Vector2f(0, -2 * JUMP_SPEED));
    _jumpHeight += .0025 * JUMP_SPEED;
}

/*
**      Increase the size of the character more or less quickly depending of the actual size.
*/
void    Character::_inflation(const float width, const float height)
{
    if (height < INFLATION_LIMIT)
        _sprite.setScale(Vector2f(width + INFLATION_SPEED1,  height + INFLATION_SPEED1));
    else
        _sprite.setScale(Vector2f(width + INFLATION_SPEED2,  height + INFLATION_SPEED2));
    if (height > 0 && _sprite.getFillColor() == Color::White)
        _sprite.setFillColor(Color::Black );
}

/*
**      Decrease the size of the character more or less quickly depending of the actual size.
*/
void    Character::_flattening(const float width, const float height)
{
    if (height > FLATTENING_LIMIT)
        _sprite.setScale(Vector2f(width - FLATTENING_SPEED1,  height - FLATTENING_SPEED1));
    else
        _sprite.setScale(Vector2f(width - FLATTENING_SPEED2,  height - FLATTENING_SPEED2));
    if (height < .25 && height > -.25)
        _sprite.setScale(Vector2f(-.25,  -.25));
    if (height <= 0 && _sprite.getFillColor() == Color::Black)
        _sprite.setFillColor(Color::White);
}

/*
**      If jump hasn't been loaded to much, set the jump height and play the bounce sound.
*/
void    Character::_releaseJump()
{
    _isFlattening = false;
    if ((_jumpHeight = _sprite.getScale().y) > JUMP_MIN_HEIGHT)
        _jumpHeight = JUMP_MIN_HEIGHT;
    if (_jumpHeight < -0.1)
        _jumpHeight = 0;
    if (_sprite.getPosition().y < GROUND_POSITION)
        _jumpHeight = 1;
    else
    {
        _bounceSound.setPlayingOffset(seconds(.3));
        _bounceSound.play();
    }
}

void    Character::_fall()
{
    float x = _sprite.getPosition().x;
    float y = _sprite.getPosition().y;
    float s = _sprite.getScale().x * CHARACTER_SIZE;
    
    if (y < GROUND_POSITION)
        _sprite.move(Vector2f(0, GRAVITY));
}

int     Character::_checkCollision()
{
    float s = CHARACTER_SIZE * _sprite.getScale().x;
    float x = _sprite.getPosition().x;
    float y = _sprite.getPosition().y;
    vector<RectangleShape>::const_iterator it = _walls.begin();
    vector<CircleShape>::iterator it2 = _coins.begin();
    
    if (s < 0)
    {
        s = -s;
        y += s * 2;
    }
    while (it2 != _coins.end())
    {
        float coinX = it2->getPosition().x;
        float coinY = it2->getPosition().y;
        
        if (((y > coinY && y - s * 2 < coinY) ||
            (y > coinY - it2->getRadius() * 2 && y - s * 2 < coinY)) &&
            ((x < coinX && x + s * 2 > coinX) ||
             (x < coinX + it2->getRadius() * 2 && x + s * 2 > coinX)))
        {
            it2->setPosition(Vector2f(WINDOW_WIDTH + rand() % WINDOW_WIDTH, rand() % (WINDOW_HEIGHT - 300) + 150));
            _score += 5;
        }
        ++it2;
    }
    while (it != _walls.end())
    {
        float wallX = it->getPosition().x;
        float wallY = it->getPosition().y;
        
        if (y - s * 2 < wallY && y >= wallY - it->getSize().y)
        {
            if (x + s <= wallX + 25 && x + s >= wallX)
                return (LEFT);
            if (x - s >= wallX && x - s <= wallX + 25)
                return (RIGHT);
            if (x - s <= wallX && x + s >= wallX + 25)
            {
                if (wallX - (x - s) > x + s - (wallX + 25))
                    return (LEFT);
                else
                    return (RIGHT);
            }
        }
        ++it;
    }
    return (0);
}

void    Character::_handleCollision()
{
    int collision = _checkCollision();
    
    if (collision == LEFT)
        _sprite.move(Vector2f(-_backgroundSpeed, 0));
    if (collision == RIGHT)
        _sprite.move(Vector2f(_backgroundSpeed, 0));
}

/*
**      Repeat each move associated to a key which haven't been released yet.
*/
void    Character::handleMoves()
{
    const float height = _sprite.getScale().y;
    const float width = _sprite.getScale().x;
    
    _handleCollision();
    if (!_isFlattening && height < 1)
        _inflation(width, height);
    if (_isFlattening && height > -1)
        _flattening(width, height);
    if (_jumpHeight < 1)
        _jump();
    if (_jumpHeight >= 1)
        _fall();
    if (_moving)
        (this->*_movesFptrs[_moving - 1])();
}

/*
**      Handle the key pressed and key released events.
*/
void    Character::handleMoves(Event event)
{
    Keyboard::Key   keysPressed[8] = {Keyboard::A, Keyboard::Q, Keyboard::Left, Keyboard::D, Keyboard::Right, Keyboard::Space, Keyboard::S, Keyboard::Down};
    int             indexes[8] = {0, 0, 0, 1, 1, 2, 2, 2};
    int             i = 0;
    
    if (event.type == Event::KeyPressed || event.type == Event::KeyReleased)
        while (i < 8 && keysPressed[i] != event.key.code)
            i += 1;
    if (i < 8 && event.type == Event::KeyPressed)
        (this->*_movesFptrs[indexes[i]])();
    if (event.type == Event::KeyReleased && i < 8 && indexes[i] == 2)
        _releaseJump();
    if (event.type == Event::KeyReleased && i < 8 &&
        ((indexes[i] - 1 == LEFT && _moving == LEFT) ||
         (indexes[i] - 1 == RIGHT && _moving == RIGHT)))
        _moving = NONE;
}

CircleShape   &Character::getSprite()
{
    return _sprite;
}

Uint32 const        Character::getScore() const
{
    return _score;
}