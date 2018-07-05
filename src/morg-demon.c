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

DO_COM( do_etoken )
{
    OBJ_INDEX_DATA	*pObjIndex;
    OBJ_DATA		*token;
    char		arg[MAX_INPUT_LENGTH];
    char		buf[MAX_STRING_LENGTH];
    int			value;

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) ) return;

    if ( !IS_DEMON( ch ) && !IS_JUDGE( ch ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( !IS_JUDGE( ch ) && ch->pcdata->power[POWER_CURRENT] < 1 )
    {
	send_to_char( "You are unable to create a energy token.\n\r", ch );
	return;
    }
    else if ( arg[0] == '\0' || !is_number( arg ) )
    {
	send_to_char( " Please specify a vale for the energy token.\n\r", ch );
	return;
    }
    else
    {
	value = atoi( arg );
	if ( !IS_IMPLEMENTOR( ch ) && ( value < 1 || value > 10000 ) )
	{
	    send_to_char( "Energy token should have a value between 1 and 10000.\n\r", ch );
	    return;
	}
	else if ( IS_IMPLEMENTOR( ch ) && ( value < 1 || value > 1000000 ) )
	{
	    send_to_char( "Energy token shold have a value between 1 and 1000000.\n\r", ch );
	    return;
	}
	else if ( value > ch->pcdata->power[POWER_CURRENT] && !IS_JUDGE( ch ) )
	{
	    sprintf( buf, "You only have %d energy points left to put into energy tokens.\n\r",
		ch->pcdata->power[POWER_CURRENT] );
	    send_to_char( buf, ch );
	    return;
	}
    }

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please imform Morglum.\n\r", ch );
	return;
    }

    if ( !IS_JUDGE( ch ) )
    {
	ch->pcdata->power[POWER_CURRENT] -= value;
	ch->pcdata->power[POWER_TOTAL]	 -= value;
    }

    if ( ch->pcdata->power[POWER_CURRENT] < 0 )
	ch->pcdata->power[POWER_CURRENT] = 0;

    if ( ch->pcdata->power[POWER_TOTAL] < 0 )
	ch->pcdata->power[POWER_TOTAL] = 0;

    token = create_object( pObjIndex, value );

    token->value[0]	= value;
    token->level	= 1;
    token->cost		= value*100;
    token->item_type	= ITEM_ETOKEN;

    obj_to_char( token, ch );

    FILL_STRING( token->questmaker, ch->name );
    FILL_STRING( token->name, "energy token" );
    sprintf( buf, "a %d point energy token", value );
    FILL_STRING( token->short_descr, buf );
    sprintf( buf, "A %d point energy token lies on the floor.", value );
    FILL_STRING( token->description, buf );

    act( "$p appears in your hands in a flash of blue flames.", ch, token, NULL, TO_CHAR );
    act( "$p appears in $n's hands in a flash of blue flames.", ch, token, NULL, TO_ROOM );

    return;
}

