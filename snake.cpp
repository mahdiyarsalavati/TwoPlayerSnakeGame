#include <iostream>
#include <ctime>
#include "clui.h"

#define width 70
#define height 20
#define MAX 200

using namespace std;

struct point
{
    int x;
    int y;
} apple, bomb;

enum handling
{
    east,
    west,
    north,
    south
};
// Linked List!
struct snake
{
    point head;
    point tail[MAX];
    int length;
    handling direction;
    string username;
    int score;
} snake1, snake2;

struct node
{
    string username;
    int score;
    node *next;
} *head = NULL;

bool isOver;
int speed;
int totalUsers;

void sortScoreTable()
{
    for (int i = 0; i < totalUsers - 1; i++)
    {
        node *node = head;
        for (int j = 0; j < totalUsers - i - 1; j++)
        {
            if (node->score < node->next->score)
            {
                swap(node->score, node->next->score);
                swap(node->username, node->next->username);
            }
            node = node->next;
        }
    }
}

void showScoreBoard()
{
    sortScoreTable();
    change_color_rgb(0, 123, 255);
    clear_screen();
    cursor_to_pos(get_window_rows() / 3, get_window_cols() / 3);
    cout << "Score Table:\n";
    reset_color();

    change_color_rgb(0, 255, 0);
    int i = 1;
    node *current = head;
    while (current != NULL)
    {
        cursor_to_pos(get_window_rows() / 3 + i + 1, get_window_cols() / 3);
        cout << i << ": Username: " << current->username << "\t........\t"
             << "Score:" << current->score << endl;
        current = current->next;
        i++;
    }

    cout << endl;
    cursor_forward(get_window_cols() / 3);
    change_color_rgb(3, 111, 252);
    cout << "Please press enter to go back to the menu...";
    reset_color();
    cin.ignore();
    getch();
}

void validatePoints(point &a, point &b, point c, point &d)
{
    if ((a.x == b.x && a.y == b.y) || (a.x == c.x && a.y == c.y) || (c.x == b.x && c.y == b.y) || (a.x == d.x && a.y == d.y) || (b.x == d.x && b.y == d.y) || (c.x == d.x && c.y == d.y) || (a.y == b.y) || (a.y == c.y) || (a.y == d.y) || (b.y == c.y) || (b.y == d.y) || (c.y == d.y))
        while ((a.x == b.x && a.y == b.y) || (a.x == c.x && a.y == c.y) || (c.x == b.x && c.y == b.y) || (a.x == d.x && a.y == d.y) || (b.x == d.x && b.y == d.y) || (c.x == d.x && c.y == d.y) || (a.y == b.y) || (a.y == c.y) || (a.y == d.y) || (b.y == c.y) || (b.y == d.y) || (c.y == d.y))
        {
            a.x = rand() % (width - 20) + 10;
            a.y = rand() % (height - 10) + 5;

            b.x = rand() % (width - 20) + 10;
            b.y = rand() % (height - 10) + 5;

            d.x = rand() % (width - 20) + 10;
            d.y = rand() % (height - 10) + 5;
        }
}

void init()
{
    isOver = false;

    clear_screen();
    change_color_rgb(0, 255, 0);
    cursor_to_pos(get_window_rows() / 3, get_window_cols() / 3 - 10);
    cout << "Choose a speed mode (1: Slow, 2: Medium, 3: Fast): ";
    reset_color();
    int mode;
    cin >> mode;
    switch (mode)
    {
    case 1:
        speed = 200;
        break;
    case 2:
        speed = 100;
        break;
    case 3:
        speed = 50;
        break;
    default:
        speed = 100;
        break;
    }

    change_color_rgb(0, 123, 255);
    cursor_to_pos(get_window_rows() / 3 + 1, get_window_cols() / 3 - 10);
    cout << "Enter username for player 1 (plays with WASD keys): ";
    cin >> snake1.username;

    reset_color();
    cursor_to_pos(get_window_rows() / 3 + 2, get_window_cols() / 3 - 10);
    cout << "Enter username for player 2 (plays with arrow keys): ";
    cin >> snake2.username;
    reset_color();

    snake1.score = 0;
    snake2.score = 0;

    snake1.direction = east;
    snake2.direction = east;

    snake1.head.x = (rand() % (width - 20)) + 10;
    snake1.head.y = (rand() % (height - 10)) + 5;

    snake2.head.x = (rand() % (width - 20)) + 10;
    snake2.head.y = (rand() % (height - 10)) + 5;

    apple.x = (rand() % (width - 20)) + 10;
    apple.y = (rand() % (height - 10)) + 5;

    bomb.x = (rand() % (width - 20)) + 10;
    bomb.y = (rand() % (height - 10)) + 5;

    validatePoints(snake1.head, snake2.head, bomb, apple);

    snake1.length = 3;
    snake2.length = 3;

    for (int i = 0; i < snake1.length; i++)
    {
        snake1.tail[i].x = snake1.head.x - i - 1;
        snake1.tail[i].y = snake1.head.y;
    }

    for (int i = 0; i < snake2.length; i++)
    {
        snake2.tail[i].x = snake2.head.x - i - 1;
        snake2.tail[i].y = snake2.head.y;
    }
}

