// license:BSD-3-Clause
// copyright-holders:Aaron Giles


//missing defines temp fix - @mypinballs
#define ADSP2101_IRQ0		0		/* IRQ0 */
#define ADSP2101_SPORT1_RX	0		/* SPORT1 receive IRQ */
#define ADSP2101_IRQ1		1		/* IRQ1 */
#define ADSP2101_SPORT1_TX	1		/* SPORT1 transmit IRQ */
#define ADSP2101_IRQ2		2		/* IRQ2 */
#define ADSP2101_SPORT0_RX	3		/* SPORT0 receive IRQ */
#define ADSP2101_SPORT0_TX	4		/* SPORT0 transmit IRQ */
#define ADSP2105_IRQ0		0		/* IRQ0 */
#define ADSP2105_SPORT1_RX	0		/* SPORT1 receive IRQ */
#define ADSP2105_IRQ1		1		/* IRQ1 */
#define ADSP2105_SPORT1_TX	1		/* SPORT1 transmit IRQ */
#define ADSP2105_IRQ2		2		/* IRQ2 */



/*===========================================================================
    ASTAT -- ALU/MAC status register
===========================================================================*/

/* flag definitions */
#define SSFLAG			0x80
#define MVFLAG			0x40
#define QFLAG			0x20
#define SFLAG			0x10
#define CFLAG			0x08
#define VFLAG			0x04
#define NFLAG			0x02
#define ZFLAG			0x01

/* extracts flags */
#define GET_SS			(adsp2100.astat & SSFLAG)
#define GET_MV			(adsp2100.astat & MVFLAG)
#define GET_Q			(adsp2100.astat &  QFLAG)
#define GET_S			(adsp2100.astat &  SFLAG)
#define GET_C			(adsp2100.astat &  CFLAG)
#define GET_V			(adsp2100.astat &  VFLAG)
#define GET_N			(adsp2100.astat &  NFLAG)
#define GET_Z			(adsp2100.astat &  ZFLAG)

/* clears flags */
#define CLR_SS			(adsp2100.astat &= ~SSFLAG)
#define CLR_MV			(adsp2100.astat &= ~MVFLAG)
#define CLR_Q			(adsp2100.astat &=  ~QFLAG)
#define CLR_S			(adsp2100.astat &=  ~SFLAG)
#define CLR_C			(adsp2100.astat &=  ~CFLAG)
#define CLR_V			(adsp2100.astat &=  ~VFLAG)
#define CLR_N			(adsp2100.astat &=  ~NFLAG)
#define CLR_Z			(adsp2100.astat &=  ~ZFLAG)

/* sets flags */
#define SET_SS			(adsp2100.astat |= SSFLAG)
#define SET_MV			(adsp2100.astat |= MVFLAG)
#define SET_Q			(adsp2100.astat |=  QFLAG)
#define SET_S			(adsp2100.astat |=  SFLAG)
#define SET_C			(adsp2100.astat |=  CFLAG)
#define SET_V			(adsp2100.astat |=  VFLAG)
#define SET_Z			(adsp2100.astat |=  ZFLAG)
#define SET_N			(adsp2100.astat |=  NFLAG)

/* flag clearing; must be done before setting */
#define CLR_FLAGS		(adsp2100.astat &= adsp2100.astat_clear)

/* compute flags */
#define CALC_Z(r)		(adsp2100.astat |= (((r) & 0xffff) == 0))
#define CALC_N(r)		(adsp2100.astat |= ((r) >> 14) & 0x02)
#define CALC_V(s,d,r)	(adsp2100.astat |= (((s) ^ (d) ^ (r) ^ ((r) >> 1)) >> 13) & 0x04)
#define CALC_C(r)		(adsp2100.astat |= ((r) >> 13) & 0x08)
#define CALC_C_SUB(r)	(adsp2100.astat |= (~(r) >> 13) & 0x08)
#define CALC_NZ(r) 		CLR_FLAGS; CALC_N(r); CALC_Z(r)
#define CALC_NZV(s,d,r) CLR_FLAGS; CALC_N(r); CALC_Z(r); CALC_V(s,d,r)
#define CALC_NZVC(s,d,r) CLR_FLAGS; CALC_N(r); CALC_Z(r); CALC_V(s,d,r); CALC_C(r)
#define CALC_NZVC_SUB(s,d,r) CLR_FLAGS; CALC_N(r); CALC_Z(r); CALC_V(s,d,r); CALC_C_SUB(r)

/* ADSP-218x constants */
static const int32_t constants[] =
{
	0x0001, 0xfffe, 0x0002, 0xfffd, 0x0004, 0xfffb, 0x0008, 0xfff7,
	0x0010, 0xffef, 0x0020, 0xffdf, 0x0040, 0xffbf, 0x0080, 0xff7f,
	0x0100, 0xfeff, 0x0200, 0xfdff, 0x0400, 0xfbff, 0x0800, 0xf7ff,
	0x1000, 0xefff, 0x2000, 0xdfff, 0x4000, 0xbfff, 0x8000, 0x7fff
};



/*===========================================================================
    MSTAT -- ALU/MAC control register
===========================================================================*/

/* flag definitions */
#define MSTAT_BANK      0x01            /* register bank select */
#define MSTAT_REVERSE   0x02            /* bit-reverse addressing enable (DAG1) */
#define MSTAT_STICKYV   0x04            /* sticky ALU overflow enable */
#define MSTAT_SATURATE  0x08            /* AR saturation mode enable */
#define MSTAT_INTEGER   0x10            /* MAC result placement; 0=fractional, 1=integer */
#define MSTAT_TIMER     0x20            /* timer enable */
#define MSTAT_GOMODE    0x40            /* go mode enable */

/* you must call this in order to change MSTAT */
inline void set_mstat(int new_value)
{
	if ((new_value ^ adsp2100.mstat) & MSTAT_BANK)
	{
		ADSPCORE temp = adsp2100.core;
		adsp2100.core = adsp2100.alt;
		adsp2100.alt = temp;
	}
	if (new_value & MSTAT_STICKYV)
		adsp2100.astat_clear = ~(CFLAG | NFLAG | ZFLAG);
	else
		adsp2100.astat_clear = ~(CFLAG | VFLAG | NFLAG | ZFLAG);
	adsp2100.mstat = new_value;
}


/*===========================================================================
    SSTAT -- stack status register
===========================================================================*/

/* flag definitions */
#define PC_EMPTY        0x01            /* PC stack empty */
#define PC_OVER         0x02            /* PC stack overflow */
#define COUNT_EMPTY     0x04            /* count stack empty */
#define COUNT_OVER      0x08            /* count stack overflow */
#define STATUS_EMPTY    0x10            /* status stack empty */
#define STATUS_OVER     0x20            /* status stack overflow */
#define LOOP_EMPTY      0x40            /* loop stack empty */
#define LOOP_OVER       0x80            /* loop stack overflow */



/*===========================================================================
    PC stack handlers
===========================================================================*/

inline uint32_t pc_stack_top(void)
{
	if (adsp2100.pc_sp > 0)
		return adsp2100.pc_stack[adsp2100.pc_sp - 1];
	else
		return adsp2100.pc_stack[0];
}

