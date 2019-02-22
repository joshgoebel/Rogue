DEBUG_STORAGE void hitMonst(byte mm, byte r, char dx, char dy) { //mm=monst[x][y](1 to 16) r=kind(0 to 25
  byte rr = random(100);
  byte dmg = 0;
  byte w = equip(3, 1);
  byte hdex = 0, hdmg = 0;
  if (w != 0) {
    hdex = pgm_read_byte(wstat[inv[w - 1].ii % 16]) + inv[w - 1].i2;
    hdmg = pgm_read_byte(wstat[inv[w - 1].ii % 16 + 1]) + inv[w - 1].i3;
  }
  byte prob = 40 + hdex * 3 + hero.level * 2;

  if (rr < prob) {
    flashMonst(mm - 1);
    dmg = (random(hdex, hdex * hdmg) * 2 + strToDmg(hero.strength + hero.ringStrength) * 2 + hero.ringDexterity + hero.level + 1) / 2;
    checkMonst(mm - 1, r, dmg, hero.x+dx, hero.y+dy);
  }
}

DEBUG_STORAGE void checkMonst(byte m, byte r, byte dmg, byte x, byte y) { //m=id(0...15), r=vari, dmg, hitpos x,y
  if (mh[m] <= dmg) {
    monst[x][y] = 0;
    ms[m] = 0;
    if(r>=22){
      hero.experience = hero.experience + pgm_read_byte(mstat[r] + 4)*100;
    } else {
      hero.experience = hero.experience + pgm_read_byte(mstat[r] + 4);
    }
    if (r == 15) {
      hero.hheld = 0;
    }
    if (pgm_read_byte(mstat[r] + 5) >= random(100)+1) {
      if(r==9){
        placeThingXY(x, y, 1);
      } else {
        placeThingXY(x, y, 0);
      }
    }
  } else {
    mh[m] = mh[m] - dmg;
    if (ms[m] / 32 > 1) {
      ms[m] = 32 + ms[m] % 32;
    }
  }
}

DEBUG_STORAGE void hitHero(byte i, byte r) { //i=kind(0 to 25 r=ID(0 to 15)
  byte prob = pgm_read_byte(mstat[i] + 3) - hero.level * 2 - hero.ringDexterity * 2;
  byte rr = random(100);
  char dmg = 0;
  byte a = equip(4, 1);
  byte ac = 0;
  if (a == 0) {
    ac = 0;
  } else {
    ac = pgm_read_byte(astat + inv[a - 1].ii % 16 ) + inv[a - 1].i2;
  }

  if (rr < prob) {
    dmg = (random(pgm_read_byte(mstat[i] + 1), pgm_read_byte(mstat[i] + 2)) * (100 - ac * 3)) / 100;
    if(dmg<0) dmg=0;
//    if (hero.hp <= dmg) {
//      death=i+2;
//      gameState = gameover;
//    } else {
//      flashHero();
//      hero.hp = hero.hp - dmg;
    flashHero();
    charon(dmg, i+4);
      if (bitRead(m1[r], 3) == 1) {
        specialAttack( i ,r );
//      }
    }
  }
}

DEBUG_STORAGE void specialAttack(byte mon, byte id) {  //mon=0 to 25 mon vari, @Pharas sharp eye

  bool hit = false;
  byte eq, ac, prob;
  switch (mon) {
    case 5:     //IceMon
      if (random(5) == 0) {
        hit = true; //flashHero();
        setActiveMessage(13);
        hero.hslep = random(5) + 4;
        if (random(20) == 0) {
//          hero.hp = 0;
//          death=1;
          byte dmg=hero.hp;
          charon(dmg,1);
        }
      }
      break;
    case 6:     //R.snake
      if (hero.strength > 3 || hasRing(5) == 0) {
        if (random(5) == 0) {
          hit = true; //flashHero();
          setActiveMessage(14);
          hero.strength--;
        }
      }
      break;
    case 9:     //Leprchaun
      if ( hero.gold > 50 && random(10) != 0 ) {
        hit = true; //flashHero();
        setActiveMessage(15);
        hero.gold = hero.gold - random(50);
        ms[id] += 6*32;
      }
      break;
    case 12:    //Aquator
      eq = equip(4, 1);
      if ( equip(4, 1) != 0 && bitRead(inv[eq - 1].i4, 3) == 0) {
        if (hasRing(9) == 0) {
          hit = true; //flashHero();
          setActiveMessage(16);
          inv[eq - 1].i2--;
        }
      }
      break;
    case 13:    //Nymph
      if (random(2) == 0) {
        byte t = random(20);
        if (bitRead(inv[t].i4, 4) == 0) {
          hit = true; //flashHero();
          setActiveMessage(15);
          deleteItem(t);
          ms[id] += 6*32;
        }
      }
      break;
    case 15:    //Flytrap
      if (!levitating()) {
        hit = true; //flashHero();
        setActiveMessage(17);
        hero.hheld = 1;
      }
      break;
    case 17:    //Wraith
      if (hero.level > 5 && random(5) == 0) {
        hit = true; //flashHero();
        setActiveMessage(20);
        hero.exp_next_level = hero.exp_next_level / 2;
        hero.experience = hero.exp_next_level - 1;
        hero.level--;
        hero.maxHP = hero.maxHP - 3 - random(7);
        if (hero.hp > hero.maxHP) hero.hp = hero.maxHP;
      }
      break;
    case 22:    //Vampire
      if (random(10) < 4) {
        hit = true; //flashHero();
        setActiveMessage(21);
        byte r = random(3);
        if (r % 2 == 0) {
          if (hasRing(5) == 0 || hero.strength > 5) {
            hero.strength--;
          }
        }
        if (r > 0) {
          if (hero.hp > 9) {
            hero.hp--;
            hero.maxHP--;
          }
        }
      }
      break;
  }
  if(hit) flashHero();
}

