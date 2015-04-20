#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <string.h>

enum GameState {isPlaying, isClosing, splashScreen, gameOver};
GameState currentGameState = splashScreen;

void DisplaySplashScreen(sf::RenderWindow &, std::vector<sf::VideoMode> &, int &, sf::VideoMode &);
void GameLoop(sf::RenderWindow &);
void DisplayGameOverScreen(sf::RenderWindow &);
void Pause(sf::RenderWindow &);
void SetTargetPosition(sf::RectangleShape &, std::vector<sf::RectangleShape> &, sf::View &);
bool IsColliding(sf::RectangleShape &, sf::RectangleShape &);
bool IsColliding(sf::RectangleShape &, std::vector<sf::RectangleShape> &, int, int);
bool RulesAreBroken(std::vector<sf::RectangleShape> &, sf::RenderWindow &);
void SetSpeed(unsigned int, sf::RenderWindow &);
std::string ConvertIntToString(int);
void PrintInstructions(sf::RenderWindow &);
void ShowOptions(sf::RenderWindow &, std::vector<sf::VideoMode> &, int &, sf::VideoMode &);

int main()
{
	srand(time(NULL));

    std::vector<sf::VideoMode> videoModes = sf::VideoMode::getFullscreenModes();
    int i, currentVideoModePos;
    for (i = 0; i < videoModes.size(); i++)
        if (videoModes[i].width < 800 || videoModes[i].height < 600 || videoModes[i].bitsPerPixel < 32)
        {
            videoModes.erase(videoModes.begin() + i);
            i--;
        }
    currentVideoModePos = videoModes.size() - 1;
    sf::VideoMode currentVideoMode = videoModes[currentVideoModePos];

	sf::RenderWindow mainWindow(currentVideoMode, "Snake", sf::Style::Close);
	mainWindow.setFramerateLimit(30);
	do
	{
		if (currentGameState == splashScreen)
			DisplaySplashScreen(mainWindow, videoModes, currentVideoModePos, currentVideoMode);
		else if (currentGameState == isPlaying)
			GameLoop(mainWindow);
		else if (currentGameState == gameOver)
			DisplayGameOverScreen(mainWindow);
	} while (currentGameState != isClosing);

	return 0;
}