inline void set_pc_stack_top(uint32_t top)
{
	if (adsp2100.pc_sp > 0)
		adsp2100.pc_stack[adsp2100.pc_sp - 1] = top;
	else
		adsp2100.pc_stack[0] = top;
}

inline void pc_stack_push(void)
{
	if (adsp2100.pc_sp < ADSP2100_PC_STACK_DEPTH)
	{
		adsp2100.pc_stack[adsp2100.pc_sp] = adsp2100.pc;
		adsp2100.pc_sp++;
		adsp2100.sstat &= ~PC_EMPTY;
	}
	else
		adsp2100.sstat |= PC_OVER;
}

inline void pc_stack_push_val(uint32_t val)
{
	if (adsp2100.pc_sp < ADSP2100_PC_STACK_DEPTH)
	{
		adsp2100.pc_stack[adsp2100.pc_sp] = val;
		adsp2100.pc_sp++;
		adsp2100.sstat &= ~PC_EMPTY;
	}
	else
		adsp2100.sstat |= PC_OVER;
}

inline void pc_stack_pop(void)
{
	if (adsp2100.pc_sp > 0)
	{
		adsp2100.pc_sp--;
		if (adsp2100.pc_sp == 0)
			adsp2100.sstat |= PC_EMPTY;
	}
	adsp2100.pc = adsp2100.pc_stack[adsp2100.pc_sp];
}

inline uint32_t pc_stack_pop_val(void)
{
	if (adsp2100.pc_sp > 0)
	{
		adsp2100.pc_sp--;
		if (adsp2100.pc_sp == 0)
			adsp2100.sstat |= PC_EMPTY;
	}
	return adsp2100.pc_stack[adsp2100.pc_sp];
}


/*===========================================================================
    CNTR stack handlers
===========================================================================*/

inline uint32_t cntr_stack_top(void)
{
	if (adsp2100.cntr_sp > 0)
		return adsp2100.cntr_stack[adsp2100.cntr_sp - 1];
	else
		return adsp2100.cntr_stack[0];
}

inline void cntr_stack_push(void)
{
	if (adsp2100.cntr_sp < ADSP2100_CNTR_STACK_DEPTH)
	{
		adsp2100.cntr_stack[adsp2100.cntr_sp] = adsp2100.cntr;
		adsp2100.cntr_sp++;
		adsp2100.sstat &= ~COUNT_EMPTY;
	}
	else
		adsp2100.sstat |= COUNT_OVER;
}

inline void cntr_stack_pop(void)
{
	if (adsp2100.cntr_sp > 0)
	{
		adsp2100.cntr_sp--;
		if (adsp2100.cntr_sp == 0)
			adsp2100.sstat |= COUNT_EMPTY;
	}
	adsp2100.cntr = adsp2100.cntr_stack[adsp2100.cntr_sp];
}


/*===========================================================================
    LOOP stack handlers
===========================================================================*/

inline uint32_t loop_stack_top(void)
{
	if (adsp2100.loop_sp > 0)
		return adsp2100.loop_stack[adsp2100.loop_sp - 1];
	else
		return adsp2100.loop_stack[0];
}

inline void loop_stack_push(uint32_t value)
{
	if (adsp2100.loop_sp < ADSP2100_LOOP_STACK_DEPTH)
	{
		adsp2100.loop_stack[adsp2100.loop_sp] = value;
		adsp2100.loop_sp++;
		adsp2100.loop = value >> 4;
		adsp2100.loop_condition = value & 15;
		adsp2100.sstat &= ~LOOP_EMPTY;
	}
	else
		adsp2100.sstat |= LOOP_OVER;
}

inline void loop_stack_pop(void)
{
	if (adsp2100.loop_sp > 0)
	{
		adsp2100.loop_sp--;
		if (adsp2100.loop_sp == 0)
		{
			adsp2100.loop = 0xffff;
			adsp2100.loop_condition = 0;
			adsp2100.sstat |= LOOP_EMPTY;
		}
		else
		{
			adsp2100.loop = adsp2100.loop_stack[adsp2100.loop_sp -1] >> 4;
			adsp2100.loop_condition = adsp2100.loop_stack[adsp2100.loop_sp - 1] & 15;
		}
	}
}


/*===========================================================================
    STAT stack handlers
===========================================================================*/

inline void stat_stack_push(void)
{
	if (adsp2100.stat_sp < ADSP2100_STAT_STACK_DEPTH)
	{
		adsp2100.stat_stack[adsp2100.stat_sp][0] = adsp2100.mstat;
		adsp2100.stat_stack[adsp2100.stat_sp][1] = adsp2100.imask;
		adsp2100.stat_stack[adsp2100.stat_sp][2] = adsp2100.astat;
		adsp2100.stat_sp++;
		adsp2100.sstat &= ~STATUS_EMPTY;
	}
	else
		adsp2100.sstat |= STATUS_OVER;
}

inline void stat_stack_pop(void)
{
	if (adsp2100.stat_sp > 0)
	{
		adsp2100.stat_sp--;
		if (adsp2100.stat_sp == 0)
			adsp2100.sstat |= STATUS_EMPTY;
	}
	set_mstat(adsp2100.stat_stack[adsp2100.stat_sp][0]);
	adsp2100.imask = adsp2100.stat_stack[adsp2100.stat_sp][1];
	adsp2100.astat = adsp2100.stat_stack[adsp2100.stat_sp][2];
	check_irqs();
}



/*===========================================================================
    condition code checking
===========================================================================*/

inline int CONDITION(int c)
{
	if (c != 14)
		return condition_table[((c) << 8) | adsp2100.astat];
	else if ((int32_t)--adsp2100.cntr > 0)
		return 1;
	else
	{
		cntr_stack_pop();
		return 0;
	}
}



/*===========================================================================
    register writing
===========================================================================*/

