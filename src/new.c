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
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "merc.h"

bool magic_immune( CHAR_DATA *ch, CHAR_DATA *victim, bool global )
{
    if ( ch == victim )
	return FALSE;
    else if ( IS_IMMORTAL( victim ) && ch != victim )
	return TRUE;
    else if ( !global && IS_AFFECTED( victim, AFF_AURA_NO_MAGIC )
	 &&	IS_MAGE( victim ) )
	return TRUE;
    else if ( IS_ITEMAFF( victim, ITEMA_REFLECT ) )
	return TRUE;
    else if ( !IS_NPC( victim ) && victim->pcdata->atm >= number_percent( ) )
	return TRUE;
    else if ( victim->in_room == NULL || IS_SET( victim->in_room->room_flags, ROOM_SAFE ) )
	return TRUE;
    else
	return FALSE;
}

void check_mortal( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( IS_NPC( ch ) || IS_NPC( victim ) ) return;

    if ( victim->position > POS_MORTAL ) return;
    return;
}

/* Shows name not ch->morph */
void act4( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
/* Next 1 line
*/
    CHAR_DATA *to_old;

    CHAR_DATA *vch = (CHAR_DATA *) arg2;
/* Next 2 lines
*/
    CHAR_DATA *familiar = NULL;
    CHAR_DATA *wizard = NULL;

    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;
/* Next 1 line
*/
    bool is_fam;

    bool is_ok;
    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL || vch->in_room == NULL)
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }

    for ( ; to != NULL; to = to->next_in_room )
    {
/* Next 2 lines
*/
	is_fam = FALSE;
	to_old = to;

	if ( type == TO_CHAR && to != ch ) continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
	if ( type == TO_ROOM && to == ch ) continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) ) continue;
	if ( to->desc != NULL && to->desc->connected != CON_PLAYING ) continue;
/* Next if statement
*/
	if ( to->desc == NULL && IS_NPC(to) && (wizard = to->wizard) != NULL )
	{
	    if (!IS_NPC(wizard) && ((familiar=wizard->pcdata->familiar) != NULL)
		&& familiar == to)
	    {
		if (to->in_room == ch->in_room &&
		    wizard->in_room != to->in_room)
		{
		    to = wizard;
		    SET_BIT(to->extra, EXTRA_CAN_SEE);
		    is_fam = TRUE;
		}
	    }
	}

	if ( to->desc == NULL || !IS_AWAKE(to) )
	{
/* Next if statement
*/
	    if (is_fam)
	    {
		REMOVE_BIT(to->extra, EXTRA_CAN_SEE);
		to = to_old;
	    }

	    continue;
	}

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL &&
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL &&
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok)
	    {
/* Next if statement
*/
		if (is_fam)
		{
		    REMOVE_BIT(to->extra, EXTRA_CAN_SEE);
		    to = to_old;
		}

		continue;
	    }
    	}

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {
		bug( "Act: missing arg2 for code %d.", *str );
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  bug( "Act: bad code %d.", *str );
			  i = " <@@@> ";				break;
		/* Thx alex for 't' idea */
		case 't': i = (char *) arg1;				break;
		case 'T': i = (char *) arg2;          			break;
		case 'n': i = PERS2( ch,  to  );			break;
		case 'N': i = PERS2( vch, to  );			break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'E': i = he_she  [URANGE(0, vch ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'M': i = him_her [URANGE(0, vch ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'S': i = his_her [URANGE(0, vch ->sex, 2)];	break;

		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'P':
		    i = can_see_obj( to, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to)
			    ? "you" : obj2->short_descr)
			    : "something";
		    break;

		case 'd':
		    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
		    {
			i = "door";
		    }
		    else
		    {
			one_argument( (char *) arg2, fname );
			i = fname;
		    }
		    break;
		}
	    }

	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';
/* Next if statement
*/
	if (is_fam)
	{
	    if (to->in_room != ch->in_room &&
		familiar != NULL && familiar->in_room == ch->in_room)
		send_to_char("[ ", to);
	    else
	    {
		REMOVE_BIT(to->extra, EXTRA_CAN_SEE);
		to = to_old;
		continue;
	    }
	}

	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );
/* Next if statement
*/
	if (is_fam)
	{
	    REMOVE_BIT(to->extra, EXTRA_CAN_SEE);
	    to = to_old;
	}

    }
    return;
}