void DisplaySplashScreen(sf::RenderWindow &mainWindow, std::vector<sf::VideoMode> &videoModes, int &currentVideoModePos, sf::VideoMode &currentVideoMode)
{
	sf::Font font1;
	font1.loadFromFile("data\\fonts\\AdobeHeitiStd-Regular.otf");
	sf::Text playText, authorName, title, instructions, options;
	playText.setFont(font1);
	authorName.setFont(font1);
	title.setFont(font1);
	instructions.setFont(font1);
	options.setFont(font1);

	title.setString("SNAKE Project");
	title.setColor(sf::Color(255, 0, 0));
	title.setOrigin(title.getGlobalBounds().width / 2, title.getGlobalBounds().height / 2);
	title.setPosition(mainWindow.getSize().x / 2, 100);
	playText.setString("Play");
	playText.setCharacterSize(35);
	playText.setOrigin(playText.getGlobalBounds().width / 2, playText.getGlobalBounds().height / 2);
	playText.setPosition(mainWindow.getSize().x / 2, mainWindow.getSize().y / 2);
	authorName.setString("©2013 Alexandru Ghergut");
	authorName.setOrigin(authorName.getGlobalBounds().width, authorName.getGlobalBounds().height);
	authorName.setPosition(mainWindow.getSize().x - 10, mainWindow.getSize().y - 5);
	instructions.setString("Instructions");
	instructions.setCharacterSize(35);
	instructions.setOrigin(instructions.getGlobalBounds().width / 2, instructions.getGlobalBounds().height / 2);
	instructions.setPosition(mainWindow.getSize().x / 2, playText.getPosition().y + 35);
	options.setString("Options (Experimental)");
	options.setCharacterSize(35);
	options.setOrigin(options.getGlobalBounds().width / 2, options.getGlobalBounds().height / 2);
	options.setPosition(mainWindow.getSize().x / 2, instructions.getPosition().y + 45);

	while (currentGameState == splashScreen)
	{
		sf::Event currentEvent;
		while (mainWindow.pollEvent(currentEvent))
		{
			switch (currentEvent.type)
			{
			case sf::Event::Closed:
				currentGameState = isClosing;
				break;
			case sf::Event::MouseButtonPressed:
				if (currentEvent.mouseButton.x > playText.getPosition().x - playText.getGlobalBounds().width / 2 && currentEvent.mouseButton.x < playText.getPosition().x + playText.getGlobalBounds().width / 2 &&
					currentEvent.mouseButton.y > playText.getPosition().y - playText.getGlobalBounds().height / 2 && currentEvent.mouseButton.y < playText.getPosition().y + playText.getGlobalBounds().height / 2)
						currentGameState = isPlaying;
                else if (currentEvent.mouseButton.x > instructions.getPosition().x - instructions.getGlobalBounds().width / 2 && currentEvent.mouseButton.x < instructions.getPosition().x + instructions.getGlobalBounds().width / 2 &&
					currentEvent.mouseButton.y > instructions.getPosition().y - instructions.getGlobalBounds().height / 2 && currentEvent.mouseButton.y < instructions.getPosition().y + instructions.getGlobalBounds().height / 2)
						PrintInstructions(mainWindow);
                  else if (currentEvent.mouseButton.x > options.getPosition().x - options.getGlobalBounds().width / 2 && currentEvent.mouseButton.x < options.getPosition().x + options.getGlobalBounds().width / 2 &&
					currentEvent.mouseButton.y > options.getPosition().y - options.getGlobalBounds().height / 2 && currentEvent.mouseButton.y < options.getPosition().y + options.getGlobalBounds().height / 2)
						ShowOptions(mainWindow, videoModes, currentVideoModePos, currentVideoMode);
				break;
			case sf::Event::MouseMoved:
				if (currentEvent.mouseMove.x > playText.getPosition().x - playText.getGlobalBounds().width / 2 && currentEvent.mouseMove.x < playText.getPosition().x + playText.getGlobalBounds().width / 2 &&
					currentEvent.mouseMove.y > playText.getPosition().y - playText.getGlobalBounds().height / 2 && currentEvent.mouseMove.y < playText.getPosition().y + playText.getGlobalBounds().height / 2)
						playText.setCharacterSize(40);
				else
					playText.setCharacterSize(35);
                if (currentEvent.mouseMove.x > instructions.getPosition().x - instructions.getGlobalBounds().width / 2 && currentEvent.mouseMove.x < instructions.getPosition().x + instructions.getGlobalBounds().width / 2 &&
					currentEvent.mouseMove.y > instructions.getPosition().y - instructions.getGlobalBounds().height / 2 && currentEvent.mouseMove.y < instructions.getPosition().y + instructions.getGlobalBounds().height / 2)
						instructions.setCharacterSize(40);
                else
                    instructions.setCharacterSize(35);
                if (currentEvent.mouseMove.x > options.getPosition().x - options.getGlobalBounds().width / 2 && currentEvent.mouseMove.x < options.getPosition().x + options.getGlobalBounds().width / 2 &&
					currentEvent.mouseMove.y > options.getPosition().y - options.getGlobalBounds().height / 2 && currentEvent.mouseMove.y < options.getPosition().y + options.getGlobalBounds().height / 2)
						options.setCharacterSize(40);
                else
                    options.setCharacterSize(35);
				break;
			default:
				break;
			}
		}

		mainWindow.clear();
		mainWindow.draw(title);
		mainWindow.draw(playText);
		mainWindow.draw(authorName);
		mainWindow.draw(instructions);
		mainWindow.draw(options);
		mainWindow.display();
	}
}

