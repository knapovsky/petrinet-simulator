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
 * Funkce pro prevod stavu a typu simulace na retezec.
 *
 */

#include "networkenum.h"

QString getStateString(NetworkStates state){

    QString m;
    switch(state){
        case sACK:
            m = "sACK";
            break;
        case sSIMRESET:
            m = "sSIMRESET";
            break;
        case sSIMRESETACK:
            m = "sSIMRESETACK";
            break;
        case sLOGIN:
            m = "sLOGIN";
            break;
        case sLOGINACK:
            m = "sLOGINACK";
            break;
        case sGETLIST:
            m = "sGETLIST";
            break;
        case sGETLISTACK:
            m = "sGETLISTACK";
            break;
        case sLIST:
            m = "sLIST";
            break;
        case sGETNET:
            m = "sGETNET";
            break;
        case sGETNETACK:
            m = "sGETNETACK";
            break;
        case sNET:
            m = "sNET";
            break;
        case sSTEP:
            m = "sSTEP";
            break;
        case sSTEPACK:
            m = "sSTEPACK";
            break;
        case sRUN:
            m = "sRUN";
            break;
        case sRUNACK:
            m = "sRUNACK";
            break;
        case sNETSTEP:
            m = "sNETSTEP";
            break;
        case sNETSTEPACK:
            m = "sNETSTEPACK";
            break;
        case sNETUPDATE:
            m = "sNETUPDATE";
            break;
        case sSIM:
            m = "sSIM";
            break;
        case sSIMACK:
            m = "sSIMACK";
            break;
        case sNETSIMACK:
            m = "sNETSIMACK";
            break;
        case sSAVENET:
            m = "sSAVENET";
            break;
        case sSAVENETACK:
            m = "sSAVENETACK";
            break;
        case sNETSAVE:
            m = "sNETSAVE";
            break;
        case sNETSAVEACK:
            m = "sNETSAVEACK";
            break;
        case sSIMSTART:
            m = "sSIMSTART";
            break;
        case sSIMSTARTACK:
            m = "sSIMSTARTACK";
            break;
        case sSIMSTARTNETACK:
            m = "sSIMSTARTNETACK";
            break;
        case sLISTACKACK:
            m = "sLISTACKACK";
            break;
        case sNETACKACK:
            m = "sNETACKACK";
            break;
        case sNETUPDATEACKACK:
            m = "sNETUPDATEACKACK";
            break;
        case sIDLE:
            m = "sIDLE";
            break;
    }

    return m;
}

QString getSimulationTypeString(simType simulationType){

    QString m;
    switch(simulationType){
        case tSTEP:
            m = "tSTEP";
            break;
        case tRUN:
            m = "tRUN";
            break;
    }

    return m;
}