void scene()
{
    clear_screen();
    change_color_rgb(214, 196, 0);
    cursor_to_pos(get_window_rows() / 3 - 1, get_window_cols() / 3 - 10);
    for (int i = 0; i < width / 2; i++)
    {
        cout << "◆ ";
    }
    cout << endl;
    reset_color();

    for (int i = 0; i < height; i++)
    {
        cursor_to_pos(get_window_rows() / 3 + i, get_window_cols() / 3 - 10);
        for (int j = 0; j < width; j++)
        {

            if (j == 0)
            {
                change_color_rgb(214, 196, 0);
                cout << "◆";
                reset_color();
            }

            else if (i == snake1.head.y && j == snake1.head.x)
            {
                change_color_rgb(54, 5, 250);
                cout << "@";
                reset_color();
            }

            else if (i == snake2.head.y && j == snake2.head.x)
                cout << "@";

            else if (i == apple.y && j == apple.x)
            {
                change_color_rgb(0, 156, 29);
                cout << "";
                reset_color();
            }

            else if (i == bomb.y && j == bomb.x)
            {
                change_color_rgb(255, 0, 51);
                cout << "*";
                reset_color();
            }

            else if (j == width - 1)
            {
                change_color_rgb(214, 196, 0);
                cout << "◆";
                reset_color();
            }
            else
            {
                int temp = 0;
                for (int z = 0; z < snake1.length; z++)
                {
                    if (snake1.tail[z].y == i && snake1.tail[z].x == j)
                    {
                        change_color_rgb(54, 5, 250);
                        cout << "@";
                        temp = 1;
                        reset_color();
                    }
                }

                for (int z = 0; z < snake2.length; z++)
                {
                    if (snake2.tail[z].y == i && snake2.tail[z].x == j)
                    {
                        cout << "@";
                        temp = 1;
                    }
                }

                if (temp == 0)
                    cout << " ";
            }
        }
        cout << endl;
    }
    cursor_forward(get_window_cols() / 3 - 10);
    change_color_rgb(214, 196, 0);
    for (int i = 0; i < width / 2; i++)
    {

        cout << "◆ ";
    }
    cout << endl;
    reset_color();
    cout << endl;
    cursor_forward(get_window_cols() / 3 - 10);
    cout << snake1.score;
    cursor_forward(32);
    cout << "vs";
    cursor_forward(32);
    cout << snake2.score << endl;
    cursor_forward(get_window_cols() / 3 - 10);
    cout << snake1.username;
    cursor_forward(50);
    cout << snake2.username << endl;
    cursor_forward(get_window_cols() / 3 - 10);
    cursor_forward(23);
    cout << "by Mahdiyar Salavati" << endl;
}

void userInput()
{
    if (is_keyboard_hit())
    {
        char input = getch();

        if (input == '\033')
        {
            getch();
            input = getch();

            switch (input)
            {
            case 'A':
                if (snake2.direction != south)
                    snake2.direction = north;
                break;
            case 'B':
                if (snake2.direction != north)
                    snake2.direction = south;
                break;
            case 'C':
                if (snake2.direction != west)
                    snake2.direction = east;
                break;
            case 'D':
                if (snake2.direction != east)
                    snake2.direction = west;
                break;
            }
        }

        else
        {

            if (input == 'a' || input == 'A')
            {
                if (snake1.direction != east)
                    snake1.direction = west;
            }
            else if (input == 's' || input == 'S')
            {
                if (snake1.direction != north)
                    snake1.direction = south;
            }
            else if (input == 'd' || input == 'D')
            {
                if (snake1.direction != west)
                    snake1.direction = east;
            }
            else if (input == 'w' || input == 'W')
            {
                if (snake1.direction != south)
                    snake1.direction = north;
            }
        }
    }
}

