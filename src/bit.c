/***************************************************************************
 *  God Wars Mud improvements copyright (C) 1994, 1995, 1996 by Richard    *
 *  Woolcock.  This mud is NOT to be copied in whole or in part, or to be  *
 *  run without the permission of Richard Woolcock.  Nobody else has       *
 *  permission to authorise the use of this code.                          *
 ***************************************************************************/

/**************************************************************
 *  Myth Mud improvements ALL by David Spink aka Morglum.     *
 *  This mud is NOT to be copied in whole or in part! No one  *
 *  has the permission to authorise the use of this code.     *
 *  In order to use this code you MUST comply with the terms  *
 *  and conditions in 'license.myth' (as shipped).            *
 **************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"




char *room_bit_name( int room_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( room_flags & ROOM_DARK         ) strcat( buf, " dark"         );
    if ( room_flags & ROOM_NO_MOB       ) strcat( buf, " no_mobs"      );
    if ( room_flags & ROOM_INDOORS      ) strcat( buf, " indoors"      );
    if ( room_flags & ROOM_PRIVATE      ) strcat( buf, " private"      );
    if ( room_flags & ROOM_SAFE         ) strcat( buf, " safe"         );
    if ( room_flags & ROOM_SOLITARY     ) strcat( buf, " solitary"     );
    if ( room_flags & ROOM_PET_SHOP     ) strcat( buf, " pet_shop"     );
    if ( room_flags & ROOM_NO_RECALL    ) strcat( buf, " no_recall"    );
    if ( room_flags & ROOM_NO_TELEPORT  ) strcat( buf, " no_teleport"  );
    if ( room_flags & ROOM_TOTAL_DARKNESS ) strcat( buf, " total_darkness" );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}



int room_name_bit( char* buf )
{
    if ( !str_cmp( buf, "dark"         ) ) return ROOM_DARK;
    if ( !str_cmp( buf, "no_mobs"      ) ) return ROOM_NO_MOB;
    if ( !str_cmp( buf, "indoors"      ) ) return ROOM_INDOORS;
    if ( !str_cmp( buf, "private"      ) ) return ROOM_PRIVATE;
    if ( !str_cmp( buf, "safe"         ) ) return ROOM_SAFE;
    if ( !str_cmp( buf, "solitary"     ) ) return ROOM_SOLITARY;
    if ( !str_cmp( buf, "pet_shop"     ) ) return ROOM_PET_SHOP;
    if ( !str_cmp( buf, "no_recall"    ) ) return ROOM_NO_RECALL;
    if ( !str_cmp( buf, "no_teleport"  ) ) return ROOM_NO_TELEPORT;
    if ( !str_cmp( buf, "total_darkness" ) ) return ROOM_TOTAL_DARKNESS;
    return 0;
}



int sector_number( char *argument )
{
    if ( !str_cmp( argument, "inside" ) )       return SECT_INSIDE;
    if ( !str_cmp( argument, "city" ) )         return SECT_CITY;
    if ( !str_cmp( argument, "field" ) )        return SECT_FIELD;
    if ( !str_cmp( argument, "forest" ) )       return SECT_FOREST;
    if ( !str_cmp( argument, "hills" ) )        return SECT_HILLS;
    if ( !str_cmp( argument, "mountain" ) )     return SECT_MOUNTAIN;
    if ( !str_cmp( argument, "swim" ) )         return SECT_WATER_SWIM;
    if ( !str_cmp( argument, "noswim" ) )       return SECT_WATER_NOSWIM;
    if ( !str_cmp( argument, "underwater" ) )   return SECT_UNDERWATER;
    if ( !str_cmp( argument, "air" ) )          return SECT_AIR;
    if ( !str_cmp( argument, "desert" ) )       return SECT_DESERT;
    return SECT_MAX;
}



char *sector_name( int sect )
{
    if ( sect == SECT_INSIDE )       return "inside";
    if ( sect == SECT_CITY )         return "city";
    if ( sect == SECT_FIELD )        return "field";
    if ( sect == SECT_FOREST )       return "forest";
    if ( sect == SECT_HILLS )        return "hills";
    if ( sect == SECT_MOUNTAIN )     return "mountain";
    if ( sect == SECT_WATER_SWIM )   return "swim";
    if ( sect == SECT_WATER_NOSWIM ) return "noswim";
    if ( sect == SECT_UNDERWATER )   return "underwater";
    if ( sect == SECT_AIR )          return "air";
    if ( sect == SECT_DESERT )       return "desert";
    return "unknown";
}



/*
 * Return ascii name of an item type.
 */
