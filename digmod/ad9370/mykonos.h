/*!
 * \file mykonos.h
 * \brief Contains macro definitions and function prototypes for mykonos.c
 */

#ifndef _MYKONOS_LIB_H_
#define _MYKONOS_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "t_mykonos.h"

#define TX_PROFILE_VALID    0x01
#define RX_PROFILE_VALID    0x02
#define ORX_PROFILE_VALID   0x04
#define SNIFF_PROFILE_VALID 0x08

/* 
 ****************************************************************************
 * General Initialization functions 
 ****************************************************************************
 */
mykonosErr_t MYKONOS_resetDevice(mykonosDevice_t *device);
mykonosErr_t MYKONOS_getDeviceRev(mykonosDevice_t *device, uint8_t *revision);
mykonosErr_t MYKONOS_setSpiSettings(mykonosDevice_t *device);
mykonosErr_t MYKONOS_verifyDeviceDataStructure(mykonosDevice_t *device);
mykonosErr_t MYKONOS_initialize(mykonosDevice_t *device); 
mykonosErr_t MYKONOS_waitForEvent(mykonosDevice_t *device, waitEvent_t waitEvent, uint32_t timeout_us);
mykonosErr_t MYKONOS_readEventStatus(mykonosDevice_t *device, waitEvent_t waitEvent, uint8_t *eventDone);
/*
 *****************************************************************************
 * PLL / Synthesizer functions
 *****************************************************************************
 */
mykonosErr_t MYKONOS_initDigitalClocks(mykonosDevice_t *device);
mykonosErr_t MYKONOS_setRfPllFrequency(mykonosDevice_t *device, mykonosRfPllName_t pllName, u64 rfPllLoFrequency_Hz);
mykonosErr_t MYKONOS_getRfPllFrequency(mykonosDevice_t *device, mykonosRfPllName_t pllName, u64 *rfPllLoFrequency_Hz);
mykonosErr_t MYKONOS_checkPllsLockStatus(mykonosDevice_t *device, uint8_t *pllLockStatus);

/*
 *****************************************************************************
 * ARM Processor Control Functions
 *****************************************************************************
 */
mykonosErr_t MYKONOS_initArm(mykonosDevice_t *device);
mykonosErr_t MYKONOS_loadArmFromBinary(mykonosDevice_t *device, uint8_t *binary, uint32_t count);
mykonosErr_t MYKONOS_verifyArmChecksum(mykonosDevice_t *device);
mykonosErr_t MYKONOS_getArmVersion(mykonosDevice_t *device, uint8_t *majorVer, uint8_t *minorVer, uint8_t *rcVer);

mykonosErr_t MYKONOS_configDpd(mykonosDevice_t *device);
mykonosErr_t MYKONOS_getDpdConfig(mykonosDevice_t *device);
mykonosErr_t MYKONOS_getDpdStatus(mykonosDevice_t *device, mykonosTxChannels_t txChannel, mykonosDpdStatus_t *dpdStatus);

mykonosErr_t MYKONOS_configClgc(mykonosDevice_t *device);
mykonosErr_t MYKONOS_getClgcConfig(mykonosDevice_t *device);
mykonosErr_t MYKONOS_getClgcStatus(mykonosDevice_t *device, mykonosTxChannels_t txChannel, mykonosClgcStatus_t *clgcStatus);

mykonosErr_t MYKONOS_runInitCals(mykonosDevice_t *device, uint32_t calMask);
mykonosErr_t MYKONOS_waitInitCals(mykonosDevice_t *device, uint32_t timeoutMs, uint8_t *errorFlag, uint8_t *errorCode);
mykonosErr_t MYKONOS_abortInitCals(mykonosDevice_t *device, uint32_t *calsCompleted);
mykonosErr_t MYKONOS_getInitCalStatus(mykonosDevice_t *device, mykonosInitCalStatus_t *initCalStatus);