char * status_name( sh_int status, sh_int class, bool expand )
{
    static char	buf[512];

    switch( status )
    {
	default :
	    strcpy( buf, expand ? "Bugged" : "<[ Whoops! ]>" );
	    break;
	case 0:
	    strcpy( buf, expand ? "an Avatar" : "Avatar" );
	    break;
	         case 1:  case 2:  case 3:  case 4:
	    switch( class )
	    {
		default:
		    strcpy( buf, expand ? "an Immortal" : "Immortal" );
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, "Recognised" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, expand ? "a Garou" : "Garou" );
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "a Chanter" : "Chanter");
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "a Lost Soul" : "Lost Soul" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, expand ? "a Servant" : "Servant" );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, "Scum" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
	case 5: case 6: case 7: case 8: case 9:
	    switch( class )
	    {
		default:
		    strcpy( buf, "Immortal" );
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, "Known" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, expand ? "a Cliath" : "Cliath" );
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "a Conjurer" : "Conjurer" );
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "a Lesser Shadow" : "LesserShadow" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, expand ? "a Loyalist" : "Loyalist" );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, "Lothesome" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
	case 10: case 11: case 12: case 13: case 14:
	    switch( class )
	    {
		default:
		    strcpy( buf, expand ? "a Godling" :  "Godling");
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, "Revered" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, expand ? "a Fostern" : "Fostern");
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "a Magician" : "Magician" );
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "a Shadow" : "Shadow" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, expand ? "an Infiltrator" : "Infiltrator" );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, "Noxious" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
	case 15: case 16: case 17: case 18: case 19:
	    switch( class )
	    {
		default:
		    strcpy( buf, expand ? "a Godling" : "Godling");
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, "Respected" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, expand ? "an Adren" :"Adren" );
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "a Magus" : "Magus" );
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "a Greater Shadow" : "GreaterShadow" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, expand ? "a Corrupter" : "Corrupter" );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, "Ransid" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
	case 20: case 21: case 22: case 23: case 24:
	    switch( class )
	    {
		default:
		    strcpy( buf, expand ? "a Demigod" : "Demigod" );
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, expand ? "a Clan Head" : "Clan Head" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, "Ahroun" );
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "a Sorcerer" : "Sorcerer" );
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "a Shadow Lord" : "Shadow Lord" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, expand ? "a Terrorist" : "Terrorist" );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, "Dishonest" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
	case 25: case 26: case 27: case 28: case 29:
	    switch( class )
	    {
		default:
		    strcpy( buf, expand ? "a Demigod" : "Demigod" );
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, expand ? "a Primogen" : "Primogen" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, expand ? "an Athro" : "Athro" );
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "a Wizard" : "Wizard" );
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "a Lesser Devil" : "Lesser Devil" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, expand ? "a Torturer" : "Torturer" );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, "Deceitful" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
	case 30: case 31: case 32: case 33: case 34:
	    switch( class )
	    {
		default:
		    strcpy( buf, expand ? "a Lesser God" : "Lesser God" );
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, expand ? "a Shiriff" : "Shiriff" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, expand ? "an Adult" : "Adult" );
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "a Magical Knight" : "Magical Knight" );
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "a Devil" : "Devil" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, expand ? "a Dark Kindred" : "Dark Kindred" );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, "Untrustworthy" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
	case 35: case 36: case 37: case 38: case 39:
	    switch( class )
	    {
		default:
		    strcpy( buf, expand ? "a Lesser God" : "Lesser God" );
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, expand ? "a Herald" : "Herald" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, expand ? "an Elder" : "Elder" );
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "a War Mage" : "War Mage" );
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "a Greater Devil" : "Greater Devil" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, expand ? "a Hellraiser" : "Hellraiser" );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, "Wicked" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
	case 40: case 41: case 42: case 43: case 44:
	    switch( class )
	    {
		default:
		    strcpy( buf, expand ? "a Greater God" : "Greater God" );
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, expand ? "a Prince" : "Prince" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, expand ? "a Sept Leader" : "Sept Leader" );
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "a Battle Mage" : "Battle Mage" );
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "a Demon Knight" : "Demon Knight" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, expand ? "a Diablerizer" : "Diablerizer"  );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, "Evil" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
	case 45: case 46: case 47: case 48: case 49:
	    switch( class )
	    {
		default:
		    strcpy( buf, expand ? "a Greater God" : "Greater God" );
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, expand ? "an Archon" : "Archon" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, expand ? "the Claw of Gaia" : "Claw of Gaia" );
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "a Mage Lord" : "Mage Lord" );
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "a Knight of Hell" : "Hell's Knight" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, expand ? "a Dark Thaumaturg" : "Dark Thaumaturg" );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, "Treacherous" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
	case 50:
	    switch( class )
	    {
		default:
		    strcpy( buf, expand ? "a Supreme God" : "Supreme God" );
		    break;
		case CLASS_VAMPIRE:
		    strcpy( buf, expand ? "a Justicar" : "Justicar" );
		    break;
		case CLASS_WEREWOLF:
		    strcpy( buf, "Gaia's Rage" );
		    break;
/*
		case CLASS_ABOMINATION:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_MAGE:
		    strcpy( buf, expand ? "the Chosen" : "Chosen" );
		    break;
/*
		case CLASS_LICH:
		    strcpy( buf, "Fill Me" );
		    break;
		case CLASS_SKINDANCER:
		    strcpy( buf, "Fill Me" );
		    break;
*/
		case CLASS_DEMON:
		    strcpy( buf, expand ? "the Lord of Hell" : "Lord of Hell" );
		    break;
/*
		case CLASS_BAALI:
		    strcpy( buf, "Jah-Bul-On" );
		    break;

		case CLASS_BLACK_SPIRAL_DANCER:
		    strcpy( buf, expand ? "the Spawn of the Wyrm" : "Wyrm's Spawn" );
		    break;

		case CLASS_NEPHANDI:
		    strcpy( buf, "Fill Me" );
		    break;
*/
	    }
	    break;
    }
