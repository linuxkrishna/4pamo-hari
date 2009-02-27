/*
ducatienabler.h
*/

#include <host_os.h>

#include <hw_defs.h>
#include <hw_mmu.h>

#ifndef _DDUCATIMMU_ENABLER_H_
#define _DDUCATIMMU_ENABLER_H_


/* ========================================================================== */
/* Success & Failure macros                                                   */
/* ========================================================================== */
#define DSP_SUCCEEDED(status)   ((int) (status) >= 0)
#define DSP_FAILED(status)      ((int) (status) <  0)


#define PAGE_SIZE_4KB       0x1000
#define PAGE_SIZE_64KB      0x10000
#define PAGE_SIZE_1MB       0x100000

/*============================================================================*/
/*  Argument specification syntax                                             */
/*============================================================================*/

#ifndef IN
#define IN                              /* The argument is INPUT  only */
#endif

#ifndef OUT
#define OUT                             /* The argument is OUTPUT only */
#endif

#ifndef OPTIONAL
#define OPTIONAL                        /* The argument is OPTIONAL    */
#endif

#ifndef CONST
#define CONST   const
#endif




#define L4_PERIPHERAL_UART3                      0x48020000
#define DSPVA_PERIPHERAL_UART3                   0x48020000

#define L4_PERIPHERAL_GPIO2                      0x48055000
#define DSPVA_PERIPHERAL_GPIO2                   0x48055000
#define L4_PERIPHERAL_GPIO3                      0x48057000
#define DSPVA_PERIPHERAL_GPIO3                   0x48057000
#define L4_PERIPHERAL_GPIO4                      0x48059000
#define DSPVA_PERIPHERAL_GPIO4                   0x48059000
#define L4_PERIPHERAL_GPIO5                      0x4805B000
#define DSPVA_PERIPHERAL_GPIO5                   0x4805B000

#define L4_PERIPHERAL_GPTIMER5                   0x49038000
#define DSPVA_PERIPHERAL_GPTIMER5                0x49038000
#define L4_PERIPHERAL_GPTIMER6                   0x4903A000
#define DSPVA_PERIPHERAL_GPTIMER6                0x4903A000
#define L4_PERIPHERAL_GPTIMER7                   0x4903C000
#define DSPVA_PERIPHERAL_GPTIMER7                0x4903C000
#define L4_PERIPHERAL_GPTIMER8                   0x4903E000
#define DSPVA_PERIPHERAL_GPTIMER8                0x4903E000

#define L4_PERIPHERAL_UART1                      0x4806A000
#define DSPVA_PERIPHERAL_UART1                   0x4806A000
#define L4_PERIPHERAL_UART2                      0x4806C000
#define DSPVA_PERIPHERAL_UART2                   0x4806C000
#define L4_PERIPHERAL_UART4                      0x4806E000
#define DSPVA_PERIPHERAL_UART4                   0x4806E000

#define L4_PERIPHERAL_MCBSP4                     0x48074000
#define DSPVA_PERIPHERAL_MCBSP4                  0x48074000
#define L4_PERIPHERAL_MCBSP5                     0x48096000
#define DSPVA_PERIPHERAL_MCBSP5                  0x48096000

#define L4_PERIPHERAL_MCSPI1                     0x48098000
#define DSPVA_PERIPHERAL_MCSPI1                  0x48098000
#define L4_PERIPHERAL_MCSPI2                     0x4809A000
#define DSPVA_PERIPHERAL_MCSPI2                  0x4809A000

#define L4_PERIPHERAL_MCBSP1                     0x49022000
#define DSPVA_PERIPHERAL_MCBSP1                  0x49022000
#define L4_PERIPHERAL_MCBSP2                     0x49024000
#define DSPVA_PERIPHERAL_MCBSP2                  0x49024000
#define L4_PERIPHERAL_MCBSP3                     0x49028000
#define DSPVA_PERIPHERAL_MCBSP3                  0x49028000

#define L4_PERIPHERAL_WDT3                       0x49030000
#define DSPVA_PERIPHERAL_WDT3                    0x49030000

#define L4_PERIPHERAL_MBOX                       0x4A0F4000
#define DSPVA_PERIPHERAL_MBOX                    0x4A0F4000


