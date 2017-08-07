// eldr_jpg.cpp
// JPEG image loader
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////
// 
// This software contains code based on the Small JPEG Decoder Library v0.93b by Rich Geldreich licensed under LGPL. Copyright (C) 1994-2000 Rich Geldreich.
// Portions of this software are based on the work of the Independent JPEG Group. Parts are copyright (C) 1991-1998, Thomas G. Lane.
// 
///////////////////////////////////////////////////////////////////


#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eloader.h"
#include "ememory.h"
#include "edisplay.h"

#include "extra/verinfo.h"


///////////////////////////////////////////////////////////////////
// Loader: Definitions                                           //
///////////////////////////////////////////////////////////////////

//Size of buffer
#define JPGD_INBUFSIZE       4096

// May need to be adjusted if support for other colorspaces/sampling factors is added
#define JPGD_MAXBLOCKSPERMCU   10

#define JPGD_MAXHUFFTABLES      8
#define JPGD_MAXQUANTTABLES     4
#define JPGD_MAXCOMPONENTS      4
#define JPGD_MAXCOMPSINSCAN     4

// Increase this if you increase the max width!
#define JPGD_MAXBLOCKSPERROW 6144

// Max. allocated blocks
#define JPGD_MAXBLOCKS        100
#define JPGD_MAX_HEIGHT      8192
#define JPGD_MAX_WIDTH       8192

// JPEG specific errors
#define JPGD_BAD_DHT_COUNTS              -200
#define JPGD_BAD_DHT_INDEX               -201
#define JPGD_BAD_DHT_MARKER              -202
#define JPGD_BAD_DQT_MARKER              -203
#define JPGD_BAD_DQT_TABLE               -204
#define JPGD_BAD_PRECISION               -205
#define JPGD_BAD_HEIGHT                  -206
#define JPGD_BAD_WIDTH                   -207
#define JPGD_TOO_MANY_COMPONENTS         -208
#define JPGD_BAD_SOF_LENGTH              -209
#define JPGD_BAD_VARIABLE_MARKER         -210
#define JPGD_BAD_DRI_LENGTH              -211
#define JPGD_BAD_SOS_LENGTH              -212
#define JPGD_BAD_SOS_COMP_ID             -213
#define JPGD_W_EXTRA_BYTES_BEFORE_MARKER -214
#define JPGD_NO_ARITHMITIC_SUPPORT       -215
#define JPGD_UNEXPECTED_MARKER           -216
#define JPGD_NOT_JPEG                    -217
#define JPGD_UNSUPPORTED_MARKER          -218
#define JPGD_BAD_DQT_LENGTH              -219
#define JPGD_TOO_MANY_BLOCKS             -221
#define JPGD_UNDEFINED_QUANT_TABLE       -222
#define JPGD_UNDEFINED_HUFF_TABLE        -223
#define JPGD_NOT_SINGLE_SCAN             -224
#define JPGD_UNSUPPORTED_COLORSPACE      -225
#define JPGD_UNSUPPORTED_SAMP_FACTORS    -226
#define JPGD_DECODE_ERROR                -227
#define JPGD_BAD_RESTART_MARKER          -228
#define JPGD_ASSERTION_ERROR             -229
#define JPGD_BAD_SOS_SPECTRAL            -230
#define JPGD_BAD_SOS_SUCCESSIVE          -231
#define JPGD_STREAM_READ                 -232
#define JPGD_NOTENOUGHMEM                -233

#define JPGD_GRAYSCALE 0
#define JPGD_YH1V1     1
#define JPGD_YH2V1     2
#define JPGD_YH1V2     3
#define JPGD_YH2V2     4

#define JPGD_FAILED  -1
#define JPGD_DONE     1
#define JPGD_OKAY     0

#define JPGD_TRUE     1
#define JPGD_FALSE    0

typedef enum
{
	M_SOF0  = 0xC0,
	M_SOF1  = 0xC1,
	M_SOF2  = 0xC2,
	M_SOF3  = 0xC3,

	M_SOF5  = 0xC5,
	M_SOF6  = 0xC6,
	M_SOF7  = 0xC7,

	M_JPG   = 0xC8,
	M_SOF9  = 0xC9,
	M_SOF10 = 0xCA,
	M_SOF11 = 0xCB,

	M_SOF13 = 0xCD,
	M_SOF14 = 0xCE,
	M_SOF15 = 0xCF,

	M_DHT   = 0xC4,

	M_DAC   = 0xCC,

	M_RST0  = 0xD0,
	M_RST1  = 0xD1,
	M_RST2  = 0xD2,
	M_RST3  = 0xD3,
	M_RST4  = 0xD4,
	M_RST5  = 0xD5,
	M_RST6  = 0xD6,
	M_RST7  = 0xD7,

	M_SOI   = 0xD8,
	M_EOI   = 0xD9,
	M_SOS   = 0xDA,
	M_DQT   = 0xDB,
	M_DNL   = 0xDC,
	M_DRI   = 0xDD,
	M_DHP   = 0xDE,
	M_EXP   = 0xDF,

	M_APP0  = 0xE0,
	M_APP15 = 0xEF,

	M_JPG0  = 0xF0,
	M_JPG13 = 0xFD,
	M_COM   = 0xFE,

	M_TEM   = 0x01,

	M_ERROR = 0x100
}JPEG_MARKER;

#define RST0 0xD0

#define QUANT_TYPE signed short
#define BLOCK_TYPE signed short

#define HUFF_EXTEND_TBL(x,s) ((x) < extend_test[s] ? (x) + extend_offset[s] : (x))
#define HUFF_EXTEND(x,s)   HUFF_EXTEND_TBL(x,s)
#define HUFF_EXTEND_P(x,s) HUFF_EXTEND_TBL(x,s)

#define CONST_BITS  13
#define PASS1_BITS  2
#define SCALEDONE ((signed long)1)
#define CONST_SCALE (SCALEDONE << CONST_BITS)
//#define FIX(x)  ((signed long) ((x) * CONST_SCALE + 0.5))

#define FIX_0_298631336  ((signed long)  2446)        /* FIX(0.298631336) */
#define FIX_0_390180644  ((signed long)  3196)        /* FIX(0.390180644) */
#define FIX_0_541196100  ((signed long)  4433)        /* FIX(0.541196100) */
#define FIX_0_765366865  ((signed long)  6270)        /* FIX(0.765366865) */
#define FIX_0_899976223  ((signed long)  7373)        /* FIX(0.899976223) */
#define FIX_1_175875602  ((signed long)  9633)        /* FIX(1.175875602) */
#define FIX_1_501321110  ((signed long)  12299)       /* FIX(1.501321110) */
#define FIX_1_847759065  ((signed long)  15137)       /* FIX(1.847759065) */
#define FIX_1_961570560  ((signed long)  16069)       /* FIX(1.961570560) */
#define FIX_2_053119869  ((signed long)  16819)       /* FIX(2.053119869) */
#define FIX_2_562915447  ((signed long)  20995)       /* FIX(2.562915447) */
#define FIX_3_072711026  ((signed long)  25172)       /* FIX(3.072711026) */

#define DESCALE(x,n)        ((((x) + (SCALEDONE << ((n) - 1)))) >> (n))
#define MULTIPLY(var,cnst)  ((var) * (cnst))

#ifndef max
#define max(a,b) (((a)>(b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a)<(b)) ? (a) : (b))
#endif

///////////////////////////////////////////////////////////////////
// Loader: Structures                                            //
///////////////////////////////////////////////////////////////////

typedef struct huff_tables_tag
{
	unsigned int  look_up[256];
	unsigned char code_size[256];
	// FIXME: Is 512 tree entries really enough to handle _all_ possible
	// code sets? I think so but not 100% positive.
	unsigned int  tree[512];
}huff_tables_t, *Phuff_tables_t;

typedef struct coeff_buf_tag
{
	unsigned char *Pdata;
	int block_num_x, block_num_y;
	int block_len_x, block_len_y;
	int block_size;
}coeff_buf_t, *Pcoeff_buf_t;

///////////////////////////////////////////////////////////////////
// Static data                                                   //
///////////////////////////////////////////////////////////////////

// Coefficients are stored in this sequence in the data stream.
const int ZAG[64] =
{
	 0,  1,  8, 16,  9,  2,  3, 10,
	17, 24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34,
	27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36,
	29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46,
	53, 60, 61, 54, 47, 55, 62, 63
};

const int AAN_SCALE_BITS = 14;
const int IFAST_SCALE_BITS = 2; // fractional bits in scale factors
/*
static signed short aan_scales[64] =
{
	16384, 22725, 21407, 19266, 16384, 12873,  8867,  4520,
	22725, 31521, 29692, 26722, 22725, 17855, 12299,  6270,
	21407, 29692, 27969, 25172, 21407, 16819, 11585,  5906,
	19266, 26722, 25172, 22654, 19266, 15137, 10426,  5315,
	16384, 22725, 21407, 19266, 16384, 12873,  8867,  4520,
	12873, 17855, 16819, 15137, 12873, 10114,  6967,  3552,
	 8867, 12299, 11585, 10426,  8867,  6967,  4799,  2446,
	 4520,  6270,  5906,  5315,  4520,  3552,  2446,  1247
};
*/
const int extend_test[16] = // entry n is 2**(n-1)
{
	0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000
};

const int extend_offset[16] = // entry n is (-1 << n) + 1
{
	0, ((-1)<<1) + 1, ((-1)<<2) + 1, ((-1)<<3) + 1, ((-1)<<4) + 1,
    ((-1)<<5) + 1, ((-1)<<6) + 1, ((-1)<<7) + 1, ((-1)<<8) + 1,
    ((-1)<<9) + 1, ((-1)<<10) + 1, ((-1)<<11) + 1, ((-1)<<12) + 1,
    ((-1)<<13) + 1, ((-1)<<14) + 1, ((-1)<<15) + 1
};

// used by huff_extend()
const int extend_mask[] =
{
  0,
  (1<<0), (1<<1), (1<<2), (1<<3),
  (1<<4), (1<<5), (1<<6), (1<<7),
  (1<<8), (1<<9), (1<<10), (1<<11),
  (1<<12), (1<<13), (1<<14), (1<<15),
  (1<<16)
};

///////////////////////////////////////////////////////////////////
// Loader: Class definition                                      //
///////////////////////////////////////////////////////////////////

class jpeg_decoder;
typedef void (*Pdecode_block_func)(jpeg_decoder *, int, int, int);

class progressive_block_decoder
{
	public:
		static void decode_block_dc_first(jpeg_decoder *Pd, int component_id, int block_x, int block_y);
		static void decode_block_dc_refine(jpeg_decoder *Pd, int component_id, int block_x, int block_y);
		static void decode_block_ac_first(jpeg_decoder *Pd, int component_id, int block_x, int block_y);
		static void decode_block_ac_refine(jpeg_decoder *Pd, int component_id, int block_x, int block_y);
};