void GameLoop(sf::RenderWindow &mainWindow)
{
    sf::View mainCamera;
    mainCamera.reset(sf::FloatRect(0, 0, 800, 600));
	mainWindow.setTitle("Snake");
	mainWindow.setFramerateLimit(8);
	sf::Music track1;
	sf::Texture backgroundT;
	sf::RectangleShape square(sf::Vector2f(20, 20));
	std::vector<sf::RectangleShape> snake(2);
	sf::RectangleShape target;
	float initialPosX1, initialPosY1, initialPosX2, initialPosY2;
	enum Sense {none, left, right, up, down};
	Sense snakeSense = none;
	unsigned int score = 0;
	std::string scoreString;

	track1.openFromFile("data/audio/Psybot - Future Technology.ogg");
	track1.setLoop(true);
	track1.setVolume(30);
	track1.play();
	if (!backgroundT.loadFromFile("data/images/spacebg.jpg"))
        currentGameState = isClosing;
	sf::Sprite backgroundS;
	backgroundS.setTexture(backgroundT);
	backgroundS.setScale(mainCamera.getSize().x / backgroundS.getGlobalBounds().width, mainCamera.getSize().y / backgroundS.getGlobalBounds().height);
	backgroundS.setColor(sf::Color(255, 255, 255, 240));
	square.setFillColor(sf::Color(0, 255, 255));
	target = square;
	target.setFillColor(sf::Color(127, 255, 0));

	unsigned int i, zeroes;
	for (i = 0; i < snake.size(); i++)
		snake[i] = square;

	snake[0].setPosition(mainCamera.getSize().x / 2, mainCamera.getSize().y / 2);
	snake[1].setPosition(snake[0].getPosition().x - snake[1].getGlobalBounds().width, snake[0].getPosition().y);

	SetTargetPosition(target, snake, mainCamera);

	const float DISTANCE = square.getGlobalBounds().width;
	bool snakeMovedRecently;
	while (currentGameState == isPlaying)
	{
		sf::Event currentEvent;
		if (mainWindow.pollEvent(currentEvent))
		{
			switch (currentEvent.type)
			{
			case sf::Event::Closed:
				currentGameState = isClosing;
				break;
			case sf::Event::KeyPressed:
				if (currentEvent.key.code == sf::Keyboard::Left && snakeSense != right)
                    snakeSense = left;
				else if (currentEvent.key.code == sf::Keyboard::Right && snakeSense != left)
                    snakeSense = right;
				else if (currentEvent.key.code == sf::Keyboard::Up && snakeSense != down)
                    snakeSense = up;
				else if (currentEvent.key.code == sf::Keyboard::Down && snakeSense != up)
                    snakeSense = down;
				else if (currentEvent.key.code == sf::Keyboard::Escape)
				{
					track1.pause();
					Pause(mainWindow);
					track1.play();
				}
				break;
			default:
				break;
			}
		}

		mainWindow.clear();
		mainWindow.setView(mainCamera);

		if (snakeSense != none)
		{
			for (i = 0; i < snake.size(); i++)
			{
                if (i == 0)
                {
                    initialPosX1 = snake[i].getPosition().x;
                    initialPosY1 = snake[i].getPosition().y;
                    switch(snakeSense)
                    {
                    case left:
                        snake[0].move(-DISTANCE, 0);
                        break;
                    case right:
                        snake[0].move(DISTANCE, 0);
                        break;
                    case up:
                        snake[0].move(0, -DISTANCE);
                        break;
                    case down:
                        snake[0].move(0, DISTANCE);
                        break;
                    default:
                        break;
                    }

                    if (IsColliding(snake[0], target))
                    {
                        if (score < 100)
                            score += 10;
                        else if (score < 1000)
                            score += 50;
                        else if (score < 10000)
                            score += 100;
                        else if (score < 100000)
                            score += 500;
                        scoreString = ConvertIntToString(score);
                        mainWindow.setTitle("Snake (Score: " + scoreString + ")");
                        snake.push_back(square);
                        SetSpeed(score, mainWindow);
                        SetTargetPosition(target, snake, mainCamera);
                    }

                }
				else
				{
					initialPosX2 = snake[i].getPosition().x;
					initialPosY2 = snake[i].getPosition().y;
					snake[i].setPosition(initialPosX1, initialPosY1);
					initialPosX1 = initialPosX2;
					initialPosY1 = initialPosY2;
				}
			}
			snakeMovedRecently = false;
		}

		mainWindow.draw(backgroundS);
		for (i = 0; i < snake.size(); i++)
			mainWindow.draw(snake[i]);
		mainWindow.draw(target);

		mainWindow.display();

		if (RulesAreBroken(snake, mainWindow))
			currentGameState = gameOver;
	}
	mainWindow.setView(mainWindow.getDefaultView());
}