static void wr_inval(int32_t val) { /* logerror("ADSP %04x: Writing to an invalid register!", adsp2100.ppc); */ }
static void wr_ax0(int32_t val)   { adsp2100.core.ax0.s = val; }
static void wr_ax1(int32_t val)   { adsp2100.core.ax1.s = val; }
static void wr_mx0(int32_t val)   { adsp2100.core.mx0.s = val; }
static void wr_mx1(int32_t val)   { adsp2100.core.mx1.s = val; }
static void wr_ay0(int32_t val)   { adsp2100.core.ay0.s = val; }
static void wr_ay1(int32_t val)   { adsp2100.core.ay1.s = val; }
static void wr_my0(int32_t val)   { adsp2100.core.my0.s = val; }
static void wr_my1(int32_t val)   { adsp2100.core.my1.s = val; }
static void wr_si(int32_t val)    { adsp2100.core.si.s = val; }
static void wr_se(int32_t val)    { adsp2100.core.se.s = (int8_t)val; }
static void wr_ar(int32_t val)    { adsp2100.core.ar.s = val; }
static void wr_mr0(int32_t val)   { adsp2100.core.mr.mrx.mr0.s = val; }
static void wr_mr1(int32_t val)   { adsp2100.core.mr.mrx.mr1.s = val; adsp2100.core.mr.mrx.mr2.s = (int16_t)val >> 15; }
static void wr_mr2(int32_t val)   { adsp2100.core.mr.mrx.mr2.s = (int8_t)val; }
static void wr_sr0(int32_t val)   { adsp2100.core.sr.srx.sr0.s = val; }
static void wr_sr1(int32_t val)   { adsp2100.core.sr.srx.sr1.s = val; }
static void wr_i0(int32_t val)    { adsp2100.i[0] = val & 0x3fff; adsp2100.base[0] = val & adsp2100.lmask[0]; }
static void wr_i1(int32_t val)    { adsp2100.i[1] = val & 0x3fff; adsp2100.base[1] = val & adsp2100.lmask[1]; }
static void wr_i2(int32_t val)    { adsp2100.i[2] = val & 0x3fff; adsp2100.base[2] = val & adsp2100.lmask[2]; }
static void wr_i3(int32_t val)    { adsp2100.i[3] = val & 0x3fff; adsp2100.base[3] = val & adsp2100.lmask[3]; }
static void wr_i4(int32_t val)    { adsp2100.i[4] = val & 0x3fff; adsp2100.base[4] = val & adsp2100.lmask[4]; }
static void wr_i5(int32_t val)    { adsp2100.i[5] = val & 0x3fff; adsp2100.base[5] = val & adsp2100.lmask[5]; }
static void wr_i6(int32_t val)    { adsp2100.i[6] = val & 0x3fff; adsp2100.base[6] = val & adsp2100.lmask[6]; }
static void wr_i7(int32_t val)    { adsp2100.i[7] = val & 0x3fff; adsp2100.base[7] = val & adsp2100.lmask[7]; }
static void wr_m0(int32_t val)    { adsp2100.m[0] = (int32_t)(val << 18) >> 18; }
static void wr_m1(int32_t val)    { adsp2100.m[1] = (int32_t)(val << 18) >> 18; }
static void wr_m2(int32_t val)    { adsp2100.m[2] = (int32_t)(val << 18) >> 18; }
static void wr_m3(int32_t val)    { adsp2100.m[3] = (int32_t)(val << 18) >> 18; }
static void wr_m4(int32_t val)    { adsp2100.m[4] = (int32_t)(val << 18) >> 18; }
static void wr_m5(int32_t val)    { adsp2100.m[5] = (int32_t)(val << 18) >> 18; }
static void wr_m6(int32_t val)    { adsp2100.m[6] = (int32_t)(val << 18) >> 18; }
static void wr_m7(int32_t val)    { adsp2100.m[7] = (int32_t)(val << 18) >> 18; }
static void wr_l0(int32_t val)    { adsp2100.l[0] = val & 0x3fff; adsp2100.lmask[0] = mask_table[val & 0x3fff]; adsp2100.base[0] = adsp2100.i[0] & adsp2100.lmask[0]; }
static void wr_l1(int32_t val)    { adsp2100.l[1] = val & 0x3fff; adsp2100.lmask[1] = mask_table[val & 0x3fff]; adsp2100.base[1] = adsp2100.i[1] & adsp2100.lmask[1]; }
static void wr_l2(int32_t val)    { adsp2100.l[2] = val & 0x3fff; adsp2100.lmask[2] = mask_table[val & 0x3fff]; adsp2100.base[2] = adsp2100.i[2] & adsp2100.lmask[2]; }
static void wr_l3(int32_t val)    { adsp2100.l[3] = val & 0x3fff; adsp2100.lmask[3] = mask_table[val & 0x3fff]; adsp2100.base[3] = adsp2100.i[3] & adsp2100.lmask[3]; }
static void wr_l4(int32_t val)    { adsp2100.l[4] = val & 0x3fff; adsp2100.lmask[4] = mask_table[val & 0x3fff]; adsp2100.base[4] = adsp2100.i[4] & adsp2100.lmask[4]; }
static void wr_l5(int32_t val)    { adsp2100.l[5] = val & 0x3fff; adsp2100.lmask[5] = mask_table[val & 0x3fff]; adsp2100.base[5] = adsp2100.i[5] & adsp2100.lmask[5]; }
static void wr_l6(int32_t val)    { adsp2100.l[6] = val & 0x3fff; adsp2100.lmask[6] = mask_table[val & 0x3fff]; adsp2100.base[6] = adsp2100.i[6] & adsp2100.lmask[6]; }
static void wr_l7(int32_t val)    { adsp2100.l[7] = val & 0x3fff; adsp2100.lmask[7] = mask_table[val & 0x3fff]; adsp2100.base[7] = adsp2100.i[7] & adsp2100.lmask[7]; }
static void wr_astat(int32_t val) { adsp2100.astat = val & 0x00ff; }
static void wr_mstat(int32_t val) { set_mstat(val & mstat_mask); }
static void wr_sstat(int32_t val) { adsp2100.sstat = val & 0x00ff; }
static void wr_imask(int32_t val) { adsp2100.imask = val & imask_mask; check_irqs(); }
static void wr_icntl(int32_t val) { adsp2100.icntl = val & 0x001f; check_irqs(); }
static void wr_cntr(int32_t val)  { cntr_stack_push(); adsp2100.cntr = val & 0x3fff; }
static void wr_sb(int32_t val)    { adsp2100.core.sb.s = (int32_t)(val << 27) >> 27; }
static void wr_px(int32_t val)    { adsp2100.px = val; }
static void wr_ifc(int32_t val)
{
	adsp2100.ifc = val;
	if (val & 0x002) adsp2100.irq_latch[ADSP2101_IRQ0] = 0;
	if (val & 0x004) adsp2100.irq_latch[ADSP2101_IRQ1] = 0;
	if (val & 0x008) adsp2100.irq_latch[ADSP2101_SPORT0_RX] = 0;
	if (val & 0x010) adsp2100.irq_latch[ADSP2101_SPORT0_TX] = 0;
	if (val & 0x020) adsp2100.irq_latch[ADSP2101_IRQ2] = 0;
	if (val & 0x080) adsp2100.irq_latch[ADSP2101_IRQ0] = 1;
	if (val & 0x100) adsp2100.irq_latch[ADSP2101_IRQ1] = 1;
	if (val & 0x200) adsp2100.irq_latch[ADSP2101_SPORT0_RX] = 1;
	if (val & 0x400) adsp2100.irq_latch[ADSP2101_SPORT0_TX] = 1;
	if (val & 0x800) adsp2100.irq_latch[ADSP2101_IRQ2] = 1;
	check_irqs();
}
static void wr_tx0(int32_t val)	{ if (sport_tx_callback) (*sport_tx_callback)(0, val); }
static void wr_tx1(int32_t val)	{ if (sport_tx_callback) (*sport_tx_callback)(1, val); }
static void wr_owrctr(int32_t val) { adsp2100.cntr = val & 0x3fff; }
static void wr_topstack(int32_t val) { pc_stack_push_val(val & 0x3fff); }

#define WRITE_REG(grp,reg,val) ((*wr_reg[grp][reg])(val))

