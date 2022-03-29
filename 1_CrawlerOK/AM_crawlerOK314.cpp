/*
 * CrawlerOK314 by Artjom Moisejev - Copyright (C) 2021 <domain_69@hotmail.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <cstdlib>

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

// returns a random number
int randomNum(int max) {
  return std::rand()%max;
}

// decide on tile type
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

// print ceiling and floor of grid-display
void printLine() {
  for (size_t i = 0; i < MAX_X + 1; i++) {
    std::cout << "--";
  }
  std::cout << std::endl;
}

// return enemyName on position xy
char getEnemyOnPos(game* world, int x, int y) {
  for (int i = 0; i < ENEMY_AMOUNT; i++) {
    if (world->enemies[i].pos.x == x && world->enemies[i].pos.y == y) {
      return world->enemies[i].enemyName;
    }
  }
  return '-';
}

// print grid-display and info
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

// generate random x and y, until they're out of the player's range.
// return this position to place an enemy
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

// place enemies at random positions and set their turns and names
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

// setup player variables
void setupPlayer(game* world) {
  world->playerOne.pos.x = PLAYER_START_X;
  world->playerOne.pos.y = PLAYER_START_Y;
  world->playerOne.playerHealth = PLAYER_START_HEALTH;
  world->playerOne.playerRelics = 0;
  world->playerOne.totalDamage = 0;
  world->playerOne.totalHealing = 0;
  world->playerOne.totalEnemies = 0;
}

// self explanatory
void printIntroduction() {
  std::cout << "Welcome!\n\n0. Move with WASD \n1. Get all relics (?) \n2. avoid danger (#) \n3. avoid enemies (x/+) \n4. Heal at (o) \n\nGood Luck!\n" << std::endl;
}

// generate a new grid and initialize setups for player & enemies
game setupWorld() {
  game newWorld;
  srand(time(NULL));
  while (newWorld.maxRelics == 0) {
    for (int y = 0; y < MAX_Y; y++) {
      for (size_t x = 0; x < MAX_X; x++) {
         randomTile(newWorld.grid[y][x], &newWorld);
      }
    }
  }
  setupPlayer(&newWorld);
  setupEnemies(&newWorld);
  printIntroduction();
  std::cout << "Choose a letter (name): ";
  std::cin >> newWorld.playerOne.playerName;
  return newWorld;
}

// check for grid-walls
bool checkMove(int i, int max) {
  return (i >= 0 && i < max);
}

// end and summarize the game
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

// add relic to inventory and check wether it was the last or not, to end the game
void relicTile(game* world) {
  world->playerOne.playerRelics++;
  if (world->playerOne.playerRelics == world->maxRelics) {
    endGame(world, true);
  }
}

// subtract damage from playerHealth and endgame if (playerHealth <= 0)
void damagePlayer(game* world, int amount) {
  world->playerOne.playerHealth -= amount;
  world->playerOne.totalDamage += amount;
  if (world->playerOne.playerHealth <= 0) {
    endGame(world, false);
  }
}

// check all Enemies for player-contact and respawn them, if needed
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

// 1/6 Chance to lose health on entering a danger-tile
void dangerTile(game* world) {
  if (randomNum(60) < 10) {
    damagePlayer(world, 1);
  }
}

// check for enemy-player-contact, if entering an enemy-tile
void enemyTile(game* world) {
  respawnEnemyIfHit(world);
}

// heal the player
void healingTile(game* world) {
  world->playerOne.playerHealth += HEALING_AMOUNT;
  world->playerOne.totalHealing += HEALING_AMOUNT;
}

// decide what to do on entering a new tile
void checkTile(game* world) {
  switch (world->grid[world->playerOne.pos.y][world->playerOne.pos.x]) {
    case '.': break;
    case '?': relicTile(world); break;
    case '#': dangerTile(world); break;
    case 'o': healingTile(world); break;
    default: break;
  }
  respawnEnemyIfHit(world);
}

// calculate player direction from enemy perspective and move towards him
// move on axis, where the distance is higher or don't move, if the distances are equal
void enemyMove(game* world, int enemyNum) {
  int xDist = world->playerOne.pos.x - world->enemies[enemyNum].pos.x;
  int xAbs = abs(xDist);
  int yDist = world->playerOne.pos.y - world->enemies[enemyNum].pos.y;
  int yAbs = abs(yDist);
  if (yAbs >= xAbs && yAbs != 0) {
    world->enemies[enemyNum].pos.y += (yDist / yAbs);
  } else if (yAbs < xAbs && xAbs != 0) {
    world->enemies[enemyNum].pos.x += (xDist / xAbs);
  }
}

// for each enemy: play animation and move if it's his turn
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

// turn phases
void makeTurn(game* world) {
  checkTile(world);
  enemyTurns(world);
  respawnEnemyIfHit(world);
}

// validate move. Yes -> make move and initialize turn-phases
void changePosition(game* world, int moveX, int moveY) {
  int x = world->playerOne.pos.x + moveX;
  int y = world->playerOne.pos.y + moveY;
  if (checkMove(x, MAX_X) && checkMove(y, MAX_Y)) {
    world->grid[world->playerOne.pos.y][world->playerOne.pos.x] = '.';
    world->playerOne.pos.x = x;
    world->playerOne.pos.y = y;
    makeTurn(world);
  }
}

// print world and decide where to move, based on input
void gameLoop(game* world) {
  while (true) {
    printWorld(world);
    srand(time(NULL));
    char choice;
    std::cin >> choice;
    //std::cin >> "\t"
    switch (choice) {
      case 'w': changePosition(world, 0, -1); break;
      case 'a': changePosition(world, -1, 0); break;
      case 's': changePosition(world, 0, 1); break;
      case 'd': changePosition(world, 1, 0); break;
      default: break;
    }
    if (world->error) {
      break;
    }
  }
}

// initialize world, make pointer and pass it to the gameLoop().
int main() {
  game newWorld = setupWorld(), *world = &newWorld;
  gameLoop(world);
  return 0;
}