void addUser(snake &SNAKE)
{
    node *current = head;

    while (current != NULL)
    {
        if (current->username == SNAKE.username)
        {
            current->score += SNAKE.score;
            return;
        }

        if (current->next == NULL)
        {
            current->next = new node{SNAKE.username, SNAKE.score, NULL};
            totalUsers++;
            return;
        }
        current = current->next;
    }

    if (head == NULL)
    {
        head = new node{SNAKE.username, SNAKE.score, NULL};
        totalUsers++;
    }
}

void replaceApple(point &APPLE, point BOMB, point SNAKE1HEAD, point SNAKE2HEAD)
{
    if ((APPLE.x == BOMB.x && APPLE.y == BOMB.y) || (APPLE.x == SNAKE1HEAD.x && APPLE.y == SNAKE1HEAD.y) || (APPLE.x == SNAKE2HEAD.x && APPLE.y == SNAKE2HEAD.y))
        while ((APPLE.x == BOMB.x && APPLE.y == BOMB.y) || (APPLE.x == SNAKE1HEAD.x && APPLE.y == SNAKE1HEAD.y) || (APPLE.x == SNAKE2HEAD.x && APPLE.y == SNAKE2HEAD.y))
        {
            APPLE.x = rand() % (width - 20) + 10;
            APPLE.y = rand() % (height - 10) + 5;
        }
}

void updateSnakePos(snake &snake)
{
    for (int i = snake.length - 1; i > 0; i--)
    {
        snake.tail[i].x = snake.tail[i - 1].x;
        snake.tail[i].y = snake.tail[i - 1].y;
    }
    snake.tail[0].x = snake.head.x;
    snake.tail[0].y = snake.head.y;
}

void process()
{
    updateSnakePos(snake1);
    updateSnakePos(snake2);

    switch (snake1.direction)
    {
    case west:
        snake1.head.x--;
        break;
    case east:
        snake1.head.x++;
        break;
    case north:
        snake1.head.y--;
        break;
    case south:
        snake1.head.y++;
        break;
    }

    switch (snake2.direction)
    {
    case west:
        snake2.head.x--;
        break;
    case east:
        snake2.head.x++;
        break;
    case north:
        snake2.head.y--;
        break;
    case south:
        snake2.head.y++;
        break;
    }

    if (snake1.head.x >= width - 1)
    {
        isOver = true;
    }
    else if (snake1.head.x < 0)
    {
        isOver = true;
    }
    if (snake1.head.y >= height)
    {
        isOver = true;
    }
    else if (snake1.head.y < 0)
    {
        isOver = true;
    }

    if (snake2.head.x >= width - 1)
    {
        isOver = true;
    }
    else if (snake2.head.x < 0)
    {
        isOver = true;
    }
    if (snake2.head.y >= height)
    {
        isOver = true;
    }
    else if (snake2.head.y < 0)
    {
        isOver = true;
    }

    if (snake1.head.x == snake2.head.x && snake1.head.y == snake2.head.y)
    {
        isOver = true;
    }

    for (int i = 3; i < snake1.length; i++)
    {
        if (snake1.head.x == snake1.tail[i].x && snake1.head.y == snake1.tail[i].y)
            isOver = true;
    }

    for (int i = 3; i < snake2.length; i++)
    {
        if (snake2.head.x == snake2.tail[i].x && snake2.head.y == snake2.tail[i].y)
            isOver = true;
    }

    for (int i = 0; i < snake2.length; i++)
    {
        if (snake1.head.x == snake2.tail[i].x && snake1.head.y == snake2.tail[i].y)
            isOver = true;
    }

    for (int i = 0; i < snake1.length; i++)
    {
        if (snake2.head.x == snake1.tail[i].x && snake2.head.y == snake1.tail[i].y)
            isOver = true;
    }

    if (snake1.head.x == bomb.x && snake1.head.y == bomb.y)
    {
        isOver = true;
    }

    if (snake2.head.x == bomb.x && snake2.head.y == bomb.y)
    {
        isOver = true;
    }

    if (snake1.head.x == apple.x && snake1.head.y == apple.y)
    {
        snake1.score++;
        snake1.length++;
        apple.x = (rand() % (width - 20)) + 10;
        apple.y = (rand() % (height - 10)) + 5;
        replaceApple(apple, bomb, snake1.head, snake2.head);
        if (speed > 10)
            speed -= 1;
    }

    if (snake2.head.x == apple.x && snake2.head.y == apple.y)
    {
        snake2.score++;
        snake2.length++;
        apple.x = (rand() % (width - 20)) + 10;
        apple.y = (rand() % (height - 10)) + 5;
        replaceApple(apple, bomb, snake1.head, snake2.head);
        if (speed > 10)
            speed -= 1;
    }

    if (isOver)
    {
        addUser(snake1);
        addUser(snake2);
    }
}