void DisplayGameOverScreen(sf::RenderWindow &mainWindow)
{
	sf::Font font1;
	font1.loadFromFile("data\\fonts\\AdobeHeitiStd-Regular.otf");
	sf::Text gameOverText, soundtrackText, tryAgain, exit;
	gameOverText.setFont(font1);
	soundtrackText.setFont(font1);
	tryAgain.setFont(font1);
	exit.setFont(font1);

	gameOverText.setString("GAME OVER");
	gameOverText.setColor(sf::Color(255, 0, 0));
	gameOverText.setOrigin(gameOverText.getGlobalBounds().width / 2, gameOverText.getGlobalBounds().height / 2);
	gameOverText.setPosition(mainWindow.getSize().x / 2, mainWindow.getSize().y / 2 - 100);
	soundtrackText.setString("Soundtrack: Psybot - Future Technology");
	soundtrackText.setCharacterSize(20);
	tryAgain.setString("Try Again");
	tryAgain.setCharacterSize(25);
	tryAgain.setOrigin(tryAgain.getGlobalBounds().width / 2, tryAgain.getGlobalBounds().height / 2);
	tryAgain.setPosition(mainWindow.getSize().x / 2, mainWindow.getSize().y / 2 - 30);
	exit.setString("Exit");
	exit.setCharacterSize(25);
	exit.setOrigin(exit.getGlobalBounds().width / 2, exit.getGlobalBounds().height / 2);
	exit.setPosition(mainWindow.getSize().x / 2, mainWindow.getSize().y / 2);

	while (currentGameState == gameOver)
	{
		sf::Event currentEvent;
		while (mainWindow.pollEvent(currentEvent))
		{
			switch (currentEvent.type)
			{
			case sf::Event::Closed:
				currentGameState = isClosing;
				break;
			case sf::Event::KeyPressed:
				if (currentEvent.key.code == sf::Keyboard::R)
					currentGameState = isPlaying;
				break;
			case sf::Event::MouseButtonPressed:
				if (currentEvent.mouseButton.x > tryAgain.getPosition().x - tryAgain.getGlobalBounds().width / 2 && currentEvent.mouseButton.x < tryAgain.getPosition().x + tryAgain.getGlobalBounds().width / 2 &&
					currentEvent.mouseButton.y > tryAgain.getPosition().y - tryAgain.getGlobalBounds().height / 2 && currentEvent.mouseButton.y < tryAgain.getPosition().y + tryAgain.getGlobalBounds().height / 2)
						currentGameState = isPlaying;
				else if (currentEvent.mouseButton.x > exit.getPosition().x - exit.getGlobalBounds().width / 2 && currentEvent.mouseButton.x < exit.getPosition().x + exit.getGlobalBounds().width / 2 &&
						currentEvent.mouseButton.y > exit.getPosition().y - exit.getGlobalBounds().height / 2 && currentEvent.mouseButton.y < exit.getPosition().y + exit.getGlobalBounds().height / 2)
							currentGameState = isClosing;
				break;
			case sf::Event::MouseMoved:
				if (currentEvent.mouseMove.x > tryAgain.getPosition().x - tryAgain.getGlobalBounds().width / 2 && currentEvent.mouseMove.x < tryAgain.getPosition().x + tryAgain.getGlobalBounds().width / 2 &&
					currentEvent.mouseMove.y > tryAgain.getPosition().y - tryAgain.getGlobalBounds().height / 2 && currentEvent.mouseMove.y < tryAgain.getPosition().y + tryAgain.getGlobalBounds().height / 2)
						tryAgain.setCharacterSize(30);
				else
					tryAgain.setCharacterSize(25);
				if (currentEvent.mouseMove.x > exit.getPosition().x - exit.getGlobalBounds().width / 2 && currentEvent.mouseMove.x < exit.getPosition().x + exit.getGlobalBounds().width / 2 &&
					currentEvent.mouseMove.y > exit.getPosition().y - exit.getGlobalBounds().height / 2 && currentEvent.mouseMove.y < exit.getPosition().y + exit.getGlobalBounds().height / 2)
						exit.setCharacterSize(30);
				else
					exit.setCharacterSize(25);
				break;

			default:
				break;
			}
		}

		mainWindow.clear();
		mainWindow.draw(soundtrackText);
		mainWindow.draw(gameOverText);
		mainWindow.draw(tryAgain);
		mainWindow.draw(exit);
		mainWindow.display();
	}
}

