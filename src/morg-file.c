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

void npack( CHAR_DATA *ch, char *argument );

DO_COM( do_leave )
{
    char	arg[MIL];
    char	buf[MSL];

    if ( IS_NPC( ch ) ) return;

    argument = one_argument( argument, arg );

    if ( ch->class != CLASS_VAMPIRE && ch->class != CLASS_WEREWOLF
    && ch->class != CLASS_DEMON )
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }

    if ( strlen( ch->clan ) < 3 )
    {
	if ( IS_WEREWOLF( ch ) )
	    send_to_char( "You need to be in a Tribe, before you can leave one.\n\r", ch );
	else
	    send_to_char( "You need to be in a Clan, before you can leave one.\n\r", ch );
	return;
    }

    if ( ch->vampgen <= 2 )
    {
	send_to_char( "No that would be silly!\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: leave [password]\n\r", ch );
	return;
    }

    if ( strcmp( crypt( arg, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	send_to_char( "Wrong Password, wait 10 seconds.\n\r", ch );
	WAIT_STATE( ch, 40 );
	return;
    }

    sprintf( buf, "You become a %s!\n\r",
	IS_WEREWOLF( ch ) ? "Orphan" : "Caitiff" );
    send_to_char( buf, ch );

    sprintf( buf, "%s has become a %s.",
	ch->name, IS_WEREWOLF( ch ) ? "Orphan" : "Caitiff" );
    class_info( buf, ch );

    FILL_STRING( ch->clan, "" );
    REMOVE_BIT( ch->extra, EXTRA_SIRE );
    REMOVE_BIT( ch->extra, EXTRA_PRINCE );
    return;
}

void class_info( char *argument, CHAR_DATA *ch )
{
    DESCRIPTOR_DATA	*d;
    CHAR_DATA		*vch;

    if ( IS_NPC( ch ) ) return;

    if ( argument[0] == '\0' ) return;

    if ( ch->class < 1 ) return;

    for ( d = descriptor_list ; d != NULL ; d = d->next )
    {
	if ( d->connected > CON_PLAYING ) continue;
	if ( d->connected < CON_PLAYING ) continue;
	if ( ( vch = d->character ) == NULL ) continue;
	if ( IS_SET( vch->deaf, CHANNEL_INFO ) ) continue;
	if ( vch->class < 1 ) continue;
	if ( vch->class != ch->class && !IS_IMMORTAL( vch ) ) continue;

	send_to_char( "#wCLASS-INFO > #r", vch );
	send_to_char( argument, vch );
	send_to_char( "\n\r", vch );
    }
    return;
}

DO_COM( do_anarch )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC( ch ) ) return;

    if ( ch->class != CLASS_VAMPIRE )
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }

    if ( ch->vampgen <= 2 )
    {
	send_to_char( "No that would be silly!\n\r", ch );
	return;
    }

    if ( strlen( ch->clan ) > 0 )
    {
	send_to_char( "But you are in a clan.\n\r", ch );
	return;
    }

    if ( !IS_MORE( ch, MORE_OUTCAST ) )
    {
	SET_BIT( ch->more, MORE_OUTCAST );
	send_to_char( "You become an anarch.\n\r", ch );
	sprintf( buf, "%s has become an Anarch!", capitalize( ch->name ) );
	class_info( buf, ch );
    }
    else
    {
	REMOVE_BIT( ch->more, MORE_OUTCAST );
	send_to_char( "You are no longer an anarch.\n\r", ch );
	sprintf( buf, "%s is no longer an Anarch!", capitalize( ch->name ) );
	class_info( buf, ch );
    }
    WAIT_STATE( ch, 20 );
    return;
}

DO_COM( do_ronin )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC( ch ) ) return;

    if ( ch->class != CLASS_WEREWOLF )
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }

    if ( ch->vampgen <= 2 )
    {
	send_to_char( "No that would be silly!\n\r", ch );
	return;
    }

    if ( strlen( ch->clan ) > 0 )
    {
	send_to_char( "But you are in a tribe.\n\r", ch );
	return;
    }

    if ( !IS_MORE( ch, MORE_OUTCAST ) )
    {
	SET_BIT( ch->more, MORE_OUTCAST );
	send_to_char( "You become a ronin.\n\r", ch );
	sprintf( buf, "%s has become a Ronin!", capitalize( ch->name ) );
	class_info( buf, ch );
    }
    else
    {
	REMOVE_BIT( ch->more, MORE_OUTCAST );
	send_to_char( "You are no longer a ronin.\n\r", ch );
	sprintf( buf, "%s is no longer a Ronin!", capitalize( ch->name ) );
	class_info( buf, ch );
    }
    WAIT_STATE( ch, 20 );
    return;
}

