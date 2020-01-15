#include <iostream>
#include "windows.h"
#include <thread>
#include <cmath>

const int MOVE_X = 1;
const int MOVE_Y = 2;

void navigateCursor(COORD *coord)
{
    COORD coordCursor;
    coordCursor.X = coord->X;
    coordCursor.Y = coord->Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordCursor);
}

void navigateCursor(DWORD MOVE, COORD *coord, int modifier)
{
    if (MOVE == MOVE_X)
        coord->X = coord->X + modifier;
    if (MOVE == MOVE_Y)
        coord->Y = coord->Y + modifier;

    COORD coordCursor;
    coordCursor.X = coord->X;
    coordCursor.Y = coord->Y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordCursor);
}

class TickTac
{

public:
    static const int __LEN = 3;
    const DWORD PLAYER_X = 1;
    const DWORD PLAYER_O = 2;

private:
    DWORD currentPlayer;
    int scoreTable[__LEN][__LEN];
    bool endGame;

public:
    TickTac()
    {

        for (int i = 0; i < __LEN; i++)
            for (int j = 0; j < __LEN; j++)
                this->scoreTable[i][j] = 0;

        this->endGame = false;
        this->currentPlayer = PLAYER_X;
        createScreen();
    }

private:
    void createScreen()
    {
        for (int i = 0; i < TickTac::__LEN; i++)
        {

            for (int j = 0; j < TickTac::__LEN; j++)
            {
                if (j < TickTac::__LEN - 1)
                    std::cout << " |";
                else
                    std::cout << " ";
            }

            std::cout << std::endl;

            if (i < TickTac::__LEN - 1)
            {
                for (int j = 0; j < TickTac::__LEN; j++)
                {
                    std::cout << "-";
                    if (j < TickTac::__LEN - 1)
                        std::cout << "|";
                }
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    void validateSecondaryDiagonal()
    {
        int j = __LEN - 1;
        int player = 0;
        int isSamePlayer = 0;

        for (int i = 0; i < __LEN; i++)
        {
            player = this->scoreTable[i][j];

            if (j == __LEN - 1)
            {
                isSamePlayer++;
                j--;
                continue;
            }
            else if (player != 0 && this->scoreTable[i - 1][j + 1] == player)
            {
                isSamePlayer++;
            }

            j--;
        }

        if (isSamePlayer == (__LEN))
            this->endGame = true;
    }


public:
    DWORD changeCurrentPlayer()
    {
        if (this->currentPlayer == PLAYER_X)
        {
            this->currentPlayer = PLAYER_O;
        }
        else
        {
            this->currentPlayer = PLAYER_X;
        }

        return this->currentPlayer;
    }

    void setCurrentPlayer(DWORD currentPlayer)
    {
        this->currentPlayer = currentPlayer;
    }

    DWORD getCurrentPlayer()
    {
        return this->currentPlayer;
    }

    void setScore(int x, int y, DWORD player)
    {
        this->scoreTable[y][x] = player;
    }

    bool validadeScore()
    {
        validateSecondaryDiagonal();
       // validateLines();

        return this->endGame;
    }

public:
    std::string __logScoreTable()
    {
        std::string table = "[\n";
        for (int i = 0; i < __LEN; i++)
        {
            for (int j = 0; j < __LEN; j++)
            {
                table += "" + std::to_string(this->scoreTable[i][j]) + ", ";
            }

            table += "\n";
        }

        table += "]";

        return table;
    }
};

bool keyPressed(int vKey)
{
    return (GetAsyncKeyState(vKey) & 0x8001) == 0x8001;
}

void setLogger(COORD *cursor, TickTac *reference)
{
    navigateCursor(new COORD{(SHORT)0, (SHORT)(TickTac::__LEN * 2 + 4)});
    std::cout << "Log system: " << std::endl;
    std::cout << "POSITION: X : " << cursor->X / 2 << std::endl;
    std::cout << "POSITION: Y : " << cursor->Y / 2 << std::endl;
    std::cout << "ENDGAME? : " << reference->validadeScore();
    std::cout << std::endl;
    std::cout << reference->__logScoreTable();

    navigateCursor(cursor);
}

void GameControls(TickTac *game, COORD cursor_pos)
{
    COORD *cursor = &cursor_pos;

    while (true)
    {
        if (keyPressed(VK_ESCAPE))
        {
            system("cls");
            break;
        }

        if (keyPressed(VK_LEFT) && (cursor->X / 2) >= 1)
            navigateCursor(MOVE_X, cursor, -2);

        if (keyPressed(VK_RIGHT) && (cursor->X / 2) < game->__LEN - 1)
            navigateCursor(MOVE_X, cursor, 2);

        if (keyPressed(VK_UP) && (cursor->Y / 2) >= 1)
            navigateCursor(MOVE_Y, cursor, -2);

        if (keyPressed(VK_DOWN) && (cursor->Y / 2) < game->__LEN - 1)
            navigateCursor(MOVE_Y, cursor, 2);

        if (keyPressed(VK_SPACE))
        {

            DWORD currentPlayer = game->getCurrentPlayer();

            game->setScore(cursor->X / 2, cursor->Y / 2, currentPlayer);

            char characterPlayer;
            if (currentPlayer == game->PLAYER_X)
                characterPlayer = 'X';
            else
                characterPlayer = 'O';

            if ((cursor->X / 2) < game->__LEN - 1)
            {
                std::cout << characterPlayer;
                navigateCursor(MOVE_X, cursor, 2);
            }
            else if ((cursor->Y / 2) < game->__LEN - 1)
            {
                std::cout << characterPlayer;
                cursor->X = 0;
                cursor->Y += 2;
                navigateCursor(cursor);
            }
            else
            {
                std::cout << characterPlayer;
                cursor->X = 0;
                cursor->Y = 0;
                navigateCursor(cursor);
            }

            game->changeCurrentPlayer();
            setLogger(cursor, game);

            bool endGame = game->validadeScore();
            if (endGame)
            {
                navigateCursor(new COORD{(SHORT)0, (SHORT)TickTac::__LEN * 2 + 15});
                std::cout << "Final de Jogo! O vencedor foi: player " << characterPlayer << std::endl;
                break;
            }
        }
    }
}

main()
{
    system("cls");

    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    COORD *p = &coord;

    TickTac *game = new TickTac();

    navigateCursor(new COORD{(SHORT)0, (SHORT)TickTac::__LEN * 2 + 2});
    std::cout << "Press esc to exit the game...";
    navigateCursor(p);

    std::thread controls(GameControls, game, coord);

    controls.join();
}
