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

DO_COM( do_chill )
{
    DESCRIPTOR_DATA	*d;
    CHAR_DATA		*victim;
    CHAR_DATA		*ach;
    char		arg[MAX_INPUT_LENGTH];
    int			sn;
    int			level;
    int			spelltype;

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) ) return;

    if ( get_tribe( ch, TRIBE_WENDIGOS ) < 3 )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' && ch->fighting == NULL )
    {
	send_to_char( "Who do you wish to use chill on?\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}
    }

    if ( ch == victim )
    {
	send_to_char( "To hot are we?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
    {
	if ( !IS_NPC( victim ) && IS_MORE( victim, MORE_GUARDIAN ) )
	{
	    guardian_message( victim );
	    act( "$n just failed to use chill on you.", ch, NULL, victim, TO_VICT );
	}
	return;
    }

    if ( IS_ITEMAFF( victim, ITEMA_ICESHIELD ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ( sn = skill_lookup( "chill touch" ) ) < 0 )
    {
	send_to_char( "Bugged, please inform an Imp.\n\r", ch );
	return;
    }

    for( d = descriptor_list ; d != NULL ; d = d->next )
    {
	if ( d->connected != CON_PLAYING ) continue;
	if ( ( ach = d->character ) == NULL ) continue;
	if ( ch->in_room == NULL || ach->in_room == NULL ) continue;
	if ( ach->in_room->area != ch->in_room->area ) continue;

	if ( ach == ch )
	    send_to_char( "You call forth the winter spirits.\n\r", ch );
	else if ( ach->in_room == ch->in_room )
	    act( "$n calls forth the winter spirts.", ch, NULL, victim, TO_ROOM );
	else
	    send_to_char( "A cold wind shrieks past you.\n\r", ch );
    }

    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 5;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;
}

DO_COM( do_moonbridge )
{
    CHAR_DATA		*victim;
    OBJ_INDEX_DATA	*pObjIndex;
    OBJ_DATA		*portal;
    char		arg[MAX_INPUT_LENGTH];
    int			duration;

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) ) return;

    if ( get_auspice( ch, AUSPICE_RAGABASH ) < 5 )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Who do you wish to send a moonbridge to?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (ch == victim)
    {
	act( "Why do that? Your already here.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( victim == ch
    ||	 IS_NPC( victim )
    ||	 !IS_WEREWOLF( victim )
    ||	 !IS_IMMUNE( victim, IMM_SUMMON )
    ||	 victim->in_room == NULL
    ||	 victim->in_room == ch->in_room )
    {
	if ( !IS_NPC( victim ) && IS_WEREWOLF( victim )
	&&	IS_MORE( victim, MORE_GUARDIAN ) )
	{
	    guardian_message( victim );
	    act( "$n just failed to create a Moon Bridge to you.", ch, NULL, victim, TO_VICT );
	}
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PORTAL ) ) == NULL )
    {
	send_to_char( "Object missing, please inform an imp.\n\r", ch );
	return;
    }

    duration = number_range( 1, 3 ) * 15;

    portal = create_object( pObjIndex, 0 );
    portal->value[0] = victim->in_room->vnum;
    portal->value[3] = ch->in_room->vnum;
    portal->timer = duration;
    if ( IS_AFFECTED( ch,AFF_SHADOWPLANE ) )
	portal->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( portal, ch->in_room );
    FILL_STRING( portal->name, "moonvridge portal" );
    FILL_STRING( portal->short_descr, "a Moon Bridge" );
    FILL_STRING( portal->description, "A Moon Bridge floats here." );

    portal = create_object( pObjIndex, 0 );
    portal->value[0] = ch->in_room->vnum;
    portal->value[3] = victim->in_room->vnum;
    portal->timer = duration;
    if ( IS_AFFECTED( ch,AFF_SHADOWPLANE ) )
	portal->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( portal, victim->in_room );
    FILL_STRING( portal->name, "moonbridge portal" );
    FILL_STRING( portal->short_descr, "a Moon Bridge" );
    FILL_STRING( portal->description, "A Moon Bridge floats here." );

    act( "$p appears in front of $n.", ch, portal, NULL, TO_ROOM );
    act( "$p appears in front of you.", ch, portal, NULL, TO_CHAR );
    act( "$p appears in front of $n.", victim, portal, NULL, TO_ROOM );
    act( "$p appears in front of you.", victim, portal, NULL, TO_CHAR );

    if ( !IS_NPC( victim ) && IS_MORE( victim , MORE_GUARDIAN ) )
    {
	guardian_message( victim );
	act( "$n has just created a Moon Bridge to you.", ch, NULL, victim, TO_VICT );
    }

    return;
}