char *item_type_name( int item_type )
{
    switch ( item_type )
    {
    case ITEM_LIGHT:		return "light";
    case ITEM_SCROLL:		return "scroll";
    case ITEM_WAND:		return "wand";
    case ITEM_STAFF:		return "staff";
    case ITEM_WEAPON:		return "weapon";
    case ITEM_TREASURE:		return "treasure";
    case ITEM_ARMOR:		return "armor";
    case ITEM_POTION:		return "potion";
    case ITEM_FURNITURE:	return "furniture";
    case ITEM_TRASH:		return "trash";
    case ITEM_CONTAINER:	return "container";
    case ITEM_DRINK_CON:	return "drink container";
    case ITEM_KEY:		return "key";
    case ITEM_FOOD:		return "food";
    case ITEM_MONEY:		return "money";
    case ITEM_BOAT:		return "boat";
    case ITEM_CORPSE_NPC:	return "npc corpse";
    case ITEM_CORPSE_PC:	return "pc corpse";
    case ITEM_FOUNTAIN:		return "fountain";
    case ITEM_PILL:		return "pill";
    case ITEM_PORTAL:		return "portal";
    case ITEM_EGG:		return "egg";
    case ITEM_VOODOO:		return "voodoo doll";
    case ITEM_STAKE:		return "stake";
    case ITEM_MISSILE:		return "missile";
    case ITEM_AMMO:		return "ammo";
    case ITEM_QUEST:		return "quest token";
    case ITEM_QUESTCARD:	return "quest card";
    case ITEM_QUESTMACHINE:	return "quest generator";
    case ITEM_SYMBOL:		return "magical symbol";
    case ITEM_BOOK:		return "book";
    case ITEM_PAGE:		return "page";
    case ITEM_TOOL:		return "tool";
    case ITEM_WALL:		return "wall";
    case ITEM_WARD:		return "ward";
    case ITEM_ETOKEN:		return "energy token";

    }

    bug( "Item_type_name: unknown type %d.", item_type );
    return "(unknown)";
}

int item_name_type( char *name )
{
    if ( !str_cmp( name, "light"     ) ) return ITEM_LIGHT;
    if ( !str_cmp( name, "scroll"    ) ) return ITEM_SCROLL;
    if ( !str_cmp( name, "wand"      ) ) return ITEM_WAND;
    if ( !str_cmp( name, "staff"     ) ) return ITEM_STAFF;
    if ( !str_cmp( name, "weapon"    ) ) return ITEM_WEAPON;
    if ( !str_cmp( name, "treasure"  ) ) return ITEM_TREASURE;
    if ( !str_cmp( name, "armor"     ) ) return ITEM_ARMOR;
    if ( !str_cmp( name, "potion"    ) ) return ITEM_POTION;
    if ( !str_cmp( name, "furniture" ) ) return ITEM_FURNITURE;
    if ( !str_cmp( name, "trash"     ) ) return ITEM_TRASH;
    if ( !str_cmp( name, "container" ) ) return ITEM_CONTAINER;
    if ( !str_cmp( name, "drink"     ) ) return ITEM_DRINK_CON;
    if ( !str_cmp( name, "key"       ) ) return ITEM_KEY;
    if ( !str_cmp( name, "food"      ) ) return ITEM_FOOD;
    if ( !str_cmp( name, "money"     ) ) return ITEM_MONEY;
    if ( !str_cmp( name, "boat"      ) ) return ITEM_BOAT;
    if ( !str_cmp( name, "corpse"    ) ) return ITEM_CORPSE_NPC;
    if ( !str_cmp( name, "fountain"  ) ) return ITEM_FOUNTAIN;
    if ( !str_cmp( name, "pill"      ) ) return ITEM_PILL;
    if ( !str_cmp( name, "portal"    ) ) return ITEM_PORTAL;
    if ( !str_cmp( name, "egg"       ) ) return ITEM_EGG;
    if ( !str_cmp( name, "stake"     ) ) return ITEM_STAKE;
    if ( !str_cmp( name, "missile"   ) ) return ITEM_MISSILE;
    if ( !str_cmp( name, "ammo"      ) ) return ITEM_AMMO;

    return 0;
}



/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name( int extra_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( extra_flags & ITEM_GLOW         ) strcat( buf, " glow"         );
    if ( extra_flags & ITEM_HUM          ) strcat( buf, " hum"          );
    if ( extra_flags & ITEM_INVIS        ) strcat( buf, " invis"        );
    if ( extra_flags & ITEM_MAGIC        ) strcat( buf, " magic"        );
    if ( extra_flags & ITEM_NODROP       ) strcat( buf, " nodrop"       );
    if ( extra_flags & ITEM_ANTI_GOOD    ) strcat( buf, " anti-good"    );
    if ( extra_flags & ITEM_ANTI_EVIL    ) strcat( buf, " anti-evil"    );
    if ( extra_flags & ITEM_ANTI_NEUTRAL ) strcat( buf, " anti-neutral" );
    if ( extra_flags & ITEM_BLESS        ) strcat( buf, " bless"        );
    if ( extra_flags & ITEM_NOREMOVE     ) strcat( buf, " noremove"     );
    if ( extra_flags & ITEM_INVENTORY    ) strcat( buf, " inventory"    );
    if ( extra_flags & ITEM_LOYAL        ) strcat( buf, " loyal"        );
    if ( extra_flags & ITEM_SHADOWPLANE  ) strcat( buf, " shadowplane"  );
    if ( extra_flags & ITEM_THROWN       ) strcat( buf, " thrown"       );
    if ( extra_flags & ITEM_KEEP         ) strcat( buf, " keep"         );
    if ( extra_flags & ITEM_VANISH       ) strcat( buf, " illusion"     );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}


