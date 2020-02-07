#include "global.h"
//#include "worldbase.h"
#include "vertex_mgr.h"

unsigned int GlobalVar::SCR_WIDTH = 800;
unsigned int GlobalVar::SCR_HEIGHT = 600;
unsigned int GlobalVar::INVALID_TEX = 99999999;
Worldbase* GlobalVar::GAME_WORLD = nullptr;
VertexMgr* GlobalVar::VERTEX_MGR = new VertexMgr();