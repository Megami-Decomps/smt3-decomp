#ifndef DDS3_DIFFICULTY_H
#define DDS3_DIFFICULTY_H

/* Battle, reward and price values the game computes. Each macro expands to the
 * value it is given, so this build is unchanged. A host that scales them for
 * its own difficulty settings defines them first. */

/* HP and MP damage of skill nskill from formation index s to formation index d. */
#ifndef DDS3_BATTLE_HP_DAMAGE
#define DDS3_BATTLE_HP_DAMAGE(v, nskill, s, d) (v)
#endif
#ifndef DDS3_BATTLE_MP_DAMAGE
#define DDS3_BATTLE_MP_DAMAGE(v, nskill, s, d) (v)
#endif

/* Percent chances, compared against a roll of 0..99: a critical hit and a hit
 * by formation index s on formation index d, and an item drop. */
#ifndef DDS3_BATTLE_CRITICAL
#define DDS3_BATTLE_CRITICAL(v, s, d) (v)
#endif
#ifndef DDS3_BATTLE_HIT
#define DDS3_BATTLE_HIT(v, s, d) (v)
#endif
#ifndef DDS3_BATTLE_DROP
#define DDS3_BATTLE_DROP(v) (v)
#endif

/* EXP and macca won in a battle, and macca a demon gives in negotiation. */
#ifndef DDS3_BATTLE_EXP
#define DDS3_BATTLE_EXP(v) (v)
#endif
#ifndef DDS3_BATTLE_MAKA
#define DDS3_BATTLE_MAKA(v) (v)
#endif
#ifndef DDS3_NEGO_MAKA
#define DDS3_NEGO_MAKA(v) (v)
#endif

/* Macca prices: a shop item (sell nonzero is the sell price), a recovery and
 * a compendium summon. */
#ifndef DDS3_SHOP_PRICE
#define DDS3_SHOP_PRICE(v, sell) (v)
#endif
#ifndef DDS3_RECOVER_PRICE
#define DDS3_RECOVER_PRICE(v) (v)
#endif
#ifndef DDS3_SUMMON_PRICE
#define DDS3_SUMMON_PRICE(v) (v)
#endif

/* Field walking distance counted toward the next random encounter. */
#ifndef DDS3_ENCOUNTER_DISTANCE
#define DDS3_ENCOUNTER_DISTANCE(v) (v)
#endif

/* HP the party loses to field damage (damage floors, pipe damage). */
#ifndef DDS3_FIELD_DAMAGE
#define DDS3_FIELD_DAMAGE(v) (v)
#endif

#endif
