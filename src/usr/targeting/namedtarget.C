/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/targeting/namedtarget.C $                             */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2012,2014              */
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
/**
 *  @file namedtarget.C
 *
 *  hostBoot - specific filters.   This was removed from ../common because
 *  that source is also compiled into the fsp.
 *
 */

//******************************************************************************
// Includes
//******************************************************************************
#include <targeting/common/commontargeting.H>
#include <targeting/common/entitypath.H>
#include <targeting/common/trace.H>
#include <attributeenums.H>
#include <targeting/common/iterators/rangefilter.H>
#include <targeting/common/predicates/predicateisfunctional.H>
#include <targeting/common/predicates/predicatepostfixexpr.H>
#include <targeting/common/utilFilter.H>
#include <targeting/namedtarget.H>

#include <sys/task.h>           // task_getcpuid()


/**
 * Miscellaneous Filter Utility Functions
 */

namespace TARGETING
{

const TARGETING::Target *   getMasterCore( )
{
    task_affinity_pin();
    task_affinity_migrate_to_master();
    uint64_t l_masterCoreID                     =   task_getcpuid() & ~7;
    task_affinity_unpin();

    const   TARGETING::Target * l_masterCore    =   NULL;

    TARGETING::Target * l_processor =   NULL;
    (void)TARGETING::targetService().masterProcChipTargetHandle( l_processor );
    FABRIC_NODE_ID_ATTR l_logicalNodeId =
                l_processor->getAttr<TARGETING::ATTR_FABRIC_NODE_ID>();
    FABRIC_CHIP_ID_ATTR l_chipId =
                l_processor->getAttr<TARGETING::ATTR_FABRIC_CHIP_ID>();

    TargetHandleList l_cores;
    getChildChiplets( l_cores,
                      l_processor,
                      TYPE_CORE,
                      true );

    TRACDCOMP( g_trac_targeting,
               "getMasterCore: found %d cores on master proc, l_mastreCoreID:0x%X",
               l_cores.size(),l_masterCoreID   );

    for (TARGETING::TargetHandleList::const_iterator
            coreIter = l_cores.begin();
            coreIter != l_cores.end();
            ++coreIter)
    {
        TARGETING::Target * l_core = *coreIter;

        CHIP_UNIT_ATTR l_coreId =
                    l_core->getAttr<TARGETING::ATTR_CHIP_UNIT>();

        uint64_t pir = l_coreId << 3;
        pir |= l_chipId << 7;
        pir |= l_logicalNodeId << 10;

        if (pir == l_masterCoreID){
            TRACFCOMP( g_trac_targeting,
                       "found master core: 0x%x, PIR=0x%x :",
                       l_coreId,
                       pir  );
            EntityPath l_path;
            l_path  =   l_core->getAttr<ATTR_PHYS_PATH>();
            l_path.dump();

            l_masterCore    =   l_core ;
            break;
        }

    }   // endfor

    return l_masterCore;
}

};  // end namespace