static void (*wr_reg[4][16])(int32_t) =
{
	{
		wr_ax0, wr_ax1, wr_mx0, wr_mx1, wr_ay0, wr_ay1, wr_my0, wr_my1,
		wr_si, wr_se, wr_ar, wr_mr0, wr_mr1, wr_mr2, wr_sr0, wr_sr1
	},
	{
		wr_i0, wr_i1, wr_i2, wr_i3, wr_m0, wr_m1, wr_m2, wr_m3,
		wr_l0, wr_l1, wr_l2, wr_l3, wr_inval, wr_inval, wr_inval, wr_inval
	},
	{
		wr_i4, wr_i5, wr_i6, wr_i7, wr_m4, wr_m5, wr_m6, wr_m7,
		wr_l4, wr_l5, wr_l6, wr_l7, wr_inval, wr_inval, wr_inval, wr_inval
	},
	{
		wr_astat, wr_mstat, wr_inval, wr_imask, wr_icntl, wr_cntr, wr_sb, wr_px,
		wr_inval, wr_tx0, wr_inval, wr_tx1, wr_ifc, wr_owrctr, wr_inval, wr_topstack
	}
};



/*===========================================================================
    register reading
===========================================================================*/

static int32_t rd_inval(void) { /* logerror( "ADSP %04x: Writing to an invalid register!", adsp2100.ppc ); */ return 0; }
static int32_t rd_ax0(void)   { return adsp2100.core.ax0.s; }
static int32_t rd_ax1(void)   { return adsp2100.core.ax1.s; }
static int32_t rd_mx0(void)   { return adsp2100.core.mx0.s; }
static int32_t rd_mx1(void)   { return adsp2100.core.mx1.s; }
static int32_t rd_ay0(void)   { return adsp2100.core.ay0.s; }
static int32_t rd_ay1(void)   { return adsp2100.core.ay1.s; }
static int32_t rd_my0(void)   { return adsp2100.core.my0.s; }
static int32_t rd_my1(void)   { return adsp2100.core.my1.s; }
static int32_t rd_si(void)    { return adsp2100.core.si.s; }
static int32_t rd_se(void)    { return adsp2100.core.se.s; }
static int32_t rd_ar(void)    { return adsp2100.core.ar.s; }
static int32_t rd_mr0(void)   { return adsp2100.core.mr.mrx.mr0.s; }
static int32_t rd_mr1(void)   { return adsp2100.core.mr.mrx.mr1.s; }
static int32_t rd_mr2(void)   { return adsp2100.core.mr.mrx.mr2.s; }
static int32_t rd_sr0(void)   { return adsp2100.core.sr.srx.sr0.s; }
static int32_t rd_sr1(void)   { return adsp2100.core.sr.srx.sr1.s; }
static int32_t rd_i0(void)    { return adsp2100.i[0]; }
static int32_t rd_i1(void)    { return adsp2100.i[1]; }
static int32_t rd_i2(void)    { return adsp2100.i[2]; }
static int32_t rd_i3(void)    { return adsp2100.i[3]; }
static int32_t rd_i4(void)    { return adsp2100.i[4]; }
static int32_t rd_i5(void)    { return adsp2100.i[5]; }
static int32_t rd_i6(void)    { return adsp2100.i[6]; }
static int32_t rd_i7(void)    { return adsp2100.i[7]; }
static int32_t rd_m0(void)    { return adsp2100.m[0]; }
static int32_t rd_m1(void)    { return adsp2100.m[1]; }
static int32_t rd_m2(void)    { return adsp2100.m[2]; }
static int32_t rd_m3(void)    { return adsp2100.m[3]; }
static int32_t rd_m4(void)    { return adsp2100.m[4]; }
static int32_t rd_m5(void)    { return adsp2100.m[5]; }
static int32_t rd_m6(void)    { return adsp2100.m[6]; }
static int32_t rd_m7(void)    { return adsp2100.m[7]; }
static int32_t rd_l0(void)    { return adsp2100.l[0]; }
static int32_t rd_l1(void)    { return adsp2100.l[1]; }
static int32_t rd_l2(void)    { return adsp2100.l[2]; }
static int32_t rd_l3(void)    { return adsp2100.l[3]; }
static int32_t rd_l4(void)    { return adsp2100.l[4]; }
static int32_t rd_l5(void)    { return adsp2100.l[5]; }
static int32_t rd_l6(void)    { return adsp2100.l[6]; }
static int32_t rd_l7(void)    { return adsp2100.l[7]; }
static int32_t rd_astat(void) { return adsp2100.astat; }
static int32_t rd_mstat(void) { return adsp2100.mstat; }
static int32_t rd_sstat(void) { return adsp2100.sstat; }
static int32_t rd_imask(void) { return adsp2100.imask; }
static int32_t rd_icntl(void) { return adsp2100.icntl; }
static int32_t rd_cntr(void)  { return adsp2100.cntr; }
static int32_t rd_sb(void)    { return adsp2100.core.sb.s; }
static int32_t rd_px(void)    { return adsp2100.px; }
static int32_t rd_rx0(void)	{ if (sport_rx_callback) return (*sport_rx_callback)(0); else return 0; }
static int32_t rd_rx1(void)	{ if (sport_rx_callback) return (*sport_rx_callback)(1); else return 0; }
static int32_t rd_stacktop(void)	{ return pc_stack_pop_val(); }

#define READ_REG(grp,reg) ((*rd_reg[grp][reg])())

static int32_t (*rd_reg[4][16])(void) =
{
	{
		rd_ax0, rd_ax1, rd_mx0, rd_mx1, rd_ay0, rd_ay1, rd_my0, rd_my1,
		rd_si, rd_se, rd_ar, rd_mr0, rd_mr1, rd_mr2, rd_sr0, rd_sr1
	},
	{
		rd_i0, rd_i1, rd_i2, rd_i3, rd_m0, rd_m1, rd_m2, rd_m3,
		rd_l0, rd_l1, rd_l2, rd_l3, rd_inval, rd_inval, rd_inval, rd_inval
	},
	{
		rd_i4, rd_i5, rd_i6, rd_i7, rd_m4, rd_m5, rd_m6, rd_m7,
		rd_l4, rd_l5, rd_l6, rd_l7, rd_inval, rd_inval, rd_inval, rd_inval
	},
	{
		rd_astat, rd_mstat, rd_sstat, rd_imask, rd_icntl, rd_cntr, rd_sb, rd_px,
		rd_rx0, rd_inval, rd_rx1, rd_inval, rd_inval, rd_inval, rd_inval, rd_stacktop
	}
};



/*===========================================================================
    Modulus addressing logic
===========================================================================*/

inline void modify_address(uint32_t ireg, uint32_t mreg)
{
	uint32_t base = adsp2100.base[ireg];
	uint32_t i = adsp2100.i[ireg];
	uint32_t l = adsp2100.l[ireg];

	i = (i + adsp2100.m[mreg]) & 0x3fff;
	if (i < base) i += l;
	else if (i >= base + l) i -= l;
	adsp2100.i[ireg] = i;
}



/*===========================================================================
    Data memory accessors
===========================================================================*/

inline void data_write_dag1(uint32_t op, int32_t val)
{
	uint32_t ireg = (op >> 2) & 3;
	uint32_t mreg = op & 3;
	uint32_t base = adsp2100.base[ireg];
	uint32_t i = adsp2100.i[ireg];
	uint32_t l = adsp2100.l[ireg];

	if ( adsp2100.mstat & MSTAT_REVERSE )
	{
		uint32_t ir = reverse_table[ i & 0x3fff ];
		WWORD_DATA(ir, val);
	}
	else
		WWORD_DATA(i, val);

	i = (i + adsp2100.m[mreg]) & 0x3fff;
	if (i < base) i += l;
	else if (i >= base + l) i -= l;

	adsp2100.i[ireg] = i;
}