class jpeg_decoder : public ClassELoaderBase
{
	friend class progressive_block_decoder;
	private:
		int image_x_size;
		int image_y_size;
		int progressive_flag;
		unsigned char *huff_num[JPGD_MAXHUFFTABLES];  // pointer to number of Huffman codes per bit size
		unsigned char *huff_val[JPGD_MAXHUFFTABLES];  // pointer to Huffman codes per bit size
		QUANT_TYPE *quant[JPGD_MAXQUANTTABLES];    // pointer to quantization tables
		int scan_type;                           // Grey, Yh1v1, Yh1v2, Yh2v1, Yh2v2, CMYK111, CMYK4114
		int comps_in_frame;                      // # of components in frame
		int comp_h_samp[JPGD_MAXCOMPONENTS];     // component's horizontal sampling factor
		int comp_v_samp[JPGD_MAXCOMPONENTS];     // component's vertical sampling factor
		int comp_quant[JPGD_MAXCOMPONENTS];      // component's quantization table selector
		int comp_ident[JPGD_MAXCOMPONENTS];      // component's ID
		int comp_h_blocks[JPGD_MAXCOMPONENTS];
		int comp_v_blocks[JPGD_MAXCOMPONENTS];
		int comps_in_scan;                       // # of components in scan
		int comp_list[JPGD_MAXCOMPSINSCAN];      // components in this scan
		int comp_dc_tab[JPGD_MAXCOMPONENTS];     // component's DC Huffman coding table selector
		int comp_ac_tab[JPGD_MAXCOMPONENTS];     // component's AC Huffman coding table selector
		int spectral_start;                 // spectral selection start
		int spectral_end;                   // spectral selection end
		int successive_low;                 // successive approximation low
		int successive_high;                // successive approximation high
		int max_mcu_x_size;                 // MCU's max. X size in pixels
		int max_mcu_y_size;                 // MCU's max. Y size in pixels
		int blocks_per_mcu;
		int max_blocks_per_row;
		int mcus_per_row, mcus_per_col;
		int mcu_org[JPGD_MAXBLOCKSPERMCU];
		int total_lines_left;               // total # lines left in image
		int mcu_lines_left;                 // total # lines left in this MCU
		int real_dest_bytes_per_scan_line;
		int dest_bytes_per_scan_line;       // rounded up
		int dest_bytes_per_pixel;           // currently, 4 (RGB) or 1 (Y)
		void *blocks[JPGD_MAXBLOCKS];       // list of all dynamically allocated blocks
		Phuff_tables_t h[JPGD_MAXHUFFTABLES];
		Pcoeff_buf_t dc_coeffs[JPGD_MAXCOMPONENTS];
		Pcoeff_buf_t ac_coeffs[JPGD_MAXCOMPONENTS];
		int eob_run;
		int block_y_mcu[JPGD_MAXCOMPONENTS];
		unsigned char *Pin_buf_ofs;
		int in_buf_left;
		int tem_flag;
		bool eof_flag;
		unsigned char padd_1[128];
		unsigned char in_buf[JPGD_INBUFSIZE + 128];
		unsigned char padd_2[128];
		int bits_left;
		union
		{
			unsigned int bit_buf;
			unsigned int bit_buf_64[2];
		};
		unsigned int saved_mm1[2];
		int restart_interval;
		int restarts_left;
		int next_restart_num;
		int max_mcus_per_row;
		int max_blocks_per_mcu;
		int max_mcus_per_col;
		unsigned int *component[JPGD_MAXBLOCKSPERMCU];   // points into the lastdcvals table
		unsigned int last_dc_val[JPGD_MAXCOMPONENTS];
		Phuff_tables_t dc_huff_seg[JPGD_MAXBLOCKSPERMCU];
		Phuff_tables_t ac_huff_seg[JPGD_MAXBLOCKSPERMCU];
		BLOCK_TYPE *block_seg[JPGD_MAXBLOCKSPERROW];
		int block_max_zag_set[JPGD_MAXBLOCKSPERROW];
		unsigned char *Psample_buf;
		//int block_num[JPGD_MAXBLOCKSPERROW];
		int crr[256];
		int cbb[256];
		int padd;
		long crg[256];
		long cbg[256];
		unsigned char *scan_line_0;
		unsigned char *scan_line_1;
		BLOCK_TYPE temp_block[64];
		int error_code;
		bool ready_flag;
		//jmp_buf jmp_state;
		int total_bytes_read;

		long streamread(void *dst, unsigned long rsize, bool *testeof);
		void free_all_blocks(void);
		void abortdecoder(int status);
		void *alloc(int n);
		void word_clear(void *p, unsigned short c, unsigned int n);
		void prep_in_buffer(void);
		void read_dht_marker(void);
		void read_dqt_marker(void);
		void read_sof_marker(void);
		void skip_variable_marker(void);
		void read_dri_marker(void);
		void read_sos_marker(void);
		int next_marker(void);
		int process_markers(void);
		void locate_soi_marker(void);
		void locate_sof_marker(void);
		int locate_sos_marker(void);
		void init(void *ldata, unsigned long lsize);
		void create_look_ups(void);
		void fix_in_buffer(void);
		void transform_row(void);
		Pcoeff_buf_t coeff_buf_open(int block_num_x, int block_num_y, int block_len_x, int block_len_y);
		void coeff_buf_read(Pcoeff_buf_t cb, int block_x, int block_y, BLOCK_TYPE *buffer);
		void coeff_buf_write(Pcoeff_buf_t cb, int block_x, int block_y, BLOCK_TYPE *buffer);
		BLOCK_TYPE *coeff_buf_getp(Pcoeff_buf_t cb, int block_x, int block_y);
		void load_next_row(void);
		void decode_next_row(void);
		void make_huff_table(int index, Phuff_tables_t hs);
		void check_quant_tables(void);
		void check_huff_tables(void);
		void calc_mcu_block_order(void);
		int init_scan(void);
		void init_frame(void);
		void process_restart(void);
		void decode_scan(Pdecode_block_func decode_block_func);
		void init_progressive(void);
		void init_sequential(void);
		void decode_start(void);
		void decode_init(void *ldata, unsigned long lsize);
		void H2V2Convert(void);
		void H2V1Convert(void);
		void H1V2Convert(void);
		void H1V1Convert(void);
		void GrayConvert(void);
		void find_eoi(void);
		inline unsigned int rol(unsigned int i, unsigned char j);
		inline unsigned int get_char(void);
		inline unsigned int get_char(bool *Ppadding_flag);
		inline void stuff_char(unsigned char q);
		inline unsigned char get_octet(void);
		inline unsigned int get_bits_1(int num_bits);
		inline unsigned int get_bits_2(int numbits);
		inline int huff_decode(Phuff_tables_t Ph);
		inline unsigned char clamp(int i);
	public:
		jpeg_decoder(void *ldata, unsigned long lsize);
		~jpeg_decoder();
		int begin(void);
		int decode(void **Pscan_line_ofs, unsigned int *Pscan_line_len);
		int get_error_code(void){ return (error_code); }
		int get_width(void){ return(image_x_size); }
		int get_height(void){ return(image_y_size); }
		int get_num_components(void){ return(comps_in_frame); }
		int get_bytes_per_pixel(void){ return(dest_bytes_per_pixel); }
		int get_bytes_per_scan_line(void){ return(image_x_size * get_bytes_per_pixel()); }
		int get_total_bytes_read(void){ return(total_bytes_read); }
};

///////////////////////////////////////////////////////////////////
// JPEG pure functions                                           //
///////////////////////////////////////////////////////////////////

void idct(BLOCK_TYPE *data, unsigned char *Pdst_ptr)
{
	signed long tmp0, tmp1, tmp2, tmp3;
	signed long tmp10, tmp11, tmp12, tmp13;
	signed long z1, z2, z3, z4, z5;
	register BLOCK_TYPE *dataptr;
	int rowctr;
	dataptr = data;
	for (rowctr = 8-1; rowctr >= 0; rowctr--)
	{
		if ((dataptr[1] | dataptr[2] | dataptr[3] | dataptr[4] | dataptr[5] | dataptr[6] | dataptr[7]) == 0)
		{
			signed short dcval = (signed short) (dataptr[0] << PASS1_BITS);
			dataptr[0] = dcval;
			dataptr[1] = dcval;
			dataptr[2] = dcval;
			dataptr[3] = dcval;
			dataptr[4] = dcval;
			dataptr[5] = dcval;
			dataptr[6] = dcval;
			dataptr[7] = dcval;
			dataptr += 8;       // advance pointer to next row
			continue;
		}
		z2 = (signed long) dataptr[2];
		z3 = (signed long) dataptr[6];
		z1 = MULTIPLY(z2 + z3, FIX_0_541196100);
		tmp2 = z1 + MULTIPLY(z3, - FIX_1_847759065);
		tmp3 = z1 + MULTIPLY(z2, FIX_0_765366865);
		tmp0 = ((signed long) dataptr[0] + (signed long) dataptr[4]) << CONST_BITS;
		tmp1 = ((signed long) dataptr[0] - (signed long) dataptr[4]) << CONST_BITS;
		tmp10 = tmp0 + tmp3;
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;
		tmp0 = (signed long) dataptr[7];
		tmp1 = (signed long) dataptr[5];
		tmp2 = (signed long) dataptr[3];
		tmp3 = (signed long) dataptr[1];
		z1 = tmp0 + tmp3;
		z2 = tmp1 + tmp2;
		z3 = tmp0 + tmp2;
		z4 = tmp1 + tmp3;
		z5 = MULTIPLY(z3 + z4, FIX_1_175875602);
		tmp0 = MULTIPLY(tmp0, FIX_0_298631336);
		tmp1 = MULTIPLY(tmp1, FIX_2_053119869);
		tmp2 = MULTIPLY(tmp2, FIX_3_072711026);
		tmp3 = MULTIPLY(tmp3, FIX_1_501321110);
		z1 = MULTIPLY(z1, - FIX_0_899976223);
		z2 = MULTIPLY(z2, - FIX_2_562915447);
		z3 = MULTIPLY(z3, - FIX_1_961570560);
		z4 = MULTIPLY(z4, - FIX_0_390180644);
		z3 += z5;
		z4 += z5;
		tmp0 += z1 + z3;
		tmp1 += z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;
		dataptr[0] = (signed short) DESCALE(tmp10 + tmp3, CONST_BITS-PASS1_BITS);
		dataptr[7] = (signed short) DESCALE(tmp10 - tmp3, CONST_BITS-PASS1_BITS);
		dataptr[1] = (signed short) DESCALE(tmp11 + tmp2, CONST_BITS-PASS1_BITS);
		dataptr[6] = (signed short) DESCALE(tmp11 - tmp2, CONST_BITS-PASS1_BITS);
		dataptr[2] = (signed short) DESCALE(tmp12 + tmp1, CONST_BITS-PASS1_BITS);
		dataptr[5] = (signed short) DESCALE(tmp12 - tmp1, CONST_BITS-PASS1_BITS);
		dataptr[3] = (signed short) DESCALE(tmp13 + tmp0, CONST_BITS-PASS1_BITS);
		dataptr[4] = (signed short) DESCALE(tmp13 - tmp0, CONST_BITS-PASS1_BITS);
		dataptr += 8;
	}
	dataptr = data;
	for (rowctr = 8 - 1; rowctr >= 0; rowctr--)
	{
		signed short i;
		if ((dataptr[8*1] | dataptr[8*2] | dataptr[8*3] | dataptr[8*4] | dataptr[8*5] | dataptr[8*6] | dataptr[8*7]) == 0)
		{
			signed short dcval = (signed short) DESCALE((signed long) dataptr[0], PASS1_BITS+3);
			if ((dcval += 128) < 0)
				dcval = 0;
			else if (dcval > 255)
				dcval = 255;
			Pdst_ptr[8*0] = (unsigned char)dcval;
			Pdst_ptr[8*1] = (unsigned char)dcval;
			Pdst_ptr[8*2] = (unsigned char)dcval;
			Pdst_ptr[8*3] = (unsigned char)dcval;
			Pdst_ptr[8*4] = (unsigned char)dcval;
			Pdst_ptr[8*5] = (unsigned char)dcval;
			Pdst_ptr[8*6] = (unsigned char)dcval;
			Pdst_ptr[8*7] = (unsigned char)dcval;
			dataptr++;
			Pdst_ptr++;
			continue;
		}
		z2 = (signed long) dataptr[8*2];
		z3 = (signed long) dataptr[8*6];
		z1 = MULTIPLY(z2 + z3, FIX_0_541196100);
		tmp2 = z1 + MULTIPLY(z3, - FIX_1_847759065);
		tmp3 = z1 + MULTIPLY(z2, FIX_0_765366865);
		tmp0 = ((signed long) dataptr[8*0] + (signed long) dataptr[8*4]) << CONST_BITS;
		tmp1 = ((signed long) dataptr[8*0] - (signed long) dataptr[8*4]) << CONST_BITS;
		tmp10 = tmp0 + tmp3;
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;
		tmp0 = (signed long) dataptr[8*7];
		tmp1 = (signed long) dataptr[8*5];
		tmp2 = (signed long) dataptr[8*3];
		tmp3 = (signed long) dataptr[8*1];
		z1 = tmp0 + tmp3;
		z2 = tmp1 + tmp2;
		z3 = tmp0 + tmp2;
		z4 = tmp1 + tmp3;
		z5 = MULTIPLY(z3 + z4, FIX_1_175875602);
		tmp0 = MULTIPLY(tmp0, FIX_0_298631336);
		tmp1 = MULTIPLY(tmp1, FIX_2_053119869);
		tmp2 = MULTIPLY(tmp2, FIX_3_072711026);
		tmp3 = MULTIPLY(tmp3, FIX_1_501321110);
		z1 = MULTIPLY(z1, - FIX_0_899976223);
		z2 = MULTIPLY(z2, - FIX_2_562915447);
		z3 = MULTIPLY(z3, - FIX_1_961570560);
		z4 = MULTIPLY(z4, - FIX_0_390180644);
		z3 += z5;
		z4 += z5;
		tmp0 += z1 + z3;
		tmp1 += z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;

		#define idctclamp(i) if (i & 0xFF00) i = (((~i) >> 15) & 0xFF);

		i = (signed short) DESCALE(tmp10 + tmp3, CONST_BITS+PASS1_BITS+3) + 128;
		idctclamp(i)
		Pdst_ptr[8*0] = (unsigned char)i;
		i = (signed short) DESCALE(tmp10 - tmp3, CONST_BITS+PASS1_BITS+3) + 128;
		idctclamp(i)
		Pdst_ptr[8*7] = (unsigned char)i;
		i = (signed short) DESCALE(tmp11 + tmp2, CONST_BITS+PASS1_BITS+3) + 128;
		idctclamp(i)
		Pdst_ptr[8*1] = (unsigned char)i;
		i = (signed short) DESCALE(tmp11 - tmp2, CONST_BITS+PASS1_BITS+3) + 128;
		idctclamp(i)
		Pdst_ptr[8*6] = (unsigned char)i;
		i = (signed short) DESCALE(tmp12 + tmp1, CONST_BITS+PASS1_BITS+3) + 128;
		idctclamp(i)
		Pdst_ptr[8*2] = (unsigned char)i;
		i = (signed short) DESCALE(tmp12 - tmp1, CONST_BITS+PASS1_BITS+3) + 128;
		idctclamp(i)
		Pdst_ptr[8*5] = (unsigned char)i;
		i = (signed short) DESCALE(tmp13 + tmp0, CONST_BITS+PASS1_BITS+3) + 128;
		idctclamp(i)
		Pdst_ptr[8*3] = (unsigned char)i;
		i = (signed short) DESCALE(tmp13 - tmp0, CONST_BITS+PASS1_BITS+3) + 128;
		idctclamp(i)
		Pdst_ptr[8*4] = (unsigned char)i;
		dataptr++;
		Pdst_ptr++;
	}
}