DO_COM( do_demonpowers )
{
    char 	buf[MAX_STRING_LENGTH];
    int		c_power = 0;
    int		o_power = 0;
    int		obj_num = 0;

    if ( IS_NPC( ch ) ) return;

    if ( !IS_DEMON( ch ) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
/*
    if ( ch->pcdata->disc[C_POWERS] < 1
    &&	 ch->pcdata->disc[O_POWERS] < 1 )
    {
	send_to_char( "You currently have no demonic gifts.\n\r", ch );
	return;
    }
*/
    c_power = ch->pcdata->disc[C_POWERS];
    o_power = ch->pcdata->disc[O_POWERS];
    obj_num = ch->pcdata->disc[10];

    send_to_char( "You have the following demonic gifts:\n\r", ch );

    buf[0] = '\0';
    send_to_char( "Body Parts :", ch );
    sprintf( buf, "%s.\n\r", display_demon_body( c_power, o_power ) );
    send_to_char( buf, ch );

    buf[0] = '\0';
    send_to_char( "Changeling powers :", ch );
    sprintf( buf, "%s.\n\r", display_demon_changeling( c_power, o_power, obj_num ) );
    send_to_char( buf, ch );

    buf[0] = '\0';
    send_to_char( "Weapon powers :", ch );
    sprintf( buf, "%s.\n\r", display_demon_weapon( c_power, o_power ) );
    send_to_char( buf, ch );

    buf[0] = '\0';
    send_to_char( "Fighting gifts :", ch );
    sprintf( buf, "%s.\n\r", display_demon_fight( c_power, o_power ) );
    send_to_char( buf, ch );

    buf[0] = '\0';
    send_to_char( "Vision Gifts :", ch );
    sprintf( buf, " Vision (%d) :", get_disc( ch, DISC_AUSPEX ) );
    send_to_char( buf, ch );
    sprintf( buf, "%s.\n\r", display_demon_vision( c_power, o_power ) );
    send_to_char( buf, ch );

    buf[0] = '\0';
    send_to_char( "Daimoinon Powers :", ch );
    sprintf( buf, "%s.\n\r", display_demon_daimoinon( c_power, o_power ) );
    send_to_char( buf, ch );

    buf[0] = '\0';
    send_to_char( "Misc. Gifts :", ch );
    sprintf( buf, "%s.\n\r", display_demon_misc( c_power, o_power ) );
    send_to_char( buf, ch );

    return;
}

void do_dinduct( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_DEMON(ch) || !IS_SET(ch->act, PLR_DEMON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        send_to_char("Syntax: Induct <player>\n\r",ch);
        return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( IS_NPC(victim) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "Not on yourself!\n\r", ch );
        return;
    }

    if ( !IS_DEMON(victim) )
    {
        send_to_char( "But they are not a demon!\n\r", ch );
        return;
    }
    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
        send_to_char( "You cannot induct an unwilling person.\n\r", ch );
        return;
    }
    if ( strlen(victim->lord) > 2 )
    {
        send_to_char( "But they already have a lord!.\n\r", ch );
        return;
    }

        act("You induct $N into your sect!",ch,NULL,victim,TO_CHAR);
        act("$n inducts $N into $s sect!",ch,NULL,victim,TO_NOTVICT);
        act("$n inducts you into $s sect!",ch,NULL,victim,TO_VICT);

        REMOVE_BIT(victim->more, MORE_OUTCAST);
        free_string(victim->lord);
        if (IS_SET(ch->act, PLR_DEMON)) victim->lord=str_dup(ch->name);
        else victim->lord=str_dup(ch->lord);
}

void do_doutcast( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_DEMON(ch) || !IS_SET(ch->act, PLR_DEMON ) )
    {
       send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        send_to_char("Syntax: Outcast <player>\n\r",ch);
        return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( IS_NPC(victim) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }
    if ( ch == victim )
    {
        send_to_char( "Not on yourself!\n\r", ch );
        return;
    }

    if ( !IS_DEMON(victim) )
    {
        send_to_char( "But they are not a demon!\n\r", ch );
        return;
    }


    if ( str_cmp(victim->lord,ch->name) )
    {
        send_to_char( "But they arent your Demon!.\n\r", ch );
        return;
    }

        act("You remove $N from your sect!",ch,NULL,victim,TO_CHAR);
        act("$n removes $N from $s sect!",ch,NULL,victim,TO_NOTVICT);
        act("$n removes you from $s sect!",ch,NULL,victim,TO_VICT);

        if (IS_EXTRA(victim,EXTRA_PRINCE)) REMOVE_BIT(victim->extra,EXTRA_PRINCE);
        if (IS_EXTRA(victim,EXTRA_SIRE)) REMOVE_BIT(victim->extra,EXTRA_SIRE);
        SET_BIT(victim->more, MORE_OUTCAST);
        free_string(victim->lord);
        victim->lord=str_dup("");
        victim->home = 3001;
}

void do_demonweapon( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_SET(ch->act, PLR_DEMON))
    {
	if (!IS_SET(ch->act, PLR_CHAMPION))
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
    }

    if (arg[0] == '\0')
    {
    send_to_char("Which demonic weapon would you like to create?.\n\r", ch);
    send_to_char("A longsword or shortsword? \n\r",ch);
    return;
    }


    if      (!str_cmp(arg,"longsword") || !str_cmp(arg,"long")) vnum = 29662;
    else if (!str_cmp(arg,"shortsword")|| !str_cmp(arg,"short")) vnum = 29663;
    else
    {
	send_to_char("Which demonic weapon would you like to create?.\n\r", ch);
    send_to_char("A longsword or shortsword? \n\r",ch);
    return;
    }
    if (ch->pcdata->power[0] < 5000 || ch->pcdata->power[1] < 5000)
    {
	send_to_char("It costs 5000 energy points to make an demonic weapon.\n\r",ch);
	return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform Morglum.\n\r",ch);
	return;
    }
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    ch->pcdata->power[0] -= 5000;
    ch->pcdata->power[1] -= 5000;

    free_string( obj->questmaker );
    obj->questmaker = str_dup( ch->name );

    free_string( obj->questowner );
    obj->questowner = str_dup( ch->name );

//    SET_BIT( obj->spectype, SITEM_SILVER);
    SET_BIT( obj->extra_flags, ITEM_LOYAL );
    SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );

    act("$p appears in your hands in a blast of flames.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in a blast of flames.",ch,obj,NULL,TO_ROOM);
    return;
}
