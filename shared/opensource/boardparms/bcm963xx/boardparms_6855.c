#include "bp_defs.h"
#include "boardparms.h"
#include "bcmSpiRes.h"

static bp_elem_t g_bcm96855ref[] = {
  {bp_cpBoardId,               .u.cp = "96855REF"},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};


bp_elem_t * g_BoardParms[] = {g_bcm96855ref, 0};


