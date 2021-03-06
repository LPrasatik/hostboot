/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/hwpf/hwp/build_winkle_images/proc_mailbox_utils/p8_mailbox_utils.H $ */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2013,2014                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
// -*- mode: C++; c-file-style: "linux";  -*-
// $Id: p8_mailbox_utils.H,v 1.2 2014/02/26 04:58:11 jmcgill Exp $
// $Source: /afs/awd/projects/eclipz/KnowledgeBase/.cvsroot/eclipz/chips/p8/working/procedures/ipl/fapi/p8_mailbox_utils.H,v $
//------------------------------------------------------------------------------
// *|
// *! (C) Copyright International Business Machines Corp. 2012
// *! All Rights Reserved -- Property of IBM
// *! ***  ***
// *|
// *! TITLE       : proc_mailbox_utils.H
// *! DESCRIPTION : Functions to calculate the mailbox values
// *!
// *! OWNER NAME  : Jeshua Smith            Email: jeshua@us.ibm.com
// *! BACKUP NAME : TBD                     Email: TBD@us.ibm.com
// *!
// *! Overview:
// *!    Utility functions to calculate each mailbox value
//------------------------------------------------------------------------------

#ifndef _P8_MAILBOX_UTILS_H_
#define _P8_MAILBOX_UTILS_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <fapi.H>

//------------------------------------------------------------------------------
// Structure definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------


extern "C"
{


//------------------------------------------------------------------------------
// function:
//      Translate a VRM-11 VID code to a voltage value
//
//
// parameters: i_vid_7_0    VRM-11 VID code
//              o_voltage   Voltage in .01mv units
//
// returns: FAPI_RC_SUCCESS if operation was successful, else error
//------------------------------------------------------------------------------
fapi::ReturnCode vid2mv(uint8_t i_vid_7_0, uint32_t &o_voltage);

//------------------------------------------------------------------------------
// function:
//      set up sbe configuration values in mbox scratch reg 1
//      (standalone_mbox0_value)
//
//  Mailbox scratch 1 (CFAM 2838, SCOM 0x50038)
//
//  Bytes 0,1   Boot frequency
//  Boot Frequency info (power management def file DPS min (% drop from
//  nominal), must cross checking between f_vmin and DPS min)
//  This is a multiplier of the processor refclk frequency based on the
//  the DPLL DIVIDER.
//
//  Bytes 2,3 EX Gard records
//  FSP provides a vector for SBE to communicate the guareded EX chiplets
//  Bits    0..3    4..7    8..11   12..15  16..19  20..23  24..27  28..31
//                  0x0000                          EX guard bits
//  One Guard bit per EX chiplet, bit location aligned to chiplet ID
//  (bit 16: EX00, bit 17: EX01, bit 18: EX02 ... bit 31: EX15)
//  Guarded EX chiplets are marked by a '0'.
//
// parameters: i_target           Reference to the processor chip target
//             o_set_data         The 32-bit mailbox value
// returns: FAPI_RC_SUCCESS if operation was successful, else error
//------------------------------------------------------------------------------
fapi::ReturnCode p8_mailbox_utils_get_mbox1( const fapi::Target &i_target, uint32_t & o_set_data );

//------------------------------------------------------------------------------
// function:
//      set up sbe configuration values in mbox scratch reg 2
//      (standalone_mbox1_value)
//
//  Bit 0 in this register is used to indicate a MPIPL
//  - The MPI flag will be evaluated by proc_sbe_ipl_seeprom to distinguish
//    between a normal and a memory preserving IPL
//  - attribute ATTR_IS_MPIPL will indicate MPIPL or not.
//  Bit 1 in this register indicates standalone/FSPless mode
//  - 0b0 = FSP present, 0b1 = standalone/FSPless mode
//  FSP provides a MPI (Memory Preserving IPL) flag and settings for the I2C
//  master bus speed calculation
//  Bits    | 0     |   1 2 3   |   4..7    8..11   12..15                    |
//          | MPI   |   000     | PIB I2C master Bit Rate Divisor (@refclock) |
//
//          |    16..19             20..23  24..27  28..31                    |
//          |               PIB I2C master Bit Rate Divisor (@PLL)            |
//
// parameters: i_target           Reference to the chip target
//             o_set_data         The 32-bit mailbox value
// returns: FAPI_RC_SUCCESS if operation was successful, else error
//------------------------------------------------------------------------------
fapi::ReturnCode p8_mailbox_utils_get_mbox2( const fapi::Target &i_target, uint32_t & o_set_data );

//------------------------------------------------------------------------------
// function:
//      set up sbe configuration values in mbox scratch reg 3
//      (standalone_mbox2_value)
//
//  32bit address of location of Hostboot image header (first block of data)
//  This is offset using an algorithm to compensate for ECC -
//  see Feature 862671 (fips810): hb pnor offset
//
// parameters: i_target           Reference to the chip target
//             o_set_data         The 32-bit mailbox value
// returns: FAPI_RC_SUCCESS if operation was successful, else error
//------------------------------------------------------------------------------
fapi::ReturnCode p8_mailbox_utils_get_mbox3( const fapi::Target &i_target, uint32_t & o_set_data );

//------------------------------------------------------------------------------
// function:
//      set up sbe configuration values in mbox scratch reg 4
//
//  Write Boot Voltage info to scratch pad 4
//
//  0:2   -> port enables (3b - system design based:
//          port 0 for non-redundant systems (100); all ports for non-redundant (111))
//  3     -> Unused
//  - current recommended default = 1000b
//  4:7   -> phase enables (4b - defined by the system power design)
//  - current recommended default = 0000b
//  8:15  -> VDD voltage (1B in VRM-11 encoded form - 6.25mV increments)
//          note: VPD is in 5mV increments
//  - current recommended default   =   0x52
//  16:23 -> VCS voltage (1B in VRM-11 encoded form - 6.25mV increments)
//          note: VPD is in 5mV increments
//  -current recommended default    =   0x4a
//  24:27 -> Unused                 =   0x00
//  28    -> Fabric wrap test       =   MNFG wrap test attribute
//  29:31 -> Fabric node ID         =   Node ID attribute
//
// parameters: i_target           Reference to the chip target
//             o_set_data         The 32-bit mailbox value
//             i_write_fbc_data   True if the mailbox value should include fabric wrap
//                                test/node ID information                             
// returns: FAPI_RC_SUCCESS if operation was successful, else error
//------------------------------------------------------------------------------
fapi::ReturnCode p8_mailbox_utils_get_mbox4( const fapi::Target &i_target, uint32_t & o_set_data,
                                             bool i_write_fbc_data);

} // extern "C"


#endif // _P8_MAILBOX_UTILS_H_