int extra_name_bit( char* buf )
{
    if (!str_cmp( buf, "glow"        ) ) return ITEM_GLOW        ;
    if (!str_cmp( buf, "hum"         ) ) return ITEM_HUM         ;
    if (!str_cmp( buf, "thrown"      ) ) return ITEM_THROWN      ;
    if (!str_cmp( buf, "invis"       ) ) return ITEM_INVIS       ;
    if (!str_cmp( buf, "magic"       ) ) return ITEM_MAGIC       ;
    if (!str_cmp( buf, "nodrop"      ) ) return ITEM_NODROP      ;
    if (!str_cmp( buf, "bless"       ) ) return ITEM_BLESS       ;
    if (!str_cmp( buf, "anti-good"   ) ) return ITEM_ANTI_GOOD   ;
    if (!str_cmp( buf, "anti-evil"   ) ) return ITEM_ANTI_EVIL   ;
    if (!str_cmp( buf, "anti-neutral") ) return ITEM_ANTI_NEUTRAL;
    if (!str_cmp( buf, "noremove"    ) ) return ITEM_NOREMOVE    ;
    if (!str_cmp( buf, "inventory"   ) ) return ITEM_INVENTORY   ;
    if (!str_cmp( buf, "loyal"       ) ) return ITEM_LOYAL       ;
    if (!str_cmp( buf, "shadowplane" ) ) return ITEM_SHADOWPLANE ;
    if (!str_cmp( buf, "illusion"    ) ) return ITEM_VANISH      ;
    return 0;
}



/*
 * Returns the name of a wear bit.
 */
char *wear_bit_name( int wear )
{
    static char buf[512];

    buf[0] = '\0';
    if ( wear & ITEM_TAKE        ) strcat( buf, " take"       );
    if ( wear & ITEM_WEAR_FINGER ) strcat( buf, " finger"     );
    if ( wear & ITEM_WEAR_NECK   ) strcat( buf, " neck"       );
    if ( wear & ITEM_WEAR_BODY   ) strcat( buf, " body"       );
    if ( wear & ITEM_WEAR_HEAD   ) strcat( buf, " head"       );
    if ( wear & ITEM_WEAR_LEGS   ) strcat( buf, " legs"       );
    if ( wear & ITEM_WEAR_FEET   ) strcat( buf, " feet"       );
    if ( wear & ITEM_WEAR_HANDS  ) strcat( buf, " hands"      );
    if ( wear & ITEM_WEAR_ARMS   ) strcat( buf, " arms"       );
    if ( wear & ITEM_WEAR_ABOUT  ) strcat( buf, " about"      );
    if ( wear & ITEM_WEAR_WAIST  ) strcat( buf, " waist"      );
    if ( wear & ITEM_WEAR_WRIST  ) strcat( buf, " wrist"      );
    if ( wear & ITEM_WIELD       ) strcat( buf, " hold"       );
    if ( wear & ITEM_WEAR_FACE   ) strcat( buf, " face"       );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}


/*
 * Returns the bit, given a certain name.
 */
int wear_name_bit( char* buf )
{
    if (!str_cmp( buf, "take"       ) ) return ITEM_TAKE;
    if (!str_cmp( buf, "finger"     ) ) return ITEM_WEAR_FINGER;
    if (!str_cmp( buf, "neck"       ) ) return ITEM_WEAR_NECK;
    if (!str_cmp( buf, "body"       ) ) return ITEM_WEAR_BODY;
    if (!str_cmp( buf, "head"       ) ) return ITEM_WEAR_HEAD;
    if (!str_cmp( buf, "legs"       ) ) return ITEM_WEAR_LEGS;
    if (!str_cmp( buf, "feet"       ) ) return ITEM_WEAR_FEET;
    if (!str_cmp( buf, "hands"      ) ) return ITEM_WEAR_HANDS;
    if (!str_cmp( buf, "arms"       ) ) return ITEM_WEAR_ARMS;
    if (!str_cmp( buf, "about"      ) ) return ITEM_WEAR_ABOUT;
    if (!str_cmp( buf, "waist"      ) ) return ITEM_WEAR_WAIST;
    if (!str_cmp( buf, "wrist"      ) ) return ITEM_WEAR_WRIST;
    if (!str_cmp( buf, "hold"       ) ) return ITEM_WIELD;
    if (!str_cmp( buf, "face"       ) ) return ITEM_WEAR_FACE;
    return 0;
}



int act_name_bit( char* buf )
{
    if ( !str_cmp( buf, "sentinel"     ) ) return ACT_SENTINEL;
    if ( !str_cmp( buf, "scavenger"    ) ) return ACT_SCAVENGER;
    if ( !str_cmp( buf, "aggressive"   ) ) return ACT_AGGRESSIVE;
    if ( !str_cmp( buf, "stay_area"    ) ) return ACT_STAY_AREA;
    if ( !str_cmp( buf, "wimpy"        ) ) return ACT_WIMPY;
    if ( !str_cmp( buf, "mount"        ) ) return ACT_MOUNT;
    if ( !str_cmp( buf, "no_parts"     ) ) return ACT_NOPARTS;
    if ( !str_cmp( buf, "no_exp"       ) ) return ACT_NOEXP;
    if ( !str_cmp( buf, "skilled"      ) ) return ACT_SKILLED;
    return 0;
}



/*
 * Return bit vector
 */
