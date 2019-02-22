void titleScreen() {
//  arduboy.setRGBled(0, 0, 0);
  locate(1, 2);
  font5x7.print(F("[@ ~]$ Rogue"));
  locate(9, 5);
  font5x7.print(F("A)New Game"));
  locate(9, 6);
  font5x7.print(F("B)Continue"));
/*
  if ((millis() / 300) % 2 == 0) {
    locate(13, 2);
    font5x7.print('_');
  } else {
    locate(13, 2);
    font5x7.print(' ');
  }
*/
  if (wiz == 1) {
    locate(0, 7);
    font5x7.print(adepth);
  }
  //  initHero();
  if (arduboy.justPressed(UP_BUTTON)) {
    wiz = 1;
    if (adepth >= 2) {
      adepth--;
    }
  }
  if (arduboy.justPressed(DOWN_BUTTON)) {
    wiz = 1;
    if (adepth <= 25) {
      adepth++;
    }
  }
  if (arduboy.justPressed(A_BUTTON)) {
    initTable();
    initHero();
    clearKnown();
    buildDungeon();
    setActiveMessage(0);
    //    clearDungeon();
    //    makeDungeon4();
    //    placeMonst();
    //    placeThing();
    //    mess(0);
    //    addBuf( dlv );
//    welc = 1;
    gameState = gameloop;
  }
  if (arduboy.justPressed(B_BUTTON) ){ //&& welc == 1) {
    if(EEPROM.read(20)==1){
      EEPROM.update(20,0);
      loadStatus();
//      hero.dlv++;
//      buildDungeon();
      gameState = landing;
    }
  }
}

void atLanding(){
  locate(5,2);
  font5x7.print(F("Next floor "));
  font5x7.print(hero.dlv);
  locate(7,5);
  font5x7.print(F("A)Go Next"));
  locate(7,6);
  font5x7.print(F("B)Save&quit"));

  if (arduboy.justPressed(A_BUTTON) ) {
    setActiveMessage(29);
    buildDungeon();
    gameState = gameloop;
  }

  if (arduboy.justPressed(B_BUTTON) ) {
    EEPROM.update(20,1);
    saveStatus();
    wiz=0;
    adepth=26;
    gameState = title;
  }
}

void gameOver() {
  wiz = 0;
  adepth = 26;
  EEPROM.update(20,0);
//  hero.hp = hero.hpm;
//  hero.hh = 400;
  //  locate(7, 3);
  //  arduboy.print(F("R.I.P."));
  //  locate(7, 6);
  //  arduboy.print(F("$"));
  //  locate(9, 6);
  //  arduboy.print(au);


  locate(6, 0);
  font5x7.print(F("---------"));
  locate(5, 1);
  font5x7.print(F("/  R.I.P. \\"));
  locate(4, 2);
  font5x7.print(F("/           \\"));
  locate(4, 3);
  font5x7.print(F("|   Hero    |"));
  locate(4, 4);
  font5x7.print(F("|           |"));
  locate(4, 5);
  font5x7.print(F("|           |"));
  locate(3, 6);
  font5x7.print(F("*|           |*"));
  locate(0, 7);
  font5x7.print(F("___)\\\\//))(//\\(/((___"));

  if (death == 0) {
    locate(7, 5);
    font5x7.print(F("Starve"));
  } else if (death == 1) {
    locate(7, 5);
    font5x7.print(F("Frozen"));
  } else if(death==2){
    locate(7, 4);
    font5x7.print(F("lack of"));
    locate(8, 5);
    font5x7.print(F("luck"));
  } else if(death==3){
    locate(7, 4);
    font5x7.print(F("hellfire"));
  } else {
    locate(7, 4);
    font5x7.print(F("Killed"));
    locate(8, 5);
    font5x7.print(F("by"));
    locate(11, 5);
    font5x7.print((char)pgm_read_byte(mname + death - 4));
  }
  locate(11 - digits(hero.au), 6);
  font5x7.print(F("$"));
  font5x7.print(hero.au);
  if (arduboy.justPressed(A_BUTTON)) {
    rank = checkHiScore();
    gameState = score;
  }
}

