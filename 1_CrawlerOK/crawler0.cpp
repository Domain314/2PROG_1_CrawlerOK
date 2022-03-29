#include <iostream>
const int MAX_X = 18;
const int MAX_Y = 10;


typedef struct World {
  char playerName;
  int playerX;
  int playerY;
  int playerHealth;
  int playerRelics;
  int maxRelics;
  char grid[MAX_Y][MAX_X];
  bool error;

  World() {
      playerX = 0;
      playerY = 0;
      playerHealth = 5;
      playerRelics = 0;
      maxRelics = 0;
      error = false;
  }
} World;

int randomNum(int max) {
  return std::rand()%max;
}

void randomTile(char& tile, World* world) {
  int rnd = randomNum(100);
  if (rnd < 40) {
    tile = '.'; // empty
  } else if (40 <= rnd && rnd < 50) {
    tile = 'o'; // health
  } else if (50 <= rnd && rnd < 60) {
    tile = '?'; // relic
    world->maxRelics++;
  } else if (60 <= rnd) {
    tile = '#'; // danger
  }
}

void printLine() {
  for (size_t i = 0; i < MAX_X + 1; i++) {
    std::cout << "--";
  }
  std::cout << std::endl;
}

void printWorld(World* world) {
  // clearing console only works for Linux/Unix and window10-TH2 or higher.
  std::cout << "\033c";
  printLine();
  for (int y = 0; y < MAX_Y; y++) {
    std::cout << "|";
    for (int x = 0; x < MAX_X; x++) {
      if (world->playerX == x && world->playerY == y) {
          std::cout << world->playerName << ' ';
      } else {
          std::cout << world->grid[y][x] << ' ';
      }

    }
    std::cout << "|" << std::endl;
  }
  printLine();
  std::cout << "Health: " << world->playerHealth << "\t\to = health" << std::endl;
  std::cout << "Relics: " << world->playerRelics << "/" << world->maxRelics << "\t\t? = relic" << std::endl;
  std::cout << "pos: " << world->playerX << ":" << world->playerY << "\t\t# = danger" << std::endl;
}

World setupWorld() {
  World newWorld;
  while (newWorld.maxRelics == 0) {
    for (int y = 0; y < MAX_Y; y++) {
      for (size_t x = 0; x < MAX_X; x++) {
         randomTile(newWorld.grid[y][x], &newWorld);
      }
    }
  }
  std::cout << "Choose a letter: ";
  std::cin >> newWorld.playerName;
  return newWorld;
}

bool checkMove(int i, int max) {
  return (i >= 0 && i < max);
}

void changePosition(World* world, int moveX, int moveY) {
  int x = world->playerX + moveX;
  int y = world->playerY + moveY;
  if (checkMove(x, MAX_X) && checkMove(y, MAX_Y)) {
    world->grid[world->playerY][world->playerX] = '.';
    world->playerX = x;
    world->playerY = y;
  }

}

void gameLoop(World* world) {
  while (true) {
    printWorld(world);
    char choice;
    std::cin >> choice;
    //std::cin >> "\t"
    switch (choice) {
      case 'w': changePosition(world, 0, -1); break;
      case 'a': changePosition(world, -1, 0); break;
      case 's': changePosition(world, 0, 1); break;
      case 'd': changePosition(world, 1, 0); break;
    }
  }
}

int main() {
  World newWorld = setupWorld(), *worldPtr = &newWorld;
  gameLoop(worldPtr);
  printWorld(worldPtr);


  return 0;
}