void do_afk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    return;
    if (ch->position == POS_FIGHTING)
    {
        send_to_char("No way!  You are still fighting!\n\r",ch);
        return;
    }

        if (IS_SET(ch->extra, EXTRA_AFK))
        {
            send_to_char( "You are no longer AFK.\n\r", ch );
            sprintf(buf,"#w%s #ris no longer AFK!#n",ch->name);
            do_info(ch,buf);
            REMOVE_BIT(ch->extra,EXTRA_AFK);
            return;
        }

        else if (!IS_SET(ch->extra,EXTRA_AFK))
        {
            send_to_char( "You are now AFK.\n\r", ch );
            sprintf(buf,"#w%s#r is now AFK!#n",ch->name);
            do_info(ch,buf);
            SET_BIT(ch->extra,EXTRA_AFK);
            WAIT_STATE(ch, 25);
            return;
        }

	else
	return;
}

void do_newbiepack( CHAR_DATA *ch, char *argument )
{

        char buf[MAX_STRING_LENGTH];

        if (IS_EXTRA(ch,EXTRA_NEWBIEPACK))
        {
            send_to_char("But you've already made your newbie pack!\n\r",ch);
            return;
        }

	  npack(ch, "pack");
	  npack(ch, "plate");
          npack(ch, "helmet");
	  npack(ch, "sleeves");
	  npack(ch, "leggings");
          npack(ch, "gloves");
          npack(ch, "boots");
          npack(ch, "bracer");
          npack(ch, "bracer");
          npack(ch, "sword");
          npack(ch, "dagger");
          npack(ch, "ring");
          npack(ch, "ring");
          npack(ch, "collar");
          npack(ch, "collar");
          npack(ch, "lantern");
	  npack(ch, "mask");
	  npack(ch, "girth");
          do_wear(ch, "sword");
          do_wear(ch, "dagger");
	  sprintf(buf,"all");
          do_wear(ch, buf);
	  sprintf(buf,"all pack");
          do_put(ch,buf);
          SET_BIT(ch->extra,EXTRA_NEWBIEPACK);

      return;
}

void npack( CHAR_DATA *ch, char *argument )
{
	OBJ_INDEX_DATA   *pObjIndex;
	OBJ_DATA	 *obj;
	char		 arg[MAX_INPUT_LENGTH];
        int		 vnum = 0;

	argument = one_argument( argument, arg );

        if (IS_NPC(ch)) return;

	if      (!str_cmp(arg, "pack"	))	vnum = 2335;
        else if (!str_cmp(arg, "plate"  ))	vnum = 253;
	else if (!str_cmp(arg, "helmet" ))	vnum = 252;
	else if (!str_cmp( arg,"sleeves" ))	vnum = 255;
	else if (!str_cmp( arg,"leggings" ))	vnum = 254;
	else if (!str_cmp( arg,"gloves" ))	vnum = 256;
	else if (!str_cmp( arg,"boots" ))	vnum = 257;
	else if (!str_cmp( arg,"bracer" ))	vnum = 258;
	else if (!str_cmp( arg,"sword" ))	vnum = 250;
	else if (!str_cmp( arg,"dagger" ))	vnum = 251;
	else if (!str_cmp( arg,"ring" ))	vnum = 259;
	else if (!str_cmp( arg,"collar" ))	vnum = 261;
	else if (!str_cmp( arg,"lantern" ))	vnum = 260;
	else if (!str_cmp( arg,"mask" ))	vnum = 2622;
	else if (!str_cmp( arg,"girth" ))	vnum = 2204;

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object Please inform Morglum.\n\r",ch);
	return;
    }

    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    return;
}

DO_COM( do_rolldirt )
{
    CHAR_DATA *vch;
    bool set_fire = FALSE;

    if ( !IS_AFFECTED( ch, AFF_FLAMING ) )
    {
	send_to_char("You are not on fire.\n\r", ch );
	return;
    }

    act("$n rolls about in the dirt.", ch, NULL, NULL, TO_ROOM );
    send_to_char( "You roll about in the dirt.\n\r", ch );

    if ( IS_AFFECTED( ch, AFF_FLAMING ) && number_percent() > 80
    &&	ch->in_room != NULL )
    {
	set_fire = TRUE;
	for ( vch = ch->in_room->people ; vch != NULL ; vch = vch->next_in_room )
	{
	    if ( number_percent() < 65 ) continue;
	    if ( !IS_HERO( vch ) ) continue;
	    if ( IS_IMMORTAL( vch ) ) continue;
	    if ( IS_AFFECTED( vch, AFF_FLAMING ) ) continue;
	    act( "A spark leaps from $n's body and sets you on fire!",ch,NULL,vch,TO_VICT);
	    act( "A spark leaps from your body and sets $N on fire!",ch,NULL,vch,TO_CHAR);
	    act( "A sprak leaps from $n's body and sets $N on fire!",ch,NULL,vch,TO_NOTVICT);
	    SET_BIT( vch->affected_by, AFF_FLAMING );
	    if ( IS_MORE( ch, MORE_FLAMES ) )
		SET_BIT( vch->more, MORE_FLAMES );
	}
    }
    if ( set_fire )
	return;

    if ( number_percent() < 25 )
    {
	send_to_char("The flames around your body die out.\n\r", ch );
	act("The flames around $n's body die out.",ch,NULL,NULL,TO_ROOM);
    REMOVE_BIT(ch->affected_by, AFF_FLAMING);
    REMOVE_BIT(ch->more, MORE_FLAMES);
    }
    return;
}

