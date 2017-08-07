// erdr_lite.h
// Edgerender implementation (light): no optimization
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

typedef bool (*erdr_func_bltfast) (BLTFAST_PARAMS);

void ERender_Get_BltFast8(void **ptr){ }
void ERender_Get_BltFast12(void **ptr){ }
void ERender_Get_BltFast16(void **ptr){ }
void ERender_Get_BltFast32(void **ptr){ }

typedef bool (*erdr_func_bltflip) (BLTFLIP_PARAMS);

void ERender_Get_BltFlip8(void **ptr){ }
void ERender_Get_BltFlip12(void **ptr){ }
void ERender_Get_BltFlip16(void **ptr){ }
void ERender_Get_BltFlip32(void **ptr){ }
