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

void spell_true_sight( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = ( CHAR_DATA * ) vo;
    AFFECT_DATA af;

    if ( magic_immune( ch, victim, FALSE ) )
    {
	send_to_char( "Nothing happens.", ch );
	return;
    }

    if ( IS_AFFECTED( victim, AFF_TRUE_SIGHT ) )
    {
	send_to_char( "You already have true sight case on you.\n\r", victim );
	return;
    }

    af.type		= sn;
    af.duration		= level / 12;
    af.location		= APPLY_NONE;
    af.modifier		= 0;
    af.bitvector	= AFF_TRUE_SIGHT;
    affect_to_char( victim, &af );
    send_to_char( "Your vision improves.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_aura_no_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = ( CHAR_DATA * ) vo;
    AFFECT_DATA af;

    if ( magic_immune( ch, victim, FALSE ) )
    {
	send_to_char( "Nothing happens.", ch );
	return;
    }

    if ( IS_AFFECTED( victim, AFF_AURA_NO_MAGIC ) )
    {
	send_to_char( "You already have an aura of no magic.\n\r", victim );
	return;
    }

    af.type		= sn;
    af.duration		= level / 24;
    af.location		= APPLY_NONE;
    af.modifier		= 0;
    af.bitvector	= AFF_AURA_NO_MAGIC;
    affect_to_char( victim, &af );
    send_to_char( "An aura of no magic surrounds you.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_read_aura( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA	*victim = ( CHAR_DATA * )vo;
    char	buf[MAX_STRING_LENGTH];
    int		con_hit, con_dam, con_ac, con_hp, con_mana, con_move;
    int		con_str, con_int, con_dex, con_wis, con_con;
    int		con_max_hp, con_max_mana, con_max_move;

    if ( magic_immune( ch, victim, FALSE ) )
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    act( "You examine $N intently.", ch, NULL, victim, TO_CHAR );
    act( "$n examines you intently.", ch, NULL, victim, TO_VICT );
    act( "$n examines $N intently.", ch, NULL, victim, TO_NOTVICT );

    sprintf( buf, "Name : %s.\n\r",
	IS_NPC( victim ) ? victim->short_descr : victim->name );
    send_to_char( buf, ch );

    sprintf( buf, "$E is %s.",
	IS_NPC( victim )	? "a NPC"	:
	IS_HERO( victim )	? "an avatar"	:
	IS_IMMORTAL( victim )	? "an immortal" : "a mortal" );
    act( buf, ch, NULL, victim, TO_CHAR );

    if ( !IS_NPC( victim ) && IS_HERO( victim ) && victim->class > 0 )
    {
	switch( victim->class )
	{
	    default :
		sprintf( buf, " " );
		break;
	    case CLASS_VAMPIRE:
		sprintf( buf, "$E is a vampire. Clan : %s. Gen : %d.",
		    strlen( victim->clan ) < 2 ? "Caitiff" : victim->clan,
		    victim->vampgen );
		break;
	    case CLASS_WEREWOLF:
		sprintf( buf, "$E is a werewolf. Tribe : %s. Gen : %d.",
		    strlen( victim->clan ) < 2 ? "Ronin" : victim->clan,
		    victim->vampgen );
		break;
	    case CLASS_ABOMINATION:
		sprintf( buf, "$E is an abomination%s",
		    victim->vampgen <= 3 ? " leader." : "." );
		break;
	    case CLASS_MAGE:
		sprintf( buf, "$E is a %s %s.",
		    victim->pcdata->runes[0] == PURPLE_MAGIC  ?	"air" :
		    victim->pcdata->runes[0] == RED_MAGIC     ?	"fire"    :
		    victim->pcdata->runes[0] == BLUE_MAGIC    ?	"water"   :
		    victim->pcdata->runes[0] == GREEN_MAGIC   ?	"earth"  :
								"spirit",
		    victim->level == LEVEL_APPRENTICE ? "apprentice" :
		    victim->level == LEVEL_MAGE	      ? "mage"       :
							"archmage" );
		break;
	    case CLASS_LICH:
		sprintf( buf, "$E is a lich%s",
		    victim->vampgen <= 3 ? " leader." : "." );
		break;
	    case CLASS_SKINDANCER:
		sprintf( buf, "$E is a skindancer%s",
		    victim->vampgen <= 3 ? " leader." : "." );
		break;
	    case CLASS_DEMON:
		sprintf( buf, "$E is a %s.",
		    IS_SET( victim->act, PLR_DEMON ) ? "demon lord"    :
		    IS_EXTRA( victim, EXTRA_PRINCE ) ? "demon prince"  :
		    IS_EXTRA( victim, EXTRA_SIRE )   ? "greater demon" :
						       "lesser demon" );
		break;
	    case CLASS_BAALI:
		sprintf( buf, "$E is a baali%s",
		    victim->vampgen <= 3 ? " leader." : "." );
		break;
	    case CLASS_BLACK_SPIRAL_DANCER:
		sprintf( buf, "$E is a bsd%s",
		    victim->vampgen <= 3 ? " leader." : "." );
		break;
	    case CLASS_NEPHANDI:
		sprintf( buf, "$E is a nephandi%s",
		    victim->vampgen <= 3 ? " leader." : "." );
		break;
	    case CLASS_GHOUL:
		sprintf( buf, "$E is a ghoul." );
		break;
	}
	act( buf, ch, NULL, victim, TO_CHAR );
    }

    sprintf( buf, "Status : %d. Alignment : %d\n\r",
	victim->race, victim->alignment );
    send_to_char( buf, ch );

    if ( !IS_NPC( victim ) && IS_EXTRA( victim, EXTRA_FAKE_CON ) )
    {
	con_hit		= victim->pcdata->fake_hit;
	con_dam		= victim->pcdata->fake_dam;
	con_ac		= victim->pcdata->fake_ac;
	con_str		= victim->pcdata->fake_stats[STAT_STR];
	con_int		= victim->pcdata->fake_stats[STAT_INT];
	con_dex		= victim->pcdata->fake_stats[STAT_DEX];
	con_wis		= victim->pcdata->fake_stats[STAT_WIS];
	con_con		= victim->pcdata->fake_stats[STAT_CON];
	con_hp		= victim->pcdata->fake_hp;
	con_mana	= victim->pcdata->fake_mana;
	con_move	= victim->pcdata->fake_move;
	con_max_hp	= victim->pcdata->fake_max_hp;
	con_max_mana	= victim->pcdata->fake_max_mana;
	con_max_move	= victim->pcdata->fake_max_move;
    }
    else
    {
	con_hit		= char_hitroll( victim );
	con_dam		= char_damroll( victim );
	con_ac		= char_ac( victim );
	con_str		= get_curr_str( victim );
	con_int		= get_curr_int( victim );
	con_dex		= get_curr_dex( victim );
	con_wis		= get_curr_wis( victim );
	con_con		= get_curr_con( victim );
	con_hp		= victim->hit;
	con_mana	= victim->mana;
	con_move	= victim->move;
	con_max_hp	= victim->max_hit;
	con_max_mana	= victim->max_mana;
	con_max_move	= victim->max_move;
    }

    sprintf( buf, "Hp : %d/%d. Mana : %d/%d. Move : %d/%d.\n\r",
	con_hp, con_max_hp, con_mana, con_max_mana, con_move, con_max_move );
    send_to_char( buf, ch );

    sprintf( buf, "Str : %d. Dex : %d. Int : %d. Wis : %d. Con : %d.\n\r",
	con_str, con_dex, con_int, con_wis, con_con );
    send_to_char( buf, ch );

    sprintf( buf, "Hit : %d. Dam : %d. Ac : %d.\n\r",
	con_hit, con_dam, con_ac );
    send_to_char( buf, ch );

    return;
}

void spell_enchant_armour( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA	*obj = ( OBJ_DATA * ) vo;
    AFFECT_DATA *paf;

    if ( obj->item_type != ITEM_ARMOR )
    {
	send_to_char( "This spell only enchants armour.\n\r", ch );
	return;
    }

    if ( IS_SET( obj->quest, QUEST_ENCHANTED )
    ||	 IS_SET( obj->quest, QUEST_ARTIFACT )
    ||	 IS_SET( obj->quest, QUEST_RELIC )
    ||	 obj->chobj != NULL )
    {
	send_to_char( "You are unable to enchant this piece of armour.\n\r", ch );
	return;
    }

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf));
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }

    paf->type		= -1;
    paf->duration	= -1;
    paf->location	= APPLY_HITROLL;
    paf->modifier	= level / 10;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf));
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }

    paf->type		= -1;
    paf->duration	= -1;
    paf->location	= APPLY_DAMROLL;
    paf->modifier	= level / 20;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    SET_BIT( obj->quest, QUEST_ENCHANTED );

    if ( IS_GOOD( ch ) )
    {
	SET_BIT( obj->extra_flags, ITEM_ANTI_EVIL );
	act( "$p glows blue.", ch, obj, NULL, TO_CHAR );
	act( "$p glows blue.", ch, obj, NULL, TO_ROOM );
    }
    else if ( IS_EVIL( ch ) )
    {
	SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );
	act( "$p glows red.", ch, obj, NULL, TO_CHAR );
	act( "$p glows red.", ch, obj, NULL, TO_ROOM );
    }
    else
    {
	SET_BIT( obj->extra_flags, ITEM_ANTI_EVIL );
	SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );
	act( "$p glows yellow.", ch, obj, NULL, TO_CHAR );
	act( "$p glows yellow.", ch, obj, NULL, TO_ROOM );
    }
    return;
}

