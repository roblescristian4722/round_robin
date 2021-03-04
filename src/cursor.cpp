#include "../define/cursor.h"

Frame::Frame(int x, int y, int w, int h, char color, bool foreground,
               std::string simbol): heightFrame(h), widthFrame(w), xFrame(x),
                                    yFrame(y), foreground(foreground),
                                    contentColor(BLANCO), simbol("*"),
                                    color(color)
{
    drawFrame(x, y, w, h, color, foreground, simbol);
    this->xPos = x + 1;
    this->yPos = y + 1;
}

Frame::~Frame()
{}

void Cursor::gotoxy(int x,int y)
{ printf("%c[%d;%df",0x1B,y,x); }

void Cursor::clrscr()
{ system(CLRSCR); }

void Cursor::hideCursor()
{ printf("\e[?25l"); }

void Cursor::showCursor()
{ printf("\e[?25h"); }

std::string Cursor::colorText(unsigned char color, std::string msg,
                              bool foreground)
{
    std::string aux;
    // foreground ANSI: "\033[0(fondo negro);3{color}m{msg}\033[0m"
    if (foreground) {
        aux = "\033[0;3";
        aux += color;
    }
    // background ANSI: "\033[4{color fondo}m\033[1;30(color foreground negro)m
    //              {msg}\033[0m"
    else {
        aux = "\033[4";
        aux += color;
        aux += "m\033[1;30";
    }
    aux += 'm';
    aux += msg;
    aux += "\033[0m";
    return aux;
}

void Cursor::rmLine(unsigned int n)
{
    while (n--) {
        // Borra la línea actual
        std::cout << "\033[2K";
        // Regresa a la línea anterior (sube 1 línea)
        std::cout << "\033[1A";
    }
}

void Cursor::drawXLine(int x, int y, int w, char color, bool foreground,
               std::string caracter)
{
    int i;
    for (i = 0; i < w; ++i) {
        gotoxy(x + i, y);
        std::cout << colorText(color, caracter);
    }
    gotoxy(i, y);
}

void Cursor::drawYLine(int x, int y, int h, char color, bool foreground,
               std::string caracter)
{
    int i;
    for (i = 0; i < h; ++i) {
        gotoxy(x, y + i);
        std::cout << colorText(color, caracter);
    }
    gotoxy(x, i);
}

void Frame::drawFrame(int x, int y, int w, int h, char color, bool foreground,
               std::string caracter)
{
    drawXLine(x, y, w, color, foreground, caracter);
    drawYLine(x, y, h, color, foreground, caracter);
    drawYLine(x + w - 1, y, h, color, foreground, caracter);
    drawXLine(x, y + h - 1, w, color, foreground, caracter);
    gotoxy(x + 1, y + 1);
}

void Frame::print(std::string msj, char color, bool newl)
{
    std::string aux;
    unsigned int times = msj.size() / (this->widthFrame - 2);
    drawFrame(this->xFrame, this->yFrame, this->widthFrame,
              times + 3, this->color, this->foreground,
              this->simbol);
    for (size_t i = 0; i < msj.size(); ++i) {
        aux = msj[i];
        std::cout << colorText(color, aux, this->foreground);

        if (++this->xPos == this->xFrame + this->widthFrame - 1) {
            this->xPos = xFrame + 1;
            ++this->yPos;
            gotoxy(this->xPos, this->yPos);
        }
    }
}
