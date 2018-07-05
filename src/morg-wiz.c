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

/* Locals */
FILE * popen   args( ( const char *command, const char *type ) );
int    pclose  args( ( FILE *stream ) );
char * fgetf   args( ( char *s, int n, register FILE *iop ) );
void syspipe args( ( CHAR_DATA *ch, char *argument ) );
#define LOG_DIR	"../log/"


void do_discset( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA	*victim;
    char        arg1[MAX_INPUT_LENGTH];
    char	arg2[MAX_INPUT_LENGTH];
    char	arg3[MAX_INPUT_LENGTH];
    sh_int	disc = 0;
    sh_int	value = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( IS_NPC( ch ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax : setdisc [victim] [disc/all] [level]\n\r\n\r", ch );
	send_to_char( " Disc being one of:\n\r", ch );
	send_to_char( "  animalism auspex celerity chimerstry\n\r", ch );
	send_to_char( "  daimoinon dominate fortitude melpominee\n\r", ch );
	send_to_char( "  necromancy obeah obfuscate obtenebration\n\r", ch );
	send_to_char( "  potence presence protean quietus serpentis\n\r", ch );
	send_to_char( "  thanatosis thaumaturgy vicissitude\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim ) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( !IS_VAMPIRE( victim ) )
    {
	send_to_char( "Only on Vampires or Vampire Hybirds.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "all" ) )
    {
	value = is_number( arg3 ) ? atoi( arg3 ) : -1;

	if ( value < 0 || value > 5 )
	{
	    send_to_char( "Range is 0 - 5.\n\r", ch );
	    return;
	}

	if ( value == 0 ) value = -1;

	for ( disc = 0 ; disc <= DISC_MAX ; disc++ )
		victim->pcdata->powers[disc] = value;

	send_to_char("Done.\n\r", ch );
	return;
    }
    else if ( !str_cmp( arg2, "animalism" ) )	disc = DISC_ANIMALISM;
    else if ( !str_cmp( arg2, "auspex" ) )	disc = DISC_AUSPEX;
    else if ( !str_cmp( arg2, "celerity" ) )	disc = DISC_CELERITY;
    else if ( !str_cmp( arg2, "chimerstry" ) )	disc = DISC_CHIMERSTRY;
    else if ( !str_cmp( arg2, "daimoinon" ) )	disc = DISC_DAIMOINON;
    else if ( !str_cmp( arg2, "dominate" ) )	disc = DISC_DOMINATE;
    else if ( !str_cmp( arg2, "fortitude" ) )   disc = DISC_FORTITUDE;
    else if ( !str_cmp( arg2, "melpominee" ) )	disc = DISC_MELPOMINEE;
    else if ( !str_cmp( arg2, "necromancy" ) )	disc = DISC_NECROMANCY;
    else if ( !str_cmp( arg2, "obeah" ) )	disc = DISC_OBEAH;
    else if ( !str_cmp( arg2, "obfuscate" ) )	disc = DISC_OBFUSCATE;
    else if ( !str_cmp( arg2, "obtenebration" ) ) disc = DISC_OBTENEBRATION;
    else if ( !str_cmp( arg2, "potence" ) )	disc = DISC_POTENCE;
    else if ( !str_cmp( arg2, "presence" ) )	disc = DISC_PRESENCE;
    else if ( !str_cmp( arg2, "protean" ) )	disc = DISC_PROTEAN;
    else if ( !str_cmp( arg2, "quietus" ) )	disc = DISC_QUIETUS;
    else if ( !str_cmp( arg2, "serpentis" ) )	disc = DISC_SERPENTIS;
    else if ( !str_cmp( arg2, "thanatosis" ) )	disc = DISC_THANATOSIS;
    else if ( !str_cmp( arg2, "thaumaturgy" ) )	disc = DISC_THAUMATURGY;
    else if ( !str_cmp( arg2, "vicissitude" ) )	disc = DISC_VICISSITUDE;
    else
    {
	do_discset( ch, "" );
	return;
    }
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    if ( value < 0 || value > 5 )
    {
	send_to_char( "Range is 0 - 5.\n\r", ch );
	return;
    }
    if ( value == 0 ) value = -1;
	 victim->pcdata->powers[disc] = value;
	 send_to_char("Done.\n\r", ch );
    return;
}

void do_giftset( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA	*victim;
    char        arg1[MAX_INPUT_LENGTH];
    char	arg2[MAX_INPUT_LENGTH];
    char	arg3[MAX_INPUT_LENGTH];
    sh_int	gift = 0;
    sh_int	type = 0;
    sh_int	value = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( IS_NPC( ch ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: giftset [victim] [gift/all] [value]\n\r\n\r", ch );
	send_to_char( "Gift being one of:\n\r", ch );
	send_to_char( "  homid lupus metis\n\r", ch );
	send_to_char( "  ragaba theurg philod gallia ahroun\n\r",ch );
	send_to_char( "  blafur bognaw chgaia fianna getfen\n\r", ch );
	send_to_char( "  glwalk redtal shlord silstr silvfa\n\r", ch );
	send_to_char( "  starga uktena wendig\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC( victim ) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( !IS_WEREWOLF( victim ) )
    {	send_to_char( "Only on Werewolves or Werewolf Hybirds.\n\r", ch);
	return;
    }

    if ( !str_cmp( arg2, "all" ) )
    {
	value = is_number( arg3 ) ? atoi( arg3 ) : -1;

	if ( value < 0 || value > 5 )
	{
	    send_to_char( "Value range is 0 - 5.\n\r", ch );
	    return;
	}

	if ( value == 0 ) value = -1;

	for ( gift = 0 ; gift <= BREED_MAX ; gift++ )
	    victim->pcdata->breed[gift] = value;

	for ( gift = 0 ; gift <= AUSPICE_MAX ; gift++ )
	    victim->pcdata->auspice[gift] = value;

	for ( gift = 0 ; gift <= TRIBE_MAX ; gift++ )
	    victim->pcdata->tribes[gift] = value;

	send_to_char( "Done.\n\r", ch );
	return;
     }

	else if ( !str_cmp( arg2, "lupus" ) )
	{ gift = BREED_LUPUS; type = 1; }
        else if ( !str_cmp( arg2, "metis" ) )
	{ gift = BREED_METIS; type = 1; }
	else if ( !str_cmp( arg2, "homid" ) )
	{ gift = BREED_HOMID; type = 1; }

    	else if ( !str_cmp( arg2, "ragaba" ) )
	{ gift = AUSPICE_RAGABASH; type = 2; }
        else if ( !str_cmp( arg2, "theurg" ) )
	{ gift = AUSPICE_THEURGE; type = 2; }
    	else if ( !str_cmp( arg2, "philod" ) )
	{ gift = AUSPICE_PHILODOX; type = 2; }
    	else if ( !str_cmp( arg2, "gallia" ) )
	{ gift = AUSPICE_GALLIARD; type = 2; }
    	else if ( !str_cmp( arg2, "ahroun" ) )
	{ gift = AUSPICE_AHROUN; type = 2; }

    else if ( !str_cmp( arg2, "blafur" ) )
	{ gift = TRIBE_BLACK_FURIES; type = 3; }
    else if ( !str_cmp( arg2, "bognaw" ) )
	{ gift = TRIBE_BONE_GNAWERS; type = 3; }
    else if ( !str_cmp( arg2, "chgaia" ) )
	{ gift = TRIBE_CHILDREN_OF_GAIA; type = 3; }
    else if ( !str_cmp( arg2, "fianna" ) )
	{ gift = TRIBE_FIANNA; type = 3; }
    else if ( !str_cmp( arg2, "getfen" ) )
	{ gift = TRIBE_GET_OF_FENRIS; type = 3; }
    else if ( !str_cmp( arg2, "glwalk" ) )
	{ gift = TRIBE_GLASS_WALKERS; type = 3; }
    else if ( !str_cmp( arg2, "redtal" ) )
	{ gift = TRIBE_RED_TALONS; type = 3; }
    else if ( !str_cmp( arg2, "shlord" ) )
	{ gift = TRIBE_SHADOW_LORDS; type = 3; }
    else if ( !str_cmp( arg2, "silstr" ) )
	{ gift = TRIBE_SILENT_STRIDERS; type = 3; }
    else if ( !str_cmp( arg2, "silvfa" ) )
	{ gift = TRIBE_SILVER_FANGS; type = 3; }
    else if ( !str_cmp( arg2, "starga" ) )
	{ gift = TRIBE_STARGAZERS; type = 3; }
    else if ( !str_cmp( arg2, "uktena" ) )
	{ gift = TRIBE_UKTENA; type = 3;}
    else if ( !str_cmp( arg2, "wendig" ) )
	{ gift = TRIBE_WENDIGOS; type = 3; }
    else
    {
	do_giftset( ch, "" );
	return;
    }

    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    if ( value < 0 || value > 5 )
    {
	send_to_char( "Value range is 0 - 5.\n\r", ch );
	return;
    }

    if ( value == 0 ) value = -1;

    if ( type == 1 )
	victim->pcdata->breed[gift] = value;
    else if ( type == 2 )
	victim->pcdata->auspice[gift] = value;
    else
	victim->pcdata->tribes[gift] = value;

    send_to_char( "Done.\n\r", ch );
    return;
}

void do_cset( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA	*victim;
    char	arg1[MAX_INPUT_LENGTH];
    char	arg2[MAX_INPUT_LENGTH];
    char	string[MAX_STRING_LENGTH];
    int		value;

    if ( IS_NPC( ch ) ) return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy(string, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: cset [victim] [option] [value]\n\r",	ch );
	send_to_char( "\n\r Option can be one of:\n\r",			ch );
	send_to_char( "  gen clan lord dlord champion prince\n\r",	ch );
        send_to_char( "  sire rogue addrunes remrunes\n\r",			ch );
	return;
    }

    value = is_number( string ) ? atoi( string ) : -1;
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }

    if ( IS_NPC( victim ) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "addrunes") )
    {
       if ( !IS_MAGE( victim ) )
       {
	   send_to_char("Only on Mages or Mage Hybrids.\n\r",ch);
	   return;
       }

       victim->pcdata->runes[1] = 2047;
       victim->pcdata->runes[2] = 1023;
       victim->pcdata->runes[3] = 15;
       send_to_char("Done.\n\r", ch);
       return;
    }
    if ( !str_cmp( arg2, "remrunes") )
    {
       victim->pcdata->runes[1] = 0;
       victim->pcdata->runes[2] = 0;
       victim->pcdata->runes[3] = 0;
       send_to_char("Done.\n\r", ch);
       return;
    }
    if ( !str_cmp( arg2, "dlord") )
    {
        if ( !IS_DEMON( victim ) )
        {
           send_to_char("Only on Demons.\n\r",ch);
           return;
        }
        if IS_SET(victim->act, PLR_DEMON)

            REMOVE_BIT(victim->act, PLR_DEMON);
        else
            SET_BIT(victim->act, PLR_DEMON);
	send_to_char("Done.\n\r",ch);
        return;
    }
    if ( !str_cmp( arg2, "rogue") )
    {
        if ( !IS_DEMON( victim ) )
        {
           send_to_char("Only on Demons.\n\r",ch);
           return;
        }
           if ( IS_MORE( victim, MORE_OUTCAST ) )

            REMOVE_BIT(victim->more, MORE_OUTCAST);
        else
            SET_BIT(victim->more, MORE_OUTCAST);
	    SET_BIT(victim->act, PLR_CHAMPION);
            REMOVE_BIT( victim->extra, EXTRA_SIRE );
            REMOVE_BIT( victim->extra, EXTRA_PRINCE );
            free_string(victim->lord);
            victim->lord=str_dup("");

        send_to_char("Done.\n\r",ch);
        return;
    }
    if ( !str_cmp( arg2, "champion") )
    {

	if (!IS_DEMON(victim) )
        {
           send_to_char("Only on Demons.\n\r",ch);
           return;
        }
        if IS_SET(victim->act, PLR_CHAMPION)

	   REMOVE_BIT(victim->act, PLR_CHAMPION);
        else
           SET_BIT(victim->act, PLR_CHAMPION);
        send_to_char("Done.\n\r",ch);
        return;
    }
    if ( !str_cmp( arg2, "prince") )
    {
     if ( IS_MAGE(victim) )
     {
        send_to_char("Not on Mages.\n\r",ch);
        return;
     }
    if (IS_EXTRA(victim,EXTRA_PRINCE))
        {
            REMOVE_BIT(victim->extra,EXTRA_PRINCE);
	    send_to_char("Done.\n\r",ch);
            return;
        }
        SET_BIT(victim->extra,EXTRA_PRINCE);
        send_to_char("Done.\n\r",ch);
        return;
     }

     if ( !str_cmp( arg2, "sire") )
    {
    if (IS_EXTRA(victim,EXTRA_SIRE))
        {
            REMOVE_BIT(victim->extra,EXTRA_SIRE);
	    send_to_char("Done.\n\r",ch);
            return;
        }
        SET_BIT(victim->extra,EXTRA_SIRE);
        send_to_char("Done.\n\r",ch);
        return;
     }

    if ( !str_cmp( arg2, "gen" ) )
    {
	if ( !IS_WEREWOLF( victim ) && !IS_VAMPIRE( victim ) && !IS_NEPHANDI( victim ) )
	{
	    send_to_char( "On werewolves and vampires only.\n\r", ch );
	    return;	}
	if (victim->class == CLASS_WEREWOLF )
	{
	    if ( value < 2 || value > 4 )
	    {
		send_to_char( "Werewolf gen must be between 2 and 4.\n\r", ch );
		return;
	    }
	}
	else if ( victim->class == CLASS_VAMPIRE )
	{
	    if ( value < 2 || value > 7 )
            {
		send_to_char( "Vampire gen must be between 2 and 7.\n\r", ch );
		return;
	    }
	}
        else if (( victim->class == CLASS_NEPHANDI ) || ( victim->class == CLASS_SKINDANCER )
             || ( victim->class == CLASS_BAALI ) || ( victim->class == CLASS_LICH )
             || ( victim->class == CLASS_BLACK_SPIRAL_DANCER )  || ( victim->class == CLASS_ABOMINATION ) )
        {
            if ( value < 3 || value > 4 )
            {
		send_to_char( "Hybrid gen can only be 3 or 4.\n\r", ch );
                return;
            }
        }
	else
	{
            send_to_char( "Can't set thier gen.\n\r", ch );
	    return;
	}
	victim->vampgen = value;
	if ( IS_VAMPIRE( victim ) ) update_gen( victim );
	send_to_char("Generation updated.\n\r", ch );
	send_to_char( "Don't forget to set up the lord.\n\r", ch );
	return;
    }
    else if ( !str_cmp( arg2, "clan" ) )
    {
	if ( victim->class != CLASS_VAMPIRE
	&&   victim->class != CLASS_WEREWOLF )
	{
	send_to_char( "Thay have no need for a clan.\n\r", ch );
	    return;
	}
        smash_tilde( string );
	if ( strlen( string ) < 0 || strlen( string ) > 30 )
	{
	    send_to_char( "Clan name must be between 0 and 30 characters.\n\r", ch );
            send_to_char( "Leave a blank to clear clan.\n\r",ch);
	    return;
	}

            free_string( victim->clan );
            victim->clan = str_dup( string );
	send_to_char( "Clan name set.\n\r", ch );
 	return;
    }

    else if ( !str_cmp( arg2, "lord" ) )
    {
	smash_tilde( string );
	if ( strlen( string ) < 0)
	{
            send_to_char( "Must contain at least one valid character name.\n\r", ch );
            send_to_char( "Leave a Blank to clear lord.\n\r",ch);
	    return;
	}
        free_string( victim->lord );
        victim->lord = str_dup( string);
	send_to_char( "Lord set.\n\r", ch );
	return;
    }
    else
    {
	do_cset( ch, "" );
	return;
    }
    return;
}

void do_dset(CHAR_DATA *ch, char *argument)
{
    char    	arg1[MAX_INPUT_LENGTH];
    char	arg2[MAX_INPUT_LENGTH];
    char	arg3[MAX_INPUT_LENGTH];
    char	arg4[MAX_INPUT_LENGTH];
    OBJ_DATA	*obj;
    int		value = 0;
    int		disc = 0;

    if ( IS_NPC( ch ) )
    {
	send_to_char( "Not while switched.\n\r", ch );
	return;
    }
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' ||	 arg3[0] == '\0' || arg4[0] == '\0' )
    {
	send_to_char( "Syntax : dset <obj> <disc> <+/-> <level>\n\r",	ch );
	send_to_char( "\n\r", ch );
	send_to_char( "Discipline being one of:\n\r", ch );
	send_to_char( " animalism auspex celerity chimerstry\n\r",	ch );
	send_to_char( " daimoinon dominate fortitude melpominee\n\r",	ch );
	send_to_char( " necromancy obeah obfuscate obtenebration\n\r",	ch );
	send_to_char( " potence presence protean quietus\n\r",		ch );
	send_to_char( " serpentis thanatosis thaumaturgy\n\r",		ch );
	send_to_char( " vicissitude\n\r",ch );
	send_to_char( "\n\r",						ch );
	send_to_char( " - : Sets to that level\n\r",			ch );
	send_to_char( " + : Adds to thier current level\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }
    value = atoi( arg4 );
    if ( value < 1 || value > 10 )
    {
	send_to_char( "Discipline range is 1 to 10.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg2, "animalism" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_ANIMALISM;
    else if ( !str_cmp( arg2, "animalism" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_ANIMALISM;
    else if ( !str_cmp( arg2, "auspex" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_AUSPEX;
    else if ( !str_cmp( arg2, "auspex" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_AUSPEX;
    else if ( !str_cmp( arg2, "celerity" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_CELERITY;
    else if ( !str_cmp( arg2, "celerity" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_CELERITY;
    else if ( !str_cmp( arg2, "chimerstry" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_CHIMERSTRY;
    else if ( !str_cmp( arg2, "chimerstry" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_CHIMERSTRY;
    else if ( !str_cmp( arg2, "daimoinon" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_DAIMOINON;
    else if ( !str_cmp( arg2, "daimoinon" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_DAIMOINON;
    else if ( !str_cmp( arg2, "dominate" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_DOMINATE;
    else if ( !str_cmp( arg2, "dominate" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_DOMINATE;
    else if ( !str_cmp( arg2, "fortitude" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_FORTITUDE;
    else if ( !str_cmp( arg2, "fortitude" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_FORTITUDE;
    else if ( !str_cmp( arg2, "melpominee" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_MELPOMINEE;
    else if ( !str_cmp( arg2, "melpominee" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_MELPOMINEE;
    else if ( !str_cmp( arg2, "necromancy" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_NECROMANCY;
    else if ( !str_cmp( arg2, "necromancy" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_NECROMANCY;
    else if ( !str_cmp( arg2, "obeah" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_OBEAH;
    else if ( !str_cmp( arg2, "obeah" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_OBEAH;
    else if ( !str_cmp( arg2, "obfuscate" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_OBFUSCATE;
    else if ( !str_cmp( arg2, "obfuscate" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_OBFUSCATE;
    else if ( !str_cmp( arg2, "obtenebration" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_OBTENEBRATION;
    else if ( !str_cmp( arg2, "obtenebration" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_OBTENEBRATION;
    else if ( !str_cmp( arg2, "potence" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_POTENCE;
    else if ( !str_cmp( arg2, "potence" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_POTENCE;
    else if ( !str_cmp( arg2, "presence" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_PRESENCE;
    else if ( !str_cmp( arg2, "presence" ) && !str_cmp( arg3, "-" ) )
        disc = APPLY_SET_PRESENCE;
    else if ( !str_cmp( arg2, "protean" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_PROTEAN;
    else if ( !str_cmp( arg2, "protean" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_PROTEAN;
    else if ( !str_cmp( arg2, "quietus" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_QUIETUS;
    else if ( !str_cmp( arg2, "quietus" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_QUIETUS;
    else if ( !str_cmp( arg2, "serpentis" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_SERPENTIS;
    else if ( !str_cmp( arg2, "serpentis" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_SERPENTIS;
    else if ( !str_cmp( arg2, "thanatosis" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_THANATOSIS;
    else if ( !str_cmp( arg2, "thanatosis" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_THANATOSIS;
    else if ( !str_cmp( arg2, "thaumaturgy" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_THAUMATURGY;
    else if ( !str_cmp( arg2, "thaumaturgy" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_THAUMATURGY;
    else if ( !str_cmp( arg2, "vicissitude" ) && !str_cmp( arg3, "+" ) )
	disc = APPLY_VICISSITUDE;
    else if ( !str_cmp( arg2, "vicissitude" ) && !str_cmp( arg3, "-" ) )
	disc = APPLY_SET_VICISSITUDE;
    else
    {
	do_dset( ch, "" );
	return;
    }
    oset_affect( ch, obj, value, disc, FALSE );
    return;
}

void do_cshow(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;
	if (IS_NPC(ch))
	 return;
	sprintf(buf, "[        Name][Level][Trust][Class][Gen][  Hit][ Mana][ Move][  HR][  DR][  AC][Quest  ][Sta ]\n\r");
	send_to_char(buf,ch);
	for (d = descriptor_list; d != NULL; d = d->next)
	{
		CHAR_DATA *wch;
		if (d->connected != CON_PLAYING) continue;
		wch = (d->original != NULL) ? d->original : d->character;
	sprintf(buf,"[%12s][%5d][%5d][%5d][%3d][%5d][%5d][%5d][%4d][%4d][%5d][%5d][%4d]\n\r",
	wch->name,
	wch->level,
	wch->trust,
	wch->class,
	wch->vampgen,
	wch->max_hit,
	wch->max_mana,
	wch->max_move,
	char_hitroll(wch),
	char_damroll(wch),
	char_ac(wch),
	wch->pcdata->quest,
	wch->race);
	send_to_char(buf,ch);
	}
return;
}

void do_recolour(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int col;
    argument=one_argument(argument,arg1);
    argument=one_argument(argument,arg2);

    if (arg1[0] == '\0' || arg2[0] == '\0')
    {
        send_to_char("Syntax: recolour <char> <new colour>\n\r",ch);
        return;
    }
    if ((victim=get_char_world(ch,arg1)) == NULL)
    {
        send_to_char("They're not here.\n\r",ch);
        return;
    }
    if (!IS_MAGE(victim))
    {
	send_to_char("Only on mages.\n\r", ch);
	return;
    }

    if (IS_NEPHANDI(victim) || IS_LICH(victim) || IS_SKINDANCER(victim))
    {
	send_to_char("They may only be red!\n\r",ch);
	return;
    }


        if (!str_cmp(arg2,"air"))
            col = 0;
        else if (!str_cmp(arg2,"fire"))
            col = 1;
        else if (!str_cmp(arg2,"water"))
            col = 2;
        else if (!str_cmp(arg2,"earth"))
            col = 3;
        else if (!str_cmp(arg2,"spirit"))
            col = 4;
        else
        {
            send_to_char("Mages types being:\n\r air, fire, water, earth, spirit.\n\r",ch);
            return;
        }
        victim->pcdata->runes[0] = col;
        sprintf(buf,"%s declared %s mage by the gods!",victim->name,capitalize(arg2));
        return;
}

void do_waitplayer(CHAR_DATA *ch, char *argument)

{
    CHAR_DATA           *victim;
    DESCRIPTOR_DATA     *d;
    char                arg1[MAX_STRING_LENGTH];
    char                arg2[MAX_STRING_LENGTH];
    sh_int              value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC( ch ) ) return;

    if ( arg1[0] == '\0' || arg2 == '\0' )
    {
        send_to_char( "Syntax: waitplayer [victim] [seconds]\n\r", ch );
        return;
    }

    if ( !str_cmp( arg1, "all" ) )
    {
        if ( !is_number( arg2 ) )
        {
            send_to_char( "Number of seconds must be a numeric.\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        if ( value < 1 || value > 30 )
        {
            send_to_char( "Range must be 1 to 30 seconds.\n\r", ch );
            return;
        }

        for ( d = descriptor_list ; d != NULL ; d = d->next )
        {
            if ( d->connected < CON_PLAYING ) continue;
            if ( d->connected > CON_PLAYING ) continue;
            if ( ( victim = d->character ) == NULL ) continue;
            if ( victim == ch ) continue;
            if ( IS_NPC( victim ) ) continue;
            if ( get_trust( ch ) <= get_trust( victim ) ) continue;
            if ( !can_see( ch, victim ) ) continue;
            if ( IS_SET( ch->act, PLR_GODLESS )
            &&  get_trust( ch ) < NO_GODLESS ) continue;

            WAIT_STATE( victim, value * 4 );
       }
        return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They are not here.\n\r", ch );
        return;
    }

    if ( IS_NPC( victim ) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

    if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_GODLESS )
    &&   get_trust( ch ) < NO_GODLESS )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }

    if ( !is_number( arg2 ) )
    {
        send_to_char( "Number of seconds must be numeric.\n\r", ch );
       return;
    }

    value = atoi( arg2 );

    if ( value < 1 || value > 30 )
    {
        send_to_char( "Value range is 1 to 30 seconds.\n\r", ch );
        return;
    }

    WAIT_STATE( victim, value * 4 );
    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_pshow(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;

if (IS_NPC(ch)) return;
sprintf(buf, "[Name        ][Class        ][Gen][ Hit ][Mana ][Move ][HR  ][DR  ][AC   ][Exp       ]\n\r");
send_to_char(buf, ch);
	for (d = descriptor_list; d != NULL; d = d->next)
	{
		CHAR_DATA *wch;
		if (d->connected != CON_PLAYING) continue;
		wch = (d->original != NULL) ? d->original : d->character;

           if ( wch->class == CLASS_VAMPIRE )
                        strcpy( arg, "Vampire" );
                    else if ( wch->class == CLASS_WEREWOLF )
                        strcpy( arg, "Werewolf" );
                    else if ( wch->class == CLASS_ABOMINATION )
                        strcpy( arg, "Abomination" );
                    else if ( wch->class == CLASS_MAGE )
                        strcpy( arg, "Mage" );
                    else if ( wch->class == CLASS_LICH )
                         strcpy( arg, "Lich" );
                    else if ( wch->class == CLASS_SKINDANCER )
                        strcpy( arg, "Skindancer" );
                    else if ( wch->class == CLASS_DEMON )
                        strcpy( arg, "Demon" );
                    else if ( wch->class == CLASS_BAALI )
                        strcpy( arg, "Baali" );
                    else if ( wch->class == CLASS_BLACK_SPIRAL_DANCER )
                        strcpy( arg, "B.S.D." );
                    else if ( wch->class == CLASS_NEPHANDI )
                        strcpy( arg, "Nephandi" );
                    else if ( wch->class == CLASS_GHOUL )
                        strcpy( arg, "Ghoul" );
                    else
                        strcpy( arg, "None" );


sprintf(buf,"[%-12s][%-13s][%3d][%5d][%5d][%5d][%4d][%4d][%5d][%10d]\n\r",
		wch->name,
		arg,
		wch->vampgen,
		wch->max_hit,
		wch->max_mana,
		wch->max_move,
		char_hitroll(wch),
		char_damroll(wch),
		char_ac(wch),
                wch->exp );
		send_to_char(buf,ch);
	}
    return;
}

void do_undeny( CHAR_DATA *ch, char *argument )
{
        char arg[MAX_INPUT_LENGTH];
        char buf[MAX_STRING_LENGTH];
        char *oldname;  CHAR_DATA *victim;
        DESCRIPTOR_DATA *d;
        ROOM_INDEX_DATA *in_room;

    sprintf(buf,"%s: Undeny %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );    if ( arg[0] == '\0' )
    {
        send_to_char( "Undeny whom?", ch );
        return;
    }
    if ((victim = get_char_world(ch, arg)) != NULL)
    {
        send_to_char("They are already online.\n\r", ch );
        return;
    }
    if (!char_exists(FALSE,arg))
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
        oldname = str_dup(ch->name);
        d = ch->desc;
        do_autosave(ch,"");
        in_room = ch->in_room;
        extract_char(ch, TRUE);
        d->character = NULL;
        load_char_obj(d, arg);
        ch = d->character;
        ch->next = char_list;
        char_list = ch;
        char_to_room(ch,in_room);

   if ( ch->pcdata->denydate > 0 )
    {
        ch->pcdata->denydate = 0;
        send_to_char( "DENY removed.\n\r", ch );
    }
    else
    {
        send_to_char("They are not DENIED.\n\r", ch );
    }
        d = ch->desc;
        do_autosave(ch,"");
        in_room = ch->in_room;
        extract_char(ch,TRUE);
        d->character = NULL;
        load_char_obj(d, oldname);
        ch = d->character;
        ch->next = char_list;
        char_list = ch; char_to_room(ch,in_room);
        return;
}

void ftalk_who( CHAR_DATA *ch )
{
    CHAR_DATA	*vch;

    send_to_char("People currently on ftalk:\n\r",ch);
    for ( vch = char_list ; vch != NULL ; vch = vch->next )
    {
        if ( IS_NPC(vch)) continue;
	if ( !can_see( ch, vch ) ) continue;
        if (!IS_MORE(vch,MORE_FTALK)) continue;
        {
            send_to_char(vch->name, ch);
            send_to_char("\n\r", ch);
        }
    }
    return;
}

void status_who( CHAR_DATA *ch )
{
    CHAR_DATA	*vch;
    char	buf[MAX_STRING_LENGTH];
    sh_int      c_s = 0;    c_s = ch->race - 1;

    if ( c_s < 0 ) c_s = 0;

    sprintf( buf, "   [ ## ] [ %-12s ] [ %-6s ]\n\r", "Name", "Bounty" );
    send_to_char( buf, ch);

    for ( vch = char_list ; vch != NULL ; vch = vch->next )
    {
	if ( IS_NPC( vch ) ) continue;
	if ( !can_see( ch, vch ) ) continue;
	if ( ch->race > 0 && vch->race < 1 )
	    sprintf( buf, "   [%3d] [ %-12s ] [ %-6d ]\n\r", vch->race, vch->name, vch->pcdata->bounty);
	else if ( ch->race <= vch->race && ch != vch && !IS_IMMORTAL( vch ) )
	    sprintf( buf, " * [%3d] [ %-12s ] [ %-6d ]\n\r", vch->race, vch->name, vch->pcdata->bounty);
	else if ( c_s < vch->race && ch != vch && !IS_IMMORTAL( vch ) )
	    sprintf( buf, " * [%3d] [ %-12s ] [ %-6d ]\n\r", vch->race, vch->name, vch->pcdata->bounty);
	else
	    sprintf( buf, "   [%3d] [ %-12s ] [ %-6d ]\n\r", vch->race, vch->name, vch->pcdata->bounty);
	send_to_char( buf, ch );
    }
    return;
}

void do_paradox( CHAR_DATA *ch, char *argument)
    {
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
      DESCRIPTOR_DATA *d;
      argument = one_argument(argument, arg);

	if (arg[0] == '\0')
       {
		send_to_char("Paradox whom?\n\r", ch );
		return;
       }
    if ( get_trust(ch) >=  MAX_LEVEL - 2 && !str_cmp(arg,"all"))
     {
        for (d = descriptor_list; d != NULL; d = d->next)
       {
	    victim = d->character;
	    if (victim == NULL || IS_NPC(victim))
             continue;
            {
       paradox(victim);
         }
      }
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
	paradox(victim);
	send_to_char("Done!\n\r", ch );
}

void do_resetpassword( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char *pwdnew;

    if ( IS_NPC(ch) )
    return;

    argument=one_argument( argument, arg1 );
    argument=one_argument( argument, arg2 );

    victim = get_char_world(ch, arg1);


    if (  ( ch->pcdata->pwd != '\0' )
    && ( arg1[0] == '\0' || arg2[0] == '\0')  )
    {
        send_to_char( "Syntax: password <char> <new>.\n\r", ch );
        return;
    }
    if( victim == '\0' )
    {
     send_to_char( "That person isn't here, they have to be here to reset pwd's.\n\r", ch);
     return;
    }
    if ( IS_NPC( victim ) )
    {
    send_to_char( "You cannot change the password of NPCs!\n\r",ch);
    return;
    }

    if (( victim->level > LEVEL_IMMORTAL ) || ( get_trust(victim) > LEVEL_IMMORTAL ))

    {
     send_to_char( "You can't change the password of a god.\n\r",ch);
     return;
    }

    if ( strlen(arg2) < 5 )
    {
        send_to_char( "New password must be at least five characters long.\n\r", ch );
        return;
    }

    pwdnew = crypt( arg2, victim->name );
    free_string( victim->pcdata->pwd );
    victim->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( victim );
    send_to_char( "Ok.\n\r", ch );
    sprintf( buf, "Your password has been changed to %s.", arg2 );
    send_to_char( buf, victim);
    return;
}

DO_COM( do_ivamp )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *vch;

    if ( IS_NPC( ch ) ) return;

    send_to_char( "Vampires on:\n\r", ch );

    send_to_char( "[      Name      ] [      Sire      ] [      Clan      ] [ Gen ] [ Blood ]\n\r", ch );

    for ( vch = char_list ; vch != NULL ; vch = vch->next )
    {
	if ( IS_NPC( vch ) ) continue;
	if ( !IS_VAMPIRE( vch ) ) continue;
	if ( !can_see( ch, vch ) ) continue;

	if ( !str_cmp( vch->name, "morglum" ) )
	    sprintf( buf2, "Morglum" );
	else
	    sprintf( buf2, "%s", capitalize( vch->name ) );

	sprintf( buf, "[%-16s] ", buf2 );
	send_to_char( buf, ch );

	send_to_char( "[", ch );
	display_sire( ch, vch->lord );
	send_to_char( "] ", ch );

	if ( strlen( vch->clan ) < 3 )
	{
	    if ( IS_ABOMINATION( vch ) )
		sprintf( buf2, "Abomination" );
	    else if ( IS_LICH( vch ) )
		sprintf( buf2, "Lich" );
	    else if ( IS_BAALI( vch ) )
		sprintf( buf2, "Baali" );
	    else if ( vch->vampgen == 1 )
		sprintf( buf2, "Master" );
	    else if ( IS_MORE( vch, MORE_OUTCAST ) )
		sprintf( buf2, "Anarch" );
	    else
		sprintf( buf2, "Caitiff" );
	}
	else
	    sprintf( buf2, "%s", vch->clan );

	sprintf( buf, "[%-16s] ", buf2 );
	send_to_char( buf, ch );

	sprintf( buf, "[ %-2d  ] ", vch->vampgen );
	send_to_char( buf, ch );

	sprintf( buf, "[ %-5d ]\n\r", vch->blood[BLOOD_CURRENT] );
	send_to_char( buf, ch );
    }
    return;
}

DO_COM( do_itribe )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *vch;

    if ( IS_NPC( ch ) ) return;

    send_to_char( "Werewolf's on:\n\r", ch );

    send_to_char( "[      Name      ] [      Sire      ] [     Tribe      ] [ Gen ]\n\r", ch );

    for ( vch = char_list ; vch != NULL ; vch = vch->next )
    {
	if ( IS_NPC( vch ) ) continue;
	if ( !IS_WEREWOLF( vch ) ) continue;
	if ( !can_see( ch, vch ) ) continue;

	if ( !str_cmp( vch->name, "morglum" ) )
	    sprintf( buf2, "Morglum" );
	else
	    sprintf( buf2, "%s", capitalize( vch->name ) );

	sprintf( buf, "[%-16s] ", buf2 );
	send_to_char( buf, ch );

	send_to_char( "[", ch );
	display_sire( ch, vch->lord );
	send_to_char( "] ", ch );

	if ( strlen( vch->clan ) < 3 )
	{
	    if ( IS_ABOMINATION( vch ) )
		sprintf( buf2, "Abomination" );
	    else if ( IS_SKINDANCER( vch ) )
		sprintf( buf2, "Skindancer" );
	    else if ( IS_BLACK_SPIRAL_DANCER( vch ) )
		sprintf( buf2, "B.S.D." );
	    else if ( vch->vampgen == 1 )
		sprintf( buf2, "Master" );
	    else if ( IS_MORE( vch, MORE_OUTCAST ) )
		sprintf( buf2, "Ronin" );
	    else
		sprintf( buf2, "Orphan" );
	}
	else
	    sprintf( buf2, "%s", vch->clan );

	sprintf( buf, "[%-16s] ", buf2 );
	send_to_char( buf, ch );

	sprintf( buf, "[ %-2d  ]\n\r", vch->vampgen );
	send_to_char( buf, ch );
    }
    return;
}

DO_COM( do_image )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *vch;

    if ( IS_NPC( ch ) ) return;

    send_to_char( "Mages on:\n\r", ch );

    send_to_char( "[      Name      ] [      Sire      ] [  Colour  ] [   Level   ]\n\r", ch );

    for ( vch = char_list ; vch != NULL ; vch = vch->next )
    {
	if ( IS_NPC( vch ) ) continue;
	if ( !IS_SET( vch->class, CLASS_MAGE ) ) continue;
	if ( !can_see( ch, vch ) ) continue;

	if ( !str_cmp( vch->name, "morglum" ) )
	    sprintf( buf2, "Morglum" );
	else
	    sprintf( buf2, "%s", capitalize( vch->name ) );

	sprintf( buf, "[%-16s] ", buf2 );
	send_to_char( buf, ch );

	send_to_char( "[", ch );
	display_sire( ch, vch->lord );
	send_to_char( "] ", ch );

	if ( vch->class != CLASS_MAGE )
	{
	    if ( IS_NEPHANDI( vch ) )
		sprintf( buf2, "Nephandi" );
	    else if ( IS_SKINDANCER( vch ) )
		sprintf( buf2, "Skindancer" );
	    else if ( IS_LICH( vch ) )
		sprintf( buf2, "Lich" );
	    else
		sprintf( buf2, "Bugged" );
	}
	else
	{
	    switch( vch->pcdata->runes[0] )
	    {
		default		  :	sprintf( buf2, "Bugged" );	break;
		case PURPLE_MAGIC :	sprintf( buf2, "Air" );	break;
		case RED_MAGIC	  :	sprintf( buf2, "Fire"    );	break;
		case BLUE_MAGIC	  :	sprintf( buf2, "Water"   );	break;
		case GREEN_MAGIC  :	sprintf( buf2, "Earth"  );	break;
		case YELLOW_MAGIC :	sprintf( buf2, "Spirit" );	break;
	    }
	}

	sprintf( buf, "[%-10s] ", buf2 );
	send_to_char( buf, ch );

	if ( vch->class != CLASS_MAGE )
	{
	    if ( IS_NEPHANDI( vch )
	    ||	 IS_LICH( vch )
	    ||	 IS_SKINDANCER( vch ) )
	    {
		if ( vch->vampgen <= 3 )
		    sprintf( buf2, "Leader" );
		else
		    sprintf( buf2, "N/A" );
	    }
	    else
		sprintf( buf2, "Bugged" );
	}
	else
	{
	    switch( vch->level )
	    {
		default		      :	sprintf( buf2, "Mortal"      );	break;
		case LEVEL_HERO	      :	sprintf( buf2, "Avatar"	     );	break;
		case LEVEL_APPRENTICE :	sprintf( buf2, "Apprentice"  );	break;
		case LEVEL_MAGE	      :	sprintf( buf2, "Mage"        );	break;
		case LEVEL_ARCHMAGE   :	sprintf( buf2, "Archmage"    );	break;
		case LEVEL_BUILDER    :	sprintf( buf2, "Builder"     );	break;
		case LEVEL_QUESTMAKER :	sprintf( buf2, "Questmaker"  );	break;
		case LEVEL_ENFORCER   :	sprintf( buf2, "Enforcer"    );	break;
		case LEVEL_JUDGE      :	sprintf( buf2, "Judge"       );	break;
		case LEVEL_HIGHJUDGE  :	sprintf( buf2, "Highjudge"   );	break;
		case LEVEL_IMPLEMENTOR:	sprintf( buf2, "Implementor" );	break;
	    }
	}

	sprintf( buf, "[%-11s]\n\r", buf2 );
	send_to_char( buf, ch );
    }
    return;
}

DO_COM( do_idemon )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *vch;

    if ( IS_NPC( ch ) ) return;

    send_to_char( "Demons on:\n\r", ch );

    send_to_char( "[      Name      ] [      Lord      ] [ Position ] [ Mag/Imm ] [       Power       ]\n\r", ch );

    for ( vch = char_list ; vch != NULL ; vch = vch->next )
    {
	if ( IS_NPC( vch ) ) continue;
	if ( !IS_DEMON( vch ) ) continue;
	if ( !can_see( ch, vch ) ) continue;

	if ( !str_cmp( vch->name, "morglum" ) )
	    sprintf( buf2, "Morglum" );
	else
	    sprintf( buf2, "%s", capitalize( vch->name ) );

	sprintf( buf, "[%-16s] ", buf2 );
	send_to_char( buf, ch );

	if ( vch->class != CLASS_DEMON )
	{
	    if ( IS_BAALI( vch ) )
		sprintf( buf2, "Baali" );
	    else if ( IS_BLACK_SPIRAL_DANCER( vch ) )
		sprintf( buf2, "B.S.D." );
	    else if ( IS_NEPHANDI( vch ) )
		sprintf( buf2, "Nephandi" );
	    else
		sprintf( buf2, "Bugged" );
	}
	else
	{
	    if ( IS_SET( vch->act, PLR_DEMON ) )
	    	sprintf( buf2, "Demon Lord" );
	    else if ( vch->lord != NULL && !str_cmp( vch->lord, "morglum" ) )
	    	sprintf( buf2, "Morglum" );
	    else if ( strlen( vch->lord ) > 2 )
	    	sprintf( buf2, "%s", capitalize( vch->lord ) );
	    else
	    	sprintf( buf2, "Bugged" );
	}

	sprintf( buf, "[%-16s] ", buf2 );
	send_to_char( buf, ch );

	if ( vch->class != CLASS_DEMON )
	{
	    if ( vch->vampgen <= 3 )
		sprintf( buf2, "Leader" );
	    else
		sprintf( buf2, "N/A" );
	}
	else
	{
	    if ( IS_SET( vch->act, PLR_DEMON ) )
		sprintf( buf2, "Lord" );
	    else if ( IS_MORE( vch, MORE_OUTCAST ) )
		sprintf( buf2, "Rogue" );
	    else if ( IS_EXTRA( vch, EXTRA_PRINCE ) )
		sprintf( buf2, "Prince" );
	    else if ( IS_EXTRA( vch, EXTRA_SIRE ) )
		sprintf( buf2, "Greater" );
	    else
		sprintf( buf2, "Lesser" );
	}

	sprintf( buf, "[%-10s] ", buf2 );
	send_to_char( buf, ch );

	sprintf( buf, "[   %3d%%  ] ", vch->pcdata->atm );
	send_to_char( buf, ch );

	sprintf( buf, "[%-9d %9d]\n\r",
	    vch->pcdata->power[0],
	    vch->pcdata->power[1] );
	send_to_char( buf, ch );
    }
    return;
}

void do_makepreg ( CHAR_DATA *ch, char *argument )
{
         CHAR_DATA   *mother;
	   CHAR_DATA   *father;
         char        arg1[MAX_INPUT_LENGTH];
         char        arg2[MAX_INPUT_LENGTH];

         if (IS_NPC( ch ) ) return;
         argument = one_argument( argument, arg1 );
         argument = one_argument(argument, arg2 );

         if ( arg1[0] == '\0' || arg2[0] == '\0' )
         {
        	send_to_char( "Syntax: makepreg [mother] [father]\n\r", ch );
        	return;
         }
         if (( mother = get_char_world( ch, arg1 ) ) == NULL )
         {
        	send_to_char( "Mother is not here.\n\r", ch );
        	return;
         }
         if ( ( father = get_char_world( ch, arg2 ) ) == NULL )
         {
	         send_to_char( "Father is not here.\n\r", ch );
                 return;
         }
         if ( IS_NPC( mother ) || IS_NPC( father ) )
         {
                 send_to_char( "Not on NPC's.\n\r", ch );
                 return;
          }
         if ( IS_EXTRA( mother, EXTRA_PREGNANT ) )
         {
                 send_to_char( "They are already pregnant.\n\r", ch );
                return;
          }

          make_preg( mother, father );
          send_to_char( "You are now pregnant.\n\r", mother );
          send_to_char( "You are going to be a father.\n\r", father );

          if ( ch != mother && ch != father )
              send_to_char( "Done.\n\r", ch );
              return;
}

void do_addlag(CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        char arg1[MAX_STRING_LENGTH];
        int x;

        argument = one_argument(argument, arg1);

        if (arg1[0] == '\0')
        {
                send_to_char("Addlag to who?", ch);
                return;
        }

        if ((victim = get_char_world(ch, arg1)) == NULL)
        {
                send_to_char("They're not here.", ch);
                return;
        }

        if ((x = atoi(argument)) <= 0)
        {
                send_to_char("That makes a LOT of sense, ya lag yourself *DUH*.", ch);
                return;
        }

        if (x > 100)
        {
                send_to_char("There's a limit to cruel and unusual punishment", ch);
                return;
        }

        send_to_char("Somebody REALLY dosen't like you", victim);
        WAIT_STATE(victim, x);
        send_to_char("That ought to show that nerdy bitch...", ch);
        return;
}

DO_COM( do_renamep )
{
    char		arg[MIL];
    char		buf[MSL];
    char		new_name[MIL];
    DESCRIPTOR_DATA	*d;
    CHAR_DATA		*victim;
    OBJ_DATA		*obj;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, new_name );

    if ( IS_NPC( ch ) ) return;

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "Rename who?\n\r", ch );
	return;
    }

    if ( new_name[0] == '\0' )
    {
	send_to_char( "Rename them to what?\n\r", ch );
	return;
    }

    if ( IS_NPC( victim ) )
    {
	send_to_char( "Use mset instead.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL || victim->desc->connected != CON_PLAYING )
    {
	send_to_char( "They are not avaliable for a rename at the moment.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL( victim ) )
    {
        send_to_char( "You are unable to rename Gods!\n\r",ch);
        return;
    }

    if ( char_exists( FILE_PLAYER, new_name ) )
    {
	send_to_char( "There is already a player saved by that name.\n\r", ch );
	return;
    }

    for ( d = descriptor_list ; d != NULL ; d = d->next )
    {
	if ( d->character != NULL && victim->desc != d )
	{
	    if ( !IS_NPC( d->character )
	    &&	 strlen( d->character->name ) > 0
	    &&	!str_cmp( d->character->name, new_name ) )
	    {
		send_to_char( "There is already a player with that name.\n\r", ch );
		return;
	    }
	}
    }

    if ( !check_parse_name( new_name ) )
    {
	send_to_char( "That is an illegal new name!\n\r", ch );
	return;
    }

    for ( obj = object_list ; obj != NULL ; obj = obj->next )
    {
	if ( strlen( obj->questowner ) < 3 ) continue;
	if ( str_cmp( victim->name, obj->questowner ) ) continue;
	FILL_STRING( obj->questowner, capitalize( new_name ) );
    }

    sprintf( buf, "%s%s", PLAYER_DIR, capitalize( victim->name ) );

    free_string( victim->name );
    victim->name = str_dup( capitalize( new_name ) );

    unlink( buf );

    do_autosave( victim, "" );

    sprintf( buf, "You have been renamed to %s.\n\r", capitalize( new_name ) );
    send_to_char( buf, victim );
    send_to_char( "Rename complete.\n\r", ch );
    return;
}

void do_resetarea(CHAR_DATA *ch, char *argument)
{
    send_to_char("You patiently twiddle your thumbs, waiting for the reset.\n\r",ch);
    send_to_char("Oh fuck this, you MAKE it reset!\n\r",ch);
    area_update();
}

// Delete players who want to be deleted.
// Also check their password for authentication - Morglum.

DO_COM ( do_delete )
{
    char pfile[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char *pArg;
    int i;

// Grab the first word, case sensitive

    {
     pArg = arg;
     while (*argument == ' ') argument++;
     while (*argument != '\0')
    {
     if (*argument == ' ')
    {
     argument++;
     break;
    }
     *pArg++ = *argument++;
    }
     *pArg = '\0';
    }

    if ( arg[0] == '\0' )
    {
        send_to_char( "Syntax: delete [password]\n\r", ch );
        send_to_char( "   If you self delete, you will #rNOT#n get reimbursed.\n\r", ch);
        send_to_char( "   Use this command very carefully.\n\r", ch);
        sprintf(log_buf, "%s is contemplating deletion.", ch->name);
        return;
    }

     if (strcmp(arg, ch->pcdata->pwd) && strcmp(crypt(arg, ch->pcdata->pwd), ch->pcdata->pwd))
    {
     send_to_char("Wrong password.\n\r", ch);
     sprintf(arg, "%s tried to delete with the wrong pwd.", ch->name);
     log_string(arg);
     return;
    }

    if (ch->vampgen == 2 || IS_SET(ch->act, PLR_DEMON) || ch->level >= LEVEL_ARCHMAGE
        || ch->level >= LEVEL_IMMORTAL )
      {
    send_to_char("Don't be stupid, you're a leader.\n\r", ch);
    return;
	}

   else
      {
     send_to_char("Character deleted.\n\r", ch);
      }

// Get name while ch is still valid

     sprintf(arg, "%s has destroyed %sself.", ch->name, (ch->sex -1) ? "her" : "him");
     log_string(arg);
     do_info(ch, arg);
     sprintf(pfile, "%s%s", PLAYER_DIR, capitalize(ch->name));
     do_quit(ch, "");
     ch = NULL;

// Doesn't HAVE to be limited to 99... but more would be stupid

     for (i = 0; i < 99; i++)
    {
     sprintf(arg, "ls %s.d%d", pfile, i);
     if (system(arg) != 0) break;
    }

// Move pfile to pfile.d*

     sprintf(arg, "mv %s %s.d%d", pfile, pfile, i);
     if (system(arg) != 0)
    {
     sprintf(pfile, "System returned non-zero from: %s", arg);
     log_string(pfile);
    }
     return;
}

DO_COM( do_pstat )
{
    char	arg[MAX_INPUT_LENGTH];
    char	buf[MAX_STRING_LENGTH];
    CHAR_DATA	*victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char("Pstat whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char("They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_GODLESS ) &&
	 get_trust( ch ) < NO_GODLESS )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    sprintf( buf, "Name : %s.\n\r",
	IS_NPC( victim )	? victim->short_descr : victim->name );
    send_to_char( buf, ch );
    sprintf( buf, "Sex : %s. Room : %d. Align : %d. Primal : %d. Quest : %d.\n\r",
	victim->sex == SEX_MALE		? "Male"	:
	victim->sex == SEX_FEMALE	? "Female"	: "None",
	victim->in_room == NULL		? 0		: victim->in_room->vnum,
	victim->alignment,
	victim->practice,
	IS_NPC( victim )		? 0		: victim->pcdata->quest
	);
    send_to_char( buf, ch );

    sprintf( buf, "Level : %d. Trust : %d. Gold : %d. Exp : %d.\n\r",
	victim->level,
	victim->trust,
	victim->gold,
	victim->exp );
    send_to_char( buf, ch );

    sprintf( buf, "Hit : %d. Dam : %d. AC : %d. Position : %s. Wimpy : %d.\n\r",
	char_hitroll( victim ),
	char_damroll( victim ),
	char_ac( victim ),
	capitalize( get_position_name( victim->position ) ),
	victim->wimpy );
    send_to_char( buf, ch );

    sprintf( buf, "HP %d/%d. Mana %d/%d. Move %d/%d.\n\r",
	victim->hit, victim->max_hit,
	victim->mana, victim->max_mana,
	victim->move, victim->max_move );
    send_to_char( buf, ch );

    sprintf( buf, "Fighting : %s. (%d)\n\r",
	victim->fighting ? victim->fighting->name  : "(None)",
	victim->fighting ? victim->fighting->level : 0 );
    send_to_char( buf, ch );

    sprintf( buf, "Pkill : %d. Pdeath : %d. Mkill : %d. Mdeath : %d.\n\r",
	IS_NPC( victim ) ? 0 : victim->pkill,
	IS_NPC( victim ) ? 0 : victim->pdeath,
	IS_NPC( victim ) ? 0 : victim->mkill,
	IS_NPC( victim ) ? 0 : victim->mdeath );
    send_to_char( buf, ch );

    sprintf( buf, "TotExp  : %12d. TotMobLev  : %10d. TotQuestPoints : %10d.\n\r",
	IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_TOTAL_XP],
	IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_TOTAL_LEVEL],
	IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_QUEST] );
    send_to_char( buf, ch );

    sprintf( buf, "HighExp : %12d. HighMobLev : %10d. TotQuests     : %10d.\n\r",
	IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_HIGH_XP],
	IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_HIGH_LEVEL],
	IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_NUM_QUEST] );
    send_to_char( buf, ch );

    if ( !IS_NPC( victim ) )
    {
	sprintf( buf, "Paradox : %d.\n\r",
	    victim->paradox[0]);
	send_to_char( buf, ch );
    }

    if ( !IS_NPC( victim ) )
    {
	sprintf( buf, "%-7s : %4d.",
	    "Unarmed", victim->wpn[0] );
	send_to_char( buf, ch );
	sprintf( buf, " %-7s : %4d.",
	    capitalize( get_type_weapon( 1 ) ), victim->wpn[1] );
	send_to_char( buf, ch );
	sprintf( buf, " %-7s : %4d.",
	    capitalize( get_type_weapon( 2 ) ), victim->wpn[2] );
	send_to_char( buf, ch );
	sprintf( buf, " %-7s : %4d.",
	    capitalize( get_type_weapon( 3 ) ), victim->wpn[3] );
	send_to_char( buf, ch );
	sprintf( buf, " %-7s : %4d.\n\r",
	    capitalize( get_type_weapon( 4 ) ), victim->wpn[4] );
	send_to_char( buf, ch );
	sprintf( buf, "%-7s : %4d.",
	    capitalize( get_type_weapon( 5 ) ), victim->wpn[5] );
	send_to_char( buf, ch );
	sprintf( buf, " %-7s : %4d.",
	    capitalize( get_type_weapon( 6 ) ), victim->wpn[6] );
	send_to_char( buf, ch );
	sprintf( buf, " %-7s : %4d.",
	    capitalize( get_type_weapon( 7 ) ), victim->wpn[7] );
	send_to_char( buf, ch );
	sprintf( buf, " %-7s : %4d.",
	    capitalize( get_type_weapon( 8 ) ), victim->wpn[8] );
	send_to_char( buf, ch );
	sprintf( buf, " %-7s : %4d.\n\r",
	    capitalize( get_type_weapon( 9 ) ), victim->wpn[9] );
	send_to_char( buf, ch );
	sprintf( buf, "%-7s : %4d.",
	    capitalize( get_type_weapon( 10 ) ), victim->wpn[10] );
	send_to_char( buf, ch );
	sprintf( buf, " %-7s : %4d.",
	    capitalize( get_type_weapon( 11 ) ), victim->wpn[11] );
	send_to_char( buf, ch );
	sprintf( buf, " %-7s : %4d.\n\r",
	    capitalize( get_type_weapon( 12 ) ), victim->wpn[12] );
	send_to_char( buf, ch );

	sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
	    "Purple",	victim->spl[PURPLE_MAGIC],
	    "Red",	victim->spl[RED_MAGIC],
	    "Blue",	victim->spl[BLUE_MAGIC],
	    "Green",	victim->spl[GREEN_MAGIC],
	    "Yellow",	victim->spl[YELLOW_MAGIC] );
	send_to_char( buf, ch );

	sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
	    "Viper",	victim->stance[STANCE_VIPER],
	    "Crane",	victim->stance[STANCE_CRANE],
	    "Crab",	victim->stance[STANCE_CRAB],
	    "Mongoose",	victim->stance[STANCE_MONGOOSE],
	    "Bull",	victim->stance[STANCE_BULL] );
	send_to_char( buf, ch );

	sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %-3d. %-8s : %3d.\n\r",
	    "Mantis",	victim->stance[STANCE_MANTIS],
	    "Dragon",	victim->stance[STANCE_DRAGON],
	    "Tiger",	victim->stance[STANCE_TIGER],
	    "Monkey",	victim->stance[STANCE_MONKEY],
	    "Swallow",	victim->stance[STANCE_SWALLOW] );
	send_to_char( buf, ch );

	sprintf( buf, "Super Stance Names : %s\n\r",
	    victim->stance[11] > 0 ? victim->pcdata->stancelist : "(None)" );
	send_to_char( buf, ch );

	sprintf( buf, "SS1 : %3d. SS2 : %3d. SS3 : %3d. SS4 : %3d. SS5 : %3d.\n\r",
	    victim->stance[11],
	    victim->stance[12],
	    victim->stance[13],
            victim->stance[14],
	    victim->stance[15] );
	send_to_char( buf, ch );
}

    sprintf( buf, "Act         : %s\n\r",
	IS_NPC( victim ) ? act_bit_name( victim->act ) : plr_bit_name( victim->act )
	);
    send_to_char( buf, ch );

    if ( !IS_NPC( victim ) )
    {
	sprintf( buf, "Mod         : %s\n\r",
	    victim->pcdata->bodymods <= 0 ? "(None)" : mod_bit_name( victim->pcdata->bodymods ) );
	send_to_char( buf, ch );
    }

    sprintf( buf, "Extra       : %s\n\r",
	victim->extra <= 0 ? "(None)" : extra_plr_bit_name( victim->extra ) );
    send_to_char( buf, ch );

    sprintf( buf, "More        : %s\n\r",
	victim->more <= 0 ? "(None)" : more_bit_name( victim->more ) );
    send_to_char( buf, ch );

    sprintf( buf, "ItemAff     : %s\n\r",
	victim->itemaffect <= 0 ? "(None)" : itemaffect_bit_name( victim->itemaffect ) );
    send_to_char( buf, ch );

    sprintf( buf, "Affected by : %s.\n\r",
	affect_bit_name( victim->affected_by ) );
    send_to_char( buf, ch );

    return;
}

void do_pmort( CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
      CHAR_DATA *victim;
	DESCRIPTOR_DATA *d;
	argument = one_argument(argument, arg);

	if (arg[0] =='\0')
 {
		send_to_char("Mort whom?\n\r", ch);
		return;}

	if ( get_trust(ch) >= MAX_LEVEL - 1 && !str_cmp(arg,"all"))
	{
		for (d = descriptor_list; d != NULL; d = d->next)
	{
		victim = d->character;

		if (victim == NULL || IS_NPC(victim))
		   continue;
	{ victim->hit = -10; }
          }
}

if ( ( victim = get_char_world( ch, arg ) ) == NULL )
{
send_to_char( "They aren't here..\n\r", ch) ;
return;
}
victim->hit = -10;
send_to_char("Ok, they're morted!\n\r", ch );
}

DO_COM( do_kindness )
{
    CHAR_DATA	*victim;
    char	arg[MIL];

    if ( IS_NPC( ch ) ) return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	STC( "Who do has shown such an act of Kindness, that they deserve to regain golconda?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	STC( "They are not here.\n\r", ch );
	return;
    }

    if ( IS_NPC( victim ) )
    {
	STC( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( !IS_VAMPIRE( victim ) )
    {
	STC( "Only vampires can regain golconda.\n\r", ch );
	return;
    }

    if ( victim->beast < 99 || victim->truebeast < 99 )
    {
	STC( "They are still able to gain golconda.\n\r", ch );
	return;
    }

    victim->truebeast--;
    act( "$n has given you the chance to regain golconda.\n\r", ch, NULL, victim, TO_VICT );
    STC( "You grant them the ability to regain golconda.\n\r", ch );
    return;
}

void do_pfilescan( CHAR_DATA *ch, char *argument )
{
   char arg1[MSL];
   char buf[MSL];
   char strsave[MSL];
   FILE *fp;

   argument = one_argument( argument, arg1 );
      if (arg1[0] == '\0')
       {
       send_to_char("syntax:\n\r",ch);
       send_to_char("pscan <all> <Searchstring>\n\r",ch);
       send_to_char("pscan <Player> <Searchstring>    \n\r",ch);
       return;
       }
    if (!str_cmp(arg1,"all"))
    {
        if (argument[0] == '\0')
        {
        send_to_char("Search for what?\n\r",ch);
        return;
        }
        sprintf(buf, "grep -i \"%s\" %s*", argument, PLAYER_DIR);
        syspipe( ch, buf );
        send_to_char("\n\rSearch Complete.\n\r",ch);
        return;
    }
    else
    {
        if (argument[0] == '\0')
        {
        send_to_char("Search for what?\n\r",ch);
        return;
        }
        sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( arg1 ) );

        if ( ( fp = fopen( strsave, "r" ) ) == NULL )
        {
        send_to_char("\n\rCharacter File Not Found\n\r",ch);
        return;
        }
        else
        {
        sprintf(buf, "grep -i \"%s\" %s%s", argument, PLAYER_DIR, capitalize( arg1 ));
        syspipe( ch, buf );
        send_to_char("\n\rSearch Complete.\n\r",ch);
        fclose( fp );
        return;
        }
    }

    return;
}

void do_logscan( CHAR_DATA *ch, char *argument )
{
   char arg1[MSL];
   char buf[MSL];
   char strsave[MSL];
   FILE *fp;


   argument = one_argument( argument, arg1 );
      if (arg1[0] == '\0')
       {
       send_to_char("syntax:\n\r",ch);
       send_to_char("lscan list\n\r",ch);
       send_to_char("lscan <Logfile> <Searchstring>    \n\r",ch);
       return;
       }

    if (!str_cmp(arg1,"list"))
    {
       sprintf( buf, "ls %s", LOG_DIR);
       syspipe( ch, buf );
       send_to_char("\n\rOK.\n\r",ch);
       return;
    }
    else
    {
        if (argument[0] == '\0')
        {
        send_to_char("Search for what?\n\r",ch);
        return;
        }
        sprintf( strsave, "%s%s", LOG_DIR, arg1 );

        if ( ( fp = fopen( strsave, "r" ) ) == NULL )
        {
        send_to_char("\n\rLog File Not Found.\n\r",ch);
        return;
        }
        else
        {
        sprintf(buf, "grep -i \"%s\" %s%s", argument, LOG_DIR, arg1);
        syspipe(ch,buf);
        send_to_char("\n\rOK.\n\r",ch);
        fclose( fp );
        return;
        }
    }


    return;
}

void syspipe( CHAR_DATA *ch, char *argument )
{
    char buf[5000];
    FILE *fp;

    fp = popen( argument, "r" );
    fgetf( buf, 5000, fp );
	send_to_char( buf, ch );
// hrmm need to develop an echo that handles the output info in pages...
    pclose( fp );

    return;
}

char *fgetf( char *s, int n, register FILE *iop )
{
    register int c;
    register char *cs;

    c = '\0';
    cs = s;
    while( --n > 0 && (c = getc(iop)) != EOF)
        if ((*cs++ = c) == '\0')
            break;
    *cs = '\0';
    return((c == EOF && cs == s) ? NULL : s);
}