DO_COM( do_gate )
{
    CHAR_DATA		*victim;
    OBJ_INDEX_DATA	*pObjIndex;
    OBJ_DATA		*portal;
    char		arg[MAX_INPUT_LENGTH];
    int			duration;

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) ) return;

    if ( get_tribe( ch, TRIBE_SILENT_STRIDERS ) < 5 )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Who do you wish to send a gate of the moon to?\n\r", ch );
	return;
    }

    if ( weather_info.sunlight != SUN_DARK )
    {
	send_to_char( "This power can only be used during the night.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (ch == victim)
    {
	act( "Why do that? Your already here.", ch, 0, victim, TO_CHAR );
	return;
    }
    if ( victim == ch
    || (IS_NPC( victim ) && victim->level > 150)
    || ( !IS_NPC( victim ) && !IS_IMMUNE( victim, IMM_SUMMON ) )
    ||	 victim->in_room == NULL
    ||	 victim->in_room == ch->in_room )
    {
	if ( !IS_NPC( victim ) && IS_MORE( victim, MORE_GUARDIAN ) )
	{
	    guardian_message( victim );
	    act( "$n just failed to create a Moon Bridge to you.", ch, NULL, victim, TO_VICT );
	}
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PORTAL ) ) == NULL )
    {
	send_to_char( "Object missing, please inform an imp.\n\r", ch );
	return;
    }

    duration = number_range( 1, 3 ) * 15;

    portal = create_object( pObjIndex, 0 );
    portal->value[0] = victim->in_room->vnum;
    portal->value[3] = ch->in_room->vnum;
    portal->timer = duration;
    if ( IS_AFFECTED( ch,AFF_SHADOWPLANE ) )
	portal->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( portal, ch->in_room );
    FILL_STRING( portal->name, "moonbridge portal" );
    FILL_STRING( portal->short_descr, "a Moon Bridge" );
    FILL_STRING( portal->description, "A Moon Bridge floats here." );

    portal = create_object( pObjIndex, 0 );
    portal->value[0] = ch->in_room->vnum;
    portal->value[3] = victim->in_room->vnum;
    portal->timer = duration;
    if ( IS_AFFECTED( ch,AFF_SHADOWPLANE ) )
	portal->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( portal, victim->in_room );
    FILL_STRING( portal->name, "moonbridge portal" );
    FILL_STRING( portal->short_descr, "a Moon Bridge" );
    FILL_STRING( portal->description, "A Moon Bridge floats here." );

    act( "$p appears in front of $n.", ch, portal, NULL, TO_ROOM );
    act( "$p appears in front of you.", ch, portal, NULL, TO_CHAR );
    act( "$p appears in front of $n.", victim, portal, NULL, TO_ROOM );
    act( "$p appears in front of you.", victim, portal, NULL, TO_CHAR );

    if ( !IS_NPC( victim ) && IS_MORE( victim , MORE_GUARDIAN ) )
    {
	guardian_message( victim );
	act( "$n has just created a Moon Bridge to you.", ch, NULL, victim, TO_VICT );
    }

    return;
}