int affect_name_bit( char* buf )
{
    if (!str_cmp( buf, "blind"         )) return AFF_BLIND;
    if (!str_cmp( buf, "invisible"     )) return AFF_INVISIBLE;
    if (!str_cmp( buf, "detect-evil"   )) return AFF_DETECT_EVIL;
    if (!str_cmp( buf, "detect-invis"  )) return AFF_DETECT_INVIS;
    if (!str_cmp( buf, "detect-magic"  )) return AFF_DETECT_MAGIC;
    if (!str_cmp( buf, "detect-hidden" )) return AFF_DETECT_HIDDEN;
    if (!str_cmp( buf, "shadowplane"   )) return AFF_SHADOWPLANE;
    if (!str_cmp( buf, "sanctuary"     )) return AFF_SANCTUARY;
    if (!str_cmp( buf, "faerie-fire"   )) return AFF_FAERIE_FIRE;
    if (!str_cmp( buf, "infrared"      )) return AFF_INFRARED;
    if (!str_cmp( buf, "curse"         )) return AFF_CURSE;
    if (!str_cmp( buf, "flaming"       )) return AFF_FLAMING;
    if (!str_cmp( buf, "poison"        )) return AFF_POISON;
    if (!str_cmp( buf, "protect"       )) return AFF_PROTECT;
    if (!str_cmp( buf, "ethereal"      )) return AFF_ETHEREAL;
    if (!str_cmp( buf, "sneak"         )) return AFF_SNEAK;
    if (!str_cmp( buf, "hide"          )) return AFF_HIDE;
    if (!str_cmp( buf, "sleep"         )) return AFF_SLEEP;
    if (!str_cmp( buf, "charm"         )) return AFF_CHARM;
    if (!str_cmp( buf, "flying"        )) return AFF_FLYING;
    if (!str_cmp( buf, "pass-door"     )) return AFF_PASS_DOOR;
    if (!str_cmp( buf, "shadowsight"   )) return AFF_SHADOWSIGHT;
    if (!str_cmp( buf, "rotting"       )) return AFF_ROT;
    if (!str_cmp( buf, "coward"        )) return AFF_FEAR;
    return 0;
}



int affect_name_loc( char* name )
{
    if ( !str_cmp( name, "none"          ) ) return APPLY_NONE;
    if ( !str_cmp( name, "strength"      ) ) return APPLY_STR;
    if ( !str_cmp( name, "dexterity"     ) ) return APPLY_DEX;
    if ( !str_cmp( name, "intelligence"  ) ) return APPLY_INT;
    if ( !str_cmp( name, "wisdom"        ) ) return APPLY_WIS;
    if ( !str_cmp( name, "constitution"  ) ) return APPLY_CON;
    if ( !str_cmp( name, "height"        ) ) return APPLY_HEIGHT;
    if ( !str_cmp( name, "weight"        ) ) return APPLY_WEIGHT;
    if ( !str_cmp( name, "mana"          ) ) return APPLY_MANA;
    if ( !str_cmp( name, "hp"            ) ) return APPLY_HIT;
    if ( !str_cmp( name, "move"          ) ) return APPLY_MOVE;
    if ( !str_cmp( name, "ac"            ) ) return APPLY_AC;
    if ( !str_cmp( name, "hit"           ) ) return APPLY_HITROLL;
    if ( !str_cmp( name, "dam"           ) ) return APPLY_DAMROLL;
    if ( !str_cmp( name, "blood_pool"    ) ) return APPLY_BLOOD_MAX;
    if ( !str_cmp( name, "blood_potency" ) ) return APPLY_BLOOD_POT;
    return APPLY_NONE;
}



int wear_name_loc( char *buf )
{
    if ( !str_cmp( buf, "light" ) )     return WEAR_LIGHT;
    if ( !str_cmp( buf, "lfinger" ) )   return WEAR_FINGER_L;
    if ( !str_cmp( buf, "rfinger" ) )   return WEAR_FINGER_R;
    if ( !str_cmp( buf, "neck1" ) )     return WEAR_NECK_1;
    if ( !str_cmp( buf, "neck2" ) )     return WEAR_NECK_2;
    if ( !str_cmp( buf, "body" ) )      return WEAR_BODY;
    if ( !str_cmp( buf, "head" ) )      return WEAR_HEAD;
    if ( !str_cmp( buf, "legs" ) )      return WEAR_LEGS;
    if ( !str_cmp( buf, "feet" ) )      return WEAR_FEET;
    if ( !str_cmp( buf, "hands" ) )     return WEAR_HANDS;
    if ( !str_cmp( buf, "arms" ) )      return WEAR_ARMS;
    if ( !str_cmp( buf, "about" ) )     return WEAR_ABOUT;
    if ( !str_cmp( buf, "waist" ) )     return WEAR_WAIST;
    if ( !str_cmp( buf, "lwrist" ) )    return WEAR_WRIST_L;
    if ( !str_cmp( buf, "rwrist" ) )    return WEAR_WRIST_R;
    if ( !str_cmp( buf, "hold" ) )      return WEAR_WIELD;
    if ( !str_cmp( buf, "face" ) )      return WEAR_FACE;
    return WEAR_NONE;
}



/*
 * Return ascii name of wear location.
 */