inline uint32_t data_read_dag1(uint32_t op)
{
	uint32_t ireg = (op >> 2) & 3;
	uint32_t mreg = op & 3;
	uint32_t base = adsp2100.base[ireg];
	uint32_t i = adsp2100.i[ireg];
	uint32_t l = adsp2100.l[ireg];
	uint32_t res;

	if (adsp2100.mstat & MSTAT_REVERSE)
	{
		uint32_t ir = reverse_table[i & 0x3fff];
		res = RWORD_DATA(ir);
	}
	else
		res = RWORD_DATA(i);

	i = (i + adsp2100.m[mreg]) & 0x3fff;
	if (i < base) i += l;
	else if (i >= base + l) i -= l;
	adsp2100.i[ireg] = i;

	return res;
}

inline void data_write_dag2(uint32_t op, int32_t val)
{
	uint32_t ireg = 4 + ((op >> 2) & 3);
	uint32_t mreg = 4 + (op & 3);
	uint32_t base = adsp2100.base[ireg];
	uint32_t i = adsp2100.i[ireg];
	uint32_t l = adsp2100.l[ireg];

	WWORD_DATA(i, val);

	i = (i + adsp2100.m[mreg]) & 0x3fff;
	if (i < base) i += l;
	else if (i >= base + l) i -= l;
	adsp2100.i[ireg] = i;
}


inline uint32_t data_read_dag2(uint32_t op)
{
	uint32_t ireg = 4 + ((op >> 2) & 3);
	uint32_t mreg = 4 + (op & 3);
	uint32_t base = adsp2100.base[ireg];
	uint32_t i = adsp2100.i[ireg];
	uint32_t l = adsp2100.l[ireg];

	uint32_t res = RWORD_DATA(i);

	i = (i + adsp2100.m[mreg]) & 0x3fff;
	if (i < base) i += l;
	else if (i >= base + l) i -= l;
	adsp2100.i[ireg] = i;

	return res;
}

/*===========================================================================
    Program memory accessors
===========================================================================*/

inline void pgm_write_dag2(uint32_t op, int32_t val)
{
	uint32_t ireg = 4 + ((op >> 2) & 3);
	uint32_t mreg = 4 + (op & 3);
	uint32_t base = adsp2100.base[ireg];
	uint32_t i = adsp2100.i[ireg];
	uint32_t l = adsp2100.l[ireg];

	WWORD_PGM(i, (val << 8) | adsp2100.px);

	i = (i + adsp2100.m[mreg]) & 0x3fff;
	if (i < base) i += l;
	else if (i >= base + l) i -= l;
	adsp2100.i[ireg] = i;
}


inline uint32_t pgm_read_dag2(uint32_t op)
{
	uint32_t ireg = 4 + ((op >> 2) & 3);
	uint32_t mreg = 4 + (op & 3);
	uint32_t base = adsp2100.base[ireg];
	uint32_t i = adsp2100.i[ireg];
	uint32_t l = adsp2100.l[ireg];
	uint32_t res;

	res = RWORD_PGM(i);
	adsp2100.px = res;
	res >>= 8;

	i = (i + adsp2100.m[mreg]) & 0x3fff;
	if (i < base) i += l;
	else if (i >= base + l) i -= l;
	adsp2100.i[ireg] = i;

	return res;
}



/*===========================================================================
    register reading
===========================================================================*/

#define ALU_GETXREG_UNSIGNED(x) (*(uint16_t *)alu_xregs[x])
#define ALU_GETXREG_SIGNED(x)   (*( int16_t *)alu_xregs[x])
#define ALU_GETYREG_UNSIGNED(y) (*(uint16_t *)alu_yregs[y])
#define ALU_GETYREG_SIGNED(y)   (*( int16_t *)alu_yregs[y])

static const void *alu_xregs[8] =
{
	&adsp2100.core.ax0,
	&adsp2100.core.ax1,
	&adsp2100.core.ar,
	&adsp2100.core.mr.mrx.mr0,
	&adsp2100.core.mr.mrx.mr1,
	&adsp2100.core.mr.mrx.mr2,
	&adsp2100.core.sr.srx.sr0,
	&adsp2100.core.sr.srx.sr1
};

static const void *alu_yregs[4] =
{
	&adsp2100.core.ay0,
	&adsp2100.core.ay1,
	&adsp2100.core.af,
	&adsp2100.core.zero
};



/*===========================================================================
	MAC register reading
===========================================================================*/

#define MAC_GETXREG_UNSIGNED(x) (*(uint16_t *)mac_xregs[x])
#define MAC_GETXREG_SIGNED(x)   (*( int16_t *)mac_xregs[x])
#define MAC_GETYREG_UNSIGNED(y) (*(uint16_t *)mac_yregs[y])
#define MAC_GETYREG_SIGNED(y)   (*( int16_t *)mac_yregs[y])

static const void *mac_xregs[8] =
{
	&adsp2100.core.mx0,
	&adsp2100.core.mx1,
	&adsp2100.core.ar,
	&adsp2100.core.mr.mrx.mr0,
	&adsp2100.core.mr.mrx.mr1,
	&adsp2100.core.mr.mrx.mr2,
	&adsp2100.core.sr.srx.sr0,
	&adsp2100.core.sr.srx.sr1
};

static const void *mac_yregs[4] =
{
	&adsp2100.core.my0,
	&adsp2100.core.my1,
	&adsp2100.core.mf,
	&adsp2100.core.zero
};



/*===========================================================================
	SHIFT register reading
===========================================================================*/

#define SHIFT_GETXREG_UNSIGNED(x) (*(uint16_t *)shift_xregs[x])
#define SHIFT_GETXREG_SIGNED(x)   (*( int16_t *)shift_xregs[x])

static const void *shift_xregs[8] =
{
	&adsp2100.core.si,
	&adsp2100.core.si,
	&adsp2100.core.ar,
	&adsp2100.core.mr.mrx.mr0,
	&adsp2100.core.mr.mrx.mr1,
	&adsp2100.core.mr.mrx.mr2,
	&adsp2100.core.sr.srx.sr0,
	&adsp2100.core.sr.srx.sr1
};



/*===========================================================================
	ALU operations (result in AR)
===========================================================================*/

