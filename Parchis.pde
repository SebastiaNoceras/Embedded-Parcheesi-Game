import processing.serial.*;

PImage imgEstrella;

//POSICIONNES INICIALES DE LAS FICHAS (en píxeles) 
int[] fichaX = {85, 85, 40, 40, 935, 935, 895, 895};
int[] fichaY = {100, 145, 100, 145, 640, 595, 640, 595};
int[] fichaXinicial = {85, 85, 40, 40, 935, 935, 895, 895};
int[] fichaYinicial = {100, 145, 100, 145, 640, 595, 640, 595};
//Posiciones de las fichas (en casillas)
int[] casilla = {0, 0, 0, 0, 0, 0, 0, 0};

int height = 90;
int width = 45;

int counter;
int counterX = 40;
int counterY = 220;

boolean estrella = false;
boolean play = false;

Serial s;

void setup()
{
  // Create window
  size(1000, 800);

  // Stop automatic draw
  noLoop();

  imgEstrella = loadImage("estrella.jpg");


  // Serial
  s = new Serial(this, "COM7", 115200);
  s.buffer(1); // Buffer 1 bytes before calling serialEvent()
  
}

void draw()
{
  dibujarTablero();
  dibujarFichas();
  dibujarEstrella();
  dibujarCronometro();
  dibujarPlay();
  
}

void keyPressed()
{
    
  switch (key)
  {
  case ' ' :
    s.write(5);
    play = true;
    break;
  case 'r':
    s.write(0);
    init();
    redraw();
    
    break;
  }
}

