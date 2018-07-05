CC      = gcc
PROF    = -g -ggdb
NOCRYPT =
C_FLAGS = -Wall -Dlinux $(PROF) $(NOCRYPT) -DMERC
L_FLAGS = -lcrypt $(PROF) -lz
LIBS    = -lcrypt

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o artifact.o bit.o \
	  board.o clan.o clan_table.o comm.o const.o core.o db.o demon.o fight.o \
	  handler.o home.o hunt.o interp.o kav_fight.o kav_info.o kav_wiz.o mage.o \
	  magic.o mccp.o more.o morg-demon.o morg-file.o morg-mage.o morg-spell.o \
	  morg-wiz.o morg-wolf.o new.o olc.o rot_wiz.o save.o special.o string.o \
	  update.o werewolf.o

merc: $(O_FILES)
	rm -f merc
	$(CC) $(L_FLAGS) -o merc $(O_FILES) $(LIBS)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<

clean:
	rm -f *.o *~ nohup.out
	rm -f ../player/*.d* *~ ../log/*.log
	rm -f ../src/merc ../area/core
	rm -f ../misc/bugs.txt *~
	rm -f ../notes/*.old *~
