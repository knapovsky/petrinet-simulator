/**
 * @file
 * @author Martin Knapovsky <xknapo02@stud.fit.vutbr.cz>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2012, Martin Knapovsky
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *   must display the following acknowledgement:
 *    This product includes software developed by the Martin Knapovsky.
 * 4. Neither the name of the Martin Knapovsky nor the
 *    names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MARTIN KNAPOVSKY ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MARTIN KNAPOVSKY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @section DESCRIPTION
 *
 * Vycty typu zprav, stavu automatu a typu simulace.
 *
 */

#ifndef NETWORKENUM_H
#define NETWORKENUM_H

#include <QString>
#include <QDebug>

/**
  * @enum Typy zprav
  */
enum MessageTypes {
    mACK = 40,
    mNACK,
    mLOGIN,
    mGETLIST,
    mGETNET,
    mSIMSTEP,
    mSAVENET,
    mNET,
    mLIST,
    mNETUPDATE,
    mSIM,
    iFAIL,
    iSUCCESS
};

/**
  * @enum Stavy automatu
  */
enum NetworkStates {
    /* Za timto ACK uz nic neni */
    sACK = 81,
    sSIMRESET,
    sSIMRESETACK,
    sLOGIN,
    sLOGINACK,
    sGETLIST,
    sGETLISTACK,
    sLIST,
    sGETNET,
    sGETNETACK,
    sNET,
    sSTEP,
    sSTEPACK,
    sRUN,
    sRUNACK,
    sNETSTEP,
    sNETSTEPACK,
    sNETUPDATE,
    sSIM,
    sSIMACK,
    sNETSIMACK,
    sSAVENET,
    sSAVENETACK,
    sNETSAVE,
    sNETSAVEACK,
    sSIMSTART,
    sSIMSTARTACK,
    sSIMSTARTNETACK,
    sLISTACKACK,
    sNETACKACK,
    sNETUPDATEACKACK,
    sIDLE
};

/**
  * @enum Typy simulace
  */
enum simType {
    tSTEP,
    tRUN
};

/**
  * Ziska retezec, ktery odpovida danemu stavu automatu
  */
QString getStateString(NetworkStates state);

/**
  * Ziska retezec, ktery odpovida danemu typu simulace
  */
QString getSimulationTypeString(simType simulationType);

#endif // NETWORKENUM_H
