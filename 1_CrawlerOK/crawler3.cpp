#include <iostream>
#include <stdlib.h>

const int MAX_X = 5;
const int MAX_Y = 5;
const int ENEMY_AMOUNT = 1;
const int ENEMY_MAX_TURN = 3;
const int ENEMY_MIN_DISTANCE = 2;
const int PLAYER_START_X = 0;
const int PLAYER_START_Y = 0;
const int PLAYER_START_HEALTH = 5;
const int HEALING_AMOUNT = 1;

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
  int totalDamage;
  int totalHealing;
  int totalEnemies;
}player;

typedef struct enemy {
  char enemyName;
  vector2 pos;
  int turn;
}enemy;

typedef struct game {
  player playerOne;
  enemy enemies[ENEMY_AMOUNT];
  int maxRelics;
  char grid[MAX_Y][MAX_X];
  bool error;

  game() {
      maxRelics = 0;
      error = false;
  }
} game;

int randomNum(int max) {
  return std::rand()%max;
}

void randomTile(char& tile, game* world) {
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

char getEnemyOnPos(game* world, int x, int y) {
  for (int i = 0; i < ENEMY_AMOUNT; i++) {
    if (world->enemies[i].pos.x == x && world->enemies[i].pos.y == y) {
      return world->enemies[i].enemyName;
    }
  }
  return '-';
}

void printWorld(game* world) {
  // clearing console only works for Linux/Unix and window10-TH2 or higher.
  std::cout << "\033c";
  printLine();
  for (int y = 0; y < MAX_Y; y++) {
    std::cout << "|";
    for (int x = 0; x < MAX_X; x++) {
      char enemyName = getEnemyOnPos(world, x, y);
      if (world->playerOne.pos.x == x && world->playerOne.pos.y == y) {
          std::cout << world->playerOne.playerName << ' ';
      } else if (enemyName != '-') {
          std::cout << enemyName << ' ';
      } else {
          std::cout << world->grid[y][x] << ' ';
      }

    }
    std::cout << "|" << std::endl;
  }
  printLine();
  std::cout << "Health: " << world->playerOne.playerHealth << "\to = health" << std::endl;
  std::cout << "Relics: " << world->playerOne.playerRelics << "/" << world->maxRelics << "\t? = relic" << std::endl;
  std::cout << "pos: " << world->playerOne.pos.x << ":" << world->playerOne.pos.y << "\t# = danger" << std::endl;
}

vector2 randomEnemyPosition(game* world) {
  vector2 enemyPos = vector2(0,0);
  while (true) {
    int rnd = randomNum(MAX_X);
    if (abs(rnd - world->playerOne.pos.x) > ENEMY_MIN_DISTANCE) {
      enemyPos.x = rnd;
      break;
    }
  }
  while (true) {
    int rnd = randomNum(MAX_Y);
    if (abs(rnd - world->playerOne.pos.y) > ENEMY_MIN_DISTANCE) {
      enemyPos.y = rnd;
      break;
    }
  }
  return enemyPos;
}

void setupEnemies(game* world) {
  for (int i = 0; i < ENEMY_AMOUNT; i++) {
    world->enemies[i].pos = randomEnemyPosition(world);
    world->enemies[i].turn = randomNum(3);
    if (randomNum(2) == 1) {
      world->enemies[i].enemyName = 'x';
    } else {
      world->enemies[i].enemyName = '+';
    }
  }
}

void setupPlayer(game* world) {
  world->playerOne.pos.x = PLAYER_START_X;
  world->playerOne.pos.y = PLAYER_START_Y;
  world->playerOne.playerHealth = PLAYER_START_HEALTH;
  world->playerOne.playerRelics = 0;
  world->playerOne.totalDamage = 0;
  world->playerOne.totalHealing = 0;
  world->playerOne.totalEnemies = 0;
}

game setupWorld() {
  game newWorld;
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

void endGame(game* world, bool outcome) {
  world->error = true;
  std::cout << "\033c";
  if (outcome) {
    std::cout << "YOU WIN!";
  } else {
    std::cout << "YOU LOST!";
  }
  std::cout << "\n\nSummary:\n";
  std::cout << "Relics: " << world->playerOne.playerRelics << "/" << world->maxRelics;
  std::cout << "\nHealth: " << world->playerOne.playerHealth;
  std::cout << "\nTotal healing: " << world->playerOne.totalHealing;
  std::cout << "\nTotal damage: " << world->playerOne.totalDamage;
  std::cout << "\nTotal enemy contact: " << world->playerOne.totalEnemies;
  std::cout << "\nLast position: " << world->playerOne.pos.x << ":" << world->playerOne.pos.y << std::endl;

}

void relicTile(game* world) {
  world->playerOne.playerRelics++;
  if (world->playerOne.playerRelics == world->maxRelics) {
    endGame(world, true);
  }
}

void damagePlayer(game* world, int amount) {
  world->playerOne.playerHealth -= amount;
  world->playerOne.totalDamage += amount;
  if (world->playerOne.playerHealth <= 0) {
    endGame(world, false);
  }
}

void respawnEnemyIfHit(game* world) {
  for (int i = 0; i < ENEMY_AMOUNT; i++) {
    if (world->enemies[i].pos.x == world->playerOne.pos.x && world->enemies[i].pos.y == world->playerOne.pos.y) {
      damagePlayer(world, 1);
      world->playerOne.totalEnemies++;
      world->enemies[i].pos = randomEnemyPosition(world);
      world->enemies[i].turn = 0;
    }
  }
}

void dangerTile(game* world) {
  if (randomNum(60) < 10) {
    damagePlayer(world, 1);
  }
}

void enemyTile(game* world) {
  respawnEnemyIfHit(world);
}

void healingTile(game* world) {
  world->playerOne.playerHealth += HEALING_AMOUNT;
  world->playerOne.totalHealing += HEALING_AMOUNT;
}

void checkTile(game* world) {
  switch (world->grid[world->playerOne.pos.y][world->playerOne.pos.x]) {
    case '.': break;
    case '?': relicTile(world); break;
    case '#': dangerTile(world); break;
    case 'o': healingTile(world); break;
    case 'x': enemyTile(world); break;
    case '+': enemyTile(world); break;
    default: break;
  }
}



void enemyMove(game* world, int enemyNum) {
  int xCoord = world->playerOne.pos.x - world->enemies[enemyNum].pos.x;
  int xAbs = abs(xCoord);
  int yCoord = world->playerOne.pos.y - world->enemies[enemyNum].pos.y;
  int yAbs = abs(yCoord);
  if (yAbs > xAbs) {
    world->enemies[enemyNum].pos.y += (yCoord / yAbs);
  } else if (yAbs < xAbs) {
    world->enemies[enemyNum].pos.x += (xCoord / xAbs);
  } else {

  }
}

void enemyTurns(game* world) {
  for (int i = 0; i < ENEMY_AMOUNT; i++) {
    world->enemies[i].enemyName = (world->enemies[i].enemyName == 'x') ? '+' : 'x';
    world->enemies[i].turn++;
    if (world->enemies[i].turn >= ENEMY_MAX_TURN) {
      enemyMove(world, i);
      world->enemies[i].turn = 0;
    }
  }
}

void startTurn(game* world) {
  checkTile(world);
  enemyTurns(world);
  respawnEnemyIfHit(world);
}

void changePosition(game* world, int moveX, int moveY) {
  int x = world->playerOne.pos.x + moveX;
  int y = world->playerOne.pos.y + moveY;
  if (checkMove(x, MAX_X) && checkMove(y, MAX_Y)) {
    world->grid[world->playerOne.pos.y][world->playerOne.pos.x] = '.';
    world->playerOne.pos.x = x;
    world->playerOne.pos.y = y;
    startTurn(world);
  }

}

void gameLoop(game* world) {
  printWorld(world);
  while (true) {
    srand(time(NULL));
    char choice;
    std::cin >> choice;
    //std::cin >> "\t"
    switch (choice) {
      case 'w': changePosition(world, 0, -1); break;
      case 'a': changePosition(world, -1, 0); break;
      case 's': changePosition(world, 0, 1); break;
      case 'd': changePosition(world, 1, 0); break;
    }
    if (world->error) {
      break;
    }
    printWorld(world);
  }
}

int main() {
  game newWorld = setupWorld(), *world = &newWorld;
  gameLoop(world);
  return 0;
}
