#include <SDL.h>
#include <stdio.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <iostream>
#include <typeinfo>
#include <SDL2/SDL_ttf.h>

#include <sstream>


const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const int RECT_WIDTH = 50;
const int RECT_HEIGHT = 50;
const int APPLE_WIDTH = 25;
const int APPLE_HEIGHT = 25;


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

auto start_manzana = std::chrono::system_clock::now();
bool flag = true;


struct SNAKE {
    SDL_Rect cabeza; // Rectangle's position and dimensions
    SDL_Rect cuerpo[100];
    int longitud = 1;

    enum Direction {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    Direction LastDirection;

    SNAKE() {
        cabeza.x = (SCREEN_WIDTH - RECT_WIDTH) / 2;
        cabeza.y = (SCREEN_HEIGHT - RECT_HEIGHT) / 2;
        cabeza.w = RECT_WIDTH;
        cabeza.h = RECT_HEIGHT;
        cuerpo[0] = cabeza;
    }

    void addCuerpo() {
        //std::cout<<"--->AddCuerpo"<< std::endl;

        // Update the first body segment to follow the head
        SDL_Rect newSegment;
        switch (LastDirection) {
            case LEFT:
                //std::cout << "Last Direction: "<< LastDirection << std::endl;

                newSegment.x=  cuerpo[longitud-1].x-RECT_WIDTH; // Move left
                newSegment.y =  cuerpo[longitud-1].y; // Move up

                break;
            case RIGHT:
                //std::cout << "Last Direction: "<< LastDirection << std::endl;

                newSegment.x =  cuerpo[longitud-1].x+RECT_WIDTH; // Move right
                newSegment.y =  cuerpo[longitud-1].y; // Move up

                break;
            case UP:
                //std::cout << "Last Direction: "<< LastDirection << std::endl;

                newSegment.x =  cuerpo[longitud-1].x; // Move right
                newSegment.y =  cuerpo[longitud-1].y+RECT_HEIGHT; // Move up

                break;
            case DOWN:
                //std::cout << "Last Direction: "<< LastDirection << std::endl;

                newSegment.x =  cuerpo[longitud-1].x; // Move right
                newSegment.y =  cuerpo[longitud-1].y-RECT_HEIGHT; // Move down

                break;
        }
        //std::cout << "Posicion de el nuevo segmento: ("<<newSegment.x<<","<<newSegment.y<<")"<<std::endl;
        //std::cout << "Posicion de el anterior segmento: ("<<cuerpo[longitud-1].x<<","<<cuerpo[longitud-1].y<<")"<<std::endl;

                  //newSegment.x = cuerpo[longitud - 1].x; // Position it at the last segment's position
        //newSegment.y = cuerpo[longitud - 1].y;
        newSegment.w = RECT_WIDTH;
        newSegment.h = RECT_HEIGHT;

        cuerpo[longitud] = newSegment;
        longitud++;

    }

