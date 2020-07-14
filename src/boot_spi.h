/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#ifndef __BOOT_SPI_H__
#define __BOOT_SPI_H__


#include "netx_io_areas.h"


#define MSK_SQI_CFG_IDLE_IO1_OE         0x01
#define SRT_SQI_CFG_IDLE_IO1_OE         0
#define MSK_SQI_CFG_IDLE_IO1_OUT        0x02
#define SRT_SQI_CFG_IDLE_IO1_OUT        1
#define MSK_SQI_CFG_IDLE_IO2_OE         0x04
#define SRT_SQI_CFG_IDLE_IO2_OE         2
#define MSK_SQI_CFG_IDLE_IO2_OUT        0x08
#define SRT_SQI_CFG_IDLE_IO2_OUT        3
#define MSK_SQI_CFG_IDLE_IO3_OE         0x10
#define SRT_SQI_CFG_IDLE_IO3_OE         4
#define MSK_SQI_CFG_IDLE_IO3_OUT        0x20
#define SRT_SQI_CFG_IDLE_IO3_OUT        5



typedef enum ENUM_SPI_MODE
{
	SPI_MODE0 = 0,
	SPI_MODE1 = 1,
	SPI_MODE2 = 2,
	SPI_MODE3 = 3
} SPI_MODE_T;


typedef enum SPI_BUS_WIDTH_ENUM
{
	SPI_BUS_WIDTH_1BIT = 0,
	SPI_BUS_WIDTH_2BIT = 1,
	SPI_BUS_WIDTH_4BIT = 2
} SPI_BUS_WIDTH_T;


typedef struct STRUCT_BOOT_SPI_CONFIGURATION
{
	unsigned long ulSpeedFifoKhz;
	unsigned long ulSpeedSqiRomKhz;
	unsigned short ausPortControl[6];
	unsigned char aucMmio[6];
	unsigned char ucDummyByte;
	unsigned char ucMode;
	unsigned char ucIdleConfiguration;
} BOOT_SPI_CONFIGURATION_T;


/* predef for the functions */
struct STRUCT_SPI_CFG;

typedef int (*PFN_SPI_INIT)(struct STRUCT_SPI_CFG *psCfg, const BOOT_SPI_CONFIGURATION_T *ptSpiCfg, unsigned int uiSpiUnit, unsigned int uiChipSelect);

typedef void (*PFN_SPI_SLAVE_SELECT_T)(const struct STRUCT_SPI_CFG *psCfg, int fIsSelected);

typedef unsigned char (*PFN_EXCHANGE_BYTE_T)(const struct STRUCT_SPI_CFG *ptCfg, unsigned char uiByte);
typedef int (*PFN_SEND_IDLE_CYCLES_T)(const struct STRUCT_SPI_CFG *psCfg, unsigned int sizIdleCycles);
typedef int (*PFN_SEND_DUMMY_T)(const struct STRUCT_SPI_CFG *psCfg, unsigned int sizDummyChars);
typedef int (*PFN_SEND_DATA_T)(const struct STRUCT_SPI_CFG *ptCfg, const unsigned char *pucData, unsigned int sizData);
typedef int (*PFN_RECEIVE_DATA_T)(const struct STRUCT_SPI_CFG *ptCfg, unsigned char *pucData, unsigned int sizData);
typedef int (*PFN_EXCHANGE_DATA_T)(const struct STRUCT_SPI_CFG *ptCfg, unsigned char *pucData, unsigned int sizData);

typedef int (*PFN_SET_NEW_SPEED_T)(const struct STRUCT_SPI_CFG *psCfg, unsigned long ulDeviceSpecificSpeed);
typedef unsigned long (*PFN_GET_DEVICE_SPEED_REPRESENTATION_T)(unsigned int uiSpeed);
typedef void (*PFN_RECONFIGURE_IOS_T)(const struct STRUCT_SPI_CFG *psCfg);

typedef int (*PFN_SET_BUS_WIDTH_T)(struct STRUCT_SPI_CFG *psCfg, SPI_BUS_WIDTH_T tBusWidth);
typedef unsigned long (*PFN_GET_DEVICE_SPECIFIC_SQIROM_CFG_T)(struct STRUCT_SPI_CFG *psCfg, unsigned int uiDummyCycles, unsigned int uiAddrBits, unsigned int uiAddressNibbles);
typedef int (*PFN_ACTIVATE_SQIROM_T)(struct STRUCT_SPI_CFG *psCfg, unsigned long ulSettings);
typedef int (*PFN_DEACTIVATE_SQIROM_T)(struct STRUCT_SPI_CFG *psCfg);

typedef void (*PFN_DEACTIVATE_T)(const struct STRUCT_SPI_CFG *psCfg);


typedef struct STRUCT_SPI_CFG
{
	void *pvArea;                   /* A pointer to the SQI/SPI unit. */
	void *pvSqiRom;                 /* A pointer to the SQI ROM area. NULL if not available. */
	unsigned long ulSpeedFifoKhz;   /* The device speed in kHz. */
	unsigned long ulSpeedSqiRomKhz; /* The device speed for the SQI ROM mode in kHz. */
	SPI_MODE_T tMode;               /* The bus mode. */
	unsigned int uiUnit;            /* The unit number. */
	unsigned int uiChipSelect;      /* The chip select index. */

	PFN_SPI_SLAVE_SELECT_T pfnSelect;
	PFN_EXCHANGE_BYTE_T pfnExchangeByte;
	PFN_SEND_IDLE_CYCLES_T pfnSendIdleCycles;
	PFN_SEND_DUMMY_T pfnSendDummy;
	PFN_SEND_DATA_T pfnSendData;
	PFN_RECEIVE_DATA_T pfnReceiveData;
	PFN_EXCHANGE_DATA_T pfnExchangeData;
	PFN_SET_NEW_SPEED_T pfnSetNewSpeed;
	PFN_GET_DEVICE_SPEED_REPRESENTATION_T pfnGetDeviceSpeedRepresentation;
	PFN_RECONFIGURE_IOS_T pfnReconfigureIos;
	PFN_SET_BUS_WIDTH_T pfnSetBusWidth;
	PFN_GET_DEVICE_SPECIFIC_SQIROM_CFG_T pfnGetDeviceSpecificSqiRomCfg;
	PFN_ACTIVATE_SQIROM_T pfnActivateSqiRom;
	PFN_DEACTIVATE_SQIROM_T pfnDeactivateSqiRom;
	PFN_DEACTIVATE_T pfnDeactivate;

	unsigned char ucDummyByte;        /* This byte is transfered in the "SendDummy" function. */
	unsigned int uiIdleConfiguration;
	unsigned long ulTrcBase;          /* the base bits of the transfer control register */
	unsigned short ausPortControl[6];
	unsigned char aucMmio[6];         /* MMIO pins */
} SPI_CFG_T;

#if ASIC_TYP!=ASIC_TYP_NETX500
void boot_spi_activate_mmio(const SPI_CFG_T *ptCfg, const HOSTMMIODEF *ptMmioValues);
void boot_spi_deactivate_mmio(const SPI_CFG_T *ptCfg, const HOSTMMIODEF *ptMmioValues);
#endif

#endif  /* __BOOT_SPI_H__ */