/*
    if ( strlen( buf ) > 15 )
    {
 *
	bugf( "Status_name too long (%s)", buf );
*
	buf[15] = '\0';
    }
*/
    return buf;
}

void stcprintf( CHAR_DATA *ch, char *txt, ... )
{
    char	buf	[2*MSL];
    va_list	args;

    va_start( args, txt );
    vsprintf( buf, txt, args );
    va_end( args );

    STC( buf, ch );
    return;
}

char *strip_small_args( char *argument )
{
    char	arg[MAX_STRING_LENGTH];
    char	buf[MAX_STRING_LENGTH];
    char	tmp[MAX_STRING_LENGTH];
    bool	found = FALSE;

    buf[0] = '\0';

    smash_tilde( argument );

    for ( ; argument[0] != '\0' ; )
    {
	argument = one_argument( argument, arg );
	if ( strlen( arg ) < 3 ) continue;
	if ( !found )
	    sprintf( tmp, "%s", arg );
	else
	    sprintf( tmp, " %s", arg );
	found = TRUE;
	strcat( buf, tmp );
    }
    return buf[0] != '\0' ? buf : "foo";
}

void bugf ( char *fmt, ... )
{
    char buf [2*MSL];
    va_list args;

    va_start ( args, fmt );
    vsprintf ( buf, fmt, args );
    va_end ( args );

    bug( buf, 0 );
    return;
}

void log_string_sil( const char *str, CHAR_DATA *ch )
{
    char logout [MAX_STRING_LENGTH];

    fprintf( stderr, "%s :: %s\n", short_date( current_time ), str );
    strcpy( logout, str );
    return;
}