void Pause(sf::RenderWindow &mainWindow)
{
	sf::Font font1;
	font1.loadFromFile("data\\fonts\\AdobeHeitiStd-Regular.otf");
	sf::Text pausedText, pressEscText;
	pausedText.setFont(font1);
	pausedText.setString("Paused...");
	pausedText.setOrigin(pausedText.getGlobalBounds().width / 2, pausedText.getGlobalBounds().height / 2);
	pausedText.setPosition(mainWindow.getSize().x / 2, mainWindow.getSize().y / 2);

	pressEscText.setFont(font1);
	pressEscText.setString("Press ESC to resume.");
	pressEscText.setCharacterSize(20);
	pressEscText.setOrigin(pressEscText.getGlobalBounds().width / 2, pressEscText.getGlobalBounds().height / 2);
	pressEscText.setPosition(mainWindow.getSize().x / 2, mainWindow.getSize().y / 2 + 40);

	sf::Event pauseEvent;
	bool isPaused = true;
	while (isPaused)
	{
		while (mainWindow.pollEvent(pauseEvent))
		{
			switch (pauseEvent.type)
			{
			case sf::Event::Closed:
                currentGameState = isClosing;
                isPaused = false;
            break;
			case sf::Event::KeyPressed:
				if (pauseEvent.key.code == sf::Keyboard::Escape)
					isPaused = false;
			}
		}

		mainWindow.clear();
		mainWindow.draw(pausedText);
		mainWindow.draw(pressEscText);
		mainWindow.display();
	}
}

void SetTargetPosition(sf::RectangleShape &target, std::vector<sf::RectangleShape> &snake, sf::View &mainCamera)
{
	do
	{
		int maxRandomX = mainCamera.getSize().x / target.getGlobalBounds().width;
		int maxRandomY = mainCamera.getSize().y / target.getGlobalBounds().height;
		target.setPosition(target.getGlobalBounds().width * (rand() % maxRandomX), target.getGlobalBounds().height * (rand() % maxRandomY));
	} while (IsColliding(target, snake, 0, (int)snake.size() - 1));
}

bool IsColliding(sf::RectangleShape &object1, sf::RectangleShape &object2)
{
	if (object1.getPosition() == object2.getPosition())
		return true;
	else
		return false;
}

bool IsColliding(sf::RectangleShape &object1, std::vector<sf::RectangleShape> &objectsVector, int startPoint, int endPoint)
{
	for (int i = startPoint; i <= endPoint; i++)
		if (objectsVector[i].getPosition() == object1.getPosition())
			return true;

	return false;
}

bool RulesAreBroken(std::vector<sf::RectangleShape> &snake, sf::RenderWindow &mainWindow)
{
	if (snake[0].getPosition().x < 0 || snake[0].getPosition().x > mainWindow.getSize().x - snake[0].getGlobalBounds().width ||
		snake[0].getPosition().y < 0 || snake[0].getPosition().y > mainWindow.getSize().y - snake[0].getGlobalBounds().height)
			return true;

	if (IsColliding(snake[0], snake, 1, int(snake.size() - 1)))
		return true;

	return false;
}