///////////////////////////////////////////////////////////////////
// JPEG progressive block decoder                                //
///////////////////////////////////////////////////////////////////

// The following methods decode the various types of blocks encountered
// in progressively encoded images.
void progressive_block_decoder::decode_block_dc_first(jpeg_decoder *Pd, int component_id, int block_x, int block_y)
{
	int s, r;
	BLOCK_TYPE *p = Pd->coeff_buf_getp(Pd->dc_coeffs[component_id], block_x, block_y);
	if ((s = Pd->huff_decode(Pd->h[Pd->comp_dc_tab[component_id]])) != 0)
	{
		r = Pd->get_bits_2(s);
		s = HUFF_EXTEND_P(r, s);
	}
	Pd->last_dc_val[component_id] = (s += Pd->last_dc_val[component_id]);
	p[0] = s << Pd->successive_low;
}

void progressive_block_decoder::decode_block_dc_refine(jpeg_decoder *Pd, int component_id, int block_x, int block_y)
{
	if (Pd->get_bits_2(1))
	{
		BLOCK_TYPE *p = Pd->coeff_buf_getp(Pd->dc_coeffs[component_id], block_x, block_y);
		p[0] |= (1 << Pd->successive_low);
	}
}

void progressive_block_decoder::decode_block_ac_first(jpeg_decoder *Pd, int component_id, int block_x, int block_y)
{
	int k, s, r;
	if (Pd->eob_run)
	{
		Pd->eob_run--;
		return;
	}
	BLOCK_TYPE *p = Pd->coeff_buf_getp(Pd->ac_coeffs[component_id], block_x, block_y);
	for (k = Pd->spectral_start; k <= Pd->spectral_end; k++)
	{
		s = Pd->huff_decode(Pd->h[Pd->comp_ac_tab[component_id]]);
		r = s >> 4;
		s &= 15;
		if (s)
		{
			if ((k += r) > 63)
			{
				Pd->abortdecoder(JPGD_DECODE_ERROR);
				return;
			}
			r = Pd->get_bits_2(s);
			s = HUFF_EXTEND_P(r, s);
			p[ZAG[k]] = s << Pd->successive_low;
		}
		else
		{
			if (r == 15)
			{
				if ((k += 15) > 63)
				{
					Pd->abortdecoder(JPGD_DECODE_ERROR);
					return;
				}
			}
			else
			{
				Pd->eob_run = 1 << r;
				if (r)
					Pd->eob_run += Pd->get_bits_2(r);
				Pd->eob_run--;
				break;
			}
		}
	}
}

void progressive_block_decoder::decode_block_ac_refine(jpeg_decoder *Pd, int component_id, int block_x, int block_y)
{
	int s, k, r;
	int p1 = 1 << Pd->successive_low;
	int m1 = (-1) << Pd->successive_low;
	BLOCK_TYPE *p = Pd->coeff_buf_getp(Pd->ac_coeffs[component_id], block_x, block_y);
	k = Pd->spectral_start;
	if (Pd->eob_run == 0)
	{
		for ( ; k <= Pd->spectral_end; k++)
		{
			s = Pd->huff_decode(Pd->h[Pd->comp_ac_tab[component_id]]);
			r = s >> 4;
			s &= 15;
			if (s)
			{
				if (s != 1)
				{
					Pd->abortdecoder(JPGD_DECODE_ERROR);
					return;
				}
				if (Pd->get_bits_2(1))
					s = p1;
				else
					s = m1;
			}
			else
			{
				if (r != 15)
				{
					Pd->eob_run = 1 << r;
					if (r)
						Pd->eob_run += Pd->get_bits_2(r);
					break;
				}
			}
			do
			{
				BLOCK_TYPE *this_coef = p + ZAG[k];
				if (*this_coef != 0)
				{
					if (Pd->get_bits_2(1))
					{
						if ((*this_coef & p1) == 0)
						{
							if (*this_coef >= 0)
								*this_coef += p1;
							else
								*this_coef += m1;
						}
					}
				}
				else
				{
					if (--r < 0)
						break;
				}
				k++;
			}while (k <= Pd->spectral_end);
			if ((s) && (k < 64))
			{
				p[ZAG[k]] = s;
			}
		}
	}
	if (Pd->eob_run > 0)
	{
		for ( ; k <= Pd->spectral_end; k++)
		{
			BLOCK_TYPE *this_coef = p + ZAG[k];
			if (*this_coef != 0)
			{
				if (Pd->get_bits_2(1))
				{
					if ((*this_coef & p1) == 0)
					{
						if (*this_coef >= 0)
							*this_coef += p1;
						else
							*this_coef += m1;
					}
				}
			}
		}
		Pd->eob_run--;
	}
}

///////////////////////////////////////////////////////////////////
// JPEG decoder backend                                          //
///////////////////////////////////////////////////////////////////

//Easy interface to read from the stream
long jpeg_decoder::streamread(void *dst, unsigned long rsize, bool *testeof)
{
	if (lptr >= maxsize)
	{
		*testeof = true;
		return(-1);
	}
	if (lptr + rsize > maxsize)
		rsize = maxsize - lptr;
	if (!ReadStream(dst, rsize))
	{
		*testeof = true;
		return(-1);
	}
	*testeof = !HaveData();
	return(rsize);
}

//Unconditionally frees all allocated blocks
void jpeg_decoder::free_all_blocks(void)
{
	for (int i = 0; i < JPGD_MAXBLOCKS; i++)
		if (blocks[i] != NULL)
		{
			ClassEMemory::DeAlloc(blocks[i]);
			blocks[i] = NULL;
		}
}

//This method handles all errors. It could easily be changed to use C++ exceptions.
void jpeg_decoder::abortdecoder(int status)
{
	error_code = status;
	free_all_blocks();
}

//Allocate a block of memory-- store block's address in list for later deallocation by free_all_blocks().
void *jpeg_decoder::alloc(int n)
{
	int i;
	for (i = 0; i < JPGD_MAXBLOCKS; i++)
	{
		if (blocks[i] == NULL)
			break;
	}
	if (i == JPGD_MAXBLOCKS)
	{
		abortdecoder(JPGD_TOO_MANY_BLOCKS);
		return(NULL);
	}
	void *q = ClassEMemory::Alloc(n + 8);
	if (q == NULL)
	{
		abortdecoder(JPGD_NOTENOUGHMEM);
		return(NULL);
	}
	ClassEMemory::Set(q, 0, n + 8);
	blocks[i] = q;
	return ((void *)(((unsigned long)q + 7) & ~7));
}

// Clear buffer to word values.
void jpeg_decoder::word_clear(void *p, unsigned short c, unsigned int n)
{
	unsigned short *ps = (unsigned short *)p;
	while (n)
	{
		*ps++ = c;
		n--;
	}
}

// Refill the input buffer. This method will sit in a loop until (A) the buffer is full or (B) the stream's read() method reports and end of file condition.
void jpeg_decoder::prep_in_buffer(void)
{
	in_buf_left = 0;
	Pin_buf_ofs = in_buf;
	if (eof_flag)
		return;
	do
	{
		int bytes_read = streamread(in_buf + in_buf_left, JPGD_INBUFSIZE - in_buf_left, &eof_flag);
		if (bytes_read == -1)
		{
			abortdecoder(JPGD_STREAM_READ);
			return;
		}
		in_buf_left += bytes_read;
	}while ((in_buf_left < JPGD_INBUFSIZE) && (!eof_flag));
	total_bytes_read += in_buf_left;
	ClassEMemory::Set16(Pin_buf_ofs + in_buf_left, 0xD9FF, 64);
}

