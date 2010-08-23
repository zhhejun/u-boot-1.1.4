/************************************************************************
 *                                                                      *
 *                          ETH_FunctionList.h                                  	*
 *                                                                      *
 *  Project Code:       Hi3560                                       	*
 *  Create Date:        2005/1/5                                      	*
 *  Author:             	tangbenbing                                     *
 *  Modify Date:        2005/5/5                                                *
 *  Document:                                                           *
 *  Function:                                                           *
 *  Others:                                                             *
 *----------------------------------------------------------------------*
 *                                                                      *
 * Copyright 2005-20** Hi3560 Team HuaWei Tech, Inc.  					*
 *                     ALL RIGHTS RESERVED                              *
 *                                                                      *
 *----------------------------------------------------------------------*
 ************************************************************************/

#ifndef ETH_FUNCTION_LIST_H
#define ETH_FUNCTION_LIST_H
extern void ETH_SetInterruptEnable(In UINT32 u32InterruptEn);
extern UINT32 ETH_ReadInterruptEnable(Out UINT32 *pu32InterruptEnAddr);
extern UINT32 ETH_ReadInterruptStatus(Out UINT32 *pu32InterruptMode);
extern UINT32 ETH_ClearInterruptStatus(In UINT32 u32InterruptClear);
extern void ETH_SetNormalInterruptStyle(In UINT32 u32InterruptNorChoi);
//extern UINT32 ETH_ReadSoftResetFinishFlag (Out UINT32 *pu32ResetFiniInf);
//extern UINT32 ETH_SoftResetPort(In UINT32 u32PortReset);
//extern void ETH_SetPortRMIIOrMII (In UINT32 u32MIIOrRMIIMode);
extern void ETH_SetEndian (In UINT32 u32InEndian, In UINT32 u32OutEndian);
extern UINT32 ETH_SetFrameQueueLength (In UINT32 u32InputLength, In UINT32 u32OutputLength);//
extern UINT32 ETH_SetPauseFrameLimitLevel (In UINT32 u32EnablePauseUpLimit, In UINT32 u32DisablePauseDownLimit);
//extern void ETH_ClearFrameQueue (In UINT32 u32CPUCleQue) ;
//extern void ETH_SetQueueAddress (In UINT32 u32InputQueueAddr,In UINT32 u32OutputQueueAddr) ;
//extern void ETH_SetRetryTimes (In UINT32 u32RetryTimes) ;
extern void ETH_ReadQueueUseLength (Out UINT32 *pu32InputQueue,Out UINT32 *pu32OutputQueue) ;
//extern void ETH_StatisticFiltFrameNumber (Out UINT32 *pu32FiltFrameCnt) ;
extern void ETH_SetFrameConfiguration (In UINT32 u32PreCodNo, In UINT32 u32FraCle, In UINT32 u32StrFraDist);
extern void ETH_SetPortStatusMode(In UINT32 u32PortStatMode);
extern void ETH_SetSpeedLinkDuplexMode(In UINT32 u32Speed, In UINT32 u32Link, In UINT32 u32Duplex);
extern void ETH_ReadSpeedLinkDuplexMode(Out UINT32 *u32Speed, Out UINT32 *u32Link, Out UINT32 *u32Duplex);
extern void ETH_ClearSpeedLinkDuplexMode(In UINT32 u32Speed, In UINT32 u32Link, In UINT32 u32Duplex);
extern void ETH_ClearPortCountInformation( In UINT32 u32Value);
extern void ETH_SetPortCntCleanEnable(In UINT32 u32CntAuCleEn);
extern void ETH_SetPortShortFrameReceive(In UINT32 u32ShorReceEn);
extern void ETH_SetPortPostFrameSend(In UINT32 u32PostFraSenEn);
extern void ETH_SetPortCountRollCtrl( In UINT32 u32Value) ;
//extern void ETH_ReadFrameCRCErrorFlag (Out UINT32 *u32RecFraCRCErr, Out UINT32 *u32TraFraCRCErr);
extern void ETH_SetFrameCRCCtlRebuild ( In UINT32 u32CRCRebuild);
extern void ETH_SetLongFrameLength( In UINT32 u32Value);
extern void ETH_SetShortFrameLength(In UINT32 u32Value);
extern void ETH_SetLoopBackEnable(In UINT32 u32ExLoop, In UINT32 u32InLoop);
extern void ETH_SetPHYExternAddress(In UINT32 u32PHYAddr)   ;
extern void ETH_SetPHYFrequency(In UINT32 u32PHYFreq)  ;
void ETH_SetPHYChipInformationAddress(In UINT16 u16PortNo, 
	In UINT32 u32PHYSpeInfAddr, In UINT32 u32PHYLinInfAddr, In UINT32 u32PHYDupInfAddr, 
	In UINT32 u32PHYSpeOffsAddr, 	In UINT32 u32PHYLinOffsAddr, In UINT32 u32PHYDupOffsAddr);