void spell_move_to_hp( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int mv = ( level * 30 );

    if ( ch->hit >= ch->max_hit )
    {
	send_to_char( "You can't get your hit points any higher.\n\r", ch );
	return;
    }

    if ( magic_immune( ch, victim, FALSE ) )
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    if ( victim->move <= 0 )
    {
	send_to_char( "They are too exhusted.\n\r", ch );
	return;
    }

    if ( ch != victim )
    {
    	act( "Streams of white light pass from $N's body into your body.", ch, NULL, victim, TO_CHAR );
    	act( "Streams of white light pass from your body into $n's body.", ch, NULL, victim, TO_VICT );
    	act( "Streams of white light pass from $N's body into $n's body.", ch, NULL, victim, TO_NOTVICT );
    }
    else
    {
	act( "Your body glows bright white.", ch, NULL, NULL, TO_CHAR );
	act( "$n's body glows bright white.", ch, NULL, NULL, TO_ROOM );
    }

    if ( victim->move < mv )
	mv = victim->move;

    victim->move -= mv;
    if ( victim->move < 0 )
	victim->move = 0;

    ch->hit += ( mv / 2 ) * 3;
    if ( ch->hit >= ch->max_hit )
	ch->hit = ch->max_hit;

    return;
}

void spell_mana_to_hp( int sn, int level, CHAR_DATA *ch, void *vo )
{
    int mn = ( level * 30 );

    act( "Your body glows bright blue.", ch, NULL, NULL, TO_CHAR );
    act( "$n's body glows bright blue.", ch, NULL, NULL, TO_ROOM );

    if ( ch->hit >= ( ch->max_mana * 2 ) )
    {
	STC( "Nothing Happens.\n\r", ch );
	return;
    }

    if ( ch->mana < mn )
	mn = ch->mana;

    ch->mana -= mn;
    if ( ch->mana < 0 )
	ch->mana = 0;

    ch->hit += ( mn / 2 ) * 3;

    if ( ch->hit >= ( ch->max_mana * 2 ) )
	ch->hit = ch->max_mana * 2;

    return;
}