mykonosErr_t MYKONOS_radioOn(mykonosDevice_t *device);
mykonosErr_t MYKONOS_radioOff(mykonosDevice_t *device);
mykonosErr_t MYKONOS_getRadioState(mykonosDevice_t *device, uint32_t *radioStatus);
mykonosErr_t MYKONOS_enableTrackingCals(mykonosDevice_t *device, uint32_t enableMask);
mykonosErr_t MYKONOS_getEnabledTrackingCals(mykonosDevice_t *device, uint32_t *enableMask);
mykonosErr_t MYKONOS_getPendingTrackingCals(mykonosDevice_t *device, uint32_t *pendingCalMask);

mykonosErr_t MYKONOS_setSnifferChannel(mykonosDevice_t *device, mykonosSnifferChannel_t snifferChannel);

/* Low level ARM functions */
mykonosErr_t MYKONOS_readArmMem(mykonosDevice_t *device, uint32_t address, uint8_t *returnData, uint32_t bytesToRead, uint8_t autoIncrement);
mykonosErr_t MYKONOS_writeArmMem(mykonosDevice_t *device, uint32_t address, uint8_t *data, uint32_t byteCount);
mykonosErr_t MYKONOS_writeArmConfig(mykonosDevice_t *device, uint8_t objectId, uint16_t offset, uint8_t *data, uint8_t byteCount);
mykonosErr_t MYKONOS_readArmConfig(mykonosDevice_t *device, uint8_t objectId, uint16_t offset, uint8_t *data, uint8_t byteCount);
mykonosErr_t MYKONOS_sendArmCommand(mykonosDevice_t *device, uint8_t opCode,  uint8_t *extendedData, uint8_t extendedDataNumBytes);
mykonosErr_t MYKONOS_readArmCmdStatus(mykonosDevice_t *device, uint16_t *errorWord, uint16_t *statusWord);
mykonosErr_t MYKONOS_readArmCmdStatusByte(mykonosDevice_t *device, uint8_t opCode, uint8_t *cmdStatByte);
mykonosErr_t MYKONOS_waitArmCmdStatus(mykonosDevice_t *device, uint8_t opCode, uint32_t timeoutMs, uint8_t *cmdStatByte);
mykonosErr_t MYKONOS_writeArmProfile(mykonosDevice_t *device);
mykonosErr_t MYKONOS_loadAdcProfiles(mykonosDevice_t *device);


/*
 *****************************************************************************
 * JESD204B Datapath Functions
 *****************************************************************************
 */
/* Functions to setup the JESD204B IP */
mykonosErr_t MYKONOS_resetDeframer(mykonosDevice_t *device);
mykonosErr_t MYKONOS_setupSerializers(mykonosDevice_t *device);
mykonosErr_t MYKONOS_setupDeserializers(mykonosDevice_t *device);
mykonosErr_t MYKONOS_setupJesd204bFramer(mykonosDevice_t *device);
mykonosErr_t MYKONOS_setupJesd204bObsRxFramer(mykonosDevice_t *device);
mykonosErr_t MYKONOS_setupJesd204bDeframer(mykonosDevice_t *device);
mykonosErr_t MYKONOS_enableRxFramerLink(mykonosDevice_t *device, uint8_t enable);
mykonosErr_t MYKONOS_enableObsRxFramerLink(mykonosDevice_t *device, uint8_t enable);

/* Functions to initialize the JESD204B link */
mykonosErr_t MYKONOS_enableMultichipSync(mykonosDevice_t *device, uint8_t enableMcs, uint8_t *mcsStatus);
mykonosErr_t MYKONOS_enableSysrefToRxFramer(mykonosDevice_t *device, uint8_t enable);
mykonosErr_t MYKONOS_enableSysrefToObsRxFramer(mykonosDevice_t *device, uint8_t enable);
mykonosErr_t MYKONOS_enableSysrefToDeframer(mykonosDevice_t *device, uint8_t enable);

