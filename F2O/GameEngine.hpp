//
//  GameEngine.hpp
//  F2O
//
//  Created by Anatole Menichetti on 31/10/2015.
//  Copyright © 2015 Anatole Menichetti. All rights reserved.
//

#ifndef GameEngine_hpp
# define GameEngine_hpp

# include <stdio.h>
# include <iostream>
# include "ResourcePath.hpp"
# include "Character.hpp"

/*
**  Configuration
*/
# define WINDOW_WIDTH       1500
# define WINDOW_HEIGHT      1000
# define TITLE              "F2O"
# define ICON               resourcePath() + "icon.png"
# define FONT               resourcePath() + "sansation.ttf"
# define THEME              resourcePath() + "theme.wav"
# define BOUNCE             resourcePath() + "Bounce.wav"
# define BACKGROUND         resourcePath() + "background.jpg"
# define BACKGROUND_SPEED   2 * GLOBAL_SPEED
# define BACKGROUND_ACCEL   .033
# define BG_MAX_SPEED       5 * GLOBAL_SPEED
# define GROUND_POSITION    (WINDOW_HEIGHT - 112)
# define WALL_SPEED         2 * GLOBAL_SPEED
# define GAME_OVER(x)       (x <= -CHARACTER_SIZE / 3 ? 1 : 0)

/*
** Main class. Represents the game core.
*/
class GameEngine
{
private:
    RenderWindow            _window;
    Image                   _icon;
    Music                   _theme;
    SoundBuffer             _bounceBuff;
    Sound                   _bounceSound;
    Font                    _font;
    Text                    _score;
    Texture                 _texture;
    Sprite                  _background[2];
    Character               _character;
    vector<RectangleShape>  _walls;
    vector<CircleShape>     _coins;
    double                  _backgroundSpeed;
    
    void                    _handleEvents();
    void                    _update();
    void                    _draw();
    void                    _reinit();
    void                     _initWall(RectangleShape &, const unsigned int,
                                       Color const &, const Vector2f &);
    void                    _initText(Text &, const std::string &, const Vector2f &,
                                      const unsigned int, const Color &) const;
    void                    _moveSprites();
    void                    _moveBackground(const unsigned short);
    bool                    _gameOver();
    void                    _showGameOver(Text &, Text &);
    bool                    _playAgain(Text &, Text &);
    
public:
    GameEngine(ContextSettings);
    
    bool    init();
    void    start();
};

#endif /* GameEngine_hpp */
