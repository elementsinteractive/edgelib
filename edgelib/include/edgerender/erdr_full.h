// erdr_full.h
// Edgerender implementation (full): full optimization for every combination
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

typedef bool (*erdr_func_bltfast) (BLTFAST_PARAMS);

void ERender_Get_BltFast8(void **ptr)
{
	erdr_func_bltfast erdr_fptr[] =
	{
		NULL,                  ERdrBltFast8_ck,          NULL,                  ERdrBltFast8_ck_cf,
		ERdrBltFast8_tn,       ERdrBltFast8_ck_tn,       ERdrBltFast8_cl,       ERdrBltFast8_ck_cl,
		ERdrBltFast8_op,       ERdrBltFast8_ck_op,       NULL,                  ERdrBltFast8_ck_cf_op,
		ERdrBltFast8_tn_op,    ERdrBltFast8_ck_tn_op,    ERdrBltFast8_cl_op,    ERdrBltFast8_ck_cl_op,
		ERdrBltFast8_ad,       ERdrBltFast8_ck_ad,       NULL,                  ERdrBltFast8_ck_cf_ad,
		ERdrBltFast8_tn_ad,    ERdrBltFast8_ck_tn_ad,    ERdrBltFast8_cl_ad,    ERdrBltFast8_ck_cl_ad,
		ERdrBltFast8_op_ad,    ERdrBltFast8_ck_op_ad,    NULL,                  ERdrBltFast8_ck_cf_op_ad,
		ERdrBltFast8_tn_op_ad, ERdrBltFast8_ck_tn_op_ad, ERdrBltFast8_cl_op_ad, ERdrBltFast8_ck_cl_op_ad,
		ERdrBltFast8_sb,       ERdrBltFast8_ck_sb,       NULL,                  ERdrBltFast8_ck_cf_sb,
		ERdrBltFast8_tn_sb,    ERdrBltFast8_ck_tn_sb,    ERdrBltFast8_cl_sb,    ERdrBltFast8_ck_cl_sb,
		ERdrBltFast8_op_sb,    ERdrBltFast8_ck_op_sb,    NULL,                  ERdrBltFast8_ck_cf_op_sb,
		ERdrBltFast8_tn_op_sb, ERdrBltFast8_ck_tn_op_sb, ERdrBltFast8_cl_op_sb, ERdrBltFast8_ck_cl_op_sb,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFast8_oh,       ERdrBltFast8_ck_oh,       NULL,                  ERdrBltFast8_ck_cf_oh,
		ERdrBltFast8_tn_oh,    ERdrBltFast8_ck_tn_oh,    ERdrBltFast8_cl_oh,    ERdrBltFast8_ck_cl_oh,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFast8_ad_oh,    ERdrBltFast8_ck_ad_oh,    NULL,                  ERdrBltFast8_ck_cf_ad_oh,
		ERdrBltFast8_tn_ad_oh, ERdrBltFast8_ck_tn_ad_oh, ERdrBltFast8_cl_ad_oh, ERdrBltFast8_ck_cl_ad_oh,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFast8_sb_oh,    ERdrBltFast8_ck_sb_oh,    NULL,                  ERdrBltFast8_ck_cf_sb_oh,
		ERdrBltFast8_tn_sb_oh, ERdrBltFast8_ck_tn_sb_oh, ERdrBltFast8_cl_sb_oh, ERdrBltFast8_ck_cl_sb_oh,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFast8_ol,       ERdrBltFast8_ck_ol,       NULL,                  ERdrBltFast8_ck_cf_ol,
		ERdrBltFast8_tn_ol,    ERdrBltFast8_ck_tn_ol,    ERdrBltFast8_cl_ol,    ERdrBltFast8_ck_cl_ol,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFast8_ad_ol,    ERdrBltFast8_ck_ad_ol,    NULL,                  ERdrBltFast8_ck_cf_ad_ol,
		ERdrBltFast8_tn_ad_ol, ERdrBltFast8_ck_tn_ad_ol, ERdrBltFast8_cl_ad_ol, ERdrBltFast8_ck_cl_ad_ol,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFast8_sb_ol,    ERdrBltFast8_ck_sb_ol,    NULL,                  ERdrBltFast8_ck_cf_sb_ol,
		ERdrBltFast8_tn_sb_ol, ERdrBltFast8_ck_tn_sb_ol, ERdrBltFast8_cl_sb_ol, ERdrBltFast8_ck_cl_sb_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFast12(void **ptr)
{
	erdr_func_bltfast erdr_fptr[] =
	{
		NULL,                   ERdrBltFast12_ck,          NULL,                   ERdrBltFast12_ck_cf,
		ERdrBltFast12_tn,       ERdrBltFast12_ck_tn,       ERdrBltFast12_cl,       ERdrBltFast12_ck_cl,
		ERdrBltFast12_op,       ERdrBltFast12_ck_op,       NULL,                   ERdrBltFast12_ck_cf_op,
		ERdrBltFast12_tn_op,    ERdrBltFast12_ck_tn_op,    ERdrBltFast12_cl_op,    ERdrBltFast12_ck_cl_op,
		ERdrBltFast12_ad,       ERdrBltFast12_ck_ad,       NULL,                   ERdrBltFast12_ck_cf_ad,
		ERdrBltFast12_tn_ad,    ERdrBltFast12_ck_tn_ad,    ERdrBltFast12_cl_ad,    ERdrBltFast12_ck_cl_ad,
		ERdrBltFast12_op_ad,    ERdrBltFast12_ck_op_ad,    NULL,                   ERdrBltFast12_ck_cf_op_ad,
		ERdrBltFast12_tn_op_ad, ERdrBltFast12_ck_tn_op_ad, ERdrBltFast12_cl_op_ad, ERdrBltFast12_ck_cl_op_ad,
		ERdrBltFast12_sb,       ERdrBltFast12_ck_sb,       NULL,                   ERdrBltFast12_ck_cf_sb,
		ERdrBltFast12_tn_sb,    ERdrBltFast12_ck_tn_sb,    ERdrBltFast12_cl_sb,    ERdrBltFast12_ck_cl_sb,
		ERdrBltFast12_op_sb,    ERdrBltFast12_ck_op_sb,    NULL,                   ERdrBltFast12_ck_cf_op_sb,
		ERdrBltFast12_tn_op_sb, ERdrBltFast12_ck_tn_op_sb, ERdrBltFast12_cl_op_sb, ERdrBltFast12_ck_cl_op_sb,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast12_oh,       ERdrBltFast12_ck_oh,       NULL,                   ERdrBltFast12_ck_cf_oh,
		ERdrBltFast12_tn_oh,    ERdrBltFast12_ck_tn_oh,    ERdrBltFast12_cl_oh,    ERdrBltFast12_ck_cl_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast12_ad_oh,    ERdrBltFast12_ck_ad_oh,    NULL,                   ERdrBltFast12_ck_cf_ad_oh,
		ERdrBltFast12_tn_ad_oh, ERdrBltFast12_ck_tn_ad_oh, ERdrBltFast12_cl_ad_oh, ERdrBltFast12_ck_cl_ad_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast12_sb_oh,    ERdrBltFast12_ck_sb_oh,    NULL,                   ERdrBltFast12_ck_cf_sb_oh,
		ERdrBltFast12_tn_sb_oh, ERdrBltFast12_ck_tn_sb_oh, ERdrBltFast12_cl_sb_oh, ERdrBltFast12_ck_cl_sb_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast12_ol,       ERdrBltFast12_ck_ol,       NULL,                   ERdrBltFast12_ck_cf_ol,
		ERdrBltFast12_tn_ol,    ERdrBltFast12_ck_tn_ol,    ERdrBltFast12_cl_ol,    ERdrBltFast12_ck_cl_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast12_ad_ol,    ERdrBltFast12_ck_ad_ol,    NULL,                   ERdrBltFast12_ck_cf_ad_ol,
		ERdrBltFast12_tn_ad_ol, ERdrBltFast12_ck_tn_ad_ol, ERdrBltFast12_cl_ad_ol, ERdrBltFast12_ck_cl_ad_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast12_sb_ol,    ERdrBltFast12_ck_sb_ol,    NULL,                   ERdrBltFast12_ck_cf_sb_ol,
		ERdrBltFast12_tn_sb_ol, ERdrBltFast12_ck_tn_sb_ol, ERdrBltFast12_cl_sb_ol, ERdrBltFast12_ck_cl_sb_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFast16(void **ptr)
{
	erdr_func_bltfast erdr_fptr[] =
	{
		NULL,                   ERdrBltFast16_ck,          NULL,                   ERdrBltFast16_ck_cf,
		ERdrBltFast16_tn,       ERdrBltFast16_ck_tn,       ERdrBltFast16_cl,       ERdrBltFast16_ck_cl,
		ERdrBltFast16_op,       ERdrBltFast16_ck_op,       NULL,                   ERdrBltFast16_ck_cf_op,
		ERdrBltFast16_tn_op,    ERdrBltFast16_ck_tn_op,    ERdrBltFast16_cl_op,    ERdrBltFast16_ck_cl_op,
		ERdrBltFast16_ad,       ERdrBltFast16_ck_ad,       NULL,                   ERdrBltFast16_ck_cf_ad,
		ERdrBltFast16_tn_ad,    ERdrBltFast16_ck_tn_ad,    ERdrBltFast16_cl_ad,    ERdrBltFast16_ck_cl_ad,
		ERdrBltFast16_op_ad,    ERdrBltFast16_ck_op_ad,    NULL,                   ERdrBltFast16_ck_cf_op_ad,
		ERdrBltFast16_tn_op_ad, ERdrBltFast16_ck_tn_op_ad, ERdrBltFast16_cl_op_ad, ERdrBltFast16_ck_cl_op_ad,
		ERdrBltFast16_sb,       ERdrBltFast16_ck_sb,       NULL,                   ERdrBltFast16_ck_cf_sb,
		ERdrBltFast16_tn_sb,    ERdrBltFast16_ck_tn_sb,    ERdrBltFast16_cl_sb,    ERdrBltFast16_ck_cl_sb,
		ERdrBltFast16_op_sb,    ERdrBltFast16_ck_op_sb,    NULL,                   ERdrBltFast16_ck_cf_op_sb,
		ERdrBltFast16_tn_op_sb, ERdrBltFast16_ck_tn_op_sb, ERdrBltFast16_cl_op_sb, ERdrBltFast16_ck_cl_op_sb,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast16_oh,       ERdrBltFast16_ck_oh,       NULL,                   ERdrBltFast16_ck_cf_oh,
		ERdrBltFast16_tn_oh,    ERdrBltFast16_ck_tn_oh,    ERdrBltFast16_cl_oh,    ERdrBltFast16_ck_cl_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast16_ad_oh,    ERdrBltFast16_ck_ad_oh,    NULL,                   ERdrBltFast16_ck_cf_ad_oh,
		ERdrBltFast16_tn_ad_oh, ERdrBltFast16_ck_tn_ad_oh, ERdrBltFast16_cl_ad_oh, ERdrBltFast16_ck_cl_ad_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast16_sb_oh,    ERdrBltFast16_ck_sb_oh,    NULL,                   ERdrBltFast16_ck_cf_sb_oh,
		ERdrBltFast16_tn_sb_oh, ERdrBltFast16_ck_tn_sb_oh, ERdrBltFast16_cl_sb_oh, ERdrBltFast16_ck_cl_sb_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast16_ol,       ERdrBltFast16_ck_ol,       NULL,                   ERdrBltFast16_ck_cf_ol,
		ERdrBltFast16_tn_ol,    ERdrBltFast16_ck_tn_ol,    ERdrBltFast16_cl_ol,    ERdrBltFast16_ck_cl_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast16_ad_ol,    ERdrBltFast16_ck_ad_ol,    NULL,                   ERdrBltFast16_ck_cf_ad_ol,
		ERdrBltFast16_tn_ad_ol, ERdrBltFast16_ck_tn_ad_ol, ERdrBltFast16_cl_ad_ol, ERdrBltFast16_ck_cl_ad_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast16_sb_ol,    ERdrBltFast16_ck_sb_ol,    NULL,                   ERdrBltFast16_ck_cf_sb_ol,
		ERdrBltFast16_tn_sb_ol, ERdrBltFast16_ck_tn_sb_ol, ERdrBltFast16_cl_sb_ol, ERdrBltFast16_ck_cl_sb_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFast32(void **ptr)
{
	erdr_func_bltfast erdr_fptr[] =
	{
		NULL,                   ERdrBltFast32_ck,          NULL,                   ERdrBltFast32_ck_cf,
		ERdrBltFast32_tn,       ERdrBltFast32_ck_tn,       ERdrBltFast32_cl,       ERdrBltFast32_ck_cl,
		ERdrBltFast32_op,       ERdrBltFast32_ck_op,       NULL,                   ERdrBltFast32_ck_cf_op,
		ERdrBltFast32_tn_op,    ERdrBltFast32_ck_tn_op,    ERdrBltFast32_cl_op,    ERdrBltFast32_ck_cl_op,
		ERdrBltFast32_ad,       ERdrBltFast32_ck_ad,       NULL,                   ERdrBltFast32_ck_cf_ad,
		ERdrBltFast32_tn_ad,    ERdrBltFast32_ck_tn_ad,    ERdrBltFast32_cl_ad,    ERdrBltFast32_ck_cl_ad,
		ERdrBltFast32_op_ad,    ERdrBltFast32_ck_op_ad,    NULL,                   ERdrBltFast32_ck_cf_op_ad,
		ERdrBltFast32_tn_op_ad, ERdrBltFast32_ck_tn_op_ad, ERdrBltFast32_cl_op_ad, ERdrBltFast32_ck_cl_op_ad,
		ERdrBltFast32_sb,       ERdrBltFast32_ck_sb,       NULL,                   ERdrBltFast32_ck_cf_sb,
		ERdrBltFast32_tn_sb,    ERdrBltFast32_ck_tn_sb,    ERdrBltFast32_cl_sb,    ERdrBltFast32_ck_cl_sb,
		ERdrBltFast32_op_sb,    ERdrBltFast32_ck_op_sb,    NULL,                   ERdrBltFast32_ck_cf_op_sb,
		ERdrBltFast32_tn_op_sb, ERdrBltFast32_ck_tn_op_sb, ERdrBltFast32_cl_op_sb, ERdrBltFast32_ck_cl_op_sb,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast32_oh,       ERdrBltFast32_ck_oh,       NULL,                   ERdrBltFast32_ck_cf_oh,
		ERdrBltFast32_tn_oh,    ERdrBltFast32_ck_tn_oh,    ERdrBltFast32_cl_oh,    ERdrBltFast32_ck_cl_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast32_ad_oh,    ERdrBltFast32_ck_ad_oh,    NULL,                   ERdrBltFast32_ck_cf_ad_oh,
		ERdrBltFast32_tn_ad_oh, ERdrBltFast32_ck_tn_ad_oh, ERdrBltFast32_cl_ad_oh, ERdrBltFast32_ck_cl_ad_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast32_sb_oh,    ERdrBltFast32_ck_sb_oh,    NULL,                   ERdrBltFast32_ck_cf_sb_oh,
		ERdrBltFast32_tn_sb_oh, ERdrBltFast32_ck_tn_sb_oh, ERdrBltFast32_cl_sb_oh, ERdrBltFast32_ck_cl_sb_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast32_ol,       ERdrBltFast32_ck_ol,       NULL,                   ERdrBltFast32_ck_cf_ol,
		ERdrBltFast32_tn_ol,    ERdrBltFast32_ck_tn_ol,    ERdrBltFast32_cl_ol,    ERdrBltFast32_ck_cl_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast32_ad_ol,    ERdrBltFast32_ck_ad_ol,    NULL,                   ERdrBltFast32_ck_cf_ad_ol,
		ERdrBltFast32_tn_ad_ol, ERdrBltFast32_ck_tn_ad_ol, ERdrBltFast32_cl_ad_ol, ERdrBltFast32_ck_cl_ad_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFast32_sb_ol,    ERdrBltFast32_ck_sb_ol,    NULL,                   ERdrBltFast32_ck_cf_sb_ol,
		ERdrBltFast32_tn_sb_ol, ERdrBltFast32_ck_tn_sb_ol, ERdrBltFast32_cl_sb_ol, ERdrBltFast32_ck_cl_sb_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

typedef bool (*erdr_func_bltflip) (BLTFLIP_PARAMS);

void ERender_Get_BltFlip8(void **ptr)
{
	erdr_func_bltflip erdr_fptr[] =
	{
		ERdrBltFlip8_,         ERdrBltFlip8_ck,          NULL,                  ERdrBltFlip8_ck_cf,
		ERdrBltFlip8_tn,       ERdrBltFlip8_ck_tn,       ERdrBltFlip8_cl,       ERdrBltFlip8_ck_cl,
		ERdrBltFlip8_op,       ERdrBltFlip8_ck_op,       NULL,                  ERdrBltFlip8_ck_cf_op,
		ERdrBltFlip8_tn_op,    ERdrBltFlip8_ck_tn_op,    ERdrBltFlip8_cl_op,    ERdrBltFlip8_ck_cl_op,
		ERdrBltFlip8_ad,       ERdrBltFlip8_ck_ad,       NULL,                  ERdrBltFlip8_ck_cf_ad,
		ERdrBltFlip8_tn_ad,    ERdrBltFlip8_ck_tn_ad,    ERdrBltFlip8_cl_ad,    ERdrBltFlip8_ck_cl_ad,
		ERdrBltFlip8_op_ad,    ERdrBltFlip8_ck_op_ad,    NULL,                  ERdrBltFlip8_ck_cf_op_ad,
		ERdrBltFlip8_tn_op_ad, ERdrBltFlip8_ck_tn_op_ad, ERdrBltFlip8_cl_op_ad, ERdrBltFlip8_ck_cl_op_ad,
		ERdrBltFlip8_sb,       ERdrBltFlip8_ck_sb,       NULL,                  ERdrBltFlip8_ck_cf_sb,
		ERdrBltFlip8_tn_sb,    ERdrBltFlip8_ck_tn_sb,    ERdrBltFlip8_cl_sb,    ERdrBltFlip8_ck_cl_sb,
		ERdrBltFlip8_op_sb,    ERdrBltFlip8_ck_op_sb,    NULL,                  ERdrBltFlip8_ck_cf_op_sb,
		ERdrBltFlip8_tn_op_sb, ERdrBltFlip8_ck_tn_op_sb, ERdrBltFlip8_cl_op_sb, ERdrBltFlip8_ck_cl_op_sb,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFlip8_oh,       ERdrBltFlip8_ck_oh,       NULL,                  ERdrBltFlip8_ck_cf_oh,
		ERdrBltFlip8_tn_oh,    ERdrBltFlip8_ck_tn_oh,    ERdrBltFlip8_cl_oh,    ERdrBltFlip8_ck_cl_oh,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFlip8_ad_oh,    ERdrBltFlip8_ck_ad_oh,    NULL,                  ERdrBltFlip8_ck_cf_ad_oh,
		ERdrBltFlip8_tn_ad_oh, ERdrBltFlip8_ck_tn_ad_oh, ERdrBltFlip8_cl_ad_oh, ERdrBltFlip8_ck_cl_ad_oh,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFlip8_sb_oh,    ERdrBltFlip8_ck_sb_oh,    NULL,                  ERdrBltFlip8_ck_cf_sb_oh,
		ERdrBltFlip8_tn_sb_oh, ERdrBltFlip8_ck_tn_sb_oh, ERdrBltFlip8_cl_sb_oh, ERdrBltFlip8_ck_cl_sb_oh,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFlip8_ol,       ERdrBltFlip8_ck_ol,       NULL,                  ERdrBltFlip8_ck_cf_ol,
		ERdrBltFlip8_tn_ol,    ERdrBltFlip8_ck_tn_ol,    ERdrBltFlip8_cl_ol,    ERdrBltFlip8_ck_cl_ol,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFlip8_ad_ol,    ERdrBltFlip8_ck_ad_ol,    NULL,                  ERdrBltFlip8_ck_cf_ad_ol,
		ERdrBltFlip8_tn_ad_ol, ERdrBltFlip8_ck_tn_ad_ol, ERdrBltFlip8_cl_ad_ol, ERdrBltFlip8_ck_cl_ad_ol,
		NULL,                  NULL,                     NULL,                  NULL,
		NULL,                  NULL,                     NULL,                  NULL,
		ERdrBltFlip8_sb_ol,    ERdrBltFlip8_ck_sb_ol,    NULL,                  ERdrBltFlip8_ck_cf_sb_ol,
		ERdrBltFlip8_tn_sb_ol, ERdrBltFlip8_ck_tn_sb_ol, ERdrBltFlip8_cl_sb_ol, ERdrBltFlip8_ck_cl_sb_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFlip12(void **ptr)
{
	erdr_func_bltflip erdr_fptr[] =
	{
		ERdrBltFlip12_,         ERdrBltFlip12_ck,          NULL,                   ERdrBltFlip12_ck_cf,
		ERdrBltFlip12_tn,       ERdrBltFlip12_ck_tn,       ERdrBltFlip12_cl,       ERdrBltFlip12_ck_cl,
		ERdrBltFlip12_op,       ERdrBltFlip12_ck_op,       NULL,                   ERdrBltFlip12_ck_cf_op,
		ERdrBltFlip12_tn_op,    ERdrBltFlip12_ck_tn_op,    ERdrBltFlip12_cl_op,    ERdrBltFlip12_ck_cl_op,
		ERdrBltFlip12_ad,       ERdrBltFlip12_ck_ad,       NULL,                   ERdrBltFlip12_ck_cf_ad,
		ERdrBltFlip12_tn_ad,    ERdrBltFlip12_ck_tn_ad,    ERdrBltFlip12_cl_ad,    ERdrBltFlip12_ck_cl_ad,
		ERdrBltFlip12_op_ad,    ERdrBltFlip12_ck_op_ad,    NULL,                   ERdrBltFlip12_ck_cf_op_ad,
		ERdrBltFlip12_tn_op_ad, ERdrBltFlip12_ck_tn_op_ad, ERdrBltFlip12_cl_op_ad, ERdrBltFlip12_ck_cl_op_ad,
		ERdrBltFlip12_sb,       ERdrBltFlip12_ck_sb,       NULL,                   ERdrBltFlip12_ck_cf_sb,
		ERdrBltFlip12_tn_sb,    ERdrBltFlip12_ck_tn_sb,    ERdrBltFlip12_cl_sb,    ERdrBltFlip12_ck_cl_sb,
		ERdrBltFlip12_op_sb,    ERdrBltFlip12_ck_op_sb,    NULL,                   ERdrBltFlip12_ck_cf_op_sb,
		ERdrBltFlip12_tn_op_sb, ERdrBltFlip12_ck_tn_op_sb, ERdrBltFlip12_cl_op_sb, ERdrBltFlip12_ck_cl_op_sb,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip12_oh,       ERdrBltFlip12_ck_oh,       NULL,                   ERdrBltFlip12_ck_cf_oh,
		ERdrBltFlip12_tn_oh,    ERdrBltFlip12_ck_tn_oh,    ERdrBltFlip12_cl_oh,    ERdrBltFlip12_ck_cl_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip12_ad_oh,    ERdrBltFlip12_ck_ad_oh,    NULL,                   ERdrBltFlip12_ck_cf_ad_oh,
		ERdrBltFlip12_tn_ad_oh, ERdrBltFlip12_ck_tn_ad_oh, ERdrBltFlip12_cl_ad_oh, ERdrBltFlip12_ck_cl_ad_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip12_sb_oh,    ERdrBltFlip12_ck_sb_oh,    NULL,                   ERdrBltFlip12_ck_cf_sb_oh,
		ERdrBltFlip12_tn_sb_oh, ERdrBltFlip12_ck_tn_sb_oh, ERdrBltFlip12_cl_sb_oh, ERdrBltFlip12_ck_cl_sb_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip12_ol,       ERdrBltFlip12_ck_ol,       NULL,                   ERdrBltFlip12_ck_cf_ol,
		ERdrBltFlip12_tn_ol,    ERdrBltFlip12_ck_tn_ol,    ERdrBltFlip12_cl_ol,    ERdrBltFlip12_ck_cl_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip12_ad_ol,    ERdrBltFlip12_ck_ad_ol,    NULL,                   ERdrBltFlip12_ck_cf_ad_ol,
		ERdrBltFlip12_tn_ad_ol, ERdrBltFlip12_ck_tn_ad_ol, ERdrBltFlip12_cl_ad_ol, ERdrBltFlip12_ck_cl_ad_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip12_sb_ol,    ERdrBltFlip12_ck_sb_ol,    NULL,                   ERdrBltFlip12_ck_cf_sb_ol,
		ERdrBltFlip12_tn_sb_ol, ERdrBltFlip12_ck_tn_sb_ol, ERdrBltFlip12_cl_sb_ol, ERdrBltFlip12_ck_cl_sb_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFlip16(void **ptr)
{
	erdr_func_bltflip erdr_fptr[] =
	{
		ERdrBltFlip16_,         ERdrBltFlip16_ck,          NULL,                   ERdrBltFlip16_ck_cf,
		ERdrBltFlip16_tn,       ERdrBltFlip16_ck_tn,       ERdrBltFlip16_cl,       ERdrBltFlip16_ck_cl,
		ERdrBltFlip16_op,       ERdrBltFlip16_ck_op,       NULL,                   ERdrBltFlip16_ck_cf_op,
		ERdrBltFlip16_tn_op,    ERdrBltFlip16_ck_tn_op,    ERdrBltFlip16_cl_op,    ERdrBltFlip16_ck_cl_op,
		ERdrBltFlip16_ad,       ERdrBltFlip16_ck_ad,       NULL,                   ERdrBltFlip16_ck_cf_ad,
		ERdrBltFlip16_tn_ad,    ERdrBltFlip16_ck_tn_ad,    ERdrBltFlip16_cl_ad,    ERdrBltFlip16_ck_cl_ad,
		ERdrBltFlip16_op_ad,    ERdrBltFlip16_ck_op_ad,    NULL,                   ERdrBltFlip16_ck_cf_op_ad,
		ERdrBltFlip16_tn_op_ad, ERdrBltFlip16_ck_tn_op_ad, ERdrBltFlip16_cl_op_ad, ERdrBltFlip16_ck_cl_op_ad,
		ERdrBltFlip16_sb,       ERdrBltFlip16_ck_sb,       NULL,                   ERdrBltFlip16_ck_cf_sb,
		ERdrBltFlip16_tn_sb,    ERdrBltFlip16_ck_tn_sb,    ERdrBltFlip16_cl_sb,    ERdrBltFlip16_ck_cl_sb,
		ERdrBltFlip16_op_sb,    ERdrBltFlip16_ck_op_sb,    NULL,                   ERdrBltFlip16_ck_cf_op_sb,
		ERdrBltFlip16_tn_op_sb, ERdrBltFlip16_ck_tn_op_sb, ERdrBltFlip16_cl_op_sb, ERdrBltFlip16_ck_cl_op_sb,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip16_oh,       ERdrBltFlip16_ck_oh,       NULL,                   ERdrBltFlip16_ck_cf_oh,
		ERdrBltFlip16_tn_oh,    ERdrBltFlip16_ck_tn_oh,    ERdrBltFlip16_cl_oh,    ERdrBltFlip16_ck_cl_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip16_ad_oh,    ERdrBltFlip16_ck_ad_oh,    NULL,                   ERdrBltFlip16_ck_cf_ad_oh,
		ERdrBltFlip16_tn_ad_oh, ERdrBltFlip16_ck_tn_ad_oh, ERdrBltFlip16_cl_ad_oh, ERdrBltFlip16_ck_cl_ad_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip16_sb_oh,    ERdrBltFlip16_ck_sb_oh,    NULL,                   ERdrBltFlip16_ck_cf_sb_oh,
		ERdrBltFlip16_tn_sb_oh, ERdrBltFlip16_ck_tn_sb_oh, ERdrBltFlip16_cl_sb_oh, ERdrBltFlip16_ck_cl_sb_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip16_ol,       ERdrBltFlip16_ck_ol,       NULL,                   ERdrBltFlip16_ck_cf_ol,
		ERdrBltFlip16_tn_ol,    ERdrBltFlip16_ck_tn_ol,    ERdrBltFlip16_cl_ol,    ERdrBltFlip16_ck_cl_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip16_ad_ol,    ERdrBltFlip16_ck_ad_ol,    NULL,                   ERdrBltFlip16_ck_cf_ad_ol,
		ERdrBltFlip16_tn_ad_ol, ERdrBltFlip16_ck_tn_ad_ol, ERdrBltFlip16_cl_ad_ol, ERdrBltFlip16_ck_cl_ad_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip16_sb_ol,    ERdrBltFlip16_ck_sb_ol,    NULL,                   ERdrBltFlip16_ck_cf_sb_ol,
		ERdrBltFlip16_tn_sb_ol, ERdrBltFlip16_ck_tn_sb_ol, ERdrBltFlip16_cl_sb_ol, ERdrBltFlip16_ck_cl_sb_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}

void ERender_Get_BltFlip32(void **ptr)
{
	erdr_func_bltflip erdr_fptr[] =
	{
		ERdrBltFlip32_,         ERdrBltFlip32_ck,          NULL,                   ERdrBltFlip32_ck_cf,
		ERdrBltFlip32_tn,       ERdrBltFlip32_ck_tn,       ERdrBltFlip32_cl,       ERdrBltFlip32_ck_cl,
		ERdrBltFlip32_op,       ERdrBltFlip32_ck_op,       NULL,                   ERdrBltFlip32_ck_cf_op,
		ERdrBltFlip32_tn_op,    ERdrBltFlip32_ck_tn_op,    ERdrBltFlip32_cl_op,    ERdrBltFlip32_ck_cl_op,
		ERdrBltFlip32_ad,       ERdrBltFlip32_ck_ad,       NULL,                   ERdrBltFlip32_ck_cf_ad,
		ERdrBltFlip32_tn_ad,    ERdrBltFlip32_ck_tn_ad,    ERdrBltFlip32_cl_ad,    ERdrBltFlip32_ck_cl_ad,
		ERdrBltFlip32_op_ad,    ERdrBltFlip32_ck_op_ad,    NULL,                   ERdrBltFlip32_ck_cf_op_ad,
		ERdrBltFlip32_tn_op_ad, ERdrBltFlip32_ck_tn_op_ad, ERdrBltFlip32_cl_op_ad, ERdrBltFlip32_ck_cl_op_ad,
		ERdrBltFlip32_sb,       ERdrBltFlip32_ck_sb,       NULL,                   ERdrBltFlip32_ck_cf_sb,
		ERdrBltFlip32_tn_sb,    ERdrBltFlip32_ck_tn_sb,    ERdrBltFlip32_cl_sb,    ERdrBltFlip32_ck_cl_sb,
		ERdrBltFlip32_op_sb,    ERdrBltFlip32_ck_op_sb,    NULL,                   ERdrBltFlip32_ck_cf_op_sb,
		ERdrBltFlip32_tn_op_sb, ERdrBltFlip32_ck_tn_op_sb, ERdrBltFlip32_cl_op_sb, ERdrBltFlip32_ck_cl_op_sb,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip32_oh,       ERdrBltFlip32_ck_oh,       NULL,                   ERdrBltFlip32_ck_cf_oh,
		ERdrBltFlip32_tn_oh,    ERdrBltFlip32_ck_tn_oh,    ERdrBltFlip32_cl_oh,    ERdrBltFlip32_ck_cl_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip32_ad_oh,    ERdrBltFlip32_ck_ad_oh,    NULL,                   ERdrBltFlip32_ck_cf_ad_oh,
		ERdrBltFlip32_tn_ad_oh, ERdrBltFlip32_ck_tn_ad_oh, ERdrBltFlip32_cl_ad_oh, ERdrBltFlip32_ck_cl_ad_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip32_sb_oh,    ERdrBltFlip32_ck_sb_oh,    NULL,                   ERdrBltFlip32_ck_cf_sb_oh,
		ERdrBltFlip32_tn_sb_oh, ERdrBltFlip32_ck_tn_sb_oh, ERdrBltFlip32_cl_sb_oh, ERdrBltFlip32_ck_cl_sb_oh,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip32_ol,       ERdrBltFlip32_ck_ol,       NULL,                   ERdrBltFlip32_ck_cf_ol,
		ERdrBltFlip32_tn_ol,    ERdrBltFlip32_ck_tn_ol,    ERdrBltFlip32_cl_ol,    ERdrBltFlip32_ck_cl_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip32_ad_ol,    ERdrBltFlip32_ck_ad_ol,    NULL,                   ERdrBltFlip32_ck_cf_ad_ol,
		ERdrBltFlip32_tn_ad_ol, ERdrBltFlip32_ck_tn_ad_ol, ERdrBltFlip32_cl_ad_ol, ERdrBltFlip32_ck_cl_ad_ol,
		NULL,                   NULL,                      NULL,                   NULL,
		NULL,                   NULL,                      NULL,                   NULL,
		ERdrBltFlip32_sb_ol,    ERdrBltFlip32_ck_sb_ol,    NULL,                   ERdrBltFlip32_ck_cf_sb_ol,
		ERdrBltFlip32_tn_sb_ol, ERdrBltFlip32_ck_tn_sb_ol, ERdrBltFlip32_cl_sb_ol, ERdrBltFlip32_ck_cl_sb_ol
	};
	ClassEMemory::Copy(ptr, (void **)erdr_fptr, sizeof(erdr_fptr));
}