/* Functions to help debug the JESD204B link */
mykonosErr_t MYKONOS_readRxFramerStatus(mykonosDevice_t *device, uint8_t *framerStatus);
mykonosErr_t MYKONOS_readOrxFramerStatus(mykonosDevice_t *device, uint8_t *obsFramerStatus);
mykonosErr_t MYKONOS_readDeframerStatus(mykonosDevice_t *device, uint8_t *deframerStatus);
mykonosErr_t MYKONOS_getDeframerFifoDepth(mykonosDevice_t *device, uint8_t *fifoDepth, uint8_t *readEnLmfcCount);

/* PRBS debug functions */
mykonosErr_t MYKONOS_enableRxFramerPrbs(mykonosDevice_t *device, mykonosPrbsOrder_t polyOrder, uint8_t enable);
mykonosErr_t MYKONOS_enableObsRxFramerPrbs(mykonosDevice_t *device, mykonosPrbsOrder_t polyOrder, uint8_t enable);
mykonosErr_t MYKONOS_rxInjectPrbsError(mykonosDevice_t *device);
mykonosErr_t MYKONOS_obsRxInjectPrbsError(mykonosDevice_t *device);
mykonosErr_t MYKONOS_enableDeframerPrbsChecker(mykonosDevice_t *device, uint8_t lanes, mykonosPrbsOrder_t polyOrder, uint8_t enable);
mykonosErr_t MYKONOS_clearDeframerPrbsCounters(mykonosDevice_t *device);
mykonosErr_t MYKONOS_readDeframerPrbsCounters(mykonosDevice_t *device, uint8_t counterSelect, uint32_t *prbsErrorCount);

/* Miscellaneous debug functions */
mykonosErr_t MYKONOS_jesd204bIlasCheck(mykonosDevice_t *device, uint16_t *mismatch);
mykonosErr_t MYKONOS_setRxFramerDataSource(mykonosDevice_t *device, uint8_t dataSource);
mykonosErr_t MYKONOS_setObsRxFramerDataSource(mykonosDevice_t *device, uint8_t dataSource);

/*
 *****************************************************************************
 * Shared Data path functions
 *****************************************************************************
 */
mykonosErr_t MYKONOS_programFir(mykonosDevice_t *device, mykonosfirName_t filterToProgram, mykonosFir_t *firFilter);

/*
 *****************************************************************************
 * Rx Data path functions
 *****************************************************************************
 */
mykonosErr_t MYKONOS_programRxGainTable(mykonosDevice_t *device, uint8_t *gainTablePtr, uint8_t numGainIndexesInTable, mykonosGainTable_t rxChannel);
mykonosErr_t MYKONOS_setRx1ManualGain(mykonosDevice_t *device, uint8_t gainIndex);
mykonosErr_t MYKONOS_setRx2ManualGain(mykonosDevice_t *device, uint8_t gainIndex);
mykonosErr_t MYKONOS_getRx1Gain(mykonosDevice_t *device, uint8_t *rx1GainIndex);
mykonosErr_t MYKONOS_getRx2Gain(mykonosDevice_t *device, uint8_t *rx2GainIndex);
mykonosErr_t MYKONOS_setupRxAgc(mykonosDevice_t *device);
mykonosErr_t MYKONOS_enableRxGainCtrSyncPulse(mykonosDevice_t *device, uint8_t enable);
mykonosErr_t MYKONOS_setRxGainControlMode(mykonosDevice_t *device, mykonosGainMode_t mode);
mykonosErr_t MYKONOS_getRx1DecPower(mykonosDevice_t *device, uint16_t *rx1DecPower_mdBFS);
mykonosErr_t MYKONOS_getRx2DecPower(mykonosDevice_t *device, uint16_t *rx2DecPower_mdBFS);

/*
 *****************************************************************************
 * Observation Rx Data path functions
 *****************************************************************************
 */