char *wear_loc_name( int wearloc )
{
    switch( wearloc )
    {
               default: return "unknown";
        case WEAR_NONE: return "in the inventory";
       case WEAR_LIGHT: return "as a light";
    case WEAR_FINGER_L: return "on the left finger";
    case WEAR_FINGER_R: return "on the right finger";
      case WEAR_NECK_1: return "around the neck (1)";
      case WEAR_NECK_2: return "around the neck (2)";
        case WEAR_BODY: return "on the body";
        case WEAR_HEAD: return "over the head";
        case WEAR_LEGS: return "on the legs";
        case WEAR_FEET: return "on the feet";
       case WEAR_HANDS: return "on the hands";
        case WEAR_ARMS: return "on the arms";
       case WEAR_ABOUT: return "about the shoulders";
       case WEAR_WAIST: return "around the waist";
     case WEAR_WRIST_L: return "on the left wrist";
     case WEAR_WRIST_R: return "on the right wrist";
       case WEAR_WIELD: return "in the hand";
        case WEAR_FACE: return "on the face";
    }
}



char *act_bit_name( int act )
{
    static char buf[512];

    buf[0] = '\0';
    if ( act & ACT_IS_NPC     ) strcat( buf, " npc"           );
    if ( act & ACT_SENTINEL   ) strcat( buf, " sentinel"      );
    if ( act & ACT_SCAVENGER  ) strcat( buf, " scavenger"     );
    if ( act & ACT_AGGRESSIVE ) strcat( buf, " aggressive"    );
    if ( act & ACT_STAY_AREA  ) strcat( buf, " stay_area"     );
    if ( act & ACT_WIMPY      ) strcat( buf, " wimpy"         );
    if ( act & ACT_PET        ) strcat( buf, " pet"           );
    if ( act & ACT_TRAIN      ) strcat( buf, " trainer"       );
    if ( act & ACT_PRACTICE   ) strcat( buf, " practitioner"  );
    if ( act & ACT_MOUNT      ) strcat( buf, " mount"         );
    if ( act & ACT_NOPARTS    ) strcat( buf, " no_parts"      );
    if ( act & ACT_NOEXP      ) strcat( buf, " no_ext"        );
    if ( act & ACT_SKILLED    ) strcat( buf, " skilled"       );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}



int get_weapon_type( char *arg )
{
    if (!str_cmp( arg, "hit"	) ) return 0;
    if (!str_cmp( arg, "slice"	) ) return 1;
    if (!str_cmp( arg, "stab"	) ) return 2;
    if (!str_cmp( arg, "slash"	) ) return 3;
    if (!str_cmp( arg, "whip"	) ) return 4;
    if (!str_cmp( arg, "claw"	) ) return 5;
    if (!str_cmp( arg, "blast"	) ) return 6;
    if (!str_cmp( arg, "pound"	) ) return 7;
    if (!str_cmp( arg, "crush"	) ) return 8;
    if (!str_cmp( arg, "grep"	) ) return 9;
    if (!str_cmp( arg, "bite"	) ) return 10;
    if (!str_cmp( arg, "pierce"	) ) return 11;
    if (!str_cmp( arg, "suction") ) return 12;
    if (!str_cmp( arg, "chop"	) ) return 13;
    return 0;
}



/*
 * Use this in dam_message in fight.c if you like.
 */
char * get_type_weapon( int arg )
{
    switch ( arg )
    {
    case 0:  return "hit";
    case 1:  return "slice";
    case 2:  return "stab";
    case 3:  return "slash";
    case 4:  return "whip";
    case 5:  return "claw";
    case 6:  return "blast";
    case 7:  return "pound";
    case 8:  return "crush";
    case 9:  return "grep";
    case 10: return "bite";
    case 11: return "pierce";
    case 12: return "suction";
    case 13: return "chop";
    }

    bug( "Get_type_weapon: unknown type %d.", arg );
    return "(unknown)";
}



int get_container_flags( char *arg )
{
    if (!str_cmp( arg, "closeable"	) ) return 1;
    if (!str_cmp( arg, "pickproof"	) ) return 2;
    if (!str_cmp( arg, "closed"		) ) return 4;
    if (!str_cmp( arg, "locked"		) ) return 8;
    return 0;
}



char * get_flags_container( int arg )
{
    static char buf[512];

    buf[0] = '\0';
    if ( arg & 1 ) strcat( buf, " closeable"	);
    if ( arg & 2 ) strcat( buf, " pickproof"	);
    if ( arg & 4 ) strcat( buf, " closed"	);
    if ( arg & 8 ) strcat( buf, " locked"	);
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}



int get_liquid_type( char *arg )
{
    if (!str_cmp( arg, "water"			) ) return 0;
    if (!str_cmp( arg, "beer"			) ) return 1;
    if (!str_cmp( arg, "wine"			) ) return 2;
    if (!str_cmp( arg, "ale"			) ) return 3;
    if (!str_cmp( arg, "dark_ale"		) ) return 4;
    if (!str_cmp( arg, "whisky"			) ) return 5;
    if (!str_cmp( arg, "lemonade"		) ) return 6;
    if (!str_cmp( arg, "firebreather"		) ) return 7;
    if (!str_cmp( arg, "local_specialty"	) ) return 8;
    if (!str_cmp( arg, "slime_mold_juice"	) ) return 9;
    if (!str_cmp( arg, "milk"			) ) return 10;
    if (!str_cmp( arg, "tea"			) ) return 11;
    if (!str_cmp( arg, "coffee"			) ) return 12;
    if (!str_cmp( arg, "blood"			) ) return 13;
    if (!str_cmp( arg, "salt_water"		) ) return 14;
    if (!str_cmp( arg, "cola"			) ) return 15;
    return 0;
}