extern void ETH_ReadPHYPortInformation( Out UINT32 *pu32PHYPortSpeed, 
									Out UINT32 *pu32PHYPortLink, Out UINT32 *pu32PHYPortDupMode) ;

extern UINT32 ETH_PhyWrite(In UINT32 u32PhyRegIndex , In UINT32 u32PhyData );
extern UINT32 ETH_PhyRead(In UINT32 u32PhyRegIndex , Out UINT32 *pu32PhyDataAddr );
//extern void ETH_SetMACFiltEnable(In UINT32 u32FilterEnable)  ;
//extern void ETH_SetOutMACFiltEnable(In UINT32 u32OutFilterEnable)  ;
extern UINT32 ETH_AddMACFiltFormContent(In UINT32 u32Index, In UINT32 u32MACLow, In UINT32 u32MACMiddle, In UINT32 u32MACHigh)  ;
extern UINT32 ETH_ReadMACFiltFormContent(In UINT32 u32Index, Out UINT32 *pu32MACLow, Out UINT32 *pu32MACMiddle, Out UINT32 *pu32MACHigh)  ;
//extern void ETH_SetInMACFiltCtrl(In UINT32 u32ReceiveCtrl)  ;
//extern void ETH_SetMACFiltCtrlGroup(In UINT32 u32CtrlEnable,In UINT32 u32ReceiveCtrl)  ;
//extern void ETH_SetMACFiltCtrlSingle(In UINT32 u32CtrlEnable,In UINT32 u32ReceiveCtrl)  ;
//extern void ETH_SetMACFiltCtrlBroadcast(In UINT32 u32CtrlEnable,In UINT32 u32ReceiveCtrl)  ;
//extern void ETH_ReadResendFrameDescribe(Out UINT32 *pu32ResendFrameDescribe)  ;
//extern UINT32 ETH_FrameReceive (Out UINT8 *pu8FrameReceiveData ,Out UINT16 *pu16Length) ;
//extern UINT32 ETH_FrameTransmit (In UINT16 u16FrameLen, In UINT8 *pu8FrameTransmitData) ;



//extern void transmit_frame_addr(void);
//extern void ETH_SetioutQueuelen(In UINT32 in_len,In UINT32 out_len);
//extern UINT32 ETH_FrameTransmitPacket (In UINT32 u32FrameLen, In UINT32 * pu32FrameTransmitAddr) ;
//extern UINT32 ETH_FrameReceivePacket (Out UINT32 *pu32FrameReceiveDataAddr ,Out UINT32 pu32Length);
extern UINT32 ETH_Init();
extern void TestCpuSentFrame(In UINT32 u32SendNumber,In UINT16 u16FrameLen, In UINT8 *pu8FrameTransmitData);

extern void ETH_SetMACFiltCtrl_destination(In UINT32 multi_ctrl,In UINT32 uni_ctrl,In UINT32 broad_ctrl);
extern void ETH_SetInMACFiltenable(In UINT32 mac1_en,In UINT32 mac0_en,In UINT32 local_mac_en) ;
extern void ETH_SetInMACFiltCtrl_local(In UINT32 mac1_ctrl,In UINT32 mac0_ctrl,In UINT32 local_mac_ctrl) ; 

#endif