#define L4_PERIPHERAL_CM1                        0x4A004000
#define DSPVA_PERIPHERAL_CM1                     0x4A004000
#define L4_PERIPHERAL_CM2_P1                     0x4A008000 //(4Kb)
#define DSPVA_PERIPHERAL_CM2_P1                  0x4A008000
#define L4_PERIPHERAL_CM2_P2                     0x4A009000 //(4Kb)
#define DSPVA_PERIPHERAL_CM2_P2                  0x4A009000

#define L4_PERIPHERAL_PRM_P1                     0x4A306000 //(4Kb)
#define DSPVA_PERIPHERAL_PRM_P1                  0x4A306000
#define L4_PERIPHERAL_PRM_P2                     0x4A307000 //(4Kb)
#define DSPVA_PERIPHERAL_PRM_P2                  0x4A307000

#define L4_PERIPHERAL_SCRM                       0x4A30A000
#define DSPVA_PERIPHERAL_SCRM                    0x4A30A000

#define L4_PERIPHERAL_GPIO1                      0x4A310000
#define DSPVA_PERIPHERAL_GPIO1                   0x4A310000



struct MMUEntry
{
	u32 ulPhysAddr ;
	u32 ulVirtAddr ;
	u32 ulSize ;
};



static const struct MMUEntry L4Map [] =
{
	//  Mailbox 4KB
	{L4_PERIPHERAL_MBOX, DSPVA_PERIPHERAL_MBOX, HW_PAGE_SIZE_4KB},
	//  SCM 4KB
	{L4_PERIPHERAL_SCRM, DSPVA_PERIPHERAL_SCRM, HW_PAGE_SIZE_4KB},
	//  PRM 8KB
	{L4_PERIPHERAL_PRM_P1, DSPVA_PERIPHERAL_PRM_P1, HW_PAGE_SIZE_4KB},
	{L4_PERIPHERAL_PRM_P2, DSPVA_PERIPHERAL_PRM_P2, HW_PAGE_SIZE_4KB},
	//  CM1 4KB
	{L4_PERIPHERAL_CM1, DSPVA_PERIPHERAL_CM1, HW_PAGE_SIZE_4KB},
	//  CM2 8KB
	{L4_PERIPHERAL_CM2_P1, DSPVA_PERIPHERAL_CM2_P1, HW_PAGE_SIZE_4KB},
	{L4_PERIPHERAL_CM2_P2, DSPVA_PERIPHERAL_CM2_P2, HW_PAGE_SIZE_4KB},
	//GP timer1 4KB
	{L4_PERIPHERAL_GPTIMER5, DSPVA_PERIPHERAL_GPTIMER5, HW_PAGE_SIZE_4KB},
	//  GP timer6 4KB
	{L4_PERIPHERAL_GPTIMER6, DSPVA_PERIPHERAL_GPTIMER6, HW_PAGE_SIZE_4KB},
	//  GP timer7 4KB
	{L4_PERIPHERAL_GPTIMER7, DSPVA_PERIPHERAL_GPTIMER7, HW_PAGE_SIZE_4KB},
	//  GP timer8 4KB
	{L4_PERIPHERAL_GPTIMER8, DSPVA_PERIPHERAL_GPTIMER8, HW_PAGE_SIZE_4KB},
	//  GPIO1 4KB
	{L4_PERIPHERAL_GPIO1, DSPVA_PERIPHERAL_GPIO1, HW_PAGE_SIZE_4KB},
	//  GPIO2 4KB
	{L4_PERIPHERAL_GPIO2, DSPVA_PERIPHERAL_GPIO2, HW_PAGE_SIZE_4KB},
	//  GPIO3 4KB
	{L4_PERIPHERAL_GPIO3, DSPVA_PERIPHERAL_GPIO3, HW_PAGE_SIZE_4KB},
	//  GPIO4 4KB
	{L4_PERIPHERAL_GPIO4, DSPVA_PERIPHERAL_GPIO4, HW_PAGE_SIZE_4KB},
	//  GPIO5 4KB
	{L4_PERIPHERAL_GPIO5, DSPVA_PERIPHERAL_GPIO5, HW_PAGE_SIZE_4KB},
	//  Watch dog timer 4KB
	{L4_PERIPHERAL_WDT3, DSPVA_PERIPHERAL_WDT3, HW_PAGE_SIZE_4KB},
	//  Mcbsp1 4KB
	{L4_PERIPHERAL_MCBSP1, DSPVA_PERIPHERAL_MCBSP1, HW_PAGE_SIZE_4KB},
	//  Mcbsp2 4KB
	{L4_PERIPHERAL_MCBSP2, DSPVA_PERIPHERAL_MCBSP2, HW_PAGE_SIZE_4KB},
	//  Mcbsp3 4KB
	{L4_PERIPHERAL_MCBSP3, DSPVA_PERIPHERAL_MCBSP3, HW_PAGE_SIZE_4KB},
	//  Mcbsp4 4KB
	{L4_PERIPHERAL_MCBSP4, DSPVA_PERIPHERAL_MCBSP4, HW_PAGE_SIZE_4KB},
	//  Mcbsp5 4KB
	{L4_PERIPHERAL_MCBSP5, DSPVA_PERIPHERAL_MCBSP5, HW_PAGE_SIZE_4KB},
	//  UART1 4KB
	{L4_PERIPHERAL_UART1, DSPVA_PERIPHERAL_UART1, HW_PAGE_SIZE_4KB},
	//  UART2 4KB
	{L4_PERIPHERAL_UART2, DSPVA_PERIPHERAL_UART2, HW_PAGE_SIZE_4KB},
	//  UART3 4KB
	{L4_PERIPHERAL_UART3, DSPVA_PERIPHERAL_UART3, HW_PAGE_SIZE_4KB},
	//  SPI1 4KB
	{L4_PERIPHERAL_MCSPI1, DSPVA_PERIPHERAL_MCSPI1, HW_PAGE_SIZE_4KB},
	//  SPI2 4KB
	{L4_PERIPHERAL_MCSPI2, DSPVA_PERIPHERAL_MCSPI2, HW_PAGE_SIZE_4KB},
	//  XTI 4KB

} ;