char * get_type_liquid( int arg )
{
    switch ( arg )
    {
    case 0:  return "water";
    case 1:  return "beer";
    case 2:  return "wine";
    case 3:  return "ale";
    case 4:  return "dark_ale";
    case 5:  return "whisky";
    case 6:  return "lemonade";
    case 7:  return "firebreather";
    case 8:  return "local_specialty";
    case 9:  return "slime_mold_juice";
    case 10: return "milk";
    case 11: return "tea";
    case 12: return "coffee";
    case 13: return "blood";
    case 14: return "sale_water";
    case 15: return "cola";
    }

    bug( "Get_type_liquid: unknown type %d.", arg );
    return "(unknown)";
}

/* Shim-mod 02/01/98 : Bit names for pstat :) */

char * mod_bit_name( int arg )
{
    static char buf[512];

    buf[0] = '\0';

    if ( arg & MOD_TUSKS	) strcat( buf, " tusks"		);
    if ( arg & MOD_HORNS	) strcat( buf, " horns"		);
    if ( arg & MOD_WINGS	) strcat( buf, " wings"		);
    if ( arg & MOD_UNFOLDED	) strcat( buf, " unfolded"	);
    if ( arg & MOD_FINS		) strcat( buf, " fins"		);
    if ( arg & MOD_TAIL		) strcat( buf, " tail"		);
    if ( arg & MOD_EXOSKELETON	) strcat( buf, " exoskeleton"	);
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}


