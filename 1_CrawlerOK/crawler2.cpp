#include <iostream>
const int MAX_X = 18;
const int MAX_Y = 10;
const int ENEMY_AMOUNT = 3;
const int PLAYER_START_X = 0;
const int PLAYER_START_Y = 0;
const int PLAYER_START_HEALTH = 5;

typedef struct vector2 {
  int x;
  int y;

  vector2() {}
  vector2(int _x, int _y) {
    x = _x;
    y = _y;
  }
}vector2;

typedef struct player {
  char playerName;
  vector2 pos;
  int playerHealth;
  int playerRelics;
}player;

typedef struct enemy {
  vector2 pos;
  int turn;
}enemy;

typedef struct World {
  player playerOne;
  enemy enemies[ENEMY_AMOUNT];
  int maxRelics;
  char grid[MAX_Y][MAX_X];
  bool error;

  World() {
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
      if (world->playerOne.pos.x == x && world->playerOne.pos.y == y) {
          std::cout << world->playerOne.playerName << ' ';
      } else {
          std::cout << world->grid[y][x] << ' ';
      }

    }
    std::cout << "|" << std::endl;
  }
  printLine();
  std::cout << "Health: " << world->playerOne.playerHealth << "\t\to = health" << std::endl;
  std::cout << "Relics: " << world->playerOne.playerRelics << "/" << world->maxRelics << "\t\t? = relic" << std::endl;
  std::cout << "pos: " << world->playerOne.pos.x << ":" << world->playerOne.pos.y << "\t\t# = danger" << std::endl;
}

vector2 randomEnemyPosition(World* world) {
  vector2 enemyPos;
  while (world->playerOne.pos.x) {
    /* code */
  }
  return enemyPos;
}

void setupEnemies(World* world) {
  for (int i = 0; i < ENEMY_AMOUNT; i++) {
    world->enemies[i].pos = randomEnemyPosition(world);
    world->playerOne.pos.y = PLAYER_START_Y;
    world->playerOne.playerHealth = PLAYER_START_HEALTH;
    world->playerOne.playerRelics = 0;
  }
  world->playerOne.pos.x = PLAYER_START_X;
  world->playerOne.pos.y = PLAYER_START_Y;
  world->playerOne.playerHealth = PLAYER_START_HEALTH;
  world->playerOne.playerRelics = 0;
}

void setupPlayer(World* world) {
  world->playerOne.pos.x = PLAYER_START_X;
  world->playerOne.pos.y = PLAYER_START_Y;
  world->playerOne.playerHealth = PLAYER_START_HEALTH;
  world->playerOne.playerRelics = 0;
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
  setupPlayer(&newWorld);
  setupEnemies(&newWorld);
  std::cout << "Choose a letter: ";
  std::cin >> newWorld.playerOne.playerName;
  return newWorld;
}

bool checkMove(int i, int max) {
  return (i >= 0 && i < max);
}

void endGame(World* world, bool outcome) {

}

void relicTile(World* world) {
  world->playerOne.playerRelics++;
  if (world->playerOne.playerRelics == world->maxRelics) {
    endGame(world, true);
  }
}

void damagePlayer(World* world, int amount) {
  world->playerOne.playerHealth -= amount;
  if (world->playerOne.playerHealth <= 0) {
    endGame(world, false);
  }
}

void dangerTile(World* world) {
  if (randomNum(60) < 10) {
    damagePlayer(world, 1);
  }
}

void enemyTile(World* world) {

}

void checkTile(World* world) {
  switch (world->grid[world->playerOne.pos.y][world->playerOne.pos.x]) {
    case '.': break;
    case '?': relicTile(world); break;
    case '#': dangerTile(world); break;
    case '%': enemyTile(world); break;
    default: break;
  }
}

void changePosition(World* world, int moveX, int moveY) {
  int x = world->playerOne.pos.x + moveX;
  int y = world->playerOne.pos.y + moveY;
  if (checkMove(x, MAX_X) && checkMove(y, MAX_Y)) {
    world->grid[world->playerOne.pos.y][world->playerOne.pos.x] = '.';
    world->playerOne.pos.x = x;
    world->playerOne.pos.y = y;

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