// Read a Huffman code table.
void jpeg_decoder::read_dht_marker(void)
{
	int i, index, count;
	unsigned int left;
	unsigned char huff_num[17];
	unsigned char huff_val[256];
	left = get_bits_1(16);
	if (left < 2)
	{
		abortdecoder(JPGD_BAD_DHT_MARKER);
		return;
	}
	left -= 2;
	while (left)
	{
		index = get_bits_1(8);
		huff_num[0] = 0;
		count = 0;
		for (i = 1; i <= 16; i++)
		{
			huff_num[i] = get_bits_1(8);
			count += huff_num[i];
		}
		if (count > 255)
		{
			abortdecoder(JPGD_BAD_DHT_COUNTS);
			return;
		}
		for (i = 0; i < count; i++)
			huff_val[i] = get_bits_1(8);
		i = 1 + 16 + count;
		if (left < (unsigned int)i)
		{
			abortdecoder(JPGD_BAD_DHT_MARKER);
			return;
		}
		left -= i;
		if ((index & 0x10) > 0x10)
		{
			abortdecoder(JPGD_BAD_DHT_INDEX);
			return;
		}
		index = (index & 0x0F) + ((index & 0x10) >> 4) * (JPGD_MAXHUFFTABLES >> 1);
		if (index >= JPGD_MAXHUFFTABLES)
		{
			abortdecoder(JPGD_BAD_DHT_INDEX);
			return;
		}
		if (!this->huff_num[index])
			this->huff_num[index] = (unsigned char *)alloc(17);
		if (!this->huff_val[index])
			this->huff_val[index] = (unsigned char *)alloc(256);
		ClassEMemory::Copy(this->huff_num[index], huff_num, 17);
		ClassEMemory::Copy(this->huff_val[index], huff_val, 256);
	}
}

// Read a quantization table.
void jpeg_decoder::read_dqt_marker(void)
{
	int n, i, prec;
	unsigned int left;
	unsigned int temp;
	left = get_bits_1(16);
	if (left < 2)
	{
		abortdecoder(JPGD_BAD_DQT_MARKER);
		return;
	}
	left -= 2;
	while (left)
	{
		n = get_bits_1(8);
		prec = n >> 4;
		n &= 0x0F;
		if (n >= JPGD_MAXQUANTTABLES)
		{
			abortdecoder(JPGD_BAD_DQT_TABLE);
			return;
		}
		if (!quant[n])
			quant[n] = (QUANT_TYPE *)alloc(64 * sizeof(QUANT_TYPE));
		for (i = 0; i < 64; i++)
		{
			temp = get_bits_1(8);
			if (prec)
				temp = (temp << 8) + get_bits_1(8);
			quant[n][i] = temp;
		}
		i = 64 + 1;
		if (prec)
			i += 64;
		if (left < (unsigned int)i)
		{
			abortdecoder(JPGD_BAD_DQT_LENGTH);
			return;
		}
		left -= i;
	}
}

// Read the start of frame (SOF) marker.
void jpeg_decoder::read_sof_marker(void)
{
	int i;
	unsigned int left;
	left = get_bits_1(16);
	if (get_bits_1(8) != 8)   // precision: sorry, only 8-bit precision is supported right now
	{
		abortdecoder(JPGD_BAD_PRECISION);
		return;
	}
	image_y_size = get_bits_1(16);
	if ((image_y_size < 1) || (image_y_size > JPGD_MAX_HEIGHT))
	{
		abortdecoder(JPGD_BAD_HEIGHT);
		return;
	}
	image_x_size = get_bits_1(16);
	if ((image_x_size < 1) || (image_x_size > JPGD_MAX_WIDTH))
	{
		abortdecoder(JPGD_BAD_WIDTH);
		return;
	}
	comps_in_frame = get_bits_1(8);
	if (comps_in_frame > JPGD_MAXCOMPONENTS)
	{
		abortdecoder(JPGD_TOO_MANY_COMPONENTS);
		return;
	}
	if (left != (unsigned int)(comps_in_frame * 3 + 8))
	{
		abortdecoder(JPGD_BAD_SOF_LENGTH);
		return;
	}
	for (i = 0; i < comps_in_frame; i++)
	{
		comp_ident[i]  = get_bits_1(8);
		comp_h_samp[i] = get_bits_1(4);
		comp_v_samp[i] = get_bits_1(4);
		comp_quant[i]  = get_bits_1(8);
	}
}

// Used to skip unrecognized markers.
void jpeg_decoder::skip_variable_marker(void)
{
	unsigned int left;
	left = get_bits_1(16);
	if (left < 2)
	{
		abortdecoder(JPGD_BAD_VARIABLE_MARKER);
		return;
	}
	left -= 2;
	while (left)
	{
		get_bits_1(8);
		left--;
	}
}

// Read a define restart interval (DRI) marker.
void jpeg_decoder::read_dri_marker(void)
{
	if (get_bits_1(16) != 4)
	{
		abortdecoder(JPGD_BAD_DRI_LENGTH);
		return;
	}
	restart_interval = get_bits_1(16);
}

// Read a start of scan (SOS) marker.
void jpeg_decoder::read_sos_marker(void)
{
	unsigned int left;
	int i, ci, n, c, cc;
	left = get_bits_1(16);
	n = get_bits_1(8);
	comps_in_scan = n;
	left -= 3;
	if ( (left != (unsigned int)(n * 2 + 3)) || (n < 1) || (n > JPGD_MAXCOMPSINSCAN) )
	{
		abortdecoder(JPGD_BAD_SOS_LENGTH);
		return;
	}
	for (i = 0; i < n; i++)
	{
		cc = get_bits_1(8);
		c = get_bits_1(8);
		left -= 2;
		for (ci = 0; ci < comps_in_frame; ci++)
			if (cc == comp_ident[ci])
				break;
		if (ci >= comps_in_frame)
		{
			abortdecoder(JPGD_BAD_SOS_COMP_ID);
			return;
		}
		comp_list[i]    = ci;
		comp_dc_tab[ci] = (c >> 4) & 15;
		comp_ac_tab[ci] = (c & 15) + (JPGD_MAXHUFFTABLES >> 1);
	}
	spectral_start  = get_bits_1(8);
	spectral_end    = get_bits_1(8);
	successive_high = get_bits_1(4);
	successive_low  = get_bits_1(4);
	if (!progressive_flag)
	{
		spectral_start = 0;
		spectral_end = 63;
	}
	left -= 3;
	while (left)                  // read past whatever is left
	{
		get_bits_1(8);
		left--;
	}
}

// Finds the next marker.
int jpeg_decoder::next_marker(void)
{
	unsigned int c, bytes;
	bytes = 0;
	do
	{
		do
		{
			bytes++;
			c = get_bits_1(8);
		} while (c != 0xFF);
		do
		{
			c = get_bits_1(8);
		} while (c == 0xFF);
	} while (c == 0);
	// If bytes > 0 here, there where extra bytes before the marker (not good).
	return(c);
}

// Process markers. Returns when an SOFx, SOI, EOI, or SOS marker is encountered.
int jpeg_decoder::process_markers(void)
{
	int c;
	for ( ; ; )
	{
		c = next_marker();
		switch (c)
		{
			case M_SOF0:
			case M_SOF1:
			case M_SOF2:
			case M_SOF3:
			case M_SOF5:
			case M_SOF6:
			case M_SOF7:
			//case M_JPG:
			case M_SOF9:
			case M_SOF10:
			case M_SOF11:
			case M_SOF13:
			case M_SOF14:
			case M_SOF15:
			case M_SOI:
			case M_EOI:
			case M_SOS:
			{
				return c;
			}
			case M_DHT:
			{
				read_dht_marker();
				break;
			}
			// Sorry, no arithmitic support at this time. Dumb patents!
			case M_DAC:
			{
				abortdecoder(JPGD_NO_ARITHMITIC_SUPPORT);
				break;
			}
			case M_DQT:
			{
				read_dqt_marker();
				break;
			}
			case M_DRI:
			{
				read_dri_marker();
				break;
			}
			//case M_APP0:  // no need to read the JFIF marker
			case M_JPG:
			case M_RST0:    // no parameters
			case M_RST1:
			case M_RST2:
			case M_RST3:
			case M_RST4:
			case M_RST5:
			case M_RST6:
			case M_RST7:
			case M_TEM:
			{
				abortdecoder(JPGD_UNEXPECTED_MARKER);
				break;
			}
			default:    // must be DNL, DHP, EXP, APPn, JPGn, COM, or RESn or APP0
			{
				skip_variable_marker();
				break;
			}
		}
	}
}

// Finds the start of image (SOI) marker. This code is rather defensive: it only checks the first 512 bytes to avoid false positives.
void jpeg_decoder::locate_soi_marker(void)
{
	unsigned int lastchar, thischar;
	unsigned long bytesleft;
	lastchar = get_bits_1(8);
	thischar = get_bits_1(8);

	// ok if it's a normal JPEG file without a special header
	if ((lastchar == 0xFF) && (thischar == M_SOI))
		return;
	bytesleft = 512;
	for ( ; ; )
	{
		if (--bytesleft == 0)
		{
			abortdecoder(JPGD_NOT_JPEG);
			return;
		}
		lastchar = thischar;
		thischar = get_bits_1(8);
		if ((lastchar == 0xFF) && (thischar == M_SOI))
			break;
	}

	// Check the next character after marker: if it's not 0xFF, it can't  be the start of the next marker, so it probably isn't a JPEG
	thischar = (bit_buf >> 8) & 0xFF;
	if (thischar != 0xFF)
		abortdecoder(JPGD_NOT_JPEG);
}

// Find a start of frame (SOF) marker.
void jpeg_decoder::locate_sof_marker(void)
{
	int c;
	locate_soi_marker();
	c = process_markers();
	switch (c)
	{
		case M_SOF2:
			progressive_flag = JPGD_TRUE;
		case M_SOF0:  // baseline DCT
		case M_SOF1:  // extended sequential DCT
		{
			read_sof_marker();
			break;
		}
		case M_SOF9:  // Arithmitic coding
		{
			abortdecoder(JPGD_NO_ARITHMITIC_SUPPORT);
			break;
		}
		default:
		{
			abortdecoder(JPGD_UNSUPPORTED_MARKER);
			break;
		}
	}
}

// Find a start of scan (SOS) marker.
int jpeg_decoder::locate_sos_marker(void)
{
	int c;
	c = process_markers();
	if (c == M_EOI)
		return(JPGD_FALSE);
	else if (c != M_SOS)
	{
		abortdecoder(JPGD_UNEXPECTED_MARKER);
		return(JPGD_FALSE);
	}
	read_sos_marker();
	return(JPGD_TRUE);
}