static void alu_op_ar(int op)
{
	int32_t xop = (op >> 8) & 7;
	int32_t yop = (op >> 11) & 3;
	int32_t res;

	//switch ((op >> 13) & 15)
	switch (op & (15<<13))  /*JB*/
	{
		case 0x00<<13:
			/* Y                Clear when y = 0 */
			res = ALU_GETYREG_UNSIGNED(yop);
			CALC_NZ(res);
			break;
		case 0x01<<13:
			/* Y + 1            PASS 1 when y = 0 */
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = yop + 1;
			CALC_NZ(res);
			if (yop == 0x7fff) SET_V;
			else if (yop == 0xffff) SET_C;
			break;
		case 0x02<<13:
			/* X + Y + C */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			yop += GET_C >> 3;
			res = xop + yop;
			CALC_NZVC(xop, yop, res);
			break;
		case 0x03<<13:
			/* X + Y            X when y = 0 */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop + yop;
			CALC_NZVC(xop, yop, res);
			break;
		case 0x04<<13:
			/* NOT Y */
			res = ALU_GETYREG_UNSIGNED(yop) ^ 0xffff;
			CALC_NZ(res);
			break;
		case 0x05<<13:
			/* -Y */
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = -yop;
			CALC_NZ(res);
			if (yop == 0x8000) SET_V;
			if (yop == 0x0000) SET_C;
			break;
		case 0x06<<13:
			/* X - Y + C - 1    X + C - 1 when y = 0 */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop - yop + (GET_C >> 3) - 1;
			CALC_NZVC_SUB(xop, yop, res);
			break;
		case 0x07<<13:
			/* X - Y */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop - yop;
			CALC_NZVC_SUB(xop, yop, res);
			break;
		case 0x08<<13:
			/* Y - 1            PASS -1 when y = 0 */
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = yop - 1;
			CALC_NZ(res);
			if (yop == 0x8000) SET_V;
			else if (yop == 0x0000) SET_C;
			break;
		case 0x09<<13:
			/* Y - X            -X when y = 0 */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = yop - xop;
			CALC_NZVC_SUB(yop, xop, res);
			break;
		case 0x0a<<13:
			/* Y - X + C - 1    -X + C - 1 when y = 0 */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = yop - xop + (GET_C >> 3) - 1;
			CALC_NZVC_SUB(yop, xop, res);
			break;
		case 0x0b<<13:
			/* NOT X */
			res = ALU_GETXREG_UNSIGNED(xop) ^ 0xffff;
			CALC_NZ(res);
			break;
		case 0x0c<<13:
			/* X AND Y */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop & yop;
			CALC_NZ(res);
			break;
		case 0x0d<<13:
			/* X OR Y */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop | yop;
			CALC_NZ(res);
			break;
		case 0x0e<<13:
			/* X XOR Y */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop ^ yop;
			CALC_NZ(res);
			break;
		case 0x0f<<13:
			/* ABS X */
			xop = ALU_GETXREG_UNSIGNED(xop);
			res = (xop & 0x8000) ? -xop : xop;
			CLR_FLAGS;
			CLR_S;
			if (xop == 0) SET_Z;
			if (xop == 0x8000) SET_N, SET_V;
			if (xop & 0x8000) SET_S;
			break;
		default:
			res = 0;    /* just to keep the compiler happy */
			break;
	}

	/* saturate */
	if ((adsp2100.mstat & MSTAT_SATURATE) && GET_V) res = GET_C ? -32768 : 32767;

	/* set the final value */
	adsp2100.core.ar.u = res;
}



/*===========================================================================
	ALU operations (result in AF)
===========================================================================*/

static void alu_op_af(int op)
{
	int32_t xop = (op >> 8) & 7;
	int32_t yop = (op >> 11) & 3;
	int32_t res;

//	switch ((op >> 13) & 15)
	switch (op & (15<<13))  /*JB*/
	{
		case 0x00<<13:
			/* Y                Clear when y = 0 */
			res = ALU_GETYREG_UNSIGNED(yop);
			CALC_NZ(res);
			break;
		case 0x01<<13:
			/* Y + 1            PASS 1 when y = 0 */
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = yop + 1;
			CALC_NZ(res);
			if (yop == 0x7fff) SET_V;
			else if (yop == 0xffff) SET_C;
			break;
		case 0x02<<13:
			/* X + Y + C */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			yop += GET_C >> 3;
			res = xop + yop;
			CALC_NZVC(xop, yop, res);
			break;
		case 0x03<<13:
			/* X + Y            X when y = 0 */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop + yop;
			CALC_NZVC(xop, yop, res);
			break;
		case 0x04<<13:
			/* NOT Y */
			res = ALU_GETYREG_UNSIGNED(yop) ^ 0xffff;
			CALC_NZ(res);
			break;
		case 0x05<<13:
			/* -Y */
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = -yop;
			CALC_NZ(res);
			if (yop == 0x8000) SET_V;
			if (yop == 0x0000) SET_C;
			break;
		case 0x06<<13:
			/* X - Y + C - 1    X + C - 1 when y = 0 */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop - yop + (GET_C >> 3) - 1;
			CALC_NZVC_SUB(xop, yop, res);
			break;
		case 0x07<<13:
			/* X - Y */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop - yop;
			CALC_NZVC_SUB(xop, yop, res);
			break;
		case 0x08<<13:
			/* Y - 1            PASS -1 when y = 0 */
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = yop - 1;
			CALC_NZ(res);
			if (yop == 0x8000) SET_V;
			else if (yop == 0x0000) SET_C;
			break;
		case 0x09<<13:
			/* Y - X            -X when y = 0 */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = yop - xop;
			CALC_NZVC_SUB(yop, xop, res);
			break;
		case 0x0a<<13:
			/* Y - X + C - 1    -X + C - 1 when y = 0 */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = yop - xop + (GET_C >> 3) - 1;
			CALC_NZVC_SUB(yop, xop, res);
			break;
		case 0x0b<<13:
			/* NOT X */
			res = ALU_GETXREG_UNSIGNED(xop) ^ 0xffff;
			CALC_NZ(res);
			break;
		case 0x0c<<13:
			/* X AND Y */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop & yop;
			CALC_NZ(res);
			break;
		case 0x0d<<13:
			/* X OR Y */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop | yop;
			CALC_NZ(res);
			break;
		case 0x0e<<13:
			/* X XOR Y */
			xop = ALU_GETXREG_UNSIGNED(xop);
			yop = ALU_GETYREG_UNSIGNED(yop);
			res = xop ^ yop;
			CALC_NZ(res);
			break;
		case 0x0f<<13:
			/* ABS X */
			xop = ALU_GETXREG_UNSIGNED(xop);
			res = (xop & 0x8000) ? -xop : xop;
			CLR_FLAGS;
			CLR_S;
			if (xop == 0) SET_Z;
			if (xop == 0x8000) SET_N, SET_V;
			if (xop & 0x8000) SET_S;
			break;
		default:
			res = 0;	/* just to keep the compiler happy */
			break;
	}

	/* set the final value */
	adsp2100.core.af.u = res;
}



/*===========================================================================
    MAC operations (result in MR)
===========================================================================*/

