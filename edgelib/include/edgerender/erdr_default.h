// erdr_default.h
// Edgerender implementation (default): standard optimization for common combinations
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

typedef bool (*erdr_func_bltfast) (BLTFAST_PARAMS);

void ERender_Get_BltFast8(void **ptr)
{
	erdr_func_bltfast erdr_fptr[] =
	{
		NULL,            ERdrBltFast8_ck,    NULL, ERdrBltFast8_ck_cf,
		ERdrBltFast8_tn, ERdrBltFast8_ck_tn, NULL, NULL,
		ERdrBltFast8_op, ERdrBltFast8_ck_op, NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		ERdrBltFast8_ad, ERdrBltFast8_ck_ad, NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		ERdrBltFast8_sb, ERdrBltFast8_ck_sb, NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		ERdrBltFast8_oh, ERdrBltFast8_ck_oh, NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		NULL,            NULL,               NULL, NULL,
		ERdrBltFast8_ol, ERdrBltFast8_ck_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFast12(void **ptr)
{
	erdr_func_bltfast erdr_fptr[] =
	{
		NULL,             ERdrBltFast12_ck,    NULL, ERdrBltFast12_ck_cf,
		ERdrBltFast12_tn, ERdrBltFast12_ck_tn, NULL, NULL,
		ERdrBltFast12_op, ERdrBltFast12_ck_op, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast12_ad, ERdrBltFast12_ck_ad, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast12_sb, ERdrBltFast12_ck_sb, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast12_oh, ERdrBltFast12_ck_oh, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast12_ol, ERdrBltFast12_ck_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFast16(void **ptr)
{
	erdr_func_bltfast erdr_fptr[] =
	{
		NULL,             ERdrBltFast16_ck,    NULL, ERdrBltFast16_ck_cf,
		ERdrBltFast16_tn, ERdrBltFast16_ck_tn, NULL, NULL,
		ERdrBltFast16_op, ERdrBltFast16_ck_op, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast16_ad, ERdrBltFast16_ck_ad, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast16_sb, ERdrBltFast16_ck_sb, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast16_oh, ERdrBltFast16_ck_oh, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast16_ol, ERdrBltFast16_ck_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFast32(void **ptr)
{
	erdr_func_bltfast erdr_fptr[] =
	{
		NULL,             ERdrBltFast32_ck,    NULL, ERdrBltFast32_ck_cf,
		ERdrBltFast32_tn, ERdrBltFast32_ck_tn, NULL, NULL,
		ERdrBltFast32_op, ERdrBltFast32_ck_op, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast32_ad, ERdrBltFast32_ck_ad, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast32_sb, ERdrBltFast32_ck_sb, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast32_oh, ERdrBltFast32_ck_oh, NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		NULL,             NULL,                NULL, NULL,
		ERdrBltFast32_ol, ERdrBltFast32_ck_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

typedef bool (*erdr_func_bltflip) (BLTFLIP_PARAMS);

void ERender_Get_BltFlip8(void **ptr)
{
	erdr_func_bltflip erdr_fptr[] =
	{
		ERdrBltFlip8_, ERdrBltFlip8_ck
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFlip12(void **ptr)
{
	erdr_func_bltflip erdr_fptr[] =
	{
		ERdrBltFlip12_, ERdrBltFlip12_ck
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFlip16(void **ptr)
{
	erdr_func_bltflip erdr_fptr[] =
	{
		ERdrBltFlip16_, ERdrBltFlip16_ck
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFlip32(void **ptr)
{
	erdr_func_bltflip erdr_fptr[] =
	{
		ERdrBltFlip32_, ERdrBltFlip32_ck
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}