void serialEvent(Serial s)
{
  int valor = 0;
  
  valor = s.read();
  
  if (valor < 9){
  centrarFichas(valor);
  actualizarFichas(valor);
  descentrarFichas(valor);
 
 
  redraw();
 
  mostrarEstrella(valor);
  } else if (valor > 9 && valor < 18) matarFicha(valor);
   else if (valor > 19 && valor < 56) actualizarCronometro(valor); 
   
  
  redraw();

  //println("snakeX: " + snakeX + ", snakeY: " + snakeY);
  
} 
//--------------------------------------------------------------------------
void dibujarTablero() {
  int x = 200;
  int y = 80;

  // Background color
  background(255, 255, 128); 
  
  // Select line and fill color
  stroke(10, 200, 10); //Sides of the figures
  strokeWeight(1);
  fill(255, 255, 255);
  
  for (int i = 0; i <= 11; i++) {
    rect(x, y, width, height);
    x = x + width;
  }
  rect(x, y, width*2, height);
  y = y + height;
  for (int i = 0; i <= 8; i++) {
    rect(x, y, height, width);
    y = y + width;
  }
  rect(x, y, width*2, height);
  x = x - width;
  for (int i = 0; i <= 11; i++) {
    rect(x, y, width, height);
    x = x - width;
  }
  rect(x, y, width*2, height);
  y = y - width;
  for (int i = 0; i <= 9; i++) {
    rect(x, y, height, width);
    y = y - width;
  }
  rect(x, y, width*2, height);
  fill(128, 128, 255); //blue
  rect(x + width * 7, y + height, width, height);
  arc(x + width * 7 + 25, y + 2 * height + 1, 130, 130, 0, PI);
  
  fill(255, 128, 128);//red
  rect(x + width * 7, y + height * 4 + width, width, height);
  arc(x + width * 7 + 25, y + 4 * height + width, 130, 130, PI, TWO_PI);
  
  fill(128, 128, 255);
  ellipse(62, 122 , 110, 110); 
  fill(255, 128, 128);
  ellipse(915, 618, 110, 110); 
  
  rect(600, 10, 130, 20);
  fill(255);
  textSize(10);
  text("Pulse 'r' para reiniciar", 608, 22); 
  
}
//--------------------------------------------------------------------------------------
void centrarFichas(int ficha)
{
  if (ficha >= 0 && ficha < 4){
    for (int i = 0; i < 4; i++){//Para cuando se quiten los bloqueos
      if (casilla[ficha] == casilla[i] && i != ficha && casilla[ficha] != 0){
        if ((casilla[ficha] > 0 && casilla[ficha] < 11) || (casilla[ficha] > 22 && casilla[ficha] < 33)){
          if (ficha < i){
            fichaX[ficha] = fichaX[ficha] - 15;
            fichaX[i] = fichaX[i] + 15;
          }else{
            fichaX[ficha] = fichaX[ficha] + 15;
            fichaX[i] = fichaX[i] - 15;
          }
        }else if ((casilla[ficha] > 10 && casilla[ficha] < 23) || (casilla[ficha] > 32 && casilla[ficha] < 40) || (casilla[ficha] == 40)){
          if (ficha < i){
            fichaY[ficha] = fichaY[ficha] - 15;
            fichaY[i] = fichaY[i] + 15;
          }else{
            fichaY[ficha] = fichaY[ficha] + 15;
            fichaY[i] = fichaY[i] - 15;
          }
        }
      }
    }
  }else{
    for (int i = 4; i < 8; i++){
      if (casilla[ficha] == casilla[i] && i != ficha && casilla[ficha] != 0){
        if ((casilla[ficha] > 0 && casilla[ficha] < 11) || (casilla[ficha] > 22 && casilla[ficha] < 33)){
          if (ficha < i){
            fichaX[ficha] = fichaX[ficha] - 15;
            fichaX[i] = fichaX[i] + 15;
          }else{
            fichaX[ficha] = fichaX[ficha] + 15;
            fichaX[i] = fichaX[i] - 15;
          }
        }else if ((casilla[ficha] > 10 && casilla[ficha] < 23) || (casilla[ficha] > 32 && casilla[ficha] < 40) || (casilla[ficha] == 40)){
          if (ficha < i){
            fichaY[ficha] = fichaY[ficha] - 15;
            fichaY[i] = fichaY[i] + 15;
          }else{
            fichaY[ficha] = fichaY[ficha] + 15;
            fichaY[i] = fichaY[i] - 15;
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------
void actualizarFichas(int ficha)
{
  if (ficha < 4){//FICHAS AZULES
    if (casilla[ficha] == 0){
      fichaX[ficha] = 200;
      fichaY[ficha] = 127;
    }
    else if (casilla[ficha] == 1) fichaY[ficha] = fichaY[ficha] + 63;
    else if (casilla[ficha] > 1 && casilla[ficha] < 10 ) fichaY[ficha] = fichaY[ficha] + 45;
    else if (casilla[ficha] == 10) fichaY[ficha] = fichaY[ficha] + 65;
    else if (casilla[ficha] == 11) fichaX[ficha] = fichaX[ficha] + 68;
    else if (casilla[ficha] > 11 && casilla[ficha] < 22) fichaX[ficha] = fichaX[ficha] + 45;
    else if (casilla[ficha] == 22) fichaX[ficha] = fichaX[ficha] + 65;
    else if (casilla[ficha] == 23) fichaY[ficha] = fichaY[ficha] - 63;
    else if (casilla[ficha] > 23 && casilla[ficha] < 32) fichaY[ficha] = fichaY[ficha] - 45;
    else if (casilla[ficha] == 32) fichaY[ficha] = fichaY[ficha] - 65;
    else if (casilla[ficha] == 33) fichaX[ficha] = fichaX[ficha] - 68;
    else if (casilla[ficha] > 33 && casilla[ficha] < 39) fichaX[ficha] = fichaX[ficha] - 45;
    else if (casilla[ficha] == 39) fichaY[ficha] = fichaY[ficha] + 90;
    else if (casilla[ficha] == 40) fichaY[ficha] = fichaY[ficha] + 90;
  } else{//FICHAS ROJAS
     if (fichaX[ficha] > 850){
      fichaX[ficha] = 783;
      fichaY[ficha] = 615;
    }else if (casilla[ficha] == 23) fichaY[ficha] = fichaY[ficha] + 63;
    else if (casilla[ficha] > 23 && casilla[ficha] < 32) fichaY[ficha] = fichaY[ficha] + 45;
    else if (casilla[ficha] == 32) fichaY[ficha] = fichaY[ficha] + 65;
    else if (casilla[ficha] == 33) fichaX[ficha] = fichaX[ficha] + 68;
    else if (casilla[ficha] > 33 && casilla[ficha] < 39) fichaX[ficha] = fichaX[ficha] + 45;
    else if (casilla[ficha] == 39) fichaY[ficha] = fichaY[ficha] - 90;
    else if (casilla[ficha] == 40) fichaY[ficha] = fichaY[ficha] - 90;
    else if (casilla[ficha] == 1) fichaY[ficha] = fichaY[ficha] - 63;
    else if (casilla[ficha] < 10 && casilla[ficha] > 1) fichaY[ficha] = fichaY[ficha] - 45;
    else if (casilla[ficha] == 10) fichaY[ficha] = fichaY[ficha] - 65;
    else if (casilla[ficha] == 11) fichaX[ficha] = fichaX[ficha] - 68;
    else if (casilla[ficha] > 11 && casilla[ficha] < 22) fichaX[ficha] = fichaX[ficha] - 45;
    else if (casilla[ficha] == 22) fichaX[ficha] = fichaX[ficha] - 65;
    }
    casilla[ficha] = casilla[ficha] + 1;
  }

//--------------------------------------------------------------------------------------
void descentrarFichas(int ficha)
{
  if (ficha >= 0 && ficha < 4){
    for (int i = 0; i < 4; i++){//Para cuando se quiten los bloqueos
      if (casilla[ficha] == casilla[i] && i != ficha && casilla[ficha] != 0){
        if ((casilla[ficha] > 0 && casilla[ficha] < 11) || (casilla[ficha] > 22 && casilla[ficha] < 33)){
          if (ficha < i){
            fichaX[ficha] = fichaX[ficha] + 15;
            fichaX[i] = fichaX[i] - 15;
          }else{
            fichaX[ficha] = fichaX[ficha] - 15;
            fichaX[i] = fichaX[i] + 15;
          }
        }else if ((casilla[ficha] > 10 && casilla[ficha] < 23) || (casilla[ficha] > 32 && casilla[ficha] < 40) || (casilla[ficha] == 40)){
          if (ficha < i){
            fichaY[ficha] = fichaY[ficha] + 15;
            fichaY[i] = fichaY[i] - 15;
          }else{
            fichaY[ficha] = fichaY[ficha] - 15;
            fichaY[i] = fichaY[i] + 15;
          }
        }
      }
    }
  } else{
    for (int i = 4; i < 8; i++){
      if (casilla[ficha] == casilla[i] && i != ficha && casilla[ficha] != 0){
        if ((casilla[ficha] > 0 && casilla[ficha] < 11) || (casilla[ficha] > 22 && casilla[ficha] < 33)){
          if (ficha < i){
            fichaX[ficha] = fichaX[ficha] + 15;
            fichaX[i] = fichaX[i] - 15;
          }else{
            fichaX[ficha] = fichaX[ficha] - 15;
            fichaX[i] = fichaX[i] + 15;
          }
        }else if ((casilla[ficha] > 10 && casilla[ficha] < 23) || (casilla[ficha] > 32 && casilla[ficha] < 40) || (casilla[ficha] == 40)){
          if (ficha < i){
            fichaY[ficha] = fichaY[ficha] + 15;
            fichaY[i] = fichaY[i] - 15;
          }else{
            fichaY[ficha] = fichaY[ficha] - 15;
            fichaY[i] = fichaY[i] + 15;
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------
void dibujarFichas()
{
  //Fichas
  for (int i = 0; i < 8; ++i){
    if (i < 4){
      stroke(0, 0, 160);
      strokeWeight(4);
    } else{
      stroke(255, 0, 0);
      strokeWeight(4);
    }
    ellipse(fichaX[i], fichaY[i], 30, 30); 
  }
  //Numero fichas
  for (int i = 0; i < 8; ++i){
    fill(0, 102, 153);
    textSize(14);
    fill(0, 102, 153);
    if (i < 4){
      text(i + 1, fichaX[i] - 4, fichaY[i] + 5); 
    }else{
      text(i - 3, fichaX[i] - 4, fichaY[i] + 5); 
    }    
  }
}
//--------------------------------------------------------------------------------------
void matarFicha(int ficha){
  fichaX[ficha - 10] = fichaXinicial[ficha - 10];
  fichaY[ficha - 10] = fichaYinicial[ficha - 10];
  casilla[ficha - 10] = 0;
}
//--------------------------------------------------------------------------------------
void mostrarEstrella(int ficha){
  if (casilla[ficha] == 41){
    estrella = true;
  }else{
    estrella = false;
  }
}
//--------------------------------------------------------------------------------------
void dibujarEstrella(){
  if (estrella == true){
    // Draw image
    image(imgEstrella, 465, 350, 50, 50);
  }
}

void dibujarCronometro(){
  textSize(32);
  text(counter , counterX, counterY);
  fill(0, 102, 153);
}

void actualizarCronometro(int segundo){ 
  if (segundo < 40){
    counterX = 40;
    counterY = 220;
    counter = segundo - 20;
  }else{
    counterX = 875;
    counterY = 540;
    counter = segundo - 40;
  } 
  
}

void dibujarPlay(){
  if (!play){
      rect(300, 320, 400, 100);
      fill(255);
      textSize(30);
      text("Pulse ESPACIO para jugar", 310, 380); 
  }
}

void init(){
  play = false;
  for (int i = 0; i < 8; i++){
    fichaX[i] = fichaXinicial[i];
    fichaY[i] = fichaYinicial[i];
    casilla[i] = 0;
  }
  
}