static void mac_op_mr(int op)
{
	int8_t shift = ((adsp2100.mstat & MSTAT_INTEGER) >> 4) ^ 1;
	int32_t xop = (op >> 8) & 7;
	int32_t yop = (op >> 11) & 3;
	int32_t temp;
	int64_t res;

//	switch ((op >> 13) & 15)
	switch (op & (15<<13))	/*JB*/
	{
		case 0x00<<13:
			/* no-op */
			return;
		case 0x01<<13:
			/* X * Y (RND) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = (int64_t)temp;
#if 0
			if ((res & 0xffff) == 0x8000) res &= ~((uint64_t)0x10000);
			else res += (res & 0x8000) << 1;
#else
			temp &= 0xffff;
			res += 0x8000;
			if ( temp == 0x8000 )
				res &= ~((uint64_t)0x10000);
#endif
			break;
		case 0x02<<13:
			/* MR + X * Y (RND) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr + (int64_t)temp;
#if 0
			if ((res & 0xffff) == 0x8000) res &= ~((uint64_t)0x10000);
			else res += (res & 0x8000) << 1;
#else
			temp &= 0xffff;
			res += 0x8000;
			if ( temp == 0x8000 )
				res &= ~((uint64_t)0x10000);
#endif
			break;
		case 0x03<<13:
			/* MR - X * Y (RND) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr - (int64_t)temp;
#if 0
			if ((res & 0xffff) == 0x8000) res &= ~((uint64_t)0x10000);
			else res += (res & 0x8000) << 1;
#else
			temp &= 0xffff;
			res += 0x8000;
			if ( temp == 0x8000 )
				res &= ~((uint64_t)0x10000);
#endif
			break;
		case 0x04<<13:
			/* X * Y (SS)       Clear when y = 0 */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = (int64_t)temp;
			break;
		case 0x05<<13:
			/* X * Y (SU) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = (int64_t)temp;
			break;
		case 0x06<<13:
			/* X * Y (US) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = (int64_t)temp;
			break;
		case 0x07<<13:
			/* X * Y (UU) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = (int64_t)temp;
			break;
		case 0x08<<13:
			/* MR + X * Y (SS) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr + (int64_t)temp;
			break;
		case 0x09<<13:
			/* MR + X * Y (SU) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr + (int64_t)temp;
			break;
		case 0x0a<<13:
			/* MR + X * Y (US) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr + (int64_t)temp;
			break;
		case 0x0b<<13:
			/* MR + X * Y (UU) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr + (int64_t)temp;
			break;
		case 0x0c<<13:
			/* MR - X * Y (SS) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr - (int64_t)temp;
			break;
		case 0x0d<<13:
			/* MR - X * Y (SU) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr - (int64_t)temp;
			break;
		case 0x0e<<13:
			/* MR - X * Y (US) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr - (int64_t)temp;
			break;
		case 0x0f<<13:
			/* MR - X * Y (UU) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr - (int64_t)temp;
			break;
		default:
			res = 0;    /* just to keep the compiler happy */
			break;
	}

	/* set the final value */
	temp = (res >> 31) & 0x1ff;
	CLR_MV;
	if (temp != 0x000 && temp != 0x1ff) SET_MV;
	adsp2100.core.mr.mr = res;
}



/*===========================================================================
    MAC operations (result in MF)
===========================================================================*/

static void mac_op_mf(int op)
{
	int8_t shift = ((adsp2100.mstat & MSTAT_INTEGER) >> 4) ^ 1;
	int32_t xop = (op >> 8) & 7;
	int32_t yop = (op >> 11) & 3;
	int32_t temp;
	int64_t res;

//	switch ((op >> 13) & 15)
	switch (op & (15<<13))	/*JB*/
	{
		case 0x00<<13:
			/* no-op */
			return;
		case 0x01<<13:
			/* X * Y (RND) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = (int64_t)temp;
#if 0
			if ((res & 0xffff) == 0x8000) res &= ~((uint64_t)0x10000);
			else res += (res & 0x8000) << 1;
#else
			temp &= 0xffff;
			res += 0x8000;
			if ( temp == 0x8000 )
				res &= ~((uint64_t)0x10000);
#endif
			break;
		case 0x02<<13:
			/* MR + X * Y (RND) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr + (int64_t)temp;
#if 0
			if ((res & 0xffff) == 0x8000) res &= ~((uint64_t)0x10000);
			else res += (res & 0x8000) << 1;
#else
			temp &= 0xffff;
			res += 0x8000;
			if ( temp == 0x8000 )
				res &= ~((uint64_t)0x10000);
#endif
			break;
		case 0x03<<13:
			/* MR - X * Y (RND) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr - (int64_t)temp;
#if 0
			if ((res & 0xffff) == 0x8000) res &= ~((uint64_t)0x10000);
			else res += (res & 0x8000) << 1;
#else
			temp &= 0xffff;
			res += 0x8000;
			if ( temp == 0x8000 )
				res &= ~((uint64_t)0x10000);
#endif
			break;
		case 0x04<<13:
			/* X * Y (SS)       Clear when y = 0 */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = (int64_t)temp;
			break;
		case 0x05<<13:
			/* X * Y (SU) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = (int64_t)temp;
			break;
		case 0x06<<13:
			/* X * Y (US) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = (int64_t)temp;
			break;
		case 0x07<<13:
			/* X * Y (UU) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = (int64_t)temp;
			break;
		case 0x08<<13:
			/* MR + X * Y (SS) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr + (int64_t)temp;
			break;
		case 0x09<<13:
			/* MR + X * Y (SU) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr + (int64_t)temp;
			break;
		case 0x0a<<13:
			/* MR + X * Y (US) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr + (int64_t)temp;
			break;
		case 0x0b<<13:
			/* MR + X * Y (UU) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr + (int64_t)temp;
			break;
		case 0x0c<<13:
			/* MR - X * Y (SS) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr - (int64_t)temp;
			break;
		case 0x0d<<13:
			/* MR - X * Y (SU) */
			xop = MAC_GETXREG_SIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr - (int64_t)temp;
			break;
		case 0x0e<<13:
			/* MR - X * Y (US) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_SIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr - (int64_t)temp;
			break;
		case 0x0f<<13:
			/* MR - X * Y (UU) */
			xop = MAC_GETXREG_UNSIGNED(xop);
			yop = MAC_GETYREG_UNSIGNED(yop);
			temp = (xop * yop) << shift;
			res = adsp2100.core.mr.mr - (int64_t)temp;
			break;
		default:
			res = 0;    /* just to keep the compiler happy */
			break;
	}

	/* set the final value */
	adsp2100.core.mf.u = (uint32_t)res >> 16;
}



/*===========================================================================
    SHIFT operations (result in SR/SE/SB)
===========================================================================*/