char * plr_bit_name( int arg )
{
    static char buf[512];

    buf[0] = '\0';

    if ( arg & PLR_IS_NPC	) strcat( buf, " npc"		);
    if ( arg & PLR_DEMON	) strcat( buf, " demon"		);
    if ( arg & PLR_CHAMPION	) strcat( buf, " champion"	);
    if ( arg & PLR_AUTOEXIT	) strcat( buf, " autoexit"	);
    if ( arg & PLR_AUTOLOOT	) strcat( buf, " autoloot"	);
    if ( arg & PLR_AUTOSAC	) strcat( buf, " autosac"	);
    if ( arg & PLR_BLANK	) strcat( buf, " blank"		);
    if ( arg & PLR_BRIEF	) strcat( buf, " brief"		);
    if ( arg & PLR_COMBINE	) strcat( buf, " combine"	);
    if ( arg & PLR_PROMPT	) strcat( buf, " prompt"	);
    if ( arg & PLR_TELNET_GA	) strcat( buf, " telnet_ga"	);
    if ( arg & PLR_HOLYLIGHT	) strcat( buf, " holylight"	);
    if ( arg & PLR_WIZINVIS	) strcat( buf, " wizinvis"	);
    if ( arg & PLR_ANSI		) strcat( buf, " ansi"		);
    if ( arg & PLR_SILENCE	) strcat( buf, " silenced"	);
    if ( arg & PLR_NO_EMOTE	) strcat( buf, " no_emote"	);
    if ( arg & PLR_NO_TELL	) strcat( buf, " no_tell"	);
    if ( arg & PLR_LOG		) strcat( buf, " log"		);
    if ( arg & PLR_RETIRE	) strcat( buf, " retire"	);
    if ( arg & PLR_FREEZE	) strcat( buf, " freeze"	);
    if ( arg & PLR_WOLFMAN	) strcat( buf, " wolfman"	);
    if ( arg & PLR_WEREWOLF	) strcat( buf, " werewolf"	);
    if ( arg & PLR_GODLESS	) strcat( buf, " godless"	);
    if ( arg & PLR_LAST_AGG	) strcat( buf, " last_agg"	);
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char * extra_plr_bit_name( int arg )
{
    static char buf[512];

    buf[0] = '\0';

    if ( arg & EXTRA_SIRE		) strcat( buf, " sire" );
    if ( arg & EXTRA_PRINCE		) strcat( buf, " prince" );
    if ( arg & EXTRA_TRUSTED		) strcat( buf, " q_trusted" );
    if ( arg & EXTRA_NEWPASS		) strcat( buf, " newpass" );
    if ( arg & EXTRA_OSWITCH		) strcat( buf, " oswitch" );
    if ( arg & EXTRA_SWITCH		) strcat( buf, " switch" );
    if ( arg & EXTRA_FAKE_CON		) strcat( buf, " fake_con" );
    if ( arg & TIED_UP			) strcat( buf, " tied_up" );
    if ( arg & GAGGED			) strcat( buf, " gagged" );
    if ( arg & BLINDFOLDED		) strcat( buf, " blindfolded" );
    if ( arg & EXTRA_PSAY		) strcat( buf, " psay" );
    if ( arg & EXTRA_DONE		) strcat( buf, " non_virgin" );
    if ( arg & EXTRA_EXP		) strcat( buf, " got_exp" );
    if ( arg & EXTRA_PREGNANT		) strcat( buf, " pregnant" );
    if ( arg & EXTRA_LABOUR		) strcat( buf, " labour" );
    if ( arg & EXTRA_BORN		) strcat( buf, " born" );
    if ( arg & EXTRA_PROMPT		) strcat( buf, " prompt" );
    if ( arg & EXTRA_MARRIED		) strcat( buf, " married" );
    if ( arg & EXTRA_DYING		) strcat( buf, " dying" );
    if ( arg & EXTRA_NON_NEWBIE	) strcat( buf, " non_newbie" );
    if ( arg & EXTRA_CALL_ALL		) strcat( buf, " call_all" );
    if ( arg & EXTRA_CAN_SEE		) strcat( buf, " can_see" );
    if ( arg & EXTRA_CLEARED_EQ	) strcat( buf, " cleared_eq" );
    if ( arg & EXTRA_AWE		) strcat( buf, " awe" );
    if ( arg & EXTRA_NO_ACTION	) strcat( buf, " no_action" );
    if ( arg & EXTRA_ILLUSION		) strcat( buf, " illusion" );
    if ( arg & EXTRA_VICISSITUDE	) strcat( buf, " visissitude" );
    if ( arg & EXTRA_INVIS_EMAIL	) strcat( buf, " invis_email" );
    if ( arg & EXTRA_AUTHORISED	) strcat( buf, " authorised" );
    if ( arg & EXTRA_AFK                ) strcat( buf, " afk" );
    if ( arg & EXTRA_NEWBIEPACK         ) strcat( buf, " newbiepack" );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char * more_bit_name( int arg )
{
    static char buf[512];


    buf[0] = '\0';
    if ( arg & MORE_FTALK		) strcat( buf, " ftalk"		);
    if ( arg & MORE_NPC			) strcat( buf, " npc"		);
    if ( arg & MORE_ANTITRIBU		) strcat( buf, " antitribu"	);
    if ( arg & MORE_MUMMIFIED		) strcat( buf, " mumified"	);
    if ( arg & MORE_DARKNESS		) strcat( buf, " darkness"	);
    if ( arg & MORE_SILENCE		) strcat( buf, " silence"	);
    if ( arg & MORE_PLASMA		) strcat( buf, " plasma"	);
    if ( arg & MORE_ASHES		) strcat( buf, " ashes"		);
    if ( arg & MORE_GUARDIAN		) strcat( buf, " guardian"	);
    if ( arg & MORE_LOYAL		) strcat( buf, " loyal"		);
    if ( arg & MORE_SINGING		) strcat( buf, " singing"	);
    if ( arg & MORE_LEADER		) strcat( buf, " leader"	);
    if ( arg & MORE_HAND_FLAME		) strcat( buf, " hand_flame"	);
    if ( arg & MORE_SPIRIT		) strcat( buf, " spirit"	);
    if ( arg & MORE_STAKED		) strcat( buf, " staked"	);
    if ( arg & MORE_DEFLECTION		) strcat( buf, " deflection"	);
    if ( arg & MORE_PROJECTION		) strcat( buf, " projection"	);
    if ( arg & MORE_CHAMPION		) strcat( buf, " champion"	);
    if ( arg & MORE_REINA		) strcat( buf, " reina"		);
    if ( arg & MORE_EVILEYE		) strcat( buf, " evileye"	);
    if ( arg & MORE_FLAMES		) strcat( buf, " flames"	);
    if ( arg & MORE_COURAGE		) strcat( buf, " courage"	);
    if ( arg & MORE_ANGER		) strcat( buf, " anger"		);
    if ( arg & MORE_ANIMAL_MASTER	) strcat( buf, " master"	);
    if ( arg & MORE_INFORM		) strcat( buf, " inform"	);
    if ( arg & MORE_CALM		) strcat( buf, " calm"		);
    if ( arg & MORE_BRISTLES		) strcat( buf, " bristles"	);
    if ( arg & MORE_OBEAH		) strcat( buf, " obeah"		);
    if ( arg & MORE_NOPAIN		) strcat( buf, " nopain"	);
    if ( arg & MORE_NEUTRAL		) strcat( buf, " neutral"	);
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char * newbits_bit_name( int arg )
{
    static char buf[512];


    buf[0] = '\0';
    if ( arg & NEWBIT_SWETALK		) strcat( buf, " swetalk");
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}


char * get_position_name( int arg )
{
    switch( arg )
    {
	case 0: return "dead";
	case 1: return "mortal";
	case 2: return "incap";
	case 3: return "stunned";
	case 4: return "sleeping";
	case 5: return "meditating";
	case 6: return "sitting";
	case 7: return "resting";
	case 8: return "fighting";
	case 9: return "standing";
    }
    bug( "Get_position_name: unknown type %d.", arg );
    return "(unknown)";
}

/*
 * Itemaffect bit names :)
 */
char * itemaffect_bit_name( int arg )
{
    static char buf[512];

    buf[0] = '\0';

    if ( arg & ITEMA_SHOCKSHIELD	) strcat( buf, " shield_shock"	);
    if ( arg & ITEMA_FIRESHIELD		) strcat( buf, " shield_fire"	);
    if ( arg & ITEMA_ICESHIELD		) strcat( buf, " shield_ice"	);
    if ( arg & ITEMA_ACIDSHIELD		) strcat( buf, " shield_acid"	);
    if ( arg & ITEMA_DBPASS		) strcat( buf, " dbpass"	);
    if ( arg & ITEMA_CHAOSSHIELD	) strcat( buf, " shield_chaos"	);
    if ( arg & ITEMA_ARTIFACT		) strcat( buf, " artifact"	);
    if ( arg & ITEMA_REGENERATE		) strcat( buf, " regen"		);
    if ( arg & ITEMA_SPEED		) strcat( buf, " speed"		);
    if ( arg & ITEMA_VORPAL		) strcat( buf, " vorpal"	);
    if ( arg & ITEMA_PEACE		) strcat( buf, " peace"		);
    if ( arg & ITEMA_RIGHT_SILVER	) strcat( buf, " r_silver"	);
    if ( arg & ITEMA_LEFT_SILVER	) strcat( buf, " l_silver"	);
    if ( arg & ITEMA_REFLECT		) strcat( buf, " shield_dark"	);
    if ( arg & ITEMA_RESISTANCE		) strcat( buf, " resis"		);
    if ( arg & ITEMA_VISION		) strcat( buf, " vision"	);
    if ( arg & ITEMA_STALKER		) strcat( buf, " stalker"	);
    if ( arg & ITEMA_VANISH		) strcat( buf, " vanish"	);
    if ( arg & ITEMA_RAGER		) strcat( buf, " rager"		);
    if ( arg & ITEMA_SHADES		) strcat( buf, " shades"	);
    if ( arg & ITEMA_SUNSHIELD		) strcat( buf, " sun_shield"	);
    if ( arg & ITEMA_RIGHT_AGG		) strcat( buf, " r_agg"		);
    if ( arg & ITEMA_LEFT_AGG		) strcat( buf, " l_agg"		);
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char * display_demon_body( int c_power, int o_power )
{
    static char buf[512];

    buf[0] = '\0';

    if ( c_power & DEM_FANGS		) strcat( buf, " fangs"		);
    if ( c_power & DEM_CLAWS		) strcat( buf, " claws"		);
    if ( c_power & DEM_HOOVES		) strcat( buf, " hooves"	);
    if ( c_power & DEM_WINGS		) strcat( buf, " wings"		);
    return ( buf[0] != '\0' ) ? buf+1 : "(None)";
}

char * display_demon_changeling( int c_power, int o_power, int obj_num )
{
    static char buf[512];

    buf[0] = '\0';

    if ( c_power & DEM_DEMONFORM	) strcat( buf, " demonform"	);
    if ( obj_num == 29662		) strcat( buf, " longsword"	);
    if ( obj_num == 29663		) strcat( buf, " shortsword"	);
    return ( buf[0] != '\0' ) ? buf+1 : "(None)";
}

char * display_demon_weapon( int c_power, int o_power )
{
    static char buf[512];

    buf[0] = '\0';

    if ( o_power & DEM_MOVE		) strcat( buf, " move"		);
    if ( o_power & DEM_LEAP		) strcat( buf, " leap"		);
    if ( o_power & DEM_MAGIC		) strcat( buf, " magic"		);
    return ( buf[0] != '\0' ) ? buf+1 : "(None)";
}

char * display_demon_fight( int c_power, int o_power )
{
    static char buf[512];

    buf[0] = '\0';

    if ( c_power & DEM_MIGHT		) strcat( buf, " might"		);
    if ( c_power & DEM_TOUGH		) strcat( buf, " tough"		);
    if ( c_power & DEM_SPEED		) strcat( buf, " speed"		);
    if ( c_power & DEM_INFERNO		) strcat( buf, " inferno"	);
//    if ( o_power & DEM_JAIL		) strcat( buf, " jail"		);
//    if ( o_power & DEM_DEMONFIRE	) strcat( buf, " demonfire"	);
//    if ( o_power & DEM_GROUP_BONUS	) strcat( buf, " group_bonus"	);
    return ( buf[0] != '\0' ) ? buf+1 : "(None)";
}

char * display_demon_vision( int c_power, int o_power )
{
    static char buf[512];

    buf[0] = '\0';

    if ( c_power & DEM_SHADOWSIGHT	) strcat( buf, " umbral"	);
    if ( c_power & DEM_SCRY		) strcat( buf, " scry"		);
    if ( c_power & DEM_EYESPY		) strcat( buf, " eyespy"	);
    return ( buf[0] != '\0' ) ? buf+1 : "(None)";
}

char * display_demon_misc( int c_power, int o_power )
{
    static char buf[512];

    buf[0] = '\0';

    if ( o_power & DEM_LIFESPAN		) strcat( buf, " lifespan"	 );
    if ( c_power & DEM_TRAVEL		) strcat( buf, " travel"	 );
    if ( c_power & DEM_FIREWALL		) strcat( buf, " firewall"	 );
    if ( c_power & DEM_IMP		) strcat( buf, " imp"		 );
//    if ( o_power & DEM_FIRE_IMMUNE	) strcat( buf, " fire_immune"	 );
//    if ( o_power & DEM_IMMUNE_DEMFIRE	) strcat( buf, " demfire_immune" );
    return ( buf[0] != '\0' ) ? buf+1 : "(None)";
}

char * display_demon_daimoinon( int c_power, int o_power )
{
    static char buf[512];

    buf[0] = '\0';

    if ( o_power & DEM_GUARDIAN		) strcat( buf, " guardian"	);
    if ( o_power & DEM_FEAR		) strcat( buf, " fear"		);
    if ( o_power & DEM_PORTAL		) strcat( buf, " portal"	);
    if ( o_power & DEM_CURSE		) strcat( buf, " curse"		);
    if ( o_power & DEM_TWIST		) strcat( buf, " soultwist"	);
//    if ( o_power & DEM_SERVANT		) strcat( buf, " servant"	);
    return ( buf[0] != '\0' ) ? buf+1 : "(None)";
}