// Reset everything to default/uninitialized state.
void jpeg_decoder::init(void *ldata, unsigned long lsize)
{
	LinkData(ldata, lsize);
	error_code = 0;
	ready_flag = false;
	image_x_size = image_y_size = 0;
	progressive_flag = JPGD_FALSE;
	ClassEMemory::Set(huff_num, 0, sizeof(huff_num));
	ClassEMemory::Set(huff_val, 0, sizeof(huff_val));
	ClassEMemory::Set(quant, 0, sizeof(quant));
	scan_type = 0;
	comps_in_frame = 0;
	ClassEMemory::Set(comp_h_samp, 0, sizeof(comp_h_samp));
	ClassEMemory::Set(comp_v_samp, 0, sizeof(comp_v_samp));
	ClassEMemory::Set(comp_quant, 0, sizeof(comp_quant));
	ClassEMemory::Set(comp_ident, 0, sizeof(comp_ident));
	ClassEMemory::Set(comp_h_blocks, 0, sizeof(comp_h_blocks));
	ClassEMemory::Set(comp_v_blocks, 0, sizeof(comp_v_blocks));
	comps_in_scan = 0;
	ClassEMemory::Set(comp_list, 0, sizeof(comp_list));
	ClassEMemory::Set(comp_dc_tab, 0, sizeof(comp_dc_tab));
	ClassEMemory::Set(comp_ac_tab, 0, sizeof(comp_ac_tab));
	spectral_start = 0;
	spectral_end = 0;
	successive_low = 0;
	successive_high = 0;
	max_mcu_x_size = 0;
	max_mcu_y_size = 0;
	blocks_per_mcu = 0;
	max_blocks_per_row = 0;
	mcus_per_row = 0;
	mcus_per_col = 0;
	ClassEMemory::Set(mcu_org, 0, sizeof(mcu_org));
	total_lines_left = 0;
	mcu_lines_left = 0;
	real_dest_bytes_per_scan_line = 0;
	dest_bytes_per_scan_line = 0;
	dest_bytes_per_pixel = 0;
	ClassEMemory::Set(blocks, 0, sizeof(blocks));
	ClassEMemory::Set(h, 0, sizeof(h));
	ClassEMemory::Set(dc_coeffs, 0, sizeof(dc_coeffs));
	ClassEMemory::Set(ac_coeffs, 0, sizeof(ac_coeffs));
	ClassEMemory::Set(block_y_mcu, 0, sizeof(block_y_mcu));
	eob_run = 0;
	ClassEMemory::Set(block_y_mcu, 0, sizeof(block_y_mcu));
	Pin_buf_ofs = in_buf;
	in_buf_left = 0;
	eof_flag = false;
	tem_flag = 0;
	ClassEMemory::Set(padd_1, 0, sizeof(padd_1));
	ClassEMemory::Set(in_buf, 0, sizeof(in_buf));
	ClassEMemory::Set(padd_2, 0, sizeof(padd_2));
	restart_interval = 0;
	restarts_left    = 0;
	next_restart_num = 0;
	max_mcus_per_row = 0;
	max_blocks_per_mcu = 0;
	max_mcus_per_col = 0;
	ClassEMemory::Set(component, 0, sizeof(component));
	ClassEMemory::Set(last_dc_val, 0, sizeof(last_dc_val));
	ClassEMemory::Set(dc_huff_seg, 0, sizeof(dc_huff_seg));
	ClassEMemory::Set(ac_huff_seg, 0, sizeof(ac_huff_seg));
	ClassEMemory::Set(block_seg, 0, sizeof(block_seg));
	Psample_buf = NULL;
	total_bytes_read = 0;
	prep_in_buffer();
	bits_left = 16;
	bit_buf_64[0] = 0;
	bit_buf_64[1] = 0;
	get_bits_1(16);
	get_bits_1(16);
	for (int i = 0; i < JPGD_MAXBLOCKSPERROW; i++)
		block_max_zag_set[i] = 64;
}

#define SCALEBITS 16
#define ONE_HALF ((long) 1 << (SCALEBITS-1))
#define FIX(x) ((long) ((x) * (1L<<SCALEBITS) + 0.5))

// Create a few tables that allow us to quickly convert YCbCr to RGB.
void jpeg_decoder::create_look_ups(void)
{
	int i, k;
	for (i = 0; i <= 255; i++)
	{
		//k = (i * 2) - 255;
		k = (i * 2) - 256; // Dec. 28 2001- change so table[128] == 0
		crr[i] = ( FIX(1.40200/2)  * k + ONE_HALF) >> SCALEBITS;
		cbb[i] = ( FIX(1.77200/2)  * k + ONE_HALF) >> SCALEBITS;
		crg[i] = (-FIX(0.71414/2)) * k;
		cbg[i] = (-FIX(0.34414/2)) * k + ONE_HALF;
	}
}

// This method throws back into the stream any bytes that where read into the bit buffer during initial marker scanning.
void jpeg_decoder::fix_in_buffer(void)
{
	/* In case any 0xFF's where pulled into the buffer during marker scanning */
	if (bits_left == 16)
		stuff_char( (unsigned char)((bit_buf >> 16) & 0xFF));
	if (bits_left >= 8)
		stuff_char( (unsigned char)((bit_buf >> 24) & 0xFF));
	stuff_char( (unsigned char)(bit_buf & 0xFF) );
	stuff_char( (unsigned char)((bit_buf >> 8) & 0xFF) );
	bits_left = 16;
	//bit_buf = 0;
	get_bits_2(16);
	get_bits_2(16);
}

// Performs a 2D IDCT over the entire row's coefficient buffer.
void jpeg_decoder::transform_row(void)
{
	BLOCK_TYPE *Psrc_ptr = block_seg[0];
	unsigned char *Pdst_ptr = Psample_buf;
	for (int i = max_blocks_per_row; i > 0; i--)
	{
		// Copy the block to a temp. buffer to prevent the IDCT
		// from modifying the entire block.
		ClassEMemory::Copy(temp_block, Psrc_ptr, 64 * sizeof(BLOCK_TYPE));
		idct(temp_block, Pdst_ptr);
		Psrc_ptr += 64;
		Pdst_ptr += 64;
	}
}

// The coeff_buf series of methods originally stored the coefficients into a "virtual" file which was located in EMS, XMS, or a disk file.
// A cache was used to make this process more efficient. Now, we can store the entire thing in RAM.
Pcoeff_buf_t jpeg_decoder::coeff_buf_open(int block_num_x, int block_num_y, int block_len_x, int block_len_y)
{
	Pcoeff_buf_t cb = (Pcoeff_buf_t)alloc(sizeof(coeff_buf_t));
	cb->block_num_x = block_num_x;
	cb->block_num_y = block_num_y;
	cb->block_len_x = block_len_x;
	cb->block_len_y = block_len_y;
	cb->block_size = (block_len_x * block_len_y) * sizeof(BLOCK_TYPE);
	cb->Pdata = (unsigned char *)alloc(cb->block_size * block_num_x * block_num_y);
	return(cb);
}

void jpeg_decoder::coeff_buf_read(Pcoeff_buf_t cb, int block_x, int block_y, BLOCK_TYPE *buffer)
{
	if (block_x >= cb->block_num_x)
	{
		abortdecoder(JPGD_ASSERTION_ERROR);
		return;
	}
	if (block_y >= cb->block_num_y)
	{
		abortdecoder(JPGD_ASSERTION_ERROR);
		return;
	}
	ClassEMemory::Copy(buffer, cb->Pdata + block_x * cb->block_size + block_y * (cb->block_size * cb->block_num_x), cb->block_size);
}

void jpeg_decoder::coeff_buf_write(Pcoeff_buf_t cb, int block_x, int block_y, BLOCK_TYPE *buffer)
{
	if (block_x >= cb->block_num_x)
	{
		abortdecoder(JPGD_ASSERTION_ERROR);
		return;
	}
	if (block_y >= cb->block_num_y)
	{
		abortdecoder(JPGD_ASSERTION_ERROR);
		return;
	}
	ClassEMemory::Copy(cb->Pdata + block_x * cb->block_size + block_y * (cb->block_size * cb->block_num_x), buffer, cb->block_size);
}

BLOCK_TYPE *jpeg_decoder::coeff_buf_getp(Pcoeff_buf_t cb, int block_x, int block_y)
{
	if (block_x >= cb->block_num_x)
	{
		abortdecoder(JPGD_ASSERTION_ERROR);
		return(NULL);
	}
	if (block_y >= cb->block_num_y)
	{
		abortdecoder(JPGD_ASSERTION_ERROR);
		return(NULL);
	}
	return (BLOCK_TYPE *)(cb->Pdata + block_x * cb->block_size + block_y * (cb->block_size * cb->block_num_x));
}

// Loads and dequantizes the next row of (already decoded) coefficients. Progressive images only.
void jpeg_decoder::load_next_row(void)
{
	int i;
	BLOCK_TYPE *p;
	QUANT_TYPE *q;
	int mcu_row, mcu_block, row_block = 0;
	int component_num, component_id;
	int block_x_mcu[JPGD_MAXCOMPONENTS];
	ClassEMemory::Set(block_x_mcu, 0, JPGD_MAXCOMPONENTS * sizeof(int));

	for (mcu_row = 0; mcu_row < mcus_per_row; mcu_row++)
	{
		int block_x_mcu_ofs = 0, block_y_mcu_ofs = 0;
		for (mcu_block = 0; mcu_block < blocks_per_mcu; mcu_block++)
		{
			component_id = mcu_org[mcu_block];
			p = block_seg[row_block];
			q = quant[comp_quant[component_id]];
			BLOCK_TYPE *pAC = coeff_buf_getp(ac_coeffs[component_id], block_x_mcu[component_id] + block_x_mcu_ofs, block_y_mcu[component_id] + block_y_mcu_ofs);
			BLOCK_TYPE *pDC = coeff_buf_getp(dc_coeffs[component_id], block_x_mcu[component_id] + block_x_mcu_ofs, block_y_mcu[component_id] + block_y_mcu_ofs);
			p[0] = pDC[0];
			ClassEMemory::Copy(&p[1], &pAC[1], 63 * sizeof(BLOCK_TYPE));
			for (i = 63; i > 0; i--)
				if (p[ZAG[i]])
					break;
			//block_num[row_block++] = i + 1;
			for ( ; i >= 0; i--)
				if (p[ZAG[i]])
					p[ZAG[i]] *= q[i];
			row_block++;

			if (comps_in_scan == 1)
				block_x_mcu[component_id]++;
			else
			{
				if (++block_x_mcu_ofs == comp_h_samp[component_id])
				{
					block_x_mcu_ofs = 0;
					if (++block_y_mcu_ofs == comp_v_samp[component_id])
					{
						block_y_mcu_ofs = 0;
						block_x_mcu[component_id] += comp_h_samp[component_id];
					}
				}
			}
		}
	}
	if (comps_in_scan == 1)
		block_y_mcu[comp_list[0]]++;
	else
	{
		for (component_num = 0; component_num < comps_in_scan; component_num++)
		{
			component_id = comp_list[component_num];
			block_y_mcu[component_id] += comp_v_samp[component_id];
		}
	}
}

// Restart interval processing.
void jpeg_decoder::process_restart(void)
{
	int i, c = 0;
	// Align to a byte boundry
	// FIXME: Is this really necessary? get_bits_2() never reads in markers!
	//get_bits_2(bits_left & 7);

	// Let's scan a little bit to find the marker, but not _too_ far.
	// 1536 is a "fudge factor" that determines how much to scan.
	for (i = 1536; i > 0; i--)
		if (get_char() == 0xFF)
			break;
	if (i == 0)
	{
		abortdecoder(JPGD_BAD_RESTART_MARKER);
		return;
	}
	for ( ; i > 0; i--)
		if ((c = get_char()) != 0xFF)
			break;
	if (i == 0)
	{
		abortdecoder(JPGD_BAD_RESTART_MARKER);
		return;
	}

	// Is it the expected marker? If not, something bad happened.
	if (c != (next_restart_num + M_RST0))
	{
		abortdecoder(JPGD_BAD_RESTART_MARKER);
		return;
	}

	// Reset each component's DC prediction values.
	ClassEMemory::Set(&last_dc_val, 0, comps_in_frame * sizeof(unsigned int));

	eob_run = 0;
	restarts_left = restart_interval;
	next_restart_num = (next_restart_num + 1) & 7;

	// Get the bit buffer going again...
	bits_left = 16;
	//bit_buf = 0;
	get_bits_2(16);
	get_bits_2(16);
}