void do_clearpks(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
	return;
    if (ch->in_room->vnum != 3054)
    {
	send_to_char("Please step into the Temple Altar to clear your records.\r\n", ch);
	return;
    }
    send_to_char("Done\r\n", ch);
    ch->pkill = 0;
    ch->pdeath = 0;
    ch->race = 0;
}

void do_bounty(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char  arg1 [MAX_INPUT_LENGTH];
    char  arg2 [MAX_INPUT_LENGTH];
    char  buf  [MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (arg1[0] =='\0' || arg2[0] == '\0')
    {
    send_to_char("Place what bounty on who's head?\n\rSyntax: Bounty <victim> <amount>\n\r", ch );
    return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL)
    {
    send_to_char( "They are currently not logged in!", ch );
    return;
    }

    if (ch == victim)
    {
	send_to_char( "You cannot place bounties on yourself!\n\r", ch);
	return;
    }

    if (IS_NPC(victim))
    {
      send_to_char( "You cannot put a bounty on NPCs!\n\r", ch);
      return;
    }

    if (!IS_HERO(victim))
    {
      send_to_char( "You cannot place bounties on mortals!\n\r", ch);
      return;
    }

    if (IS_IMMORTAL(victim))
    {
	send_to_char( "You cannot place bounties on gods!\n\r", ch);
	return;
    }

    if ( is_number( arg2 ) )
    {
    int amount;
    amount = atoi(arg2);

    if (amount == 0) amount = 1;

    if (amount < 50)
    {
        send_to_char( "You have to put at least 50 questpoints as the bounty.\n\r", ch);
	return;
    }

    if (ch->pcdata->quest < amount)
    {
        send_to_char( "You don't have that many questpoints!\n\r", ch);
        return;
    }

    ch->pcdata->quest -= amount;
    victim->pcdata->bounty += amount;
    sprintf( buf, "You have placed a %d qp bounty on %s.\n\r", amount, victim->name);
    send_to_char(buf,ch);
    sprintf(buf, "%s is now worth a %d qp bounty.", victim->name,victim->pcdata->bounty );
    do_info(ch,buf);
    return;
  }
  send_to_char("The amount has to be a number\n\rSyntax: Bounty <victim> <value>.\n\r", ch);
  return;
}

void do_apply ( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch))
        send_to_char("Only Players can be Quest Trusted.\n\r",ch);

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Usage: APPLY QUEST\n\r", ch );
        return;
    }
    if (!str_cmp( arg1, "quest" ))
    {
        if (IS_EXTRA(ch, EXTRA_TRUSTED))
                {
                send_to_char("You are already Quest Trusted\n\r",ch);
    return;
                }
        send_to_char("Your application for Questing Trust has been recieved\n\r",ch);
        send_to_char("and is being processed.....\n\r\n\r",ch);
        if ( ( ch->pcdata->score[SCORE_NUM_QUEST] >= 10) &&
           (ch->pcdata->score[SCORE_QUEST] >= 500))
        {
        send_to_char("You have been blessed with Questing Trust.\n\r",ch);
        SET_BIT(ch->extra, EXTRA_TRUSTED);
        return;
        }
        else
        {
        send_to_char("Your Application has been Denied!\n\r",ch);
        send_to_char("You need to complete 10 quests,\n\r",ch);
        send_to_char("Must have 500 qp on hand.\n\r",ch);
        return;
        }
}
}

void do_selfrestore( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (ch->in_room == NULL || ch->in_room->vnum != 3054)
    {
        send_to_char("You can only get a restore at the Temple Altar of Midgaard.\n\r",ch);
        return;
    }
        if ( ch->exp < 20000)
        {
            send_to_char("You need 20,000 xp to restore self.\n\r", ch);
            return;
        }
    ch->exp   -= 20000;
    do_restore(ch,"self");
    send_to_char("God answers you, you are restored.\n\r",ch);
    return;
}