byte digits(long int num) {
  byte ans = 1;
  while (num > 10) {
    ans++;
    num /= 10;
  }
  return ans;
}

void gameWinner() {
  wiz = 0;
  adepth = 26;
  EEPROM.update(20,0);
//  welc = 0;

  const byte YGT[8][3] = {
    {0b00101001, 0b00101000, 0b00000000},
    {0b00101010, 0b10101000, 0b00000000},
    {0b00010010, 0b10101000, 0b00000000},
    {0b00010001, 0b00010000, 0b00000000},
    {0b10100100, 0b11001100, 0b10111000},
    {0b11101010, 0b10101100, 0b10010000},
    {0b10101110, 0b10101000, 0b10010000},
    {0b10101010, 0b11001100, 0b10010000}
  };

  //  locate(4, 3);
  //  arduboy.print(F("You made it!"));

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 21; x++) {
      locate(x, y);
      if (bitRead(YGT[y][x / 8], 7 - x % 8) == 1) {
        font5x7.print('@');
      } else {
        font5x7.print(F(" "));
      }
    }
  }

  if (arduboy.justPressed(A_BUTTON)) {
    rank = checkHiScore();
    gameState = score;
  }
}

void showHighScores() {

  //  byte rank = checkHiScore();

  locate(8, 0);
  font5x7.print(F("Score"));
  //  locate(0, 0);
  //  arduboy.print(rank);
  locate(12, 7);
  font5x7.print(F("B)Clear"));

  for (int i = 0; i < 5; i++) {
    if (rank == i + 1) {
//      arduboy.setTextBackground(WHITE);
//      arduboy.fillRect(0,(2+i)*8,128,8,WHITE);
//      font5x7.setTextColor(BLACK);
    locate(0, 2 + i);
    font5x7.print('>');
    } else {
//      arduboy.setTextBackground(BLACK);
//      font5x7.setTextColor(WHITE);
    }
//    locate(0, 2 + i);
//    font5x7.print(F("                     "));
    locate(1, 2 + i);
    font5x7.print(i + 1);
    locate(3, 2 + i);
    font5x7.print(glory[4 - i].gold);
    locate(14, 2 + i);
    if (glory[4 - i].depth == 255) {
      font5x7.print(F("Win"));
    } else {
      font5x7.print(glory[4 - i].depth);
    }
  }
//  arduboy.setTextBackground(BLACK);
//  font5x7.setTextColor(WHITE);

  if (arduboy.justPressed(A_BUTTON)) {
    gameState = title;
  }
  if (arduboy.justPressed(B_BUTTON)) {
    clearHiScore();
  }

}

byte checkHiScore() {
  Score tglory;
  byte result = 0;
  if (hero.dlv == 0) hero.dlv = 255;
  if (glory[0].gold < hero.au) {
    glory[0] = {hero.au, hero.dlv};
    for (int i = 0; i < 5; ++i) {
      for (int j = i + 1; j < 5; ++j) {
        if ( glory[i].gold > glory[j].gold ) {
          tglory = glory[i];
          glory[i] = glory[j];
          glory[j] = tglory;
        }
      }
    }
    saveHiScore();
    for (int i = 0; i < 5; i++) {
      if (glory[i].gold == hero.au) {
        result = 5 - i;
      }
    }
  }
  return result;
}

void gameLoop() {
  byte mm, r;

  if (arduboy.justPressed(LEFT_BUTTON)) {
    heroMove(1);
  }

  if (arduboy.justPressed(RIGHT_BUTTON)) {
    heroMove(3);
  }

  if (arduboy.justPressed(UP_BUTTON)) {
    heroMove(2);
  }

  if (arduboy.justPressed(DOWN_BUTTON)) {
    if (arduboy.buttonsState() == 24) {
      ss = 1;
    } else {
      heroMove(4);
    }
  }

  if (arduboy.justPressed(A_BUTTON)) {
    if (ss == 1) {
      ss = 0;
    } else {
      if (dungeon[hero.hx][hero.hy] >= 11 && dungeon[hero.hx][hero.hy] <= 16 && hero.hheld == 0) {
        hero.hmdet = 0;
        hero.hisee = 0;
        if (inv[hero.im - 1].ii == 144) {
          hero.dlv--;
        } else {
          hero.dlv++;
        }
        if (hero.dlv == 0) {
          gameState = winner;
        } else {
          gameState = landing;
        }
//        gameState = landing;
//        buildDungeon();
      } else {
        //      ss = 1;
//        clearBuf();
        search();
        wakeUp();
        tweatHero();
        moveMonst();
      }
    }
  }

  if (arduboy.justPressed(B_BUTTON)) {
    ss = 0;
    setActiveMessage(29);
//    clearBuf();
    inventry(0);
  }

  makeKnown();
  if (hero.hblnd == 0) drawMap();
  drawHero();
  if (hero.hblnd == 0) drawThing();
  if (hero.hblnd == 0) drawMonst();
  if (ss == 1) {
    showStatus();
  }
  showMsg();
}