    void ActualizarCuerpos(){
        //std::cout<<"--->ActualizarCuerpos"<< std::endl;
        for(size_t i=longitud-1; i > 0; i--){
            //std::cout << "Posicion de el nuevo segmento "<<i<<" : ("<<cuerpo[i].x<<","<<cuerpo[i].y<<")";
            //std::cout << " --> Posicion anterior de este segmento ("<<cuerpo[i-1].x<<","<<cuerpo[i-1].y<<")"<<std::endl;

            cuerpo[i].x = cuerpo[i-1].x;
            cuerpo[i].y = cuerpo[i-1].y;
        }


        //for (size_t i = 0; i < longitud; i ++){
            //std::cout << "Posicion de el segmento "<<i<<" : ("<<cuerpo[i].x<<","<<cuerpo[i].y<<")"<<std::endl;
        //}
        //std::cout<<"--->ActualizarCuerpos-------------fin"<< std::endl;

    }
    void continuar() {
        //CODIGO PARA MANTENER MOVIMIENTO
        switch (LastDirection) {
            case LEFT:
                cuerpo[0].x -= RECT_WIDTH; // Move left
                break;
            case RIGHT:
                cuerpo[0].x += RECT_WIDTH; // Move right
                break;
            case UP:
                cuerpo[0].y -= RECT_HEIGHT; // Move up
                break;
            case DOWN:
                cuerpo[0].y += RECT_HEIGHT; // Move down
                break;
        }
    }
    void mover_keyword(SDL_Event &e) {
        //CODIGO PARA INICIAR MOVIMIENTO
        if (e.type == SDL_QUIT) {
            // User clicks the window close button
            // Quit the application
            SDL_Quit();

        }else if (
                    e.type == SDL_KEYDOWN) {
                // Handle key presses to move the rectangle
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        //cabeza.x -= RECT_WIDTH; // Move left
                        if (LastDirection != RIGHT){
                            LastDirection = LEFT;
                        }

                        break;
                    case SDLK_RIGHT:
                        //cabeza.x += RECT_WIDTH; // Move right
                        if (LastDirection != LEFT) {

                            LastDirection = RIGHT;
                        }
                        break;
                    case SDLK_UP:
                        //cabeza.y -= RECT_HEIGHT; // Move up
                        if (LastDirection != DOWN) {

                            LastDirection = UP;
                        }
                        break;
                    case SDLK_DOWN:
                        //cabeza.y += RECT_HEIGHT; // Move down
                        if (LastDirection != UP) {

                            LastDirection = DOWN;
                        }
                        break;
                }
            }
            }
        };

bool colision_rect(SDL_Rect &rect1, SDL_Rect &rect2) {
        // Check for horizontal collision
        bool xCollision = (rect1.x < rect2.x + rect2.w) && (rect1.x + rect1.w > rect2.x);
        // Check for vertical collision
        bool yCollision = (rect1.y < rect2.y + rect2.h) && (rect1.y + rect1.h > rect2.y);

        return xCollision && yCollision;
    }

struct MANZANA{
    SDL_Rect cuerpo;