char *short_date( time_t time )
{
    static char	buf[20];
    char	tmp[20];
    char	*date;

    if ( time < 0 )
    {
	bugf( "Invalid integer time: (%d)", time );
	time = current_time;
    }

    date = ctime( &time );

    if ( date[8] == ' ' )
	buf[0] = '0';
    else
	buf[0] = date[8];

    buf[1] = date[9];
    buf[2] = '/';

    tmp[0] = date[4];
    tmp[1] = date[5];
    tmp[2] = date[6];
    tmp[3] = '\0';

    if		( !str_cmp( tmp, "jan" ) ) { buf[3] = '0'; buf[4] = '1'; }
    else if	( !str_cmp( tmp, "feb" ) ) { buf[3] = '0'; buf[4] = '2'; }
    else if	( !str_cmp( tmp, "mar" ) ) { buf[3] = '0'; buf[4] = '3'; }
    else if	( !str_cmp( tmp, "apr" ) ) { buf[3] = '0'; buf[4] = '4'; }
    else if	( !str_cmp( tmp, "may" ) ) { buf[3] = '0'; buf[4] = '5'; }
    else if	( !str_cmp( tmp, "jun" ) ) { buf[3] = '0'; buf[4] = '6'; }
    else if	( !str_cmp( tmp, "jul" ) ) { buf[3] = '0'; buf[4] = '7'; }
    else if	( !str_cmp( tmp, "aug" ) ) { buf[3] = '0'; buf[4] = '8'; }
    else if	( !str_cmp( tmp, "sep" ) ) { buf[3] = '0'; buf[4] = '9'; }
    else if	( !str_cmp( tmp, "oct" ) ) { buf[3] = '1'; buf[4] = '0'; }
    else if	( !str_cmp( tmp, "nov" ) ) { buf[3] = '1'; buf[4] = '1'; }
    else if	( !str_cmp( tmp, "dec" ) ) { buf[3] = '1'; buf[4] = '2'; }
    else	{ buf[3] = '9'; buf[4] = '9'; }

    buf[5] = '/';

    buf[6] = date[22];
    buf[7] = date[23];
    buf[8] = ' ';

    buf[9] = date[11];
    buf[10] = date[12];
    buf[11] = ':';
    buf[12] = date[14];
    buf[13] = date[15];
    buf[14] = '\0';

    return buf;
}

void display_sire( CHAR_DATA *ch, char *argument )
{
    char sire1[MAX_STRING_LENGTH];
    char sire2[MAX_STRING_LENGTH];
    char sire3[MAX_STRING_LENGTH];
    char sire4[MAX_STRING_LENGTH];
    char sire5[MAX_STRING_LENGTH];
    char sire6[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if ( strlen( argument ) < 3 )
    {
	sprintf( buf, "%-16s", "None" );
	send_to_char( buf, ch );
	return;
    }

    argument = one_argument( argument, sire1 );
    argument = one_argument( argument, sire2 );
    argument = one_argument( argument, sire3 );
    argument = one_argument( argument, sire4 );
    argument = one_argument( argument, sire5 );
    argument = one_argument( argument, sire6 );

    if ( sire1 != '\0' ) sire1[0] = UPPER( sire1[0] );
    if ( sire2 != '\0' ) sire2[0] = UPPER( sire2[0] );
    if ( sire3 != '\0' ) sire3[0] = UPPER( sire3[0] );
    if ( sire4 != '\0' ) sire4[0] = UPPER( sire4[0] );
    if ( sire5 != '\0' ) sire5[0] = UPPER( sire5[0] );
    if ( sire6 != '\0' ) sire6[0] = UPPER( sire6[0] );

    if	    ( sire1[0] == '\0' )
	sprintf( buf, "%-16s", "None" );
    else if ( sire2[0] == '\0' )
	sprintf( buf, "%-16s", sire1 );
    else if ( sire3[0] == '\0' )
	sprintf( buf, "%-16s", sire2 );
    else if ( sire4[0] == '\0' )
	sprintf( buf, "%-16s", sire3 );
    else if ( sire5[0] == '\0' )
	sprintf( buf, "%-16s", sire4 );
    else if ( sire6[0] == '\0' )
	sprintf( buf, "%-16s", sire5 );
    else
	sprintf( buf, "%-16s", sire6 );

    if ( !str_cmp( buf, "morglum" ) )
    {
	sprintf( buf, "%-16s", "Morglum" );
	send_to_char( buf, ch );
    }
    else
	send_to_char( buf, ch );
    return;
}
