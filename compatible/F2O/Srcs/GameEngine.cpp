//
//  GameEngine.cpp
//  F2O
//
//  Created by Anatole Menichetti on 31/10/2015.
//  Copyright © 2015 Anatole Menichetti. All rights reserved.
//
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "GameEngine.hpp"


GameEngine::GameEngine(ContextSettings settings) :
    _window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE, Style::Default, settings),
    _character(_backgroundSpeed, _walls, _coins, _bounceSound),
    _walls(5), _coins(3), _backgroundSpeed(BACKGROUND_SPEED), _gameStarted(false)
{}

/*
**      Init the game engine components:
**      Window, icon, font, theme, background, text.
*/
bool    GameEngine::init()
{
    srand(getpid() * time(0));
    if (!_icon.loadFromFile(ICON))
        return false;
    _window.setIcon(_icon.getSize().x, _icon.getSize().y, _icon.getPixelsPtr());
    if (!_texture.loadFromFile(BACKGROUND))
        return false;
    if (!_font.loadFromFile(FONT))
        return false;
    if (!_theme.openFromFile(THEME))
        return false;
    if (!_bounceBuff.loadFromFile(BOUNCE))
        return false;
    _bounceSound.setBuffer(_bounceBuff);
    _bounceSound.setVolume(10);
    _theme.play();
    
    _background[0].setTexture(_texture);
    _background[0].scale(Vector2f(.75, .75));
    _background[1].setTexture(_texture);
    _background[1].scale(Vector2f(.75, .75));
    _background[0].setPosition(Vector2f(0, 0));
    _background[1].setPosition(Vector2f(WINDOW_WIDTH, 0));
    _initText(_score, "0", Vector2f(WINDOW_WIDTH - 98, 35), 50, Color::Black);
    
    _initWall(_walls[0], 500, Color::Black, Vector2f(WINDOW_WIDTH, GROUND_POSITION));
    _initWall(_walls[1], 200, Color::Black, Vector2f(WINDOW_WIDTH, 200));
    _initWall(_walls[2], WINDOW_HEIGHT - GROUND_POSITION - 1, Color::White, Vector2f(WINDOW_WIDTH + rand() % 200, WINDOW_HEIGHT));
    _initWall(_walls[3], WINDOW_HEIGHT - GROUND_POSITION - 1, Color::White, Vector2f(WINDOW_WIDTH + rand() % 200 + 500, WINDOW_HEIGHT));
    _initWall(_walls[4], GROUND_POSITION - CHARACTER_SIZE * 4, Color::Black, Vector2f(WINDOW_WIDTH * 1.5, GROUND_POSITION - CHARACTER_SIZE * 4));
    
    _coins[0].setFillColor(Color::Black);
    _coins[0].setPosition(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
    _coins[0].setRadius(25);
    _coins[1].setFillColor(Color::Black);
    _coins[1].setPosition(Vector2f(WINDOW_WIDTH * 1.5, WINDOW_HEIGHT / 2));
    _coins[1].setRadius(15);
    _coins[2].setFillColor(Color::Black);
    _coins[2].setPosition(Vector2f(WINDOW_WIDTH * 2, WINDOW_HEIGHT / 2));
    _coins[2].setRadius(35);
    return true;
}

void    GameEngine::_reinit()
{
    if (_theme.getStatus() != Music::Stopped)
        _theme.stop();
    _theme.play();
    _background[0].setPosition(Vector2f(0, 0));
    _background[1].setPosition(Vector2f(WINDOW_WIDTH, 0));
    _initText(_score, "0", Vector2f(WINDOW_WIDTH - 98, 35), 50, Color::Black);
    
    _initWall(_walls[0], 500, Color::Black, Vector2f(WINDOW_WIDTH, GROUND_POSITION));
    _initWall(_walls[1], 200, Color::Black, Vector2f(WINDOW_WIDTH, 200));
    _initWall(_walls[2], WINDOW_HEIGHT - GROUND_POSITION - 1, Color::White, Vector2f(WINDOW_WIDTH + rand() % 200, WINDOW_HEIGHT));
    _initWall(_walls[3], WINDOW_HEIGHT - GROUND_POSITION - 1, Color::White, Vector2f(WINDOW_WIDTH + rand() % 200 + 500, WINDOW_HEIGHT));
    _initWall(_walls[4], GROUND_POSITION - CHARACTER_SIZE * 4, Color::Black, Vector2f(WINDOW_WIDTH * 1.5, GROUND_POSITION - CHARACTER_SIZE * 4));
    
    _coins[0].setFillColor(Color::Black);
    _coins[0].setPosition(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
    _coins[0].setRadius(25);
    _coins[1].setFillColor(Color::Black);
    _coins[1].setPosition(Vector2f(WINDOW_WIDTH * 1.5, WINDOW_HEIGHT / 2));
    _coins[1].setRadius(15);
    _coins[2].setFillColor(Color::Black);
    _coins[2].setPosition(Vector2f(WINDOW_WIDTH * 2, WINDOW_HEIGHT / 2));
    _coins[2].setRadius(35);
    
    _backgroundSpeed = BACKGROUND_SPEED;
    _character.reinit();
}

void    GameEngine::_initWall(RectangleShape &wall, const unsigned int size,
                              Color const &color, const Vector2f &pos)
{
    wall.setSize(Vector2f(25, size));
    wall.setOrigin(0, size);
    wall.setFillColor(color);
    wall.setPosition(pos);
}

void    GameEngine::_initText(Text &txt, const std::string &str,const Vector2f &pos,
                              const unsigned int size, const Color &color) const
{
    txt.setString(str);
    txt.setColor(color);
    txt.setCharacterSize(size);
    txt.setFont(_font);
    txt.setPosition(pos);
}

bool    GameEngine::_displayStartMenu()
{
    Text welcome, startMsg, tips;
    
    _initText(welcome, "Welcome in F2O", Vector2f(WINDOW_WIDTH / 2 - 1400 / 4, 300), 100, Color::Black);
    _initText(startMsg, "press any key to start or echap to quit.", Vector2f(WINDOW_WIDTH / 2 - 1600 / 4, 400), 50, Color::Black);
    _initText(tips, "You can move left using ARROW LEFT/A/Q.\nYou can move right using ARROW RIGHT/D.\nYou can jump or charge a jump using SPACE/ARROW DOWN/S.", Vector2f(WINDOW_WIDTH / 2 - 1300 / 4, GROUND_POSITION + 10), 25, Color::White);
    
    Event event;
    while (_window.pollEvent(event))
    {
        if (event.type == Event::Closed ||
            (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
            _window.close();
        if (event.type == Event::KeyPressed && event.key.code != Keyboard::Escape)
            return (_gameStarted = true);
    }
    _window.clear();
    _window.draw(_background[0]);
    _window.draw(welcome);
    _window.draw(startMsg);
    _window.draw(tips);
    _window.display();
    return (true);
}

/*
**      Game loop.
*/
void    GameEngine::start()
{
    while (_window.isOpen())
    {
        if (_gameStarted)
        {
            if (GAME_OVER(_character.getSprite().getPosition().x))
            {
                if (_gameOver())
                {
                    _window.close();
                    return;
                }
                else
                    _reinit();
            }
            _handleEvents();
            _moveSprites();
            _update();
        }
        else
            _displayStartMenu();
    }
}

void    GameEngine::_update()
{
    ostringstream os;
    
    os << _character.getScore();
    if (os.str() != _score.getString())
        _score.setString(os.str());
    _window.clear();
    _draw();
    _window.display();
}

/*
**      Draw game in main window.
*/
void    GameEngine::_draw()
{
    vector<RectangleShape>::const_iterator it = _walls.begin();
    vector<CircleShape>::const_iterator it2 = _coins.begin();
    
    _window.draw(_background[0]);
    _window.draw(_background[1]);
    _window.draw(_score);
    _window.draw(_character.getSprite());
    while (it != _walls.end())
    {
        if (it->getPosition().x < WINDOW_WIDTH)
            _window.draw(*it);
        ++it;
    }
    while (it2 != _coins.end())
    {
        if (it->getPosition().x < WINDOW_WIDTH)
            _window.draw(*it2);
        ++it2;
    }
}

/*
**      Handle specific events.
*/
void    GameEngine::_handleEvents()
{
    Event event;
    while (_window.pollEvent(event))
    {
        if (event.type == Event::Closed ||
            (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
            _window.close();
        _character.handleMoves(event);
    }
    _character.handleMoves();
}

bool    GameEngine::_gameOver()
{
    Text        game, over, retry, highscore;
    
    Vector2f    pos(WINDOW_WIDTH / 3 - 5 *
                 game.getCharacterSize() / 4, WINDOW_HEIGHT * 3/4);
    fstream     hsFile(".HS", ifstream::in);
    string      hsStr = "";
    
    if (hsFile.is_open())
    {
        Uint32      hs = 0;
        
        getline(hsFile, hsStr);
        istringstream is(hsStr);
        is >> hs;
        if (hs < _character.getScore())
        {
            hsFile.close();
            hsFile.open(".HS", ifstream::out | ifstream::trunc);
            string str = _score.getString();
            hsFile << str;
            hsStr = "New high score !";
        }
        else
            hsStr = "High score : " + hsStr + "";
        hsFile.close();
    }
    _initText(retry, "press any key to retry or echap to quit.", Vector2f(WINDOW_WIDTH / 2 - 1600 / 4, WINDOW_HEIGHT * 3/4 + 280), 50, Color::Black);
    _initText(game, "GAME", pos, 1, Color::Black);
    pos.x = WINDOW_WIDTH * 2 / 3 - 4 * game.getCharacterSize() / 4;
    _initText(over, "OVER", pos, 1, Color::Black);
    _initText(highscore, hsStr, Vector2f(WINDOW_WIDTH / 2 - hsStr.length() * 75 / 4, WINDOW_HEIGHT / 2), 75, Color(55, 190, 190));

    return (_showGameOver(game, over, retry, highscore));
}

bool    GameEngine::_showGameOver(Text &game, Text &over, Text &retry, Text &highScore)
{
    Event event;
    unsigned int cpt = 1;
    
    while (_score.getPosition().x > WINDOW_WIDTH / 2 -
           _score.getString().getSize() * _score.getCharacterSize() / 4)
    {
        _score.setCharacterSize(cpt);
        _score.move(Vector2f(-5, 1));
        Color   color = _score.getColor();
        if (++cpt < 190)
        {
            color.r += 2;
            color.g += 1;
            color.b += 1;
            _score.setColor(color);
            game.setCharacterSize(cpt);
            game.move(Vector2f(-1, -1));
            over.setCharacterSize(cpt);
            over.move(Vector2f(-1, -1));
            retry.move(Vector2f(0, -2));
        }
        _window.clear();
        _window.draw(_background[0]);
        _window.draw(_background[1]);
        _window.draw(_score);
        _window.draw(game);
        _window.draw(over);
        _window.draw(retry);
        _window.draw(highScore);
        _window.display();
        while (_window.pollEvent(event))
        {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
            _window.close();
        }
    }
    while (_window.waitEvent(event))
    {
        if (event.type == Event::Closed ||
            (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
        {
            _window.close();
            return (true);
        }
        if (event.type == Event::KeyPressed && event.key.code != Keyboard::Escape)
            return (false);
    }
    return (true);
}

void    GameEngine::_moveBackground(const unsigned short id)
{
    _background[id].move(Vector2f(-_backgroundSpeed, 0));
    if (_background[id].getPosition().x < 0 && _background[!id].getPosition().x < -25 &&
        _background[id].getPosition().x > _background[!id].getPosition().x)
    {
        _background[!id].setPosition(WINDOW_WIDTH, 0);
        
        int height = rand() % (GROUND_POSITION - CHARACTER_SIZE - 300) + 200;
        
        _initWall(_walls[0], height, Color::Black, Vector2f(WINDOW_WIDTH, GROUND_POSITION));
        height = GROUND_POSITION - height - CHARACTER_SIZE * (rand() % 4 + 4);
        _initWall(_walls[1], height, Color::Black, Vector2f(WINDOW_WIDTH, height));
        
        if (rand() % 2 && _walls[2].getPosition().x >= WINDOW_WIDTH)
            _walls[2].setPosition(Vector2f(WINDOW_WIDTH, _walls[2].getPosition().y));
        else
            if (rand() % 2 && _walls[3].getPosition().x >= WINDOW_WIDTH)
                _walls[3].setPosition(Vector2f(WINDOW_WIDTH, _walls[3].getPosition().y));
        
        if (_backgroundSpeed < BG_MAX_SPEED)
            _backgroundSpeed += BACKGROUND_ACCEL;
    }
}

void    GameEngine::_moveSprites()
{
    vector<RectangleShape>::iterator it = _walls.begin();
    vector<CircleShape>::iterator it2 = _coins.begin();
    
    _moveBackground(0);
    _moveBackground(1);
    while (it != _walls.end())
    {
        if (it->getPosition().x + it->getSize().x >= 0)
            it->move(Vector2f(-_backgroundSpeed, 0));
        ++it;
    }
    while (it2 != _coins.end())
    {
        if (it2->getPosition().x + it2->getRadius() * 2 >= 0)
            it2->move(Vector2f(-_backgroundSpeed / (4 - int(it2->getRadius()) % 3), 0));
        else
            it2->setPosition(Vector2f(WINDOW_WIDTH + rand() % WINDOW_WIDTH, rand() % (WINDOW_HEIGHT - 300) + 150));
        ++it2;
    }
    if (_walls[2].getPosition().x + _walls[2].getSize().x < 0)
    {
        _walls[2].setPosition(Vector2f(WINDOW_WIDTH + rand() % 500, _walls[2].getPosition().y));
    }
    if (_walls[3].getPosition().x + _walls[3].getSize().x < 0)
        _walls[3].setPosition(Vector2f(WINDOW_WIDTH + rand() % 500 + 300 + _walls[2].getPosition().x, _walls[2].getPosition().y));
    if (_walls[4].getPosition().x + _walls[4].getSize().x < 0)
    {
        if (_walls[0].getPosition().x > WINDOW_WIDTH / 4 && _walls[0].getPosition().x < WINDOW_WIDTH * 3 / 4)
            _walls[4].setPosition(Vector2f(WINDOW_WIDTH, GROUND_POSITION - CHARACTER_SIZE * 4 - rand() % (CHARACTER_SIZE * 4)));
    }
}
