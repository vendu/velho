#include <string.h>
#include <mjolnir/conf.h>
#include <mjolnir/mjol.h>
#include <mjolnir/obj.h>
#include <mjolnir/cell.h>
#include <mjolnir/obj.h>
#include <mjolnir/chr.h>

#if (DNG_MJOLNIR)

static mjolmkfunc      *dnginitobjtab[256];
uint8_t                 mjolcmdcanmovetomap[256];
uint8_t                 mjolcmdhasdirmap[256];
uint8_t                 mjolcmdhasargmap[256];

void
dnginitobjmjol(void)
{
/* objects */
    dnginitobjtab[MJOLNIR_OBJ_FLOOR] = mjolmkfloor;
    dnginitobjtab[MJOLNIR_OBJ_CORRIDOR] = mjolmkcorridor;
    dnginitobjtab[MJOLNIR_OBJ_HORIZONTAL_WALL] = mjolmkhorizwall;
    dnginitobjtab[MJOLNIR_OBJ_VERTICAL_WALL] = mjolmkvertwall;
    dnginitobjtab[MJOLNIR_OBJ_DOOR] = mjolmkdoor;
    dnginitobjtab[MJOLNIR_OBJ_FOOD] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_WATER] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_FOUNTAIN] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_GOLD] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_POTION] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_PLANT] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_PUNCHCARD] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_TAPE] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_STAIR_DOWN] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_STAIR_UP] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_STATUE] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_TRAP] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_WAND] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_SCROLL] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_RING] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_WHIP] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_ARMOR] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_CHAIN] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_CHEST] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_SUBMACHINE_GUN] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_HONEY] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_KNIFE] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_LOCKPICK] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_LASER] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_MACE] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_MAINFRAME] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_PIPE] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_PISTOL] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_SWORD] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_TELEPORT] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_WELL] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_CROSS] = NULL;
    dnginitobjtab[MJOLNIR_OBJ_ALTAR] = NULL;
/* characters */
    dnginitobjtab[MJOLNIR_CHAR_PLAYER] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_ANT] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_ALIEN] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_BEE] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_BEE_QUEEN] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_DOG] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_DEITY] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_GHOUL] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_HUMAN] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_ORACLE] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_REMNANTS] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_UNICORN] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_THOR] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_VAMPIRE] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_DRACULA] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_WOLF] = NULL;
    dnginitobjtab[MJOLNIR_CHAR_ZOMBIE] = NULL;
    /* special items */
    dnginitobjtab[MJOLNIR_ITEM_DEMON_WHIP] = NULL;
    dnginitobjtab[MJOLNIR_ITEM_JATIMATIC] = NULL;
    dnginitobjtab[MJOLNIR_ITEM_MJOLNIR] = NULL;
    dnginitobjtab[MJOLNIR_ITEM_PLEIGNIR] = NULL;
    dnginitobjtab[MJOLNIR_ITEM_STORMBRINGER] = NULL;
    dnginitobjtab[MJOLNIR_ITEM_EXCALIBUR] = NULL;
    dnginitobjtab[MJOLNIR_ITEM_HOLY_CROSS] = NULL;
    dnginitobjtab[MJOLNIR_ITEM_HOLY_WATER] = NULL;
    dnginitobjtab[MJOLNIR_ITEM_SILVER_BULLET ] = NULL;

    return;
}

struct mjolobj *
dngmkobjmjol(struct celldng *dng, long type, long flg, struct dngobj *data)
{
    mjolmkfunc     *mkfunc = dnginitobjtab[type];
    struct mjolobj *obj = (mkfunc) ? mkfunc() : NULL;

    if (obj) {
        obj->data.flg = flg;
        memcpy(&obj->data, data, sizeof(struct dngobj));
    }

    return obj;
}

#endif /* DNG_MJOLNIR */