// Decodes and dequantizes the next row of coefficients.
void jpeg_decoder::decode_next_row(void)
{
	int row_block = 0;

	// Clearing the entire row block buffer can take a lot of time!
	// Instead of clearing the entire buffer each row, keep track
	// of the number of nonzero entries written to each block and do
	// selective clears.
	//ClassEMemory::Set(block_seg[0], 0, mcus_per_row * blocks_per_mcu * 64 * sizeof(BLOCK_TYPE));

	for (int mcu_row = 0; mcu_row < mcus_per_row; mcu_row++)
	{
		if ((restart_interval) && (restarts_left == 0))
			process_restart();
		for (int mcu_block = 0; mcu_block < blocks_per_mcu; mcu_block++)
		{
			int k, component_id = mcu_org[mcu_block];
			BLOCK_TYPE *p = block_seg[row_block];
			QUANT_TYPE *q = quant[comp_quant[component_id]];
			int r, s;
			if ((s = huff_decode(h[comp_dc_tab[component_id]])) != 0)
			{
				r = get_bits_2(s);
				s = HUFF_EXTEND(r, s);
			}
			last_dc_val[component_id] = (s += last_dc_val[component_id]);
			p[0] = s * q[0];
			int prev_num_set = block_max_zag_set[row_block];
			Phuff_tables_t Ph = h[comp_ac_tab[component_id]];
			for (k = 1; k < 64; k++)
			{
				s = huff_decode(Ph);
				r = s >> 4;
				s &= 15;
				if (s)
				{
					if (r)
					{
						if ((k + r) > 63)
						{
							abortdecoder(JPGD_DECODE_ERROR);
							return;
						}
						if (k < prev_num_set)
						{
							int n = min(r, prev_num_set - k);
							int kt = k;
							while (n--)
								p[ZAG[kt++]] = 0;
						}
						k += r;
					}
					r = get_bits_2(s);
					s = HUFF_EXTEND(r, s);
					//assert(k < 64);
					p[ZAG[k]] = s * q[k];
				}
				else
				{
					if (r == 15)
					{
						if ((k + 15) > 63)
						{
							abortdecoder(JPGD_DECODE_ERROR);
							return;
						}

						if (k < prev_num_set)
						{
							int n = min(16, prev_num_set - k);		//bugfix Dec. 19, 2001 - was 15!
							int kt = k;
							while (n--)
								p[ZAG[kt++]] = 0;
						}
						k += 15;
					}
					else
					{
						//while (k < 64)
						//  p[ZAG[k++]] = 0;
						break;
					}
				}
			}
			if (k < prev_num_set)
			{
				int kt = k;
				while (kt < prev_num_set)
					p[ZAG[kt++]] = 0;
			}
			block_max_zag_set[row_block] = k;
			//block_num[row_block++] = k;
			row_block++;
		}
		restarts_left--;
	}
}

// YCbCr H1V1 (1x1:1:1, 3 blocks per MCU) to 24-bit RGB
void jpeg_decoder::H1V1Convert(void)
{
	int row = max_mcu_y_size - mcu_lines_left;
	unsigned char *d = scan_line_0;
	unsigned char *s = Psample_buf + row * 8;
	for (int i = max_mcus_per_row; i > 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			int y = s[j];
			int cb = s[64+j];
			int cr = s[128+j];
			d[0] = clamp(y + crr[cr]);
			d[1] = clamp(y + ((crg[cr] + cbg[cb]) >> 16));
			d[2] = clamp(y + cbb[cb]);
			d += 4;
		}
		s += 64*3;
	}
}

// YCbCr H2V1 (2x1:1:1, 4 blocks per MCU) to 24-bit RGB
void jpeg_decoder::H2V1Convert(void)
{
	int row = max_mcu_y_size - mcu_lines_left;
	unsigned char *d0 = scan_line_0;
	unsigned char *y = Psample_buf + row * 8;
	unsigned char *c = Psample_buf + 2*64 + row * 8;
	for (int i = max_mcus_per_row; i > 0; i--)
	{
		for (int l = 0; l < 2; l++)
		{
			for (int j = 0; j < 4; j++)
			{
				int cb = c[0];
				int cr = c[64];
				int rc = crr[cr];
				int gc = ((crg[cr] + cbg[cb]) >> 16);
				int bc = cbb[cb];
				int yy = y[j<<1];
				d0[0] = clamp(yy+rc);
				d0[1] = clamp(yy+gc);
				d0[2] = clamp(yy+bc);
				yy = y[(j<<1)+1];
				d0[4] = clamp(yy+rc);
				d0[5] = clamp(yy+gc);
				d0[6] = clamp(yy+bc);
				d0 += 8;
				c++;
			}
			y += 64;
		}
		y += 64*4 - 64*2;
		c += 64*4 - 8;
	}
}

// YCbCr H2V1 (1x2:1:1, 4 blocks per MCU) to 24-bit RGB
void jpeg_decoder::H1V2Convert(void)
{
	int row = max_mcu_y_size - mcu_lines_left;
	unsigned char *d0 = scan_line_0;
	unsigned char *d1 = scan_line_1;
	unsigned char *y;
	unsigned char *c;
	if (row < 8)
		y = Psample_buf + row * 8;
	else
		y = Psample_buf + 64*1 + (row & 7) * 8;
	c = Psample_buf + 64*2 + (row >> 1) * 8;
	for (int i = max_mcus_per_row; i > 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			int cb = c[0+j];
			int cr = c[64+j];
			int rc = crr[cr];
			int gc = ((crg[cr] + cbg[cb]) >> 16);
			int bc = cbb[cb];
			int yy = y[j];
			d0[0] = clamp(yy+rc);
			d0[1] = clamp(yy+gc);
			d0[2] = clamp(yy+bc);
			yy = y[8+j];
			d1[0] = clamp(yy+rc);
			d1[1] = clamp(yy+gc);
			d1[2] = clamp(yy+bc);
			d0 += 4;
			d1 += 4;
		}
		y += 64*4;
		c += 64*4;
	}
}

// Y (1 block per MCU) to 8-bit greyscale
void jpeg_decoder::GrayConvert(void)
{
	int row = max_mcu_y_size - mcu_lines_left;
	unsigned char *d = scan_line_0;
	unsigned char *s = Psample_buf + row * 8;
	for (int i = max_mcus_per_row; i > 0; i--)
	{
		#if 0
			d[0] = s[0];
			d[1] = s[1];
			d[2] = s[2];
			d[3] = s[3];
			d[4] = s[4];
			d[5] = s[5];
			d[6] = s[6];
			d[7] = s[7];
		#endif
		*(unsigned int *)d = *(unsigned int *)s;
		*(unsigned int *)(&d[4]) = *(unsigned int *)(&s[4]);
		s += 64;
		d += 8;
	}
}

// Find end of image (EOI) marker, so we can return to the user the exact size of the input stream.
void jpeg_decoder::find_eoi(void)
{
	if (!progressive_flag)
	{
		// Attempt to read the EOI marker.
		//get_bits_2(bits_left & 7);

		// Prime the bit buffer
		bits_left = 16;
		//bit_buf = 0;
		get_bits_1(16);
		get_bits_1(16);

		// The next marker _should_ be EOI
		process_markers();
	}
	total_bytes_read -= in_buf_left;
}

// Returns the next scan line.
// Returns JPGD_DONE if all scan lines have been returned.
// Returns JPGD_OKAY if a scan line has been returned.
// Returns JPGD_FAILED if an error occured.
int jpeg_decoder::decode(void **Pscan_line_ofs, unsigned int *Pscan_line_len)
{
	if ((error_code) || (!ready_flag))
		return (JPGD_FAILED);
	if (total_lines_left == 0)
		return (JPGD_DONE);
	if (mcu_lines_left == 0)
	{
		//if (setjmp(jmp_state))
		//	return (JPGD_DECODE_ERROR);
		if (progressive_flag)
			load_next_row();
		else
			decode_next_row();
		if (total_lines_left <= max_mcu_y_size)
			find_eoi();
		transform_row();
		mcu_lines_left = max_mcu_y_size;
	}
	switch (scan_type)
	{
		case JPGD_YH2V2:
		{
			if ((mcu_lines_left & 1) == 0)
			{
				H2V2Convert();
				*Pscan_line_ofs = scan_line_0;
			}
			else
				*Pscan_line_ofs = scan_line_1;
			break;
		}
		case JPGD_YH2V1:
		{
			H2V1Convert();
			*Pscan_line_ofs = scan_line_0;
			break;
		}
		case JPGD_YH1V2:
		{
			if ((mcu_lines_left & 1) == 0)
			{
				H1V2Convert();
				*Pscan_line_ofs = scan_line_0;
			}
			else
				*Pscan_line_ofs = scan_line_1;
			break;
		}
		case JPGD_YH1V1:
		{
			H1V1Convert();
			*Pscan_line_ofs = scan_line_0;
			break;
		}
		case JPGD_GRAYSCALE:
		{
			GrayConvert();
			*Pscan_line_ofs = scan_line_0;
			break;
		}
	}
	*Pscan_line_len = real_dest_bytes_per_scan_line;
	mcu_lines_left--;
	total_lines_left--;
	return (JPGD_OKAY);
}

// Creates the tables needed for efficient Huffman decoding.
void jpeg_decoder::make_huff_table(int index, Phuff_tables_t hs)
{
	int p, i, l, si;
	unsigned char huffsize[257];
	unsigned int huffcode[257];
	unsigned int code;
	unsigned int subtree;
	int code_size;
	int lastp;
	int nextfreeentry;
	int currententry;
	p = 0;
	for (l = 1; l <= 16; l++)
	{
		for (i = 1; i <= huff_num[index][l]; i++)
			huffsize[p++] = l;
	}
	huffsize[p] = 0;
	lastp = p;
	code = 0;
	si = huffsize[0];
	p = 0;
	while (huffsize[p])
	{
		while (huffsize[p] == si)
		{
			huffcode[p++] = code;
			code++;
		}
		code <<= 1;
		si++;
	}
	ClassEMemory::Set(hs->look_up, 0, sizeof(hs->look_up));
	ClassEMemory::Set(hs->tree, 0, sizeof(hs->tree));
	ClassEMemory::Set(hs->code_size, 0, sizeof(hs->code_size));
	nextfreeentry = -1;
	p = 0;
	while (p < lastp)
	{
		i = huff_val[index][p];
		code = huffcode[p];
		code_size = huffsize[p];
		hs->code_size[i] = code_size;
		if (code_size <= 8)
		{
			code <<= (8 - code_size);
			for (l = 1 << (8 - code_size); l > 0; l--)
			{
				hs->look_up[code] = i;
				code++;
			}
		}
		else
		{
			subtree = (code >> (code_size - 8)) & 0xFF;
			currententry = hs->look_up[subtree];
			if (currententry == 0)
			{
				hs->look_up[subtree] = currententry = nextfreeentry;
				nextfreeentry -= 2;
			}
			code <<= (16 - (code_size - 8));
			for (l = code_size; l > 9; l--)
			{
				if ((code & 0x8000) == 0)
					currententry--;
				if (hs->tree[-currententry - 1] == 0)
				{
					hs->tree[-currententry - 1] = nextfreeentry;
					currententry = nextfreeentry;
					nextfreeentry -= 2;
				}
				else
					currententry = hs->tree[-currententry - 1];
				code <<= 1;
			}
			if ((code & 0x8000) == 0)
				currententry--;
			hs->tree[-currententry - 1] = i;
		}
		p++;
	}
}

// Verifies the quantization tables needed for this scan are available.
void jpeg_decoder::check_quant_tables(void)
{
	int i;
	for (i = 0; i < comps_in_scan; i++)
		if (quant[comp_quant[comp_list[i]]] == NULL)
		{
			abortdecoder(JPGD_UNDEFINED_QUANT_TABLE);
			return;
		}
}