DO_COM( do_brew )
{
    OBJ_DATA	*obj;
    char	arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) ) return;

    if ( get_tribe( ch, TRIBE_BONE_GNAWERS ) < 1 )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Brew what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char( "That isn't here.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_FOUNTAIN )
    {
	send_to_char( "You can only brew fountains.\n\r", ch );
	return;
    }

    if ( strlen( obj->questmaker ) > 2 && obj->questmaker != NULL
    &&	 !str_cmp( obj->questmaker, ch->name ) )
    {
	send_to_char( "You can't brew a fountain that belongs to someone else.\n\r", ch );
	return;
    }

    if ( obj->value[2] == 1 )
    {
	obj->value[2] = 0;
	act( "You transform the beer in $p into water.", ch, obj, NULL, TO_CHAR );
	act( "$n transforms the beer in $p into water.", ch, obj, NULL, TO_ROOM );
    }
    else if ( obj->value[2] == 0 )
    {
	obj->value[2] = 1;
	act( "You transform the water in $p into beer.", ch, obj, NULL, TO_CHAR );
	act( "$n transforms the water in $p into beer.", ch, obj, NULL, TO_ROOM );
    }
    else
	send_to_char( "You can only transform water and beer.\n\r", ch );

    return;
}

DO_COM( do_klaive )
{
    OBJ_INDEX_DATA	*pObjIndex;
    OBJ_DATA		*klaive;

    if ( IS_NPC( ch ) ) return;

    if ( get_auspice( ch, AUSPICE_GALLIARD ) < 4 )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( ch->practice < 85 )
    {
	send_to_char( "It costs 85 primal to create a lesser Klaive.\n\r", ch );
	return;
    }

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform an imp.\n\r", ch );
	return;
    }

    klaive = create_object( pObjIndex, 60 );

    FILL_STRING( klaive->name, "lesser klaive" );
    FILL_STRING( klaive->short_descr, "a lesser Klaive" );
    FILL_STRING( klaive->description, "A lesser Klaive is lying here." );
    FILL_STRING( klaive->questmaker, ch->name );
    FILL_STRING( klaive->questowner, ch->name );
    klaive->weight = 1;
    klaive->quest = 0;
    SET_BIT( klaive->spectype, SITEM_SILVER);
    SET_BIT( klaive->quest, QUEST_RELIC );
    SET_BIT( klaive->quest, QUEST_NOGIFT );
    SET_BIT( klaive->extra_flags, ITEM_LOYAL );
    SET_BIT( klaive->quest, QUEST_SPELLPROOF );
    klaive->resistance = 1;
    klaive->item_type = ITEM_WEAPON;
    klaive->wear_flags = ITEM_WIELD + ITEM_TAKE;
    klaive->value[0] = 0;
    klaive->value[1] = 25;
    klaive->value[2] = 50;
    klaive->value[3] = 1;

    ch->practice -= 85;

    obj_to_char( klaive, ch );

    act( "You reach into the air and produce $p.", ch, klaive, NULL, TO_CHAR );
    act( "$n reaches into the air and produce $p.", ch, klaive, NULL, TO_ROOM );

    return;
}

void do_phase(CHAR_DATA *ch, char *argument)
{
    if (!IS_OUTSIDE(ch))
    {
    send_to_char("You can't see the moon indoors.\n\r", ch);
    return;
    }

// Werewolves can sence the moonphases anytime

    if (weather_info.moonlight == MOON_DOWN && !IS_WEREWOLF(ch))
    {
    send_to_char("The moon is not up.\n\r", ch);
        return;
    }

    if (weather_info.moonphase == MOON_NEW)
    {
    send_to_char("The moon is new.\n\r", ch);
    return;
    }

    if (weather_info.moonphase == MOON_CRESCENT)
    {
    send_to_char("The moon is crescent.\n\r", ch);
    return;
    }

    if (weather_info.moonphase == MOON_HALF)
    {
    send_to_char("The moon is half tonight.\n\r", ch);
    return;
    }

    if (weather_info.moonphase == MOON_GIBBOUS)
    {
    send_to_char("The moon is in its gibbous phase.\n\r", ch);
    return;
    }

    if (weather_info.moonphase == MOON_FULL)
    {
    send_to_char("The moon is full tonight.\n\r", ch);
    return;
    }

    if (weather_info.moonphase == MOON_WANING)
    {
    send_to_char("The moon is waning.\n\r", ch);
    return;
    }

    else
    {
    send_to_char("bug, please inform Morglum.\n\r", ch);
    return;
    }

}