    MANZANA(){
        cuerpo.x = rand() % (SCREEN_WIDTH - APPLE_WIDTH);
        cuerpo.y = rand() % (SCREEN_HEIGHT - APPLE_HEIGHT);
        cuerpo.w = APPLE_WIDTH;
        cuerpo.h = APPLE_HEIGHT;

    }
    bool manzana_in_snake(SNAKE &snake){
        bool flag_manzana = false;
        for(size_t i = 1; i < snake.longitud; i++){
            if(colision_rect(snake.cuerpo[i], cuerpo)){
                flag_manzana = true;
            }
        }
        return flag_manzana;
    }
    void randomizePosition(SNAKE &snake) {
        do {
            cuerpo.x = rand() % (SCREEN_WIDTH - APPLE_WIDTH);
            cuerpo.y = rand() % (SCREEN_HEIGHT - APPLE_HEIGHT);

        } while (manzana_in_snake(snake));

    }
};
    bool init() {


        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return false;
        }
        if (TTF_Init() == -1) {
            // Handle the initialization error
            printf("SDL_ttf could not initialize! SDL_Error: %s\n", SDL_GetError());

        }

        gWindow = SDL_CreateWindow("Moving Rectangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        if (gRenderer == NULL) {
            printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        // Initialize the rectangle's position


        return true;
    }
    void close() {
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        SDL_Quit();
    }

    int main(int argc, char *args[]) {
        SDL_Rect prueba;
        prueba.x = 0;
        prueba.y = 0;
        prueba.w = 350;
        prueba.h = 350;

        SNAKE snake;
        MANZANA manzana;

        if (!init()) {
            printf("Failed to initialize!\n");
            return 1;
        }

        bool quit = false;
        SDL_Event e;
        std::ostringstream oss;


        while (!quit) {

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                snake.mover_keyword(e);
            }
            TTF_Font *font = TTF_OpenFont("/home/diego/CLionProjects/Learning/OpenSans-Light.ttf", 12);
            if(font == nullptr){
                std::cout<<"No se pudo cargar correctamente la fuente"<<std::endl;
                return 1;
            }

            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Set background color to black
            SDL_RenderClear(gRenderer);
            oss.str(" ");
            oss.clear();
            oss << snake.longitud;
            SDL_Color textColor = {255, 255, 255};

            SDL_Surface *textSurface = TTF_RenderText_Solid(font, ("SCORE: " + oss.str()).c_str(), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

            // Render the text texture on the screen
            SDL_Rect textRect;
            textRect.x = 10;
            textRect.y = 10;
            textRect.w = textSurface->w;
            textRect.h = textSurface->h;

            SDL_RenderCopy(gRenderer, textTexture, nullptr, &textRect);
            snake.ActualizarCuerpos();

            snake.continuar();// Move the snake based on the LastDirection


            //COLOREAR EL CUERPO
            for (int i = 0; i < snake.longitud; i++) {
                SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Set rectangle color to red
                SDL_RenderFillRect(gRenderer, &(snake.cuerpo[i]));
            }
            //COLOREAR LA MANZANA
            SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); // Set rectangle color to red
            SDL_RenderFillRect(gRenderer, &(manzana.cuerpo));



            if (colision_rect((snake.cuerpo[0]), (manzana.cuerpo))) {
                //std::cout << "Colision serpiente-manzana" << std::endl;

                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255); // Set rectangle color to red
                SDL_RenderFillRect(gRenderer, &(manzana.cuerpo));



                if (flag) {
                    start_manzana = std::chrono::system_clock::now(); // Reset the timer
                    manzana.randomizePosition(snake);
                    flag = false;
                }
                snake.addCuerpo();
                //std::cout << "Cuerpo añadido" << std::endl;

            }
            SDL_RenderPresent(gRenderer); // Update the screen

            for(int i= 1; i < snake.longitud; i++) {
                //std::cout << "Colision serpiente-serpiente" << std::endl;

                //std::cout <<"i: "<< i << std::endl;
                //std::cout << "longitud serpiente "<<snake.longitud << std::endl;
                //std::cout << "Comparamos cuerpo "<< 0 << " con cuerpo " << i << std::endl;
                //std::cout << "Cuerpo  "<< 0 << " ("<<snake.cuerpo[0].x<<";"<<snake.cuerpo[0].y<<")" << std::endl;
                //std::cout << "Cuerpo  "<< i << " ("<<snake.cuerpo[i].x<<";"<<snake.cuerpo[i].y<<")" << std::endl;

                if (colision_rect((snake.cuerpo[0]), (snake.cuerpo[i]))) {
                    //std::cout << "----------------------------------------HAN COLISIONADO-----------------------------------------------------------------"<< std::endl;

                    TTF_Font *font = TTF_OpenFont("/home/diego/CLionProjects/Learning/OpenSans-Light.ttf", 12);

                    if (font == nullptr) {
                        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
                        // Tratar el error apropiadamente (salir del juego o usar una fuente predeterminada).
                    } else {
                        SDL_Color textColor = {255, 255, 255};
                        SDL_Surface* textSurface = TTF_RenderText_Solid(font, "MUERTE", textColor);
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
                        SDL_FreeSurface(textSurface);

                        // Render the text texture on the screen
                        SDL_Rect textRect;
                        textRect.x = SCREEN_WIDTH / 2 - textSurface->w / 2;
                        textRect.y = SCREEN_HEIGHT / 2 - textSurface->h / 2;
                        textRect.w = textSurface->w;
                        textRect.h = textSurface->h;
                        SDL_RenderCopy(gRenderer, textTexture, nullptr, &textRect);

                        SDL_RenderPresent(gRenderer); // Update the screen
                        SDL_Delay(3000); // Delay to control the display time

                        TTF_CloseFont(font); // Close the font
                        SDL_DestroyTexture(textTexture);

                        close();
                        return 1;

                    }
                }else{
                    //std::cout << "NO HAY COLISION"<< std::endl;
                }

            }
            auto end_manzana = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end_manzana - start_manzana;
            if (elapsed_seconds.count() > 1) {
                flag = true; // Reset the flag after one second
            }


            SDL_FreeSurface(textSurface);

            SDL_DestroyTexture(textTexture);
            //std::cout << "frame restart\n";
            SDL_Delay(250); // Delay to control the speed of movement

            // You can add additional logic here
        }

        close();
        return 0;
    }