// Verifies that all the Huffman tables needed for this scan are available.
void jpeg_decoder::check_huff_tables(void)
{
	int i;
	for (i = 0; i < comps_in_scan; i++)
	{
		if ((spectral_start == 0) && (huff_num[comp_dc_tab[comp_list[i]]] == NULL))
		{
			abortdecoder(JPGD_UNDEFINED_HUFF_TABLE);
			return;
		}
		if ((spectral_end > 0) && (huff_num[comp_ac_tab[comp_list[i]]] == NULL))
		{
			abortdecoder(JPGD_UNDEFINED_HUFF_TABLE);
			return;
		}
	}
	for (i = 0; i < JPGD_MAXHUFFTABLES; i++)
		if (huff_num[i])
		{
			if (!h[i])
				h[i] = (Phuff_tables_t)alloc(sizeof(huff_tables_t));
			make_huff_table(i, h[i]);
		}
	for (i = 0; i < blocks_per_mcu; i++)
	{
		dc_huff_seg[i] = h[comp_dc_tab[mcu_org[i]]];
		ac_huff_seg[i] = h[comp_ac_tab[mcu_org[i]]];
		component[i]   = &last_dc_val[mcu_org[i]];
	}
}

// Determines the component order inside each MCU. Also calcs how many MCU's are on each row, etc.
void jpeg_decoder::calc_mcu_block_order(void)
{
	int component_num, component_id;
	int max_h_samp = 0, max_v_samp = 0;
	for (component_id = 0; component_id < comps_in_frame; component_id++)
	{
		if (comp_h_samp[component_id] > max_h_samp)
			max_h_samp = comp_h_samp[component_id];
		if (comp_v_samp[component_id] > max_v_samp)
			max_v_samp = comp_v_samp[component_id];
	}
	for (component_id = 0; component_id < comps_in_frame; component_id++)
	{
		comp_h_blocks[component_id] = ((((image_x_size * comp_h_samp[component_id]) + (max_h_samp - 1)) / max_h_samp) + 7) / 8;
		comp_v_blocks[component_id] = ((((image_y_size * comp_v_samp[component_id]) + (max_v_samp - 1)) / max_v_samp) + 7) / 8;
	}
	if (comps_in_scan == 1)
	{
		mcus_per_row = comp_h_blocks[comp_list[0]];
		mcus_per_col = comp_v_blocks[comp_list[0]];
	}
	else
	{
		mcus_per_row = (((image_x_size + 7) / 8) + (max_h_samp - 1)) / max_h_samp;
		mcus_per_col = (((image_y_size + 7) / 8) + (max_v_samp - 1)) / max_v_samp;
	}
	if (comps_in_scan == 1)
	{
		mcu_org[0] = comp_list[0];
		blocks_per_mcu = 1;
	}
	else
	{
		blocks_per_mcu = 0;
		for (component_num = 0; component_num < comps_in_scan; component_num++)
		{
			int num_blocks;
			component_id = comp_list[component_num];
			num_blocks = comp_h_samp[component_id] * comp_v_samp[component_id];
			while (num_blocks--)
				mcu_org[blocks_per_mcu++] = component_id;
		}
	}
}

// Starts a new scan.
int jpeg_decoder::init_scan(void)
{
	if (!locate_sos_marker())
		return(JPGD_FALSE);
	calc_mcu_block_order();
	check_huff_tables();
	check_quant_tables();
	ClassEMemory::Set(last_dc_val, 0, comps_in_frame * sizeof(unsigned int));
	eob_run = 0;
	if (restart_interval)
	{
		restarts_left = restart_interval;
		next_restart_num = 0;
	}
	fix_in_buffer();
	return(JPGD_TRUE);
}

// Starts a frame. Determines if the number of components or sampling factors are supported.
void jpeg_decoder::init_frame(void)
{
	int i;
	unsigned char *q;
	if (comps_in_frame == 1)
	{
		scan_type          = JPGD_GRAYSCALE;
		max_blocks_per_mcu = 1;
		max_mcu_x_size     = 8;
		max_mcu_y_size     = 8;
	}
	else if (comps_in_frame == 3)
	{
		if ( ((comp_h_samp[1] != 1) || (comp_v_samp[1] != 1)) || ((comp_h_samp[2] != 1) || (comp_v_samp[2] != 1)) )
		{
			abortdecoder(JPGD_UNSUPPORTED_SAMP_FACTORS);
			return;
		}
		if ((comp_h_samp[0] == 1) && (comp_v_samp[0] == 1))
		{
			scan_type          = JPGD_YH1V1;
			max_blocks_per_mcu = 3;
			max_mcu_x_size     = 8;
			max_mcu_y_size     = 8;
		}
		else if ((comp_h_samp[0] == 2) && (comp_v_samp[0] == 1))
		{
			scan_type          = JPGD_YH2V1;
			max_blocks_per_mcu = 4;
			max_mcu_x_size     = 16;
			max_mcu_y_size     = 8;
		}
		else if ((comp_h_samp[0] == 1) && (comp_v_samp[0] == 2))
		{
			scan_type          = JPGD_YH1V2;
			max_blocks_per_mcu = 4;
			max_mcu_x_size     = 8;
			max_mcu_y_size     = 16;
		}
		else if ((comp_h_samp[0] == 2) && (comp_v_samp[0] == 2))
		{
			scan_type          = JPGD_YH2V2;
			max_blocks_per_mcu = 6;
			max_mcu_x_size     = 16;
			max_mcu_y_size     = 16;
		}
		else
		{
			abortdecoder(JPGD_UNSUPPORTED_SAMP_FACTORS);
			return;
		}
	}
	else
	{
		abortdecoder(JPGD_UNSUPPORTED_COLORSPACE);
		return;
	}
	max_mcus_per_row = (image_x_size + (max_mcu_x_size - 1)) / max_mcu_x_size;
	max_mcus_per_col = (image_y_size + (max_mcu_y_size - 1)) / max_mcu_y_size;

	// these values are for the *destination* pixels: after conversion
	if (scan_type == JPGD_GRAYSCALE)
		dest_bytes_per_pixel = 1;
	else
		dest_bytes_per_pixel = 4;

	dest_bytes_per_scan_line = ((image_x_size + 15) & 0xFFF0) * dest_bytes_per_pixel;
	real_dest_bytes_per_scan_line = (image_x_size * dest_bytes_per_pixel);

	// Initialize two scan line buffers.
	// FIXME: Only the V2 sampling factors need two buffers.
	scan_line_0         = (unsigned char *)alloc(dest_bytes_per_scan_line + 8);
	ClassEMemory::Set(scan_line_0, 0, dest_bytes_per_scan_line);
	scan_line_1         = (unsigned char *)alloc(dest_bytes_per_scan_line + 8);
	ClassEMemory::Set(scan_line_1, 0, dest_bytes_per_scan_line);
	max_blocks_per_row = max_mcus_per_row * max_blocks_per_mcu;

	// Should never happen
	if (max_blocks_per_row > JPGD_MAXBLOCKSPERROW)
	{
		abortdecoder(JPGD_ASSERTION_ERROR);
		return;
	}

	// Allocate the coefficient buffer, enough for one row's worth of MCU's
	q = (unsigned char *)alloc(max_blocks_per_row * 64 * sizeof(BLOCK_TYPE) + 8);

	// Align to 8-byte boundry, for MMX code
	q = (unsigned char *)(((unsigned long)q + 7) & ~7);

	// The block_seg[] array's name dates back to the
	// 16-bit assembler implementation. "seg" stood for "segment".
	for (i = 0; i < max_blocks_per_row; i++)
		block_seg[i] = (BLOCK_TYPE *)(q + i * 64 * sizeof(BLOCK_TYPE));
	
	for (i = 0; i < max_blocks_per_row; i++)
		block_max_zag_set[i] = 64;
	Psample_buf = (unsigned char *)(((unsigned long)alloc(max_blocks_per_row * 64 + 8) + 7) & ~7);
	total_lines_left = image_y_size;
	mcu_lines_left = 0;
	create_look_ups();
}

// Decode a scan in a progressively encoded image.
void jpeg_decoder::decode_scan(Pdecode_block_func decode_block_func)
{
	int mcu_row, mcu_col, mcu_block;
	int block_x_mcu[JPGD_MAXCOMPONENTS], block_y_mcu[JPGD_MAXCOMPONENTS];
	ClassEMemory::Set(block_y_mcu, 0, sizeof(block_y_mcu));
	for (mcu_col = 0; mcu_col < mcus_per_col; mcu_col++)
	{
		int component_num, component_id;
		ClassEMemory::Set(block_x_mcu, 0, sizeof(block_x_mcu));
		for (mcu_row = 0; mcu_row < mcus_per_row; mcu_row++)
		{
			int block_x_mcu_ofs = 0, block_y_mcu_ofs = 0;
			if ((restart_interval) && (restarts_left == 0))
				process_restart();
			for (mcu_block = 0; mcu_block < blocks_per_mcu; mcu_block++)
			{
				component_id = mcu_org[mcu_block];
				decode_block_func(this, component_id, block_x_mcu[component_id] + block_x_mcu_ofs, block_y_mcu[component_id] + block_y_mcu_ofs);
				if (comps_in_scan == 1)
					block_x_mcu[component_id]++;
				else
				{
					if (++block_x_mcu_ofs == comp_h_samp[component_id])
					{
						block_x_mcu_ofs = 0;
						if (++block_y_mcu_ofs == comp_v_samp[component_id])
						{
							block_y_mcu_ofs = 0;
							block_x_mcu[component_id] += comp_h_samp[component_id];
						}
					}
				}
			}
			restarts_left--;
		}
		if (comps_in_scan == 1)
			block_y_mcu[comp_list[0]]++;
		else
		{
			for (component_num = 0; component_num < comps_in_scan; component_num++)
			{
				component_id = comp_list[component_num];
				block_y_mcu[component_id] += comp_v_samp[component_id];
			}
		}
	}
}

// Decode a progressively encoded image.
void jpeg_decoder::init_progressive(void)
{
	int i;
	if (comps_in_frame == 4)
	{
		abortdecoder(JPGD_UNSUPPORTED_COLORSPACE);
		return;
	}

	// Allocate the coefficient buffers.
	for (i = 0; i < comps_in_frame; i++)
	{
		dc_coeffs[i] = coeff_buf_open(max_mcus_per_row * comp_h_samp[i], max_mcus_per_col * comp_v_samp[i], 1, 1);
		ac_coeffs[i] = coeff_buf_open(max_mcus_per_row * comp_h_samp[i], max_mcus_per_col * comp_v_samp[i], 8, 8);
	}
	for ( ; ; )
	{
		int dc_only_scan, refinement_scan;
		Pdecode_block_func decode_block_func;
		if (!init_scan())
			break;
		dc_only_scan    = (spectral_start == 0);
		refinement_scan = (successive_high != 0);
		if ((spectral_start > spectral_end) || (spectral_end > 63))
		{
			abortdecoder(JPGD_BAD_SOS_SPECTRAL);
			return;
		}
		if (dc_only_scan)
		{
			if (spectral_end)
			{
				abortdecoder(JPGD_BAD_SOS_SPECTRAL);
				return;
			}
		}
		else if (comps_in_scan != 1)  /* AC scans can only contain one component */
		{
			abortdecoder(JPGD_BAD_SOS_SPECTRAL);
			return;
		}
		if ((refinement_scan) && (successive_low != successive_high - 1))
		{
			abortdecoder(JPGD_BAD_SOS_SUCCESSIVE);
			return;
		}
		if (dc_only_scan)
		{
			if (refinement_scan)
				decode_block_func = progressive_block_decoder::decode_block_dc_refine;
			else
				decode_block_func = progressive_block_decoder::decode_block_dc_first;
		}
		else
		{
			if (refinement_scan)
				decode_block_func = progressive_block_decoder::decode_block_ac_refine;
			else
				decode_block_func = progressive_block_decoder::decode_block_ac_first;
		}
		decode_scan(decode_block_func);
		//get_bits_2(bits_left & 7);
		bits_left = 16;
		//bit_buf = 0;
		get_bits_1(16);
		get_bits_1(16);
	}
	comps_in_scan = comps_in_frame;
	for (i = 0; i < comps_in_frame; i++)
		comp_list[i] = i;
	calc_mcu_block_order();
}