static void shift_op(int op)
{
	int8_t sc = (int8_t)adsp2100.core.se.s;
	int32_t xop = (op >> 8) & 7;
	uint32_t res;

//	switch ((op >> 11) & 15)
	switch (op & (15<<11))	/*JB*/
	{
		case 0x00<<11:
			/* LSHIFT (HI) */
			xop = SHIFT_GETXREG_UNSIGNED(xop) << 16;
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? ((uint32_t)xop >> -sc) : 0;
			adsp2100.core.sr.sr = res;
			break;
		case 0x01<<11:
			/* LSHIFT (HI, OR) */
			xop = SHIFT_GETXREG_UNSIGNED(xop) << 16;
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? ((uint32_t)xop >> -sc) : 0;
			adsp2100.core.sr.sr |= res;
			break;
		case 0x02<<11:
			/* LSHIFT (LO) */
			xop = SHIFT_GETXREG_UNSIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : 0;
			adsp2100.core.sr.sr = res;
			break;
		case 0x03<<11:
			/* LSHIFT (LO, OR) */
			xop = SHIFT_GETXREG_UNSIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : 0;
			adsp2100.core.sr.sr |= res;
			break;
		case 0x04<<11:
			/* ASHIFT (HI) */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : (xop >> 31);
			adsp2100.core.sr.sr = res;
			break;
		case 0x05<<11:
			/* ASHIFT (HI, OR) */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : (xop >> 31);
			adsp2100.core.sr.sr |= res;
			break;
		case 0x06<<11:
			/* ASHIFT (LO) */
			xop = SHIFT_GETXREG_SIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : (xop >> 31);
			adsp2100.core.sr.sr = res;
			break;
		case 0x07<<11:
			/* ASHIFT (LO, OR) */
			xop = SHIFT_GETXREG_SIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : (xop >> 31);
			adsp2100.core.sr.sr |= res;
			break;
		case 0x08<<11:
			/* NORM (HI) */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			if (sc > 0)
			{
				xop = ((uint32_t)xop >> 1) | ((adsp2100.astat & CFLAG) << 28);
				res = xop >> (sc - 1);
			}
			else res = (sc > -32) ? (xop << -sc) : 0;
			adsp2100.core.sr.sr = res;
			break;
		case 0x09<<11:
			/* NORM (HI, OR) */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			if (sc > 0)
			{
				xop = ((uint32_t)xop >> 1) | ((adsp2100.astat & CFLAG) << 28);
				res = xop >> (sc - 1);
			}
			else res = (sc > -32) ? (xop << -sc) : 0;
			adsp2100.core.sr.sr |= res;
			break;
		case 0x0a<<11:
			/* NORM (LO) */
			xop = SHIFT_GETXREG_UNSIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop >> sc) : 0;
			else res = (sc > -32) ? (xop << -sc) : 0;
			adsp2100.core.sr.sr = res;
			break;
		case 0x0b<<11:
			/* NORM (LO, OR) */
			xop = SHIFT_GETXREG_UNSIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop >> sc) : 0;
			else res = (sc > -32) ? (xop << -sc) : 0;
			adsp2100.core.sr.sr |= res;
			break;
		case 0x0c<<11:
			/* EXP (HI) */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			res = 0;
			if (xop < 0)
			{
				SET_SS;
				while ((xop & 0x40000000) != 0) res++, xop <<= 1;
			}
			else
			{
				CLR_SS;
				xop |= 0x8000;
				while ((xop & 0x40000000) == 0) res++, xop <<= 1;
			}
			adsp2100.core.se.s = -(int16_t)res;
			break;
		case 0x0d<<11:
			/* EXP (HIX) */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			if (GET_V)
			{
				adsp2100.core.se.s = 1;
				if (xop < 0) CLR_SS;
				else SET_SS;
			}
			else
			{
				res = 0;
				if (xop < 0)
				{
					SET_SS;
					while ((xop & 0x40000000) != 0) res++, xop <<= 1;
				}
				else
				{
					CLR_SS;
					xop |= 0x8000;
					while ((xop & 0x40000000) == 0) res++, xop <<= 1;
				}
				adsp2100.core.se.s = -(int16_t)res;
			}
			break;
		case 0x0e<<11:
			/* EXP (LO) */
			if (adsp2100.core.se.s == -15)
			{
				xop = SHIFT_GETXREG_SIGNED(xop);
				res = 15;
				if (GET_SS)
					while ((xop & 0x8000) != 0) res++, xop <<= 1;
				else
				{
					xop = (xop << 1) | 1;
					while ((xop & 0x10000) == 0) res++, xop <<= 1;
				}
				adsp2100.core.se.s = -(int16_t)res;
			}
			break;
		case 0x0f<<11:
			/* EXPADJ */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			res = 0;
			if (xop < 0)
				while ((xop & 0x40000000) != 0) res++, xop <<= 1;
			else
			{
				xop |= 0x8000;
				while ((xop & 0x40000000) == 0) res++, xop <<= 1;
			}
			if ((int16_t)res < -adsp2100.core.sb.s)
				adsp2100.core.sb.s = -(int16_t)res;
			break;
	}
}



/*===========================================================================
    Immediate SHIFT operations (result in SR/SE/SB)
===========================================================================*/

static void shift_op_imm(int op)
{
	int8_t sc = (int8_t)op;
	int32_t xop = (op >> 8) & 7;
	uint32_t res;

//	switch ((op >> 11) & 15)
	switch (op & (15<<11))	/*JB*/
	{
		case 0x00<<11:
			/* LSHIFT (HI) */
			xop = SHIFT_GETXREG_UNSIGNED(xop) << 16;
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? ((uint32_t)xop >> -sc) : 0;
			adsp2100.core.sr.sr = res;
			break;
		case 0x01<<11:
			/* LSHIFT (HI, OR) */
			xop = SHIFT_GETXREG_UNSIGNED(xop) << 16;
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? ((uint32_t)xop >> -sc) : 0;
			adsp2100.core.sr.sr |= res;
			break;
		case 0x02<<11:
			/* LSHIFT (LO) */
			xop = SHIFT_GETXREG_UNSIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : 0;
			adsp2100.core.sr.sr = res;
			break;
		case 0x03<<11:
			/* LSHIFT (LO, OR) */
			xop = SHIFT_GETXREG_UNSIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : 0;
			adsp2100.core.sr.sr |= res;
			break;
		case 0x04<<11:
			/* ASHIFT (HI) */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : (xop >> 31);
			adsp2100.core.sr.sr = res;
			break;
		case 0x05<<11:
			/* ASHIFT (HI, OR) */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : (xop >> 31);
			adsp2100.core.sr.sr |= res;
			break;
		case 0x06<<11:
			/* ASHIFT (LO) */
			xop = SHIFT_GETXREG_SIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : (xop >> 31);
			adsp2100.core.sr.sr = res;
			break;
		case 0x07<<11:
			/* ASHIFT (LO, OR) */
			xop = SHIFT_GETXREG_SIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop << sc) : 0;
			else res = (sc > -32) ? (xop >> -sc) : (xop >> 31);
			adsp2100.core.sr.sr |= res;
			break;
		case 0x08<<11:
			/* NORM (HI) */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			if (sc > 0)
			{
				xop = ((uint32_t)xop >> 1) | ((adsp2100.astat & CFLAG) << 28);
				res = xop >> (sc - 1);
			}
			else res = (sc > -32) ? (xop << -sc) : 0;
			adsp2100.core.sr.sr = res;
			break;
		case 0x09<<11:
			/* NORM (HI, OR) */
			xop = SHIFT_GETXREG_SIGNED(xop) << 16;
			if (sc > 0)
			{
				xop = ((uint32_t)xop >> 1) | ((adsp2100.astat & CFLAG) << 28);
				res = xop >> (sc - 1);
			}
			else res = (sc > -32) ? (xop << -sc) : 0;
			adsp2100.core.sr.sr |= res;
			break;
		case 0x0a<<11:
			/* NORM (LO) */
			xop = SHIFT_GETXREG_UNSIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop >> sc) : 0;
			else res = (sc > -32) ? (xop << -sc) : 0;
			adsp2100.core.sr.sr = res;
			break;
		case 0x0b<<11:
			/* NORM (LO, OR) */
			xop = SHIFT_GETXREG_UNSIGNED(xop);
			if (sc > 0) res = (sc < 32) ? (xop >> sc) : 0;
			else res = (sc > -32) ? (xop << -sc) : 0;
			adsp2100.core.sr.sr |= res;
			break;
	}
}