mykonosErr_t MYKONOS_setDefaultObsRxPath(mykonosDevice_t *device,  mykonosObsRxChannels_t defaultObsRxCh);
mykonosErr_t MYKONOS_setObsRxPathSource(mykonosDevice_t *device, mykonosObsRxChannels_t obsRxCh);
mykonosErr_t MYKONOS_getObsRxPathSource(mykonosDevice_t *device, mykonosObsRxChannels_t *obsRxCh);
mykonosErr_t MYKONOS_setObsRxManualGain(mykonosDevice_t *device, mykonosObsRxChannels_t obsRxCh, uint8_t gainIndex);
mykonosErr_t MYKONOS_getObsRxGain(mykonosDevice_t *device, uint8_t *gainIndex);
mykonosErr_t MYKONOS_setupObsRxAgc(mykonosDevice_t *device);
mykonosErr_t MYKONOS_enableObsRxGainCtrSyncPulse(mykonosDevice_t *device, uint8_t enable);
mykonosErr_t MYKONOS_setObsRxGainControlMode(mykonosDevice_t *device, mykonosGainMode_t mode);
mykonosErr_t MYKONOS_getObsRxDecPower(mykonosDevice_t *device, uint16_t *obsRxDecPower_mdBFS);

/*
 *****************************************************************************
 * Tx Data path functions
 *****************************************************************************
 */
mykonosErr_t MYKONOS_setTx1Attenuation(mykonosDevice_t *device, uint16_t tx1Attenuation_mdB);
mykonosErr_t MYKONOS_setTx2Attenuation(mykonosDevice_t *device, uint16_t tx2Attenuation_mdB);
mykonosErr_t MYKONOS_getTx1Attenuation(mykonosDevice_t *device, uint16_t *tx1Attenuation_mdB);
mykonosErr_t MYKONOS_getTx2Attenuation(mykonosDevice_t *device, uint16_t *tx2Attenuation_mdB);
mykonosErr_t MYKONOS_getTxFilterOverRangeStatus(mykonosDevice_t *device, uint8_t *txFilterStatus);
mykonosErr_t MYKONOS_enableTxNco(mykonosDevice_t *device, uint8_t enable, int32_t tx1ToneFreq_kHz, int32_t tx2ToneFreq_kHz);

/*
 *****************************************************************************
 * PA Protection Functions
 *****************************************************************************
 */
mykonosErr_t MYKONOS_setupPaProtection(mykonosDevice_t *device, uint16_t powerThreshold, uint8_t attenStepSize, uint8_t avgDuration, uint8_t stickyFlagEnable, uint8_t txAttenControlEnable);
mykonosErr_t MYKONOS_enablePaProtection(mykonosDevice_t *device, uint8_t enable);
mykonosErr_t MYKONOS_getDacPower(mykonosDevice_t *device, mykonosTxChannels_t channel, uint16_t *channelPower);
mykonosErr_t MYKONOS_getPaProtectErrorFlagStatus(mykonosDevice_t *device, uint8_t *errorFlagStatus);
mykonosErr_t MYKONOS_clearPaErrorFlag(mykonosDevice_t *device);

/*
 *****************************************************************************
 * Low level functions and helper functions. BBP should not need to call
 *****************************************************************************
 */
const char* getMykonosErrorMessage(mykonosErr_t errorCode);
mykonosErr_t MYKONOS_initSubRegisterTables(mykonosDevice_t *device);
mykonosErr_t MYKONOS_setTxPfirSyncClk(mykonosDevice_t *device); /* Helper function */
mykonosErr_t MYKONOS_setRxPfirSyncClk(mykonosDevice_t *device); /* Helper function */

mykonosErr_t MYKONOS_getRegValue(mykonosDevice_t *device,uint16_t addr, uint8_t *RegValue);

#ifdef __cplusplus
}
#endif

#endif