void SetSpeed(unsigned int score, sf::RenderWindow &mainWindow)
{
	switch(score)
	{
	case 50:
		mainWindow.setFramerateLimit(13);
		break;
	case 100:
		mainWindow.setFramerateLimit(15);
		break;
	case 500:
		mainWindow.setFramerateLimit(16);
		break;
	case 1000:
		mainWindow.setFramerateLimit(18);
		break;
	case 5000:
		mainWindow.setFramerateLimit(20);
		break;
	default:
		break;
	}
}

std::string ConvertIntToString(int n)
{
    char charArray[n + 1];
    int position = 0;
    while (n)
    {
        charArray[position++] = n % 10 + '0';
        n = n / 10;
    }
    charArray[position] = '\0';

    strrev(charArray);
    std::string scoreString = charArray;
    return scoreString;
}

void MoveSnakeHead(std::vector<sf::RectangleShape> &snake, int snakeSense, const float DISTANCE, float &initialPosX1, float &initialPosY1, bool &snakeMovedRecently)
{
        snakeMovedRecently = true;
        enum Sense {none, left, right, up, down};
        initialPosX1 = snake[0].getPosition().x;
        initialPosY1 = snake[0].getPosition().y;
        switch (snakeSense)
        {
        case left:
            snake[0].move(-DISTANCE, 0);
            break;
        case right:
            snake[0].move(DISTANCE, 0);
            break;
        case up:
            snake[0].move(0, -DISTANCE);
            break;
        case down:
            snake[0].move(0, DISTANCE);
            break;
        default:
            break;
        }
}

void PrintInstructions(sf::RenderWindow &mainWindow)
{
    bool showingInstructions = true;
    sf::Font font1;
    font1.loadFromFile("data/fonts/AdobeHeitiStd-Regular.otf");
    sf::Text instructions, backButton;
    instructions.setFont(font1);
    backButton.setFont(font1);
    instructions.setString("Navigation\nUse the arrow keys to control the snake.\nESC - Pause or Resume");
    instructions.setCharacterSize(25);
    instructions.setOrigin(instructions.getGlobalBounds().width / 2, 0);
    instructions.setPosition(mainWindow.getSize().x / 2, 0);
    backButton.setString("< Back");
    backButton.setCharacterSize(35);
    backButton.setPosition(10, mainWindow.getSize().y - backButton.getGlobalBounds().height - 15);

    while (showingInstructions)
    {
        sf::Event currentEvent;
        while (mainWindow.pollEvent(currentEvent))
        {
            switch (currentEvent.type)
            {
            case sf::Event::Closed:
            {
                showingInstructions = false;
                currentGameState = isClosing;
            }
            break;
            case sf::Event::MouseButtonPressed:
                if (currentEvent.mouseButton.x > backButton.getPosition().x && currentEvent.mouseButton.x < backButton.getPosition().x + backButton.getGlobalBounds().width &&
                    currentEvent.mouseButton.y > backButton.getPosition().y && currentEvent.mouseButton.y < backButton.getPosition().y + backButton.getGlobalBounds().height)
                        showingInstructions = false;
            break;
            case sf::Event::MouseMoved:
                if (currentEvent.mouseMove.x > backButton.getPosition().x && currentEvent.mouseMove.x < backButton.getPosition().x + backButton.getGlobalBounds().width &&
                    currentEvent.mouseMove.y > backButton.getPosition().y && currentEvent.mouseMove.y < backButton.getPosition().y + backButton.getGlobalBounds().height)
                        backButton.setCharacterSize(40);
                else
                    backButton.setCharacterSize(35);
            break;
            }
        }

        mainWindow.clear();
        mainWindow.draw(instructions);
        mainWindow.draw(backButton);
        mainWindow.display();
    }
}

