/* Name : Snake Game 
   Author : Subh and prantik
   Date : 21 Dec, 2024
   License : MIT
*/



#include <cstdlib>
#include <ctime>
#ifdef _WIN32
    #include <pdcurses.h> 
    #include <windows.h>  
#else
    #include <ncurses.h>  
    #include <unistd.h>   
#endif

const int WIDTH = 40;
const int HEIGHT = 20;

class Snake {
private:
    int x[100], y[100];
    int length;
    int direction; // 1:up, 2:down, 3:left, 4:right
    
public:
    Snake() {
        x[0] = WIDTH / 2;
        y[0] = HEIGHT / 2;
        length = 1;
        direction = 4;
    }
    
    void changeDir(int input) {
        switch(input) {
            case 'w':
            case KEY_UP:
                if(direction != 2) direction = 1;
                break;
            case 's':
            case KEY_DOWN:
                if(direction != 1) direction = 2;
                break;
            case 'a':
            case KEY_LEFT:
                if(direction != 4) direction = 3;
                break;
            case 'd':
            case KEY_RIGHT:
                if(direction != 3) direction = 4;
                break;
        }
    }
    
    void move() {
        // Move body
        for(int i = length; i > 0; i--) {
            x[i] = x[i-1];
            y[i] = y[i-1];
        }
        
        // Move head
        switch(direction) {
            case 1: y[0]--; break;
            case 2: y[0]++; break;
            case 3: x[0]--; break;
            case 4: x[0]++; break;
        }
    }
    
    bool checkCollision() {
        if(x[0] <= 0 || x[0] >= WIDTH-1 || y[0] <= 0 || y[0] >= HEIGHT-1)
            return true;
            
        for(int i = 1; i < length; i++) {
            if(x[0] == x[i] && y[0] == y[i])
                return true;
        }
        return false;
    }
    
    bool eatFood(int foodX, int foodY) {
        if(x[0] == foodX && y[0] == foodY) {
            length++;
            return true;
        }
        return false;
    }
    
    int getX(int i) { return x[i]; }
    int getY(int i) { return y[i]; }
    int getLength() { return length; }
};

class Game {
private:
    Snake snake;
    int foodX, foodY;
    int score;
    bool gameOver;
    WINDOW *gameWin;
    
    void generateFood() {
        foodX = rand() % (WIDTH-2) + 1;
        foodY = rand() % (HEIGHT-2) + 1;
    }
    
    void draw() {
        werase(gameWin);
        box(gameWin, 0, 0);
        
        
        mvwaddch(gameWin, foodY, foodX, '*');
        
        
        for(int i = 0; i < snake.getLength(); i++) {
            if(i == 0)
                mvwaddch(gameWin, snake.getY(i), snake.getX(i), 'O');
            else
                mvwaddch(gameWin, snake.getY(i), snake.getX(i), 'o');
        }
        
        
        mvwprintw(gameWin, HEIGHT, 1, "Score: %d", score);
        wrefresh(gameWin);
    }
    
public:
    Game() {
        
        initscr();
        noecho();
        curs_set(0);
        keypad(stdscr, TRUE);
        timeout(100);
        
        
        gameWin = newwin(HEIGHT + 1, WIDTH, 0, 0);
        keypad(gameWin, TRUE);
        
        score = 0;
        gameOver = false;
        generateFood();
    }
    
    ~Game() {
        delwin(gameWin);
        endwin();
    }
    
    void play() {
        while(!gameOver) {

            draw();
            
            
            int input = getch();
            if(input == 'x' || input == 'X')
                gameOver = true;
            else if(input != ERR)
                snake.changeDir(input);
            
            
            snake.move();
            
            
            if(snake.checkCollision())
                gameOver = true;
            
            
            if(snake.eatFood(foodX, foodY)) {
                score += 10;
                generateFood();
            }
            
            
            #ifdef _WIN32
                Sleep(100);  
            #else
                usleep(100000);  
            #endif
        }
        
        
        wclear(gameWin);
        mvwprintw(gameWin, HEIGHT/2, WIDTH/2 - 5, "Game Over!");
        mvwprintw(gameWin, HEIGHT/2 + 1, WIDTH/2 - 8, "Final Score: %d", score);
        wrefresh(gameWin);
        timeout(-1);  
        getch();
    }
};

int main() {
    
    srand(static_cast<unsigned>(time(0)));
    
    
    Game game;
    
    
    mvprintw(0, WIDTH + 2, "Snake Game Controls:");
    mvprintw(2, WIDTH + 2, "Arrow Keys or WASD - Move");
    mvprintw(3, WIDTH + 2, "x - Exit Game");
    refresh();
    
    
    game.play();
    
    return 0;
}