/* ========================================================================== */
/* Define the return code type                                                */
/* ========================================================================== */
typedef u32   DSP_STATUS ;


/* Types of mapping attributes */

/* MPU address is virtual and needs to be translated to physical addr */
#define DSP_MAPVIRTUALADDR          0x00000000
#define DSP_MAPPHYSICALADDR         0x00000001

/* Mapped data is big endian */
#define DSP_MAPBIGENDIAN            0x00000002
#define DSP_MAPLITTLEENDIAN         0x00000000

/* Element size is based on DSP r/w access size */
#define DSP_MAPMIXEDELEMSIZE        0x00000004

/*
 * Element size for MMU mapping (8, 16, 32, or 64 bit)
 * Ignored if DSP_MAPMIXEDELEMSIZE enabled
 */
#define DSP_MAPELEMSIZE8            0x00000008
#define DSP_MAPELEMSIZE16           0x00000010
#define DSP_MAPELEMSIZE32           0x00000020
#define DSP_MAPELEMSIZE64           0x00000040

#define DSP_MAPVMALLOCADDR         0x00000080


//  ============================================================================
//  name    PageInfo
//
//  desc    Attributes of L2 page tables for DSP MMU.
//  ============================================================================
struct PageInfo
{
    u32 numEntries;             // Number of valid PTEs in the L2 PT
};



struct pgtbl_initattrb
{
	u32       L1size;           // Size of the L1 PT
	int        L1align;          // Alignment for L1 PT (eg, 12 for 4KB)
	u32       L2NumPages;       // Number of allocated L2 PT
};


//	........................................................................
//	func	PrintPTEs
//
//	desc	Print the DSP MMU Table Entries
//
//	ret 	VOID
//	........................................................................
void PrintPTEs (bool aShowInvEntries, bool aShowRepeatEntries);

u32 initpage_attributes (u32 L1Size, u32 L1Align, u32 L2NumOfPages);
u32 MMUInit(u32 aPhysAddr, int aSize);
u32 MMUDeInit(void);


#endif // _DDUCATIMMU_ENABLER_H_


