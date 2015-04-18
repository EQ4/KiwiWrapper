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

#ifndef __DEF_KIWI_GUI_JUCE_VIEW__
#define __DEF_KIWI_GUI_JUCE_VIEW__

#include "KiwiGuiJuceEvent.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      JVIEW                                       //
    // ================================================================================ //
    
	class jView : public GuiView, public Component, public ApplicationCommandTarget
    {
    private:
        const wJuceGuiDeviceManager m_device;
    public:
        jView(sJuceGuiDeviceManager device, sGuiController ctrl) noexcept;
        ~jView();
        
        void redraw() override;
        void move() override;
        void resize() override;
        void addToDesktop() override;
        void removeFromDesktop() override;
        void setMinimize(const bool state) override;
        void addChild(sGuiView child) override;
        void removeChild(sGuiView child) override;
        
        void paint(Graphics& g) override;
        void mouseEnter(const juce::MouseEvent& e) override;
        void mouseExit(const juce::MouseEvent& e) override;
        void mouseDown(const juce::MouseEvent& e) override;
        void mouseUp(const juce::MouseEvent& e) override;
        void mouseDrag(const juce::MouseEvent& e) override;
        void mouseMove(const juce::MouseEvent& e) override;
        void mouseDoubleClick(const juce::MouseEvent& e) override;
        void mouseWheelMove(const juce::MouseEvent& e, const MouseWheelDetails& wheel) override;
        void focusGained(FocusChangeType cause) override;
        void focusLost(FocusChangeType cause) override;
        bool keyPressed(const KeyPress& key) override;
        
        ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands(Array <CommandID>& commands) override;
        void getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result) override;
        bool perform(const InvocationInfo& info) override;
    };
    
    typedef shared_ptr<jView> sjView;
}

#endif

#endif