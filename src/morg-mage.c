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
#include <unistd.h>
#include "merc.h"

#define	TYPE_RUNE	0
#define	TYPE_GLYPH	1
#define	TYPE_SIGIL	2

int	count_stones		args( ( int stones, int type ) );

DO_COM( do_learn )
{
    char	arg1[MAX_INPUT_LENGTH];
    char	arg2[MAX_INPUT_LENGTH];
    char	buf[MAX_STRING_LENGTH];
    int		rune;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC( ch ) ) return;

    if ( !IS_MAGE( ch ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax : learn [rune] [type]\n\r", ch );
	send_to_char( "\n\rRune being one of:\n\r", ch );
	send_to_char( "  rune glyph sigil\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "rune" ) )
    {
	if	( !str_cmp( arg2, "fire"   ) )	rune = RUNE_FIRE;
	else if	( !str_cmp( arg2, "air"    ) )	rune = RUNE_AIR;
	else if ( !str_cmp( arg2, "earth"  ) )	rune = RUNE_EARTH;
	else if ( !str_cmp( arg2, "water"  ) )	rune = RUNE_WATER;
	else if ( !str_cmp( arg2, "dark"   ) )	rune = RUNE_DARK;
	else if ( !str_cmp( arg2, "light"  ) )	rune = RUNE_LIGHT;
	else if ( !str_cmp( arg2, "life"   ) )	rune = RUNE_LIFE;
	else if ( !str_cmp( arg2, "death"  ) )	rune = RUNE_DEATH;
	else if ( !str_cmp( arg2, "mind"   ) )	rune = RUNE_MIND;
	else if ( !str_cmp( arg2, "spirit" ) )	rune = RUNE_SPIRIT;
	else
	{
	    do_learn( ch, "" );
	    send_to_char( "\n\rRune can be one of:\n\r", ch );
	    send_to_char( "  fire air earth water dark\n\r", ch );
	    send_to_char( "  light life death mind spirit\n\r", ch );
	    return;
	}

	if ( IS_SET( ch->pcdata->runes[1], rune ) )
	{
	    send_to_char( "You already know that rune.\n\r", ch );
	    return;
	}
/* 10 primal for first, 20 for second etc */
	if ( ch->practice <
	( count_stones( ch->pcdata->runes[1], TYPE_RUNE ) + 1 ) * 10 )
	{
	    sprintf( buf, "You require %d primal to learn that rune.\n\r",
		( count_stones( ch->pcdata->runes[1], TYPE_RUNE ) + 1 ) * 10 );
	    send_to_char( buf, ch );
	    return;
	}
	ch->practice -= (count_stones(ch->pcdata->runes[1],TYPE_RUNE)+1)*10;
	SET_BIT( ch->pcdata->runes[1], rune );
	send_to_char( "You know can now create that type of rune.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "glyph" ) )
    {
	if 	( !str_cmp( arg2, "creation"	   ) ) rune = GLYPH_CREATION;
	else if ( !str_cmp( arg2, "destruction"    ) ) rune = GLYPH_DESTRUCTION;
	else if ( !str_cmp( arg2, "summoning"	   ) ) rune = GLYPH_SUMMONING;
	else if ( !str_cmp( arg2, "transformation" ) ) rune = GLYPH_TRANSFORMATION;
	else if ( !str_cmp( arg2, "transportation" ) ) rune = GLYPH_TRANSPORTATION;
	else if ( !str_cmp( arg2, "enhancement"	   ) ) rune = GLYPH_ENHANCEMENT;
	else if ( !str_cmp( arg2, "reduction"	   ) ) rune = GLYPH_REDUCTION;
	else if ( !str_cmp( arg2, "control"	   ) ) rune = GLYPH_CONTROL;
	else if ( !str_cmp( arg2, "protection"	   ) ) rune = GLYPH_PROTECTION;
	else if ( !str_cmp( arg2, "information"	   ) ) rune = GLYPH_INFORMATION;
	else
	{
	    do_learn( ch, "" );
	    send_to_char( "\n\rGlyph can be one of:\n\r", ch );
	    send_to_char( "  creation destruction summoning\n\r", ch );
	    send_to_char( "  transformation transportation\n\r", ch );
	    send_to_char( "  enhancement reduction control\n\r", ch );
	    send_to_char( "  protection information\n\r", ch );
	    return;
	}

	if ( IS_SET( ch->pcdata->runes[2], rune ) )
	{
	    send_to_char( "You already know that rune.\n\r", ch );
	    return;
	}
/* 10 primal for first, 20 for second etc */
	if ( ch->practice <
	( count_stones( ch->pcdata->runes[2], TYPE_GLYPH ) + 1 ) * 10 )
	{
	    sprintf( buf, "You require %d primal to learn that rune.\n\r",
		( count_stones( ch->pcdata->runes[2], TYPE_GLYPH ) + 1 ) * 10 );
	    send_to_char( buf, ch );
	    return;
	}
	ch->practice -= (count_stones(ch->pcdata->runes[2],TYPE_GLYPH)+1)*10;
	SET_BIT( ch->pcdata->runes[2], rune );
	send_to_char( "You know can now create that type of glyph.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "sigil" ) )
    {
	if 	( !str_cmp( arg2, "self" 	) ) rune = SIGIL_SELF;
	else if ( !str_cmp( arg2, "targeting" 	) ) rune = SIGIL_TARGETING;
	else if ( !str_cmp( arg2, "area" 	) ) rune = SIGIL_AREA;
	else if ( !str_cmp( arg2, "object" 	) ) rune = SIGIL_OBJECT;
	else
	{
	    do_learn( ch, "" );
	    send_to_char( "\n\rSigil can be one of:\n\r", ch );
	    send_to_char( "  self targeting area object\n\r", ch );
	    return;
	}

	if ( IS_SET( ch->pcdata->runes[3], rune ) )
	{
	    send_to_char( "You already know that sigil.\n\r", ch );
	    return;
	}
/* 20 primal for first, 40 for second etc */
	if ( ch->practice <
	( count_stones( ch->pcdata->runes[3], TYPE_SIGIL ) + 1 ) * 20 )
	{
	    sprintf( buf, "You require %d primal to learn that sigil.\n\r",
		( count_stones( ch->pcdata->runes[3], TYPE_SIGIL ) + 1 ) * 20 );
	    send_to_char( buf, ch );
	    return;
	}
	ch->practice -= (count_stones(ch->pcdata->runes[3],TYPE_SIGIL)+1)*20;
	SET_BIT( ch->pcdata->runes[3], rune );
	send_to_char( "You know can now create that type of sigil.\n\r", ch );
	return;
    }
    do_learn( ch, "" );
    return;
}

int count_stones( int stones, int type )
{
    int	count = 0;

    if ( type == TYPE_RUNE )
    {
	if ( IS_SET( stones, RUNE_FIRE	) ) count++;
	if ( IS_SET( stones, RUNE_AIR	) ) count++;
	if ( IS_SET( stones, RUNE_EARTH	) ) count++;
	if ( IS_SET( stones, RUNE_WATER	) ) count++;
	if ( IS_SET( stones, RUNE_DARK	) ) count++;
	if ( IS_SET( stones, RUNE_LIGHT	) ) count++;
	if ( IS_SET( stones, RUNE_LIFE	) ) count++;
	if ( IS_SET( stones, RUNE_DEATH	) ) count++;
	if ( IS_SET( stones, RUNE_MIND	) ) count++;
	if ( IS_SET( stones, RUNE_SPIRIT) ) count++;
	return count;
    }
    else if ( type == TYPE_GLYPH )
    {
	if ( IS_SET( stones, GLYPH_CREATION		) ) count++;
	if ( IS_SET( stones, GLYPH_DESTRUCTION		) ) count++;
	if ( IS_SET( stones, GLYPH_SUMMONING		) ) count++;
	if ( IS_SET( stones, GLYPH_TRANSFORMATION	) ) count++;
	if ( IS_SET( stones, GLYPH_TRANSPORTATION	) ) count++;
	if ( IS_SET( stones, GLYPH_ENHANCEMENT		) ) count++;
	if ( IS_SET( stones, GLYPH_REDUCTION		) ) count++;
	if ( IS_SET( stones, GLYPH_CONTROL		) ) count++;
	if ( IS_SET( stones, GLYPH_PROTECTION		) ) count++;
	if ( IS_SET( stones, GLYPH_INFORMATION		) ) count++;
	return count;
    }
    else if ( type == TYPE_SIGIL )
    {
	if ( IS_SET( stones, SIGIL_SELF		) ) count++;
	if ( IS_SET( stones, SIGIL_TARGETING	) ) count++;
	if ( IS_SET( stones, SIGIL_AREA		) ) count++;
	if ( IS_SET( stones, SIGIL_OBJECT	) ) count++;
	return count;
    }
    bug( "Wrong stone type.", 0 );
    return 0;
}

DO_COM( do_mageupkeep )
{
    char buf[MAX_STRING_LENGTH];
    bool need_line = FALSE;

    if ( IS_NPC( ch ) ) return;

    if ( !IS_MAGE( ch ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	send_to_char("                               -= Mage upkeep =-\n\r",ch);
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);

    if ( IS_SET( ch->pcdata->runes[1], RUNE_MASTER ) )
    {
	send_to_char( "You can create the master rune.\n\r", ch );
	need_line = TRUE;
    }

    if ( strlen( ch->pcdata->conding ) > 1 )
    {
	need_line = TRUE;
	sprintf( buf, "You have been conditioned by %s%s\n\r",
	    ch->pcdata->conding,
	    IS_MORE( ch, MORE_LOYAL ) ? " and are loyal." :
				        "." );
	send_to_char( buf, ch );
    }

    if ( strlen( ch->pcdata->love ) > 1 )
    {
	need_line = TRUE;
	sprintf( buf, "You are in love with %s.\n\r",
	    ch->pcdata->love );
	send_to_char( buf, ch );
    }

    if ( IS_EXTRA( ch, EXTRA_SIRE ) )
    {
	need_line = TRUE;
	STC( "You can create an apprentice.\n\r", ch );
    }

	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	send_to_char("                                  -= Runes =-\n\r",ch);
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    sprintf( buf, "%-12s[%s] %-12s[%s] %-12s[%s] %-12s[%s] %-12s[%s]\n\r",
	"Fire",    IS_SET( ch->pcdata->runes[1], RUNE_FIRE   ) ? "X" : " ",
	"Air",     IS_SET( ch->pcdata->runes[1], RUNE_AIR    ) ? "X" : " ",
	"Earth",   IS_SET( ch->pcdata->runes[1], RUNE_EARTH  ) ? "X" : " ",
	"Water",   IS_SET( ch->pcdata->runes[1], RUNE_WATER  ) ? "X" : " ",
	"Dark",    IS_SET( ch->pcdata->runes[1], RUNE_DARK   ) ? "X" : " " );
    send_to_char( buf, ch );
    sprintf( buf, "%-12s[%s] %-12s[%s] %-12s[%s] %-12s[%s] %-12s[%s]\n\r",
	"Light",   IS_SET( ch->pcdata->runes[1], RUNE_LIGHT  )  ? "X" : " ",
	"Life",    IS_SET( ch->pcdata->runes[1], RUNE_LIFE   )  ? "X" : " ",
	"Death",   IS_SET( ch->pcdata->runes[1], RUNE_DEATH  )  ? "X" : " ",
	"Mind",    IS_SET( ch->pcdata->runes[1], RUNE_MIND   )  ? "X" : " ",
	"Spirit",  IS_SET( ch->pcdata->runes[1], RUNE_SPIRIT )  ? "X" : " " );
    send_to_char( buf, ch );
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	send_to_char("                                  -= Glyphs =-\n\r",ch);
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    sprintf( buf, "%-16s[%s] %-16s[%s] %-16s[%s] %-16s[%s]\n\r",
	"Creation",
	IS_SET( ch->pcdata->runes[2], GLYPH_CREATION       )  ? "X" : " ",
	"Destruction",
	IS_SET( ch->pcdata->runes[2], GLYPH_DESTRUCTION    )  ? "X" : " ",
	"Summoning",
	IS_SET( ch->pcdata->runes[2], GLYPH_SUMMONING      )  ? "X" : " ",
	"Transformation",
	IS_SET( ch->pcdata->runes[2], GLYPH_TRANSFORMATION )  ? "X" : " " );
    send_to_char( buf, ch );
    sprintf( buf, "%-16s[%s] %-16s[%s] %-16s[%s] %-16s[%s]\n\r",
	"Transportation",
	IS_SET( ch->pcdata->runes[2], GLYPH_TRANSPORTATION )  ? "X" : " ",
	"Enhancement",
	IS_SET( ch->pcdata->runes[2], GLYPH_ENHANCEMENT    )  ? "X" : " ",
	"Reduction",
	IS_SET( ch->pcdata->runes[2], GLYPH_REDUCTION      )  ? "X" : " ",
	"Control",
	IS_SET( ch->pcdata->runes[2], GLYPH_CONTROL        )  ? "X" : " " );
    send_to_char( buf, ch );
    sprintf( buf, "%-16s[%s] %-16s[%s]\n\r",
	"Protection",
	IS_SET( ch->pcdata->runes[2], GLYPH_PROTECTION     )  ? "X" : " ",
	"Information",
	IS_SET( ch->pcdata->runes[2], GLYPH_INFORMATION    )  ? "X" : " " );
    send_to_char( buf, ch );
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	send_to_char("                                  -= Sigils =-\n\r",ch);
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    sprintf( buf, "%-16s[%s] %-16s[%s] %-16s[%s] %-16s[%s]\n\r",
	"Self",
	IS_SET( ch->pcdata->runes[3], SIGIL_SELF      )  ? "X" : " ",
	"Targeting",
	IS_SET( ch->pcdata->runes[3], SIGIL_TARGETING )  ? "X" : " ",
	"Area",
	IS_SET( ch->pcdata->runes[3], SIGIL_AREA      )  ? "X" : " ",
	"Object",
	IS_SET( ch->pcdata->runes[3], SIGIL_OBJECT    )  ? "X" : " " );
    send_to_char( buf, ch );
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    return;
}

DO_COM( do_favourmage )
{
    CHAR_DATA	*victim;
    char	arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) ) return;

    if ( ch->class != CLASS_MAGE )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: favourmage [victim]\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC( victim ) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( victim->class != CLASS_MAGE )
    {
	send_to_char( "Only on mages silly!\n\r", ch );
	return;
    }

    if ( ch->pcdata->runes[0] != victim->pcdata->runes[0] )
    {
	send_to_char( "Stick to your own mages please.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) != LEVEL_MAGE )
    {
	send_to_char( "Not on Apprentices!\n\r", ch );
	return;
    }

    if ( !IS_EXTRA( victim, EXTRA_SIRE ) )
    {
	SET_BIT( victim->extra, EXTRA_SIRE );
	act( "You grant $N permission to create an apprentice.", ch, NULL, victim, TO_CHAR );
	act( "$n has granted you permission to create an apprentice.", ch, NULL, victim, TO_VICT );
    }
    else
    {
	REMOVE_BIT( victim->extra, EXTRA_SIRE );
	act( "You remove $N's permission to create an apperentice.", ch, NULL, victim, TO_CHAR );
	act( "$n has removed your permission to create an apprentice.", ch, NULL, victim, TO_VICT );
    }
    return;
}

DO_COM( do_teachmastery )
{
    CHAR_DATA	*victim;
    char	arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) )
    return;

    if ( !IS_MAGE( ch ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( !IS_SET( ch->pcdata->runes[1], RUNE_MASTER ) )
    {
	send_to_char( "You do not even know about the master rune?\n\r", ch );
	send_to_char( "How can you teach it?\n\r", ch );
	return;
    }

    if ( get_trust( ch ) < LEVEL_MAGE )
    {
	send_to_char( "You can't teach anyone about the master rune.\n\r", ch );
	return;
    }

    if ( !IS_EXTRA( ch, EXTRA_SIRE ) && get_trust( ch ) < LEVEL_ARCHMAGE )
    {
	STC( "You haven't been granted permission to teach the master rune.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC( victim ) )
    {
	send_to_char( "You can't teach NPC's about the master rune.\n\r", ch );
	return;
    }

    if ( ch->class != victim->class )
    {
	send_to_char( "You can only teach people within the same class as yourself.\n\r", ch );
	return;
    }

    if ( ch->pcdata->runes[0] != victim->pcdata->runes[0] )
    {
	send_to_char( "You can only teach people within the same colour of magic.\n\r", ch );
	return;
    }

    if ( victim->spl[0] < 240 || victim->spl[1] < 240 || victim->spl[2] < 240
    ||	 victim->spl[3] < 240 || victim->spl[4] < 240 )
    {
	send_to_char( "They don't have max level in all spell colours.\n\r", ch );
	return;
    }

    if ( count_stones( victim->pcdata->runes[1], TYPE_RUNE  ) < 10
    ||	 count_stones( victim->pcdata->runes[2], TYPE_GLYPH ) < 10
    ||	 count_stones( victim->pcdata->runes[3], TYPE_SIGIL ) < 4 )
    {
	send_to_char( "They haven't gained enough knowledge about mages to learn about the master rune.\n\r", ch );
	return;
    }

    if ( victim->max_mana < 10000 )
    {
	send_to_char( "They require 10 thousand mana before they learn about the master rune.\n\r", ch );
	return;
    }

    if ( victim->exp < 10000000 )
    {
	send_to_char( "They require 10 million exp to learn about the master rune.\n\r", ch );
	return;
    }

    if ( ch->exp < 1000000 )
    {
	send_to_char( "You require 1 million exp to teach them about the master rune.\n\r", ch );
	return;
    }

    victim->exp -= 10000000;
    ch->exp	-= 1000000;

    SET_BIT( victim->pcdata->runes[1], RUNE_MASTER );
    victim->level = LEVEL_MAGE;
    victim->trust = LEVEL_MAGE;

    REMOVE_BIT( ch->extra, EXTRA_SIRE );

    act( "You teach $N about the master rune.", ch, NULL, victim, TO_CHAR );
    act( "$n teaches you about the master rune.", ch, NULL, victim, TO_VICT );
    act( "$n teaches $N about the master rune.", ch, NULL, victim, TO_NOTVICT );

    return;
}

void do_mageq( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch))
     return;

    if ( !IS_MAGE( ch ) )
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("What piece of mage armor do you wish to create?.\n\r", ch);
    send_to_char("A Girth, Mask or Cape? \n\r",ch);
    return;
    }


    if      (!str_cmp(arg,"girth"     )) vnum = 29697;
    else if (!str_cmp(arg,"mask"   )) vnum = 29696;
    else if (!str_cmp(arg,"cape"    )) vnum = 29698;
    else
    {
	send_to_char("What piece of mage armor do you wish to create?.\n\r", ch);
    send_to_char("A Girth, Mask or Cape? \n\r",ch);
    return;
    }
    if (ch->practice < 85)
    {
	send_to_char("It costs 85 primal to create a piece of mage armor\n\r",ch);
	return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform Morglum.\n\r",ch);
	return;
    }
    ch->practice -= 85;
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);

    free_string( obj->questmaker );
    obj->questmaker = str_dup( ch->name );

    free_string( obj->questowner );
    obj->questowner = str_dup( ch->name );

    SET_BIT( obj->quest, QUEST_NOGIFT );
    SET_BIT( obj->extra_flags, ITEM_LOYAL );
    REMOVE_BIT( obj->spectype, SITEM_DEMONIC );
    SET_BIT( obj->spectype, SITEM_MAGE );

    act("$p appears in a elemental flash.",ch,obj,NULL,TO_CHAR);
    act("$p appears in a elemental flash.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_advance( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA	*victim;
    char        arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg);


    if (IS_NPC(ch)) return;


    if (!IS_MAGE(ch))
    {
       send_to_char("Huh?\n\r",ch);
       return;
    }

    if ( get_trust( ch ) < LEVEL_ARCHMAGE )
    {
	send_to_char("Only the Archmage can advance you to mage!.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch);
	return;
    }
    if ( IS_NPC( victim ) )
    {
	send_to_char( "You cant do that FOOL!.\n\r", ch );
	return;
    }
    if (ch == victim)
    {
        send_to_char("Why would you want to do that?\n\r",ch);
        return;
    }
    if ( ch->class != victim->class )
    {
	send_to_char( "You can only teach people within the same class as yourself.\n\r", ch );
	return;
    }
    if ( ch->pcdata->runes[0] != victim->pcdata->runes[0] )
    {
	send_to_char( "You can only advance people within the same colour of magic.\n\r", ch );
	return;
    }
    if ( victim->spl[0] < 240 || victim->spl[1] < 240
       || victim->spl[2] < 240  || victim->spl[3] < 240 || victim->spl[4] < 240 )
    {
        send_to_char( "They need complete master [240] in all spells.\n\r", ch );
	return;
    }
    if ( victim->max_mana < 5000 )
    {
	send_to_char( "They require five thousand mana before they can be advanced to Mage level.\n\r",ch );
 	return;
    }
    if ( victim->exp < 15000000 )
    {
	send_to_char( "They require 15 million exp to advance to Mage level .\n\r", ch );
	return;
    }
    if ( ch->exp < 1000000 )
    {	send_to_char( "You require 1 million exp to advance them to Mage level.\n\r", ch );
	return;
    }
    victim->exp -= 15000000;
    ch->exp	-= 1000000;

    victim->pcdata->runes[1] = 2047;
    victim->pcdata->runes[2] = 1023;
    victim->pcdata->runes[3] = 15;
    victim->level = LEVEL_MAGE;
    victim->trust = LEVEL_MAGE;

    act( "You advance $N to a mage.", ch, NULL, victim, TO_CHAR );
    act( "$n advances you to a mage.", ch, NULL, victim, TO_VICT );
    act( "$n advances $N to a mage.", ch, NULL, victim, TO_NOTVICT );
    return;
}

void do_color( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char clan[MAX_INPUT_LENGTH];
    char type[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_MAGE(ch))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    send_to_char("[      Name      ] [   Colour   ] [    Level   ] [ Hits  % ] [ Mana  % ] [ Vital % ] [ Exp ]\n\r",ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
        if ( IS_NPC(gch) ) continue;
        if ( !IS_MAGE(gch) ) continue;
        if ( IS_IMMORTAL( gch )         &&      !can_see( ch, gch ) ) continue;

        if ( gch->class == CLASS_LICH )
            sprintf(clan,"Lich");
        else if ( gch->class == CLASS_NEPHANDI )
            sprintf(clan,"Nephandi");
        else if ( gch->class == CLASS_SKINDANCER )
            sprintf(clan, "Skindancer");

        else if ( gch->pcdata->runes[0] == 1 )
             sprintf(clan, "Fire");
        else if ( gch->pcdata->runes[0] == 2 )
             sprintf(clan, "Water");
        else if ( gch->pcdata->runes[0] == 3 )
             sprintf(clan, "Earth");
        else if ( gch->pcdata->runes[0] == 4 )
             sprintf(clan, "Spirit");
        else if ( gch->pcdata->runes[0] == 0 )
              sprintf(clan, "Air");
        else
            sprintf(clan,"Bugged");

        if (gch->level == 4 )
            sprintf(type, "Apprentice");
        else if (gch->level == 5 )
            sprintf(type, "Mage");
        else if (gch->level == 6 )
            sprintf(type, "ArchMage");

        {

            sprintf( buf,
            "[%-16s] [%-12s] [%-12s] [%-6d%3d] [%-6d%3d] [%-6d%3d] [%-5d]\n\r",

                capitalize( gch->name ), clan, type,
                gch->hit,  (gch->hit  * 100 / gch->max_hit ),
                gch->mana, (gch->mana * 100 / gch->max_mana ),
                gch->move, (gch->move * 100 / gch->max_move),
                gch->exp);
                send_to_char( buf, ch );
        }
      }
    return;
 }

DO_COM( do_magecommands )
{
    char buf [MAX_STRING_LENGTH];
    int  cmd, col;

    col = 0;

    for ( cmd = 0 ; cmd_table[cmd].name[0] != '\0' ; cmd++ )
    {
	if ( cmd_table[cmd].level >= LEVEL_APPRENTICE
	&&	cmd_table[cmd].level <= LEVEL_ARCHMAGE
	&&	get_trust( ch ) >= cmd_table[cmd].level )
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}
