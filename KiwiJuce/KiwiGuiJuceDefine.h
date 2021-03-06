/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifdef __KIWI_JUCE_WRAPPER__

#ifndef __DEF_KIWI_GUI_JUCE_DEFINE__
#define __DEF_KIWI_GUI_JUCE_DEFINE__

#include <JuceHeader.h>
#include "../../KiwiGui/KiwiGui.h"

namespace Kiwi
{
    class KiwiJuceGuiDeviceManager;
    typedef shared_ptr<KiwiJuceGuiDeviceManager>        sJuceGuiDeviceManager;
    typedef shared_ptr<const KiwiJuceGuiDeviceManager>  scJuceGuiDeviceManager;
    typedef weak_ptr<KiwiJuceGuiDeviceManager>          wJuceGuiDeviceManager;
    typedef weak_ptr<const KiwiJuceGuiDeviceManager>    wcJuceGuiDeviceManager;
}

#endif

#endif