void jpeg_decoder::init_sequential(void)
{
	if (!init_scan())
		abortdecoder(JPGD_UNEXPECTED_MARKER);
}

void jpeg_decoder::decode_start(void)
{
	init_frame();
	if (progressive_flag)
		init_progressive();
	else
		init_sequential();
}

// Find the start of the JPEG file and reads enough data to determine its size, number of components, etc.
void jpeg_decoder::decode_init(void *ldata, unsigned long lsize)
{
	init(ldata, lsize);
	locate_sof_marker();
}

// Call get_error_code() after constructing to determine if the stream was valid or not.
// You may call the get_width(), get_height(), etc. methods after the constructor is called.
// You may then either destruct the object, or begin decoding the image by calling begin(), then decode().
jpeg_decoder::jpeg_decoder(void *ldata, unsigned long lsize)
{
	//if (setjmp(jmp_state))
	//	return;
	decode_init(ldata, lsize);
}

// Completely destroys the decoder object. May be called at any time.
jpeg_decoder::~jpeg_decoder()
{
	free_all_blocks();
}

// If you wish to decompress the image, call this method after constructing the object.
// If JPGD_OKAY is returned you may then call decode() to fetch the scan lines.
int jpeg_decoder::begin(void)
{
	if (ready_flag)
		return (JPGD_OKAY);
	if (error_code)
		return (JPGD_FAILED);
	//if (setjmp(jmp_state))
	//	return (JPGD_FAILED);
	decode_start();
	ready_flag = true;
	return (JPGD_OKAY);
}

// YCbCr H2V2 (2x2:1:1, 6 blocks per MCU) to 24-bit RGB
// This case is very popular, so it's important that it's fast.
// If this module is compiled with the Intel Compiler the faster
// MMX specific version will also be available.
// FIXME: Create all-asm version, so Intel Compiler isn't needed.
void jpeg_decoder::H2V2Convert(void)
{
	int row = max_mcu_y_size - mcu_lines_left;
	unsigned char *d0 = scan_line_0;
	unsigned char *d1 = scan_line_1;
	unsigned char *y;
	unsigned char *c;
	if (row < 8)
		y = Psample_buf + row * 8;
	else
		y = Psample_buf + 64*2 + (row & 7) * 8;
	c = Psample_buf + 64*4 + (row >> 1) * 8;
	for (int i = max_mcus_per_row; i > 0; i--)
	{
		for (int l = 0; l < 2; l++)
		{
			for (int j = 0; j < 8; j += 2)
			{
				int cb = c[0];
				int cr = c[64];
				int rc = crr[cr];
				int gc = ((crg[cr] + cbg[cb]) >> 16);
				int bc = cbb[cb];
				int yy = y[j];
				d0[0] = clamp(yy+rc);
				d0[1] = clamp(yy+gc);
				d0[2] = clamp(yy+bc);
				yy = y[j+1];
				d0[4] = clamp(yy+rc);
				d0[5] = clamp(yy+gc);
				d0[6] = clamp(yy+bc);
				yy = y[j+8];
				d1[0] = clamp(yy+rc);
				d1[1] = clamp(yy+gc);
				d1[2] = clamp(yy+bc);
				yy = y[j+8+1];
				d1[4] = clamp(yy+rc);
				d1[5] = clamp(yy+gc);
				d1[6] = clamp(yy+bc);
				d0 += 8;
				d1 += 8;
				c++;
			}
			y += 64;
		}
		y += 64*6 - 64*2;
		c += 64*6 - 8;
	}
}

// Logical rotate left operation.
inline unsigned int jpeg_decoder::rol(unsigned int i, unsigned char j)
{
	return ((i << j) | (i >> (32 - j)));
}

// Retrieve one character from the input stream.
inline unsigned int jpeg_decoder::get_char(void)
{
	// Any bytes remaining in buffer?
	if (!in_buf_left)
	{
		// Try to get more bytes.
		prep_in_buffer();
		// Still nothing to get?
		if (!in_buf_left)
		{
			// Padd the end of the stream with 0xFF 0xD9 (EOI marker)
			// FIXME: Is there a better padding pattern to use?
			int t = tem_flag;
			tem_flag ^= 1;
			if (t)
				return (0xD9);
			else
				return (0xFF);
		}
	}
	unsigned int c = *Pin_buf_ofs++;
	in_buf_left--;
	return(c);
}

// Same as previus method, except can indicate if the character is a "padd" character or not.
inline unsigned int jpeg_decoder::get_char(bool *Ppadding_flag)
{
	if (!in_buf_left)
	{
		prep_in_buffer();
		if (!in_buf_left)
		{
			*Ppadding_flag = true;
			int t = tem_flag;
			tem_flag ^= 1;
			if (t)
				return (0xD9);
			else
				return (0xFF);
		}
	}
	*Ppadding_flag = false;
	unsigned int c = *Pin_buf_ofs++;
	in_buf_left--;
	return(c);
}

// Inserts a previously retrieved character back into the input buffer.
inline void jpeg_decoder::stuff_char(unsigned char q)
{
	*(--Pin_buf_ofs) = q;
	in_buf_left++;
}

// Retrieves one character from the input stream, but does not read past markers. Will continue to return 0xFF when a marker is encountered.
// FIXME: Bad name?
inline unsigned char jpeg_decoder::get_octet(void)
{
	bool padding_flag;
	int c = get_char(&padding_flag);
	if (c == 0xFF)
	{
		if (padding_flag)
			return (0xFF);
		c = get_char(&padding_flag);
		if (padding_flag)
		{
			stuff_char(0xFF);
			return (0xFF);
		}
		if (c == 0x00)
			return (0xFF);
		else
		{
			stuff_char(c);
			stuff_char(0xFF);
			return (0xFF);
		}
	}
	return(c);
}

// Retrieves a variable number of bits from the input stream. Does not recognize markers.
inline unsigned int jpeg_decoder::get_bits_1(int num_bits)
{
	unsigned int i;
	i = (bit_buf >> (16 - num_bits)) & ((1 << num_bits) - 1);
	if ((bits_left -= num_bits) <= 0)
	{
		bit_buf = rol(bit_buf, num_bits += bits_left);
		unsigned int c1 = get_char();
		unsigned int c2 = get_char();
		bit_buf = (bit_buf & 0xFFFF) | (((unsigned long)c1) << 24) | (((unsigned long)c2) << 16);
		bit_buf = rol(bit_buf, -bits_left);
		bits_left += 16;
	}
	else
		bit_buf = rol(bit_buf, num_bits);
	return(i);
}

// Retrieves a variable number of bits from the input stream. Markers will not be read into the input bit buffer.
// Instead, an infinite number of all 1's will be returned when a marker is encountered.
// FIXME: Is it better to return all 0's instead, like the older implementation?
inline unsigned int jpeg_decoder::get_bits_2(int numbits)
{
	unsigned int i;
	i = (bit_buf >> (16 - numbits)) & ((1 << numbits) - 1);
	if ((bits_left -= numbits) <= 0)
	{
		bit_buf = rol(bit_buf, numbits += bits_left);
		unsigned int c1 = get_octet();
		unsigned int c2 = get_octet();
		bit_buf = (bit_buf & 0xFFFF) | (((unsigned long)c1) << 24) | (((unsigned long)c2) << 16);
		bit_buf = rol(bit_buf, -bits_left);
		bits_left += 16;
	}
	else
		bit_buf = rol(bit_buf, numbits);
	return(i);
}

// Decodes a Huffman encoded symbol.
inline int jpeg_decoder::huff_decode(Phuff_tables_t Ph)
{
	int symbol;

	// Check first 8-bits: do we have a complete symbol?
	if ((symbol = Ph->look_up[(bit_buf >> 8) & 0xFF]) < 0)
	{
		// Decode more bits, use a tree traversal to find symbol.
		get_bits_2(8);
		do
		{
			symbol = Ph->tree[~symbol + (1 - get_bits_2(1))];
		} while (symbol < 0);
	}
	else
		get_bits_2(Ph->code_size[symbol]);
	return(symbol);
}

// Clamps a value between 0-255.
inline unsigned char jpeg_decoder::clamp(int i)
{
	if (i & 0xFFFFFF00)
		i = (((~i) >> 31) & 0xFF);
	return(i);
}

///////////////////////////////////////////////////////////////////
// Loader: Public                                                //
///////////////////////////////////////////////////////////////////

//Start the loader
ERESULT ELoader_JPG::Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags)
{
	#if (EDITION_SDK < 1000)
		return(E_NOFEATURE);
	#else
		jpeg_decoder *decodelib = new jpeg_decoder(ldata, lsize);
		if (decodelib != NULL)
		{
			ERESULT result;
			if (decodelib->get_error_code() != 0)
			{
				delete decodelib;
				return(E_UNSUPPORTED);
			}
			ELDR_2DCALLBACKINFO cbinfo;
			cbinfo.nativedisplaymode = nativedisplaymode;
			cbinfo.createflags = createflags;
			cbinfo.paletteindex = 0;
			cbinfo.pixelindex = 0;
			cbinfo.usepalette = false;
			cbinfo.usetransparency = false;
			cbinfo.compressedformat = NULL;
			cbinfo.width = decodelib->get_width();
			cbinfo.height = decodelib->get_height();
			result = surface->LdrOnCreate(&cbinfo);
			if (result != E_OK)
			{
				delete decodelib;
				return(result);
			}
			if (decodelib->begin())
			{
				delete decodelib;
				return (E_ERROR);
			}
			unsigned char *Pbuf = NULL;
			Pbuf = (unsigned char *)ClassEMemory::Alloc(decodelib->get_width() * 4);
			if (!Pbuf)
			{
				delete decodelib;
				return(E_NOMEMORY);
			}
			int lines_decoded = 0;
			for ( ; ; )
			{
				void *Pscan_line_ofs;
				unsigned int scan_line_len;
				if (decodelib->decode(&Pscan_line_ofs, &scan_line_len))
					break;
				lines_decoded++;
				result = E_OK;
				if (decodelib->get_num_components() == 3)
				{
					unsigned char *Psb = (unsigned char *)Pscan_line_ofs;
					unsigned long *Pdb = (unsigned long *)Pbuf;
					int src_bpp = decodelib->get_bytes_per_pixel();
					for (int x = decodelib->get_width(); x > 0; x--, Psb += src_bpp, Pdb++)
						Pdb[0] = E_RGBA(Psb[0], Psb[1], Psb[2], 255);
					cbinfo.streamdata = Pbuf;
					cbinfo.streamsize = cbinfo.width * 4;
					result = surface->LdrOnPixelStream(&cbinfo);
				}
				else
				{
					unsigned char *Psb = (unsigned char *)Pscan_line_ofs;
					unsigned long *Pdb = (unsigned long *)Pbuf;
					int src_bpp = decodelib->get_bytes_per_pixel();
					for (int x = decodelib->get_width(); x > 0; x--, Psb += src_bpp, Pdb++)
						Pdb[0] = E_RGBA(Psb[0], Psb[0], Psb[0], 255);
					cbinfo.streamdata = Pbuf;
					cbinfo.streamsize = cbinfo.width * 4;
					result = surface->LdrOnPixelStream(&cbinfo);
				}
				if (result != E_OK)
				{
					ClassEMemory::DeAlloc(Pbuf);
					delete decodelib;
					return(E_ERROR);
				}
			}
			if (Pbuf != NULL)
				ClassEMemory::DeAlloc(Pbuf);
			delete decodelib;
			return(E_OK);
		}
		else
			return(E_NOMEMORY);
		return(E_UNSUPPORTED);
	#endif
}