int showMenu()
{
    change_color_rgb(0, 255, 0);
    int menu_input;
    clear_screen();

    if (totalUsers == 0)
    {

        cursor_forward(get_window_cols() / 3);
        cout << "                  _             \n";
        cursor_forward(get_window_cols() / 3);
        cout << "  ___ _ __   __ _| | _____ \n";
        cursor_forward(get_window_cols() / 3);
        cout << " / __| '_ \\ / _` | |/ / _ \\\n";
        cursor_forward(get_window_cols() / 3);
        cout << " \\__ \\ | | | (_| |   <  __/\n";
        cursor_forward(get_window_cols() / 3);
        cout << " |___/_| |_|\\__,_|_|\\_\\___|\n";
        cout << endl;
        cursor_forward(get_window_cols() / 3);
        cout << " Mahdiyar Salavati -- 402243080 \n";
        cursor_to_pos(get_window_rows() / 3, get_window_cols() / 3);
        cout << "Welcome!\n";
    }
    cursor_to_pos(get_window_rows() / 3 + 1, get_window_cols() / 3);
    if (totalUsers == 0)
    {
        cout << "1. Start a new game\n";
    }
    else
    {
        cout << "1. Play again\n";
    }
    cursor_to_pos(get_window_rows() / 3 + 2, get_window_cols() / 3);
    cout << "2. Score Board\n";
    cursor_to_pos(get_window_rows() / 3 + 3, get_window_cols() / 3);
    cout << "3. Edit username\n";
    cursor_to_pos(get_window_rows() / 3 + 4, get_window_cols() / 3);
    cout << "4. Exit the game\n";
    reset_color();
    change_color_rgb(0, 123, 255);
    cursor_to_pos(get_window_rows() / 3 + 5, get_window_cols() / 3);
    cout << "Please choose an option: ";
    cin >> menu_input;
    reset_color();
    cout << endl;
    return menu_input;
}

string winnerFinder()
{
    if (snake1.score > snake2.score)
    {
        return snake1.username;
    }
    else if (snake2.score > snake1.score)
    {
        return snake2.username;
    }
    else
    {
        return "No one";
    }
}

void gameOver()
{
    clear_screen();
    play_beep();
    cursor_to_pos(get_window_rows() / 3, get_window_cols() / 3);
    change_background_color(BG_RED);
    cout << "GAME OVER\n";
    reset_color();
    change_background_color(BG_GREEN);
    cursor_to_pos(get_window_rows() / 3 + 1, get_window_cols() / 3);
    cout << winnerFinder() << " wins!";
    reset_color();
    cout << endl;
    cursor_forward(get_window_cols() / 3);
    change_color_rgb(3, 111, 252);
    cout << "Please press enter to go back to the menu...";
    cout << endl;
    reset_color();
    getch();
}

void noPlayersAlert()
{
    cursor_forward(get_window_cols() / 3);
    cout << "There are no players" << endl;
    cursor_forward(get_window_cols() / 3);
    change_color_rgb(3, 111, 252);
    cout << "Please press enter to go back to the menu...";
    reset_color();
    cin.ignore();
    getch();
}

int main()
{
    srand(time(0));
    int input;

    while (true)
    {
        input = showMenu();

        if (input == 4)
        {
            break;
        }
        if (input == 3)
        {
            if (totalUsers == 0)
            {
                noPlayersAlert();
            }
            else
            {
                node *current = head;
                int i = 0;
                while (current != NULL)
                {
                    if (current->username == snake1.username || current->username == snake2.username)
                    {
                        cursor_to_pos(2 * get_window_rows() / 3 + i, get_window_cols() / 3);
                        cout << "Enter new username for " << current->username << ": ";
                        cin >> current->username;
                        i++;
                    }
                    current = current->next;
                }
            }
        }

        if (input == 2)
        {
            if (totalUsers == 0)
            {
                noPlayersAlert();
            }
            else
            {
                showScoreBoard();
            }
        }

        if (input == 1)
        {
            init();
            while (!isOver)
            {
                scene();
                userInput();
                process();
                delay(speed);
            }
            gameOver();
        }
    }

    return 0;
}