DO_COM( do_hearttwister )
{
    CHAR_DATA	*victim;
    char	arg[MAX_INPUT_LENGTH];
    char	buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) ) return;

    if ( get_auspice( ch, AUSPICE_GALLIARD ) < 5 )
    {
    STC( "Huh?\n\r", ch );
    return;
    }

    if ( ch->pcdata->heart_twister > 0 )
    {
	sprintf( buf, "You have to wait %d %s, before you can use heart twister again.\n\r",
	    ch->pcdata->heart_twister,
	    ch->pcdata->heart_twister == 1 ? "second" : "seconds" );
	send_to_char( buf, ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Who do you wish to use heart twister on?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }

    if ( IS_NPC( victim ) )
    {
	send_to_char( "You can't use heart twister on NPC's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You can't use heart twister on yourself.\n\r", ch );
	return;
    }

    if ( !IS_IMMUNE( victim, IMM_VAMPIRE ) )
    {
	send_to_char( "They refuse to have heart twister used on them.\n\r", ch );
	return;
    }

    ch->pcdata->heart_twister = 60;

    act( "$n places $s hands over your chest.", ch, NULL, victim, TO_VICT );
    act( "You place your hands over $N's chest.", ch, NULL, victim, TO_CHAR );
    act( "$n places $s hands over $N's chest.", ch, NULL, victim, TO_NOTVICT );

    if ( IS_IMMUNE( victim, IMM_STAKE ) )
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    if ( number_bits( 3 ) != 0 )
    {
	send_to_char( "Nothing Happens.\n\r", ch );
	return;
    }

    if ( strlen( victim->pcdata->love ) > 2 )
    {
	sprintf( buf, "$n removes your love for %s.",
	    capitalize( victim->pcdata->love ) );
	act( buf, ch, NULL, victim, TO_VICT );
	sprintf( buf, "You remove $N's love for %s.",
	    capitalize( victim->pcdata->love ) ) ;
	act( buf, ch, NULL, victim, TO_CHAR );
	FILL_STRING( victim->pcdata->love, "" );
	return;
    }
    else if ( strlen( victim->pcdata->conding ) > 2 )
    {
	sprintf( buf, "$n removes your conditioning of %s.",
	    capitalize( victim->pcdata->conding ) );
	act( buf, ch, NULL, victim, TO_VICT );
	sprintf( buf, "You remove $N's conditioning of %s.",
	    capitalize( victim->pcdata->conding ) );
	act( buf, ch, NULL, victim, TO_CHAR );
	FILL_STRING( victim->pcdata->conding, "" );
	REMOVE_BIT( victim->more, MORE_LOYAL );
	return;
    }

    send_to_char( "Nothing Happens.\n\r", ch );
    return;
}

