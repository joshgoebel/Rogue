DEBUG_STORAGE void flashHero(){
  if(!blind()) {
    drawMap();
    drawThing();
    drawMonst();
  }
//  arduboy.setTextBackground(WHITE);
//  arduboy.fillRect(hx*6+1, hy*8, 6,8,WHITE);
//  font5x7.setTextColor(BLACK);
  locate(hero.x, hero.y);
//  font5x7.print('@');
  font5x7.print('/');
  arduboy.display();
  delay(100);
//  arduboy.setTextBackground(BLACK);
//  font5x7.setTextColor(WHITE);
}

DEBUG_STORAGE void flashMonst(byte num){
  if(!blind()) {
    drawMap();
    drawThing();
    drawMonst();
  }
//  arduboy.setTextBackground(WHITE);
//  arduboy.fillRect(mx[num]*6+1, my[num]*8, 6,8,WHITE);
//  font5x7.setTextColor(BLACK);
  locate(mx[num], my[num]);
  if(!blind() && bitRead(m1[num],6)==0){
//    font5x7.print((char)pgm_read_byte(mname+(ms[num] % 32)));
    font5x7.print('/');
  } else {
    font5x7.print(' ');
  }
  arduboy.display();
  delay(100);
//  arduboy.setTextBackground(BLACK);
//  font5x7.setTextColor(WHITE);
}

DEBUG_STORAGE void initHero(){
  for(byte i=0; i<maxItems; i++){
    deleteItem(i);
  }
  memset(&hero, 0, sizeof(hero));

  hero.level = 1;
  hero.dungeon_level = 1, hero.strength = 16, hero.maxStrength = hero.strength;

  // all status effects start at 0 by default - confusion, blindness, etc.

  // gold = 0
  // turns = 0
  // experience = 0

  hero.hp = 16, hero.maxHP = hero.hp, hero.hunger = 400, hero.exp_next_level = 10;

  hero.itemCount = 5;
  inv[0].ii = 2 * 16 + 0; inv[0].i1 =  1; inv[0].i2 = 0; inv[0].i3 = 0; inv[0].i4 = 0b10000000;
  inv[1].ii = 3 * 16 + 0; inv[1].i1 =  1; inv[1].i2 = 1; inv[1].i3 = 1; inv[1].i4 = 0b00110000;
  inv[2].ii = 3 * 16 + 3; inv[2].i1 =  1; inv[2].i2 = 1; inv[2].i3 = 0; inv[2].i4 = 0b00100000;
  inv[3].ii = 3 * 16 + 5; inv[3].i1 = 30; inv[3].i2 = 0; inv[3].i3 = 0; inv[3].i4 = 0b10100000;
  inv[4].ii = 4 * 16 + 1; inv[4].i1 =  1; inv[4].i2 = 1; inv[4].i3 = 0; inv[4].i4 = 0b00110000;
//  ii[5] = 6 * 16 + findScroll(11); i1[5] = 5; i2[5] = 0; i3[5] = 0; i4[5] = 0b10000000;

}

DEBUG_STORAGE void initTable(){
  const byte vari[4] = {maxPotions, maxScrolls, maxRings, maxWands};
  for (byte v = 0; v < 4; v++) {
    // generate ordered lookup table
    for (byte i = 0; i < maxPotions; i++) {
      ttab[v][i] = i;
    }
    // randomize it
    for (byte i = 0; i < 50; i++) {
      byte r1 = random(vari[v]);
      byte r2 = random(vari[v]);
      byte tmp = ttab[v][r1];
      ttab[v][r1] = ttab[v][r2];
      ttab[v][r2] = tmp;
    }
  }
  for(byte i=0; i<4; i++){
    tknow[i]=0;
  }
}