void ShowOptions(sf::RenderWindow &mainWindow, std::vector<sf::VideoMode> &videoModes, int &currentVideoModePos, sf::VideoMode &currentVideoMode)
{
    bool showingOptions = true;
    sf::Font font1;
    font1.loadFromFile("data/fonts/AdobeHeitiStd-Regular.otf");
    sf::Text currentResolutionText, resolution, changeArrowLeft1, changeArrowRight1;

    currentResolutionText.setFont(font1);
    currentResolutionText.setString("Current Resolution: ");
    currentResolutionText.setCharacterSize(25);
    currentResolutionText.setPosition(10, 20);
    resolution.setFont(font1);
    resolution.setString(ConvertIntToString(currentVideoMode.width) + " x " + ConvertIntToString(currentVideoMode.height) + " " + ConvertIntToString(currentVideoMode.bitsPerPixel) + " bit");
    resolution.setCharacterSize(25);
    resolution.setPosition(mainWindow.getSize().x / 2 + 30, 20);
    changeArrowLeft1.setFont(font1);
    changeArrowRight1.setFont(font1);
    changeArrowLeft1.setString("<");
    changeArrowRight1.setString(">");
    changeArrowLeft1.setCharacterSize(25);
    changeArrowRight1.setCharacterSize(25);
    changeArrowLeft1.setPosition(resolution.getPosition().x - 30, resolution.getPosition().y);
    changeArrowRight1.setPosition(resolution.getPosition().x + resolution.getGlobalBounds().width + 10, resolution.getPosition().y);

    while (showingOptions)
    {
        sf::Event currentEvent;
        while (mainWindow.pollEvent(currentEvent))
        {
            switch(currentEvent.type)
            {
            case sf::Event::Closed:
                showingOptions = false;
                currentGameState = isClosing;
                break;
            case sf::Event::MouseButtonPressed:
                if (currentEvent.mouseButton.x > changeArrowLeft1.getPosition().x && currentEvent.mouseButton.x < changeArrowLeft1.getPosition().x + changeArrowLeft1.getGlobalBounds().width &&
                    currentEvent.mouseButton.y > changeArrowLeft1.getPosition().y && currentEvent.mouseButton.y < changeArrowLeft1.getPosition().y + changeArrowLeft1.getGlobalBounds().height)
                {
                    if (currentVideoModePos < videoModes.size() - 1)
                    {
                        currentVideoModePos++;
                        currentVideoMode = videoModes[currentVideoModePos];
                        resolution.setString(ConvertIntToString(currentVideoMode.width) + " x " + ConvertIntToString(currentVideoMode.height) + " " + ConvertIntToString(currentVideoMode.bitsPerPixel) + " bit");
                        changeArrowRight1.setPosition(resolution.getPosition().x + resolution.getGlobalBounds().width + 10, resolution.getPosition().y);
                    }
                }
                else if (currentEvent.mouseButton.x > changeArrowRight1.getPosition().x && currentEvent.mouseButton.x < changeArrowRight1.getPosition().x + changeArrowRight1.getGlobalBounds().width &&
                    currentEvent.mouseButton.y > changeArrowRight1.getPosition().y && currentEvent.mouseButton.y < changeArrowRight1.getPosition().y + changeArrowRight1.getGlobalBounds().height)
                {
                    if (currentVideoModePos > 0)
                    {
                        currentVideoModePos--;
                        currentVideoMode = videoModes[currentVideoModePos];
                        resolution.setString(ConvertIntToString(currentVideoMode.width) + " x " + ConvertIntToString(currentVideoMode.height) + " " + ConvertIntToString(currentVideoMode.bitsPerPixel) + " bit");
                        changeArrowRight1.setPosition(resolution.getPosition().x + resolution.getGlobalBounds().width + 10, resolution.getPosition().y);
                    }
                }
                break;
            case sf::Event::KeyPressed:
                if (currentEvent.key.code == sf::Keyboard::Return)
                    mainWindow.create(currentVideoMode, "Snake");
                else if (currentEvent.key.code == sf::Keyboard::Escape)
                    showingOptions = false;
                break;
            default:
                break;
            }
        }
        mainWindow.clear();
        mainWindow.draw(currentResolutionText);
        mainWindow.draw(resolution);
        mainWindow.draw(changeArrowLeft1);
        mainWindow.draw(changeArrowRight1);
        mainWindow.display();
    }
}