DO_COM( do_circleattack )
{
    CHAR_DATA	*vch;
    sh_int	dam = 0;
    sh_int	num  =0;
    char	buf[MAX_STRING_LENGTH];

    if ( IS_NPC( ch ) ) return;

    if ( get_tribe( ch, TRIBE_GLASS_WALKERS ) < 5 )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( ch->pcdata->circleattack > 0 )
    {
	sprintf( buf, "You have to wait %d %s, before you can use circleattack again.\n\r",
	    ch->pcdata->circleattack,
	    ch->pcdata->circleattack == 1 ? "second" : "seconds" );
	send_to_char( buf, ch );
	return;
    }

    if ( ( vch = ch->fighting ) == NULL )
    {
	send_to_char( "Your not fighting anyone.\n\r", ch );
	return;
    }

    num = 0;

    ch->pcdata->circleattack = 60;

    for ( vch = ch->in_room->people ; vch != NULL ; vch = vch->next_in_room )
    {
	if ( vch == ch ) continue;
	if ( vch->fighting == NULL ) continue;
	if ( vch->fighting != ch ) continue;

	num++;

	dam = number_range( 500, 2000 );

	if ( number_bits( 3 ) == 0 )
	{
	    dam *= 3;
	    dam += number_bits( 3 );
	}

	act( "You manage to get behind $N, and push $M to the ground.", ch, NULL, vch, TO_CHAR );
	act( "$n manages to get behind you, and pushes you to the ground.", ch, NULL, vch, TO_VICT );
	act( "$n manages to get behind $N, and pushes $M to the ground.", ch, NULL, vch, TO_NOTVICT );
	WAIT_STATE( vch, 6 );
    }
    return;
}

