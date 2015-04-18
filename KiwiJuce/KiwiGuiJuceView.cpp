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

#include "KiwiGuiJuceView.h"
#include "KiwiGuiJuceDevice.h"

namespace Kiwi
{	
	// ================================================================================ //
	//										JVIEW										//
	// ================================================================================ //

    jView::jView(sJuceGuiDeviceManager device, sGuiController ctrl) noexcept : GuiView(ctrl),
    m_device(device)
    {
        setInterceptsMouseClicks(ctrl->wantMouse(), true);
        setWantsKeyboardFocus(ctrl->wantKeyboard());
        setBounds(toJuce<int>(ctrl->getBounds()));
        sJuceGuiDeviceManager mng = m_device.lock();
        if(mng && ctrl->wantActions())
        {
            mng->registerAllCommandsForTarget(this);
            this->addKeyListener(mng->getKeyMappings());
        }
    }
    
    jView::~jView()
    {
        ;
    }
    
    void jView::redraw()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            repaint();
        }
    }
    
    void jView::move()
    {
        sGuiController ctrl = getController();
        if(ctrl)
        {
            setBounds(toJuce<int>(ctrl->getBounds()));
        }
    }
    
    void jView::resize()
    {
        sGuiController ctrl = getController();
        if(ctrl)
        {
            setBounds(toJuce<int>(ctrl->getBounds()));
        }
    }
    
    void jView::addChild(sGuiView child)
    {
        if(child)
        {
            sjView jchild = dynamic_pointer_cast<jView>(child);
            if(jchild)
            {
                addAndMakeVisible(jchild.get());
                jchild->setBounds(toJuce<int>(jchild->getController()->getBounds()));
            }
        }
    }
    
    void jView::removeChild(sGuiView child)
    {
        if(child)
        {
            sjView jchild = dynamic_pointer_cast<jView>(child);
            if(jchild)
            {
                removeChildComponent(jchild.get());
            }
        }
    }
    
    void jView::addToDesktop()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            Component::setVisible(true);
            Component::addToDesktop(ComponentPeer::windowHasDropShadow);
            Component::toFront(true);
        }
    }
    
    void jView::removeFromDesktop()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            Component::setVisible(false);
            Component::removeFromDesktop();
        }
    }
    
    void jView::setMinimize(const bool state)
    {
        ComponentPeer* const peer = getPeer();
        if(peer)
        {
            if(peer->isMinimised() != state)
            {
                peer->setMinimised(state);
            }
        }
    }
    
    void jView::paint(Graphics& g)
    {
        sGuiController ctrl = getController();
        if(ctrl)
        {
            jSketch d(g, g.getClipBounds());
            ctrl->draw(d);
        }
    }
    
    void jView::mouseDown(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Down, e));
    }
    
    void jView::mouseDrag(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Drag, e));
    }
    
    void jView::mouseUp(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Up, e));
    }
    
    void jView::mouseMove(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Move, e));
    }
    
    void jView::mouseEnter(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Enter, e));
    }
    
    void jView::mouseExit(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Leave, e));
    }
    
    void jView::mouseDoubleClick(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::DoubleClick, e));
    }
    
    void jView::mouseWheelMove(const juce::MouseEvent& event, const MouseWheelDetails& wheel)
    {
        receive(jEventMouse(event, wheel));
    }
    
    void jView::focusGained(FocusChangeType cause)
    {
        receive(KeyboardFocusIn);
    }
    
    void jView::focusLost(FocusChangeType cause)
    {
        receive(KeyboardFocusOut);
    }
    
    bool jView::keyPressed(const KeyPress& key)
    {
        return receive(jEventKeyboard(key));
    }
    
    ApplicationCommandTarget* jView::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void jView::getAllCommands(Array <CommandID>& commands)
    {
        vector<ulong> codes = getActionCodes();
        for(vector<ulong>::size_type i = 0; i < codes.size(); i++)
        {
            commands.add(CommandID(codes[i]));
        }
    }
    
    void jView::getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result)
    {
        Action action = getAction(ulong(commandID));
        result.setInfo(translate(action.name), translate(action.description), action.category, 0);
        result.addDefaultKeypress(action.event.getCharacter(), action.event.getModifiers());
        //result.setActive(getDesktopWindowStyleFlags() & DocumentWindow::closeButton);
    }
    
    bool jView::perform(const InvocationInfo& info)
    {
        return performAction(ulong(info.commandID));
    }
}

#endif