void buildDungeon() {
  clearDungeon();
  makeDungeon4();
  placeMonst();
  placeThing();
  //  mess(0);
  //  addBuf((char)dlv);
}

void heroMove(byte dir) {
  if (hero.hconf > 0) {
    dir = random(4) + 1;
  }
  char dx = (dir - 2) * (dir % 2);
  char dy = (dir - 3) * ((dir - 1) % 2);
  byte mm, r;

  if (hero.hslep > 0) {
    dx = 0;
    dy = 0;
  }
  ss = 0;
  if (monst[hero.hx + dx][hero.hy + dy] == 0) {
    if ((hero.hy + dy) <= 7 && (hero.hy + dy) >= 0 && (hero.hx + dx) >= 0 && (hero.hx + dx) <= 20
        && dungeon[hero.hx + dx][hero.hy + dy] >= 1 && dungeon[hero.hx + dx][hero.hy + dy] <= 200) {
      if (hero.hheld == 0) {
        hero.hx = hero.hx + dx;
        hero.hy = hero.hy + dy;
      }
      setActiveMessage(29);
//      clearBuf();
      if (hero.hlevi == 0) {
        checkThing(hero.hx, hero.hy);
        if(dungeon[hero.hx][hero.hy] >= 31 && dungeon[hero.hx][hero.hy] <= 106){
          dungeon[hero.hx][hero.hy] += 80;
        }
        if(dungeon[hero.hx][hero.hy] >= 111 && dungeon[hero.hx][hero.hy] <= 186){
          traped( dungeon[hero.hx][hero.hy]/10-11 );
        }
      }
    }
  } else {
    mm = monst[hero.hx + dx][hero.hy + dy];
    r = ms[mm - 1] % 32;
    setActiveMessage(29);
//    clearBuf();
    hitMonst(mm, r, dx, dy);
  }
  wakeUp();
  tweatHero();
  if (hero.hfast > 0) {
    if (hero.ht % 2 == 0) {
      moveMonst();
    }
  } else {
    moveMonst();
  }
}

// bear?

void traped(byte vari){
  byte dmg=0;
  if(random(2)==0){
    flashHero();

    switch (vari){
      case 0:     //door
        hero.dlv++;
        buildDungeon();
        break;
      case 1:     //arrow
        dmg = random(3)+1;
        charon(dmg, 2);
        break;
      case 2:     //sleep
        if(hero.hslep==0) hero.hslep=5;
        break;
      case 3:     //telport
        teleportHero();
        break;
      case 4:     //poison
        if (hero.st > 3 || hasRing(5) == 0) hero.st--;
        break;
      case 5:     //rust
        byte eq = equip(4, 1);
        if ( equip(4, 1) != 0 && bitRead(inv[eq - 1].i4, 3) == 0) {
          if (hasRing(9) == 0) {
            inv[eq - 1].i2--;
          }
        }
        break;
    }
  }
  setActiveMessage( 23 + vari);
//  for(int i=0; i<8; i++){
//    gbuf[i]=pgm_read_byte(trap[vari]+i);
//  }
//  addBuf(" trap");
}

void charon(byte dmg, byte reason){
  if( hero.hp <= dmg ) {
    death = reason;
    gameState = gameover;
  } else {
    hero.hp=hero.hp-dmg;
  }
}
// 0 :steave
// 1 :frozen
// 2 :lack of luck
// 3 :hellfire
// 4-:killed by X