DO_COM( do_grab )
{
    CHAR_DATA	*victim;
    CHAR_DATA	*rch;
    char	arg[MAX_INPUT_LENGTH];
    char	buf[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) ) return;

    if ( get_tribe( ch, TRIBE_STARGAZERS ) < 5 )
    {
    STC( "Huh?\n\r", ch );
    return;
    }

    if ( ch->pcdata->umbra_timer > 0 )
    {
	sprintf( buf, "You have to wait %d %s, before you can use grab again.\n\r",
	    ch->pcdata->umbra_timer,
	    ch->pcdata->umbra_timer == 1 ? "second" : "seconds" );
	send_to_char( buf, ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
	    send_to_char( "Grab who into the umbra?\n\r", ch );
	else
	    send_to_char( "Grab who out of the umbra?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_SHADOWPLANE )
    &&	 IS_AFFECTED( victim, AFF_SHADOWPLANE ) )
    {
	send_to_char( "They are already in the umbra.\n\r", ch );
	return;
    }
    else if ( !IS_AFFECTED( ch, AFF_SHADOWPLANE )
    &&	      !IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
    {
	send_to_char( "They are already in the real world.\n\r", ch );
	return;
    }

    if ( !IS_NPC( victim ) && !IS_IMMUNE( victim, IMM_VAMPIRE ) )
    {
	send_to_char( "They have to be a willing target.\n\r", ch );
	return;
    }

    if ( victim->fighting != NULL && victim->in_room != NULL )
    {
	stop_fighting( victim, TRUE );
	for ( rch = victim->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	    if ( rch->fighting == victim )
		stop_fighting( rch, TRUE );
	}
    }

    if ( IS_AFFECTED( ch, AFF_SHADOWPLANE ) )
    {
	act("You grab hold of $N, and pull $M into the umbra.",ch,NULL,victim,TO_CHAR);
	act("$n grabs hold of you, and pulls you into the umbra.",ch,NULL,victim,TO_VICT);
	act("$N vanishes from sight.",ch,NULL,victim,TO_NOTVICT);
	SET_BIT( victim->affected_by, AFF_SHADOWPLANE );
	ch->pcdata->umbra_timer = 150;
	if ( !IS_NPC( victim ) && !IS_WEREWOLF( victim ) )
	    victim->pcdata->umbra = 60;
	return;
    }
    else
    {
	act("You grab hold of $N, and pull $M into the real world.",ch,NULL,victim,TO_CHAR);
	act("$n grabs hold of you, and pulls you into the real world.",ch,NULL,victim,TO_VICT);
	act("$N fades back into existance.",ch,NULL,victim,TO_NOTVICT);
	REMOVE_BIT( victim->affected_by, AFF_SHADOWPLANE );
	ch->pcdata->umbra_timer = 150;
	if ( !IS_NPC( victim ) )
	    victim->pcdata->umbra = 0;
	return;
    }
    return;
}

DO_COM( do_minorcreate )
{
    OBJ_INDEX_DATA	*pObjIndex;
    OBJ_DATA		*obj;
    char		buf[MAX_STRING_LENGTH];
    char		obj_name[MAX_STRING_LENGTH];

    if ( IS_NPC( ch ) ) return;

    if ( get_tribe( ch, TRIBE_BONE_GNAWERS ) < 4 )
    {
    STC( "Huh?\n\r", ch );
    return;
    }

    if ( ch->pcdata->minor_create > 0 )
    {
	sprintf( buf, "You have to wait %d %s, before you can use minor creation again.\n\r",
	    ch->pcdata->minor_create,
	    ch->pcdata->minor_create == 1 ? "second" : "seconds" );
	send_to_char( buf, ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Minor create what?\n\r", ch );
	return;
    }

    if ( strlen( argument ) < 3 )
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    if ( ch->practice < 15 )
    {
	send_to_char( "You require 15 primal to minor create something.\n\r", ch );
	return;
    }

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error....missing object, please inform Morglum.\n\r", ch );
	return;
    }
    if ( !IS_IMMORTAL( ch ) )
	ch->pcdata->minor_create = 30;

    strcpy( obj_name, argument );
    strcpy( buf, strip_small_args( argument ) );

    ch->practice -= 15;
    obj = create_object( pObjIndex, 12 );

    FILL_STRING( obj->name, buf );
    FILL_STRING( obj->short_descr, obj_name );
    if ( is_in( obj_name, "|pair of*" ) )
	sprintf( buf, "%s are lying here.", obj_name );
    else
	sprintf( buf, "%s is lying here.", obj_name );
    FILL_STRING( obj->description, buf );
    FILL_STRING( obj->questmaker, ch->name );
    FILL_STRING( obj->questowner, ch->name );
    obj->weight = 1;

    if ( is_in( obj_name, "|torch*light*lantern*" ) )
    {
	obj->item_type = ITEM_LIGHT;
	obj->wear_flags = ITEM_WIELD + ITEM_TAKE;
	obj->value[0] = 0;
	obj->value[1] = 0;
	obj->value[2] = -1;
	obj->value[3] = 0;
    }
    else if ( is_in( obj_name, "|bag*container*" ) )
    {
	obj->item_type = ITEM_CONTAINER;
	obj->wear_flags = ITEM_TAKE;
	obj->value[0] = 500;
	obj->value[1] = 1;
	obj->value[2] = -1;
	obj->value[3] = 0;
    }
    else if ( is_in( obj_name, "|canoe*boat*raft*" ) )
    {
	obj->item_type = ITEM_BOAT;
	obj->wear_flags = ITEM_TAKE;
	obj->value[0] = 0;
	obj->value[1] = 0;
	obj->value[2] = 0;
	obj->value[3] = 0;
    }
    else if ( is_in( obj_name, "|can*bottle*barrel*glass*" ) )
    {
	obj->item_type = ITEM_FOUNTAIN;
	obj->wear_flags = ITEM_TAKE;
	obj->value[0] = 1000;
	obj->value[1] = 1000;
	if ( is_in( obj_name, "|beer*" ) )
	    obj->value[2] = 1;
	else if ( is_in( obj_name, "|wine*" ) )
	    obj->value[2] = 2;
	else if ( is_in( obj_name, "|dark ale*" ) )
	    obj->value[2] = 4;
	else if ( is_in( obj_name, "|ale*" ) )
	    obj->value[2] = 3;
	else if ( is_in( obj_name, "|whisky*" ) )
	    obj->value[2] = 5;
	else if ( is_in( obj_name, "|lemonade*" ) )
	    obj->value[2] = 6;
	else if ( is_in( obj_name, "|firebreather*" ) )
	    obj->value[2] = 7;
	else if ( is_in( obj_name, "|local speciality*" ) )
	    obj->value[2] = 8;
	else if ( is_in( obj_name, "|slime*" ) )
	    obj->value[2] = 9;
	else if ( is_in( obj_name, "|milk*" ) )
	    obj->value[2] = 10;
	else if ( is_in( obj_name, "|tea*" ) )
	    obj->value[2] = 11;
	else if ( is_in( obj_name, "|coffee*" ) )
	    obj->value[2] = 12;
	else if ( is_in( obj_name, "|blood*" ) )
	    obj->value[2] = 13;
	else if ( is_in( obj_name, "|salt water*" ) )
	    obj->value[2] = 14;
	else if ( is_in( obj_name, "|cola*coke*" ) )
	    obj->value[2] = 15;
	else
	    obj->value[2] = 0;
	if ( is_in( obj_name, "|poison*" ) )
	    obj->value[3] = 1;
	else
	    obj->value[3] = 0;
    }
    else if ( is_in( obj_name, "|pie*cake*chocolate*pizza*apple*banana*" )
    ||	is_in( obj_name, "|strawberry*chip*sausage*" ) )
    {
	obj->item_type = ITEM_FOOD;
	obj->wear_flags = ITEM_TAKE;
	obj->value[0] = 8;
	obj->value[1] = 0;
	obj->value[2] = 0;
	if ( is_in( obj_name, "|poison*" ) )
	    obj->value[3] = 1;
	else
	    obj->value[3] = 0;
    }
    else if ( all_in( obj_name, "&wood*stake*" ) )
    {
	obj->item_type = ITEM_STAKE;
	obj->wear_flags = ITEM_TAKE + ITEM_WIELD;
	obj->value[0] = 0;
	obj->value[1] = 0;
	obj->value[2] = 0;
	obj->value[3] = 0;
    }
    else if ( is_in( obj_name, "|ring*collar*amulet*cape*breastplate*helmet*" )
    ||	is_in( obj_name, "|hat*legging*trouser*jean*boot*shoe*trainer*" )
    ||	is_in( obj_name, "|gauntlet*glove*plate*sleeve*belt*watch*bracer*" )
    ||	is_in( obj_name, "|shield*mask*visor*" ) )
    {
	obj->item_type = ITEM_ARMOR;
	obj->wear_flags = ITEM_TAKE;
	obj->value[0] = 15;
	obj->value[1] = 0;
	obj->value[2] = 0;
	obj->value[3] = 0;
	if ( is_in( obj_name, "|ring*" ) )
	    obj->wear_flags += ITEM_WEAR_FINGER;
	else if ( is_in( obj_name, "|collar*amulet*" ) )
	    obj->wear_flags += ITEM_WEAR_NECK;
	else if ( is_in( obj_name, "|breastplate*" ) )
	    obj->wear_flags += ITEM_WEAR_BODY;
	else if ( is_in( obj_name, "|helmet*hat*" ) )
	    obj->wear_flags += ITEM_WEAR_HEAD;
	else if ( is_in( obj_name, "|legging*trouser*jean*" ) )
	    obj->wear_flags += ITEM_WEAR_LEGS;
	else if ( is_in( obj_name, "|boot*shoe*trainer*" ) )
	    obj->wear_flags += ITEM_WEAR_FEET;
	else if ( is_in( obj_name, "|gauntlet*glove*" ) )
	    obj->wear_flags += ITEM_WEAR_HANDS;
	else if ( is_in( obj_name, "|sleeve*plate*" ) )
	    obj->wear_flags += ITEM_WEAR_ARMS;
	else if ( is_in( obj_name, "|belt*" ) )
	    obj->wear_flags += ITEM_WEAR_WAIST;
	else if ( is_in( obj_name, "|watch*bracer*" ) )
	    obj->wear_flags += ITEM_WEAR_WRIST;
	else if ( is_in( obj_name, "|mask*visor*" ) )
	    obj->wear_flags += ITEM_WEAR_FACE;
	else
	    obj->wear_flags += ITEM_WIELD;
	if ( is_in( obj_name, "|silver*" ) )
	    SET_BIT( obj->spectype, SITEM_SILVER );
    }
    else if ( is_in( obj_name, "|sword*blade*dagger*whip*claw*club*" )
    ||	is_in( obj_name, "|blast*grep*hoover*suck*" ) )
    {
	obj->item_type = ITEM_WEAPON;
	obj->wear_flags = ITEM_TAKE + ITEM_WIELD;
	obj->value[0] = 0;
	obj->value[1] = 10;
	obj->value[2] = 20;
	if ( is_in( obj_name, "|sword*" ) )
	    obj->value[3] = 3;
	else if ( is_in( obj_name, "|dagger*" ) )
	{
	    if ( number_percent( ) <= 50 )
		obj->value[3] = 2;
	    else
		obj->value[3] = 11;
	}
	else if ( is_in( obj_name, "|whip*" ) )
	    obj->value[3] = 4;
	else if ( is_in( obj_name, "|claw*" ) )
	    obj->value[3] = 5;
	else if ( is_in( obj_name, "|club*" ) )
	    obj->value[3] = number_range( 7, 8 );
	else if ( is_in( obj_name, "|blast*" ) )
	    obj->value[3] = 6;
	else if ( is_in( obj_name, "|grep*" ) )
	    obj->value[3] = 9;
	else if ( is_in( obj_name, "|hoover*suck*" ) )
	    obj->value[3] = 12;
	else
	    obj->value[3] = 1;
	if ( is_in( obj_name, "|silver*" ) )
	    SET_BIT( obj->spectype, SITEM_SILVER );
    }
    else
    {
	obj->item_type = ITEM_TRASH;
	obj->wear_flags = ITEM_TAKE;
	obj->value[0] = 0;
	obj->value[1] = 0;
	obj->value[2] = 0;
	obj->value[3] = 0;
    }

    obj_to_char( obj, ch );

    act( "You reach into the air and produce $p.",ch,obj,NULL,TO_CHAR);
    act( "$n reaches into the air and produces $p.",ch,obj,NULL,TO_ROOM);
    return;
}

DO_COM( do_earthquake )
{
    int		sn = 0;
    sh_int	level = 0;
    char	buf[MAX_STRING_LENGTH];

    if ( IS_NPC( ch ) ) return;

    if ( get_tribe( ch, TRIBE_WENDIGOS ) < 2 )
    {
    STC( "Huh?\n\r", ch );
    return;
    }

    if ( ch->pcdata->earthquake > 0 )
    {
	sprintf( buf, "You have to wait %d %s, before you can use earthquake again.\n\r",
	    ch->pcdata->earthquake,
	    ch->pcdata->earthquake == 1 ? "second" : "seconds" );
	send_to_char( buf, ch );
	return;
    }

    if ( ( sn = skill_lookup( "earthquake" ) ) < 0 )
    {
	send_to_char( "Bugged, please inform Morglum.\n\r", ch );
	return;
    }

    ch->pcdata->earthquake = 60;

    level = get_tribe( ch, TRIBE_WENDIGOS ) * 100;

    send_to_char( "You stamp your foot onto the ground incredibly hard!\n\r", ch );
    act( "$n stamps $s foot onto the ground incredibly hard!",ch,NULL,NULL,TO_ROOM);

    spell_earthquake( sn, level, ch, ch );

    return;
}

DO_COM( do_rageme )
{
    if ( IS_NPC( ch ) ) return;

    if ( !IS_WEREWOLF( ch ) )
    {
    STC( "Huh?\n\r", ch );
    return;
    }

    if (IS_SET(ch->act, PLR_WOLFMAN) && ch->pcdata->wolf > 100 )
    {
    	send_to_char("But you are already in a rage!\n\r",ch);
	return;
    }

    if ( ch->pcdata->rage )
    {
	send_to_char( "You stop raging.\n\r", ch );
	ch->pcdata->rage = FALSE;
	return;
    }
    send_to_char("You start snarling angrilly.\n\r",ch);
    act("$n starts snarling angrilly.",ch,NULL,NULL,TO_ROOM);
    ch->pcdata->rage = TRUE;
}
