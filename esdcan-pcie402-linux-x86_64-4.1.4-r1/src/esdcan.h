/* -*- esdcan-c -*-
 * FILE NAME esdcan.h
 *           copyright 2002 - 2021 by esd electronics gmbh
 *
 */
/************************************************************************
 *
 *  Copyright (c) 2002 - 2021 by esd electronics gmbh
 *
 *  This software is copyrighted by and is the sole property of
 *  esd gmbh.  All rights, title, ownership, or other interests
 *  in the software remain the property of esd gmbh. This
 *  software may only be used in accordance with the corresponding
 *  license agreement.  Any unauthorized use, duplication, transmission,
 *  distribution, or disclosure of this software is expressly forbidden.
 *
 *  This Copyright notice may not be removed or modified without prior
 *  written consent of esd gmbh.
 *
 *  esd gmbh, reserves the right to modify this software without notice.
 *
 *  esd electronics gmbh                   Tel. +49-511-37298-0
 *  Vahrenwalder Strasse 207               Fax. +49-511-37298-68
 *  30165 Hannover                         https://www.esd.eu
 *  Germany                                sales@esd.eu
 *
 *************************************************************************/
/*! \file esdcan.h
 *  \brief Contains CAN_OCB, CAN_NODE-defines and related stuff.
 *
 *  \par General rules:
 *
 */

#ifndef __ESDCAN_H__
#define __ESDCAN_H__

#include <osif.h>
#include <boardrc.h>
#include <cm.h>

#ifndef OSIF_KERNEL
#error "This file may be used in the kernel-context, only! Not for application-use!!!"
#endif

#ifdef NUC_TX_TS
# define TX_TS_WIN_DEF 10
#else
# define TX_TS_WIN_DEF 0
#endif

#ifdef OSIF_USE_PREEMPT_RT_IMPLEMENTATION
 #define TX_STATE_PENDING_WRITE      0x01
 #define TX_STATE_PENDING_SEND       0x02
 #define TX_STATE_ABORT              0x10
 #define TX_STATE_SIGNAL_INTERRUPT   0x20
 #define TX_STATE_CLOSING            0x40

 #define RX_STATE_PENDING_READ       0x01
 #define RX_STATE_PENDING_TAKE       0x02
 #define RX_STATE_ABORT              0x10
 #define RX_STATE_SIGNAL_INTERRUPT   0x20
 #define RX_STATE_CLOSING            0x40
#else
 #define TX_STATE_PENDING_WRITE      0x01
 #define TX_STATE_PENDING_SEND       0x02
 #define TX_STATE_ABORTING           0x80

 #define RX_STATE_PENDING_READ       0x01
 #define RX_STATE_PENDING_TAKE       0x02
 #define RX_STATE_CLOSING            0x40
 #define RX_STATE_ABORTING           0x80
#endif

#define CLOSE_STATE_CLOSE_DONE       0x00000001
#define CLOSE_STATE_HANDLE_CLOSED    0x80000000

typedef struct _CAN_OCB   CAN_OCB;
typedef struct _OSIF_CARD OSIF_CARD;
typedef struct _CAN_NODE  CAN_NODE;
typedef struct _CAN_CARD  CAN_CARD;


struct _CAN_OCB {
        NUC_OCB          *nuc_ocb;
        VOID             *cif_ocb;
        CAN_NODE         *node;
        CM               *rx_cm;
        struct _rx {
                UINT32            tout;
                INT32             result;
                volatile INT32    state;
                UINT32            cm_count;
                UINT32            cm_size;
#ifdef BOARD_CAN_FD
                CAN_MSG_X        *cmbuf;
                CAN_MSG_X        *cmbuf_in;
#else
                CAN_MSG_T        *cmbuf;
                CAN_MSG_T        *cmbuf_in;
#endif
                UINT8            *user_buf;
        } rx;
        struct _tx {
                CM               *cm;
                UINT32            tout;
                INT32             result;
                volatile INT32    state;
                UINT32            cm_count;
                UINT32            cm_size;
                UINT8            *user_buf;
                CM               *cm_buf;
        } tx;
        INT32             minor;
        UINT32            mode;
        UINT32            filter_cmd;
        struct _filter20b {
                UINT32 acr;
                UINT32 amr;
        } filter20b;
        volatile UINT32   close_state;
        struct file      *file;
        /* !!!Leave at end, Linux specific!!! */
#ifdef OSIF_USE_PREEMPT_RT_IMPLEMENTATION
        wait_queue_head_t wqRx;
        wait_queue_head_t wqTx;
        wait_queue_head_t wqCloseDelay;
#else
        struct semaphore  sema_rx;
        struct semaphore  sema_tx;
        struct semaphore  sema_tx_abort; /* uninterruptible */
        struct semaphore  sema_close;
#endif
};


#ifdef BOARD_CAN_FD
#define MAX_NUM_DLC     16      /* number of different length DLC codes */
#else
#define MAX_NUM_DLC     (8+1)   /* number of different length DLC codes */
#endif

typedef struct _CAN_NODE_STAT {
        CAN_BUS_STAT    cbs;            /* struct to interface to NTCAN */
        /* driver internal part */
        uint32_t        msg_count_std[MAX_NUM_DLC];     /*# of 2.0A msgs*/
        uint32_t        msg_count_ext[MAX_NUM_DLC];     /*# of 2.0B msgs*/
} CAN_NODE_STAT;


struct _CAN_NODE {
        NUC_NODE        *nuc_node;
        VOID            *cif_node;
        UINT32          features;    /* flags are ored in layer's attach function */
        UINT32          mode;        /* enabled features       */
        UINT32          status;      /* node status flags (e.g. CANIO_STATUS_WRONG_* + CANIO_STATUS_MASK_BUSSTATE) */
        VOID            *base[4];    /* base[0] = sja address  */
        CAN_CARD        *crd;
        INT32           net_no;      /* counts nets over all cards, globally */
        INT32           node_no;     /* counts nets _per_ card, locally */
        OSIF_MUTEX      lock;
        OSIF_IRQ_MUTEX  lock_irq;
        volatile INT32  irq_flag;
        UINT32          ctrl_clock;  /* frequency of CAN controller */
        UINT8           ctrl_type;   /* type of CAN controller (use CANIO_CANCTL_xxx defines) */
        UINT8           trx_type;    /* Type of CAN transceiver (use CANIO_TRX_XXX defines) */
        UINT32          tx_ts_win;
        CAN_NODE_STAT   can_stat;       /* driver internal node statistic */
        /* !!!Leave at end, Linux specific!!! */
        wait_queue_head_t  wqRxNotify;
};

struct _CAN_CARD {
        VOID            *base[8];
        UINT32          range[8];
        UINT32          irq[8];
        CAN_NODE        *node[NODES_PER_CARD];
        INT32           num_nodes;              /* # of active nodes on this card for NTCAN_INFO */
        INT32           card_no;
        UINT32          features;
        CARD_IRQ        irqs[8];
        VERSION96       version_firmware;
        VERSION96       version_firmware2;      /* for updatable second firmware e.g. Cypress Bootloader USB400    */
                                                /* This is NOT the IRIG FW version which is read via the IRIG lib. */
        VERSION96       version_hardware;
        CARD_IDENT      *pCardIdent;
        UINT32          serial;
        UINT16          board_status;           /* board status flags (wrong firmware/hardware) */
        UINT8           doNotTouch;             /* TRUE if HW must not be touched */
        UINT8           reserved;
#if defined (CIF_CARD)
        /* board specific part (see boardrc.h)... */
        CIF_CARD
#endif
};

#endif
