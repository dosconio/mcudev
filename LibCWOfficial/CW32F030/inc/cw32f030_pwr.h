/**
 * @file cw32f030_pwr.h
 * @author P&S (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-06-21
 *
 * @copyright Copyright (c) 2021
 *
 */
/*******************************************************************************
*
* �������ɺ�������Ϣ
* �人��Դ�뵼�����޹�˾������ʹ�����б�̴���ʾ���ķ�ר���İ�Ȩ���ɣ��������ɴ�
* ���ɸ��������ض���Ҫ�����Ƶ����ƹ��ܡ����ݲ��ܱ��ų����κη�����֤���人��Դ��
* �������޹�˾������򿪷��̺͹�Ӧ�̶Գ������֧�֣�����У����ṩ�κ���ʾ��
* ���ı�֤�������������������ڰ������й������ԡ�������ĳ���ض���;�ͷ���Ȩ�ı�֤
* ��������
* ���ۺ������Σ��人��Դ�뵼�����޹�˾������򿪷��̻�Ӧ�̾��������и����
* ��ʹ����֪�䷢���Ŀ�����ʱ��Ҳ����ˣ����ݵĶ�ʧ���𻵣�ֱ�ӵġ��ر�ġ�������
* ���ӵ��𺦣����κκ���Ծ����𺦣�������ҵ�����롢������Ԥ�ڿɽ�ʡ����
* ��ʧ��
* ĳЩ˾��Ͻ����������ֱ�ӵġ������Ļ����Ե������κε��ų������ƣ����ĳЩ��
* ȫ�������ų������ƿ��ܲ�������������
*
*******************************************************************************/

#ifndef __CW32F030_PWR_H
#define __CW32F030_PWR_H

/*****************************************************************************
 * Include files
 *****************************************************************************/
#include "base_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup LpmGroup Low Power Management (LPM)
 **
 **
 ******************************************************************************/
//@{
    
/******************************************************************************
 ** Global pre-processor symbols/macros ('#define')                            
 ******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/** @defgroup PWR_Sevonpend
  * @{
  */
#define PWR_Sevonpend_Disable		((uint32_t)0x00000000)
#define PWR_Sevonpend_Enable		((uint32_t)0x00000010)

#define IS_PWR_SEVONPEND(SEVONPEND) (((SEVONPEND) == PWR_Sevonpend_Disable) || \
                           	   	   	 ((SEVONPEND) == PWR_Sevonpend_Enable))

/** @defgroup PWR_SleepDeep
  * @{
  */
#define PWR_SleepDeep_Disable		((uint32_t)0x00000000)
#define PWR_SleepDeep_Enable		((uint32_t)0x00000004)

#define IS_PWR_SLEEPDEEP(SLEEPDEEP) (((SLEEPDEEP) == PWR_SleepDeep_Disable) || \
                           	   	   	 ((SLEEPDEEP) == PWR_SleepDeep_Enable))

/** @defgroup PWR_SleepOnExit
  * @{
  */
#define PWR_SleepOnExit_Disable		((uint32_t)0x00000000)
#define PWR_SleepOnExit_Enable		((uint32_t)0x00000002)

#define IS_PWR_SLEEPONEXIT(SLEEPONEXIT) (((SLEEPONEXIT) == PWR_SleepOnExit_Disable) || \
                           	   	   	   	 ((SLEEPONEXIT) == PWR_SleepOnExit_Enable))

/**
 ******************************************************************************
 ** @brief Low Power Mode ���ýṹ�嶨��
 *****************************************************************************/
typedef struct
{
    uint32_t   PWR_Sevonpend;        // ʹ��:ÿ�����жϻ����һ���¼�,���ʹ��WFE����,������ڻ��Ѵ�����.
    uint32_t   PWR_SleepDeep;        // ʹ��:ִ��WFI�����������;��ʹ��:ִ��WFI��������.
    uint32_t   PWR_SleepOnExit;      // ʹ��:�˳��쳣����������ʱ,�������Զ���������ģʽ;��ʹ��:�����Խ�ֹ.
}PWR_InitTypeDef;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
//�������ü���������
//�͹���ģʽ����
void PWR_Config(PWR_InitTypeDef* PWR_InitTypeDef);
//����͹���ģʽ
void PWR_GotoLpmMode(void);

//@} // PWRGroup

#ifdef __cplusplus
#endif


#endif /* __CW32F030_PWR_H */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/

