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

#include "KiwiGuiJuceObject.h"

namespace Kiwi
{	
	// ================================================================================ //
	//										JBOX										//
	// ================================================================================ //
	
	jObject::jObject(sGuiObject object, sPatcherView patcherview) : ObjectView(object, patcherview),
	m_framesize(2)
    {
		setBounds(toJuce<int>(getDisplayBounds()));
        setInterceptsMouseClicks(false, true);
        setWantsKeyboardFocus(false);
		setMouseClickGrabsKeyboardFocus(false);
    }
	
    jObject::~jObject()
    {
		;
    }
	
	void jObject::init()
	{
		Kiwi::sWriter writer = dynamic_pointer_cast<Kiwi::Writer>(getObject());
		if (writer)
		{
			Kiwi::Writer::sTextField textfield = writer->getTextField();
			if (textfield)
			{
				m_label = make_shared<jLabel>(textfield);
				
				if (m_label)
				{
					m_label->setBounds(toJuce<int>(ObjectView::getBounds()).withPosition(m_framesize, m_framesize));
					addAndMakeVisible(m_label.get());
					textfield->addView(m_label);
				}
			}
		}
	}
	
	Kiwi::Rectangle jObject::getDisplayBounds() const noexcept
	{
		return ObjectView::getBounds().expanded(m_framesize);
	}

	Kiwi::Point jObject::getDisplayPosition() const noexcept
	{
		return ObjectView::getPosition() - m_framesize;
	}

	Kiwi::Point jObject::getDisplaySize() const noexcept
	{
		return ObjectView::getSize() + m_framesize * 2;
	}
	
	ulong jObject::resizerKnock(Kiwi::Point const& pt) const noexcept
	{
		ulong borderFlag = Knock::BorderZone::None;
		const bool growy = (m_label != nullptr);
		
		if(isSelected())
		{
			const Kiwi::Point localPoint = pt - getDisplayPosition();
			if(localPoint.y() <= m_framesize*2)
			{
				borderFlag |= Knock::BorderZone::Top;
			}
			if(localPoint.x() >= getDisplaySize().x() - m_framesize*2)
			{
				borderFlag |= Knock::BorderZone::Right;
			}
			if(localPoint.y() >= getDisplaySize().y() - m_framesize*2)
			{
				borderFlag |= Knock::BorderZone::Bottom;
			}
			if(localPoint.x() <= m_framesize*2)
			{
				borderFlag |= Knock::BorderZone::Left;
			}
			
			if(growy)
			{
				if((borderFlag & Knock::BorderZone::Top || borderFlag & Knock::BorderZone::Bottom))
				{
					if(localPoint.x() <= m_framesize*4)
					{
						borderFlag |= Knock::BorderZone::Left;
					}
					else if(localPoint.x() >= getDisplaySize().x() - m_framesize*4)
					{
						borderFlag |= Knock::BorderZone::Right;
					}
					
					borderFlag &= ~Knock::BorderZone::Top;
					borderFlag &= ~Knock::BorderZone::Bottom;
				}
			}
		}
		
		return borderFlag;
	}
	
	void jObject::checkVisibilityAndInteractionMode()
	{
        sGuiObject object = getObject();
		if(object)
		{
            if(Component::isVisible() && !ObjectView::isVisible())
			{
				setVisible(false);
			}
			else if(!Component::isVisible() && !!ObjectView::isVisible())
			{
				setVisible(true);
			}
			else
			{
				redraw();
			}
			
            const bool acceptClick = ObjectView::isVisible() && getPatcherLockStatus() && !object->getIgnoreClick();
			setInterceptsMouseClicks(acceptClick, acceptClick);
			
            sKeyboarder keyboarder = dynamic_pointer_cast<Keyboarder>(object);
			if(keyboarder)
			{
				setWantsKeyboardFocus(acceptClick);
				setMouseClickGrabsKeyboardFocus(acceptClick);
			}
		}
	}
    
    void jObject::redraw()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
			repaint();
        }
	}
	
	void jObject::textFieldEditorShown()
	{
		;
	}
	
	void jObject::textfieldTextChanged()
	{
		if (m_label)
		{
			if (m_label->isBeingEdited())
			{
				/*
				String text = m_label->getText(true);
				Kiwi::Font font = getObject()->getFont();
				juce::Font jfont(font.getName(), (float)font.getSize(), font.getStyle());
				double textwidth = jfont.getStringWidthFloat(text);
				getObject()->setAttributeValue(AttrObject::Tag_size, {textwidth + 10, getSize().y()});
				*/
			}
		}
	}
	
    void jObject::grabKeyboardFocus()
    {
		if (m_label)
		{
			m_label->showEditor();
		}
		else
		{
			juce::Component::grabKeyboardFocus();
		}
    }
	
    void jObject::positionChanged()
    {
        setTopLeftPosition(toJuce<int>(getDisplayPosition()));
		
		if (m_label)
		{
			m_label->setBounds(toJuce<int>(ObjectView::getBounds()).withPosition(m_framesize, m_framesize));
		}
    }
    
    void jObject::sizeChanged()
    {
		const Kiwi::Point pt = getDisplaySize();
        setSize(round(pt.x()), round(pt.y()));
		
		if (m_label)
		{
			m_label->setBounds(toJuce<int>(ObjectView::getBounds()).withPosition(m_framesize, m_framesize));
		}
    }
	
	void jObject::patcherViewSelectionStatusChanged()
	{
		redraw();
	}
	
	void jObject::presentationStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
	void jObject::patcherViewLockStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
	void jObject::patcherViewPresentationStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
    void jObject::paint(Graphics& g)
    {
        if(Component::isVisible())
        {
			sGuiObject object = getObject();
			if (object)
			{
				const bool edit = !getPatcherLockStatus();
				const bool presentation = getPatcherPresentationStatus();
				
				const Kiwi::Rectangle localObjectFrame = getDisplayBounds().withZeroOrigin();
				const Kiwi::Rectangle localObjectBounds = ObjectView::getBounds().withPosition(Kiwi::Point(m_framesize, m_framesize));
				
				const juce::Rectangle<int> jlocalObjectBounds = toJuce<int>(localObjectBounds.withZeroOrigin());
				g.beginTransparencyLayer(1);
				
				JDoodle d(g, jlocalObjectBounds);
				g.setOrigin(m_framesize, m_framesize);
				g.reduceClipRegion(jlocalObjectBounds);
				
				object->draw(d);
				
				g.endTransparencyLayer();
			 
				//Paint Object frame :
				if(edit)
				{
					JDoodle d(g, localObjectFrame);
					
					const bool growy = (m_label != nullptr);
					const Kiwi::Color ioColor = Kiwi::Color(0.3, 0.3, 0.3);
					const Kiwi::Color presentationColor = Kiwi::Color(0., 0.8, 0.);
					const Kiwi::Color selectionColor = presentation ? presentationColor : Kiwi::Color(0., 0.6, 0.9);
					
					if(isSelected())
					{
						if (growy)
						{
							d.setColor(selectionColor);
							// left
							d.fillRectangle(0, 0, m_framesize*4, m_framesize);
							d.fillRectangle(0, m_framesize, m_framesize, localObjectFrame.height() - m_framesize*2);
							d.fillRectangle(0, localObjectFrame.height() - m_framesize, m_framesize*4, m_framesize);
							// right
							d.fillRectangle(localObjectFrame.width() - m_framesize*4, 0, m_framesize*4, m_framesize);
							d.fillRectangle(localObjectFrame.width() - m_framesize, m_framesize, m_framesize, localObjectFrame.height() - m_framesize*2);
							d.fillRectangle(localObjectFrame.width() - m_framesize*4, localObjectFrame.height() - m_framesize, m_framesize*4, m_framesize);
						}
						else
						{
							d.setColor(selectionColor);
							d.drawRectangle(localObjectFrame.reduced(m_framesize*0.5), m_framesize);
							
							d.setColor(selectionColor.darker(0.1));
							d.drawRectangle(localObjectFrame.reduced(0.5), 1);
						}
					}
					else if(!presentation)
					{
						const ulong ninlets = object->getNumberOfInlets();
						const ulong noutlets= object->getNumberOfOutlets();
						
						d.setColor(ioColor);
						for(ulong i = 1; i <= ninlets; i++)
						{
							d.fillRectangle(getInletBounds(i) - getDisplayPosition());
						}
						
						for(ulong i = 1; i <= noutlets; i++)
						{
							d.fillRectangle(getOutletBounds(i) - getDisplayPosition());
						}
						
						if (isIncludeInPresentation())
						{
							d.setColor(presentationColor.withAlpha(0.2));
							d.drawRectangle(localObjectBounds, 3);
						}
					}
				}
			}
        }
    }

    void jObject::mouseDown(const MouseEvent& e)
    {
        sMouser object = dynamic_pointer_cast<Mouser>(getObject());
        if(object)
        {
			object->receive(jEventMouse(Mouser::Event::Type::Down, e));
        }
    }
    
    void jObject::mouseDrag(const MouseEvent& e)
    {
        sMouser object = dynamic_pointer_cast<Mouser>(getObject());
        if(object)
        {
            object->receive(jEventMouse(Mouser::Event::Type::Drag, e));
        }
    }
    
    void jObject::mouseUp(const MouseEvent& e)
    {
        sMouser object = dynamic_pointer_cast<Mouser>(getObject());
        if(object)
        {
            object->receive(jEventMouse(Mouser::Event::Type::Up, e));
        }
    }
    
    void jObject::mouseMove(const MouseEvent& e)
    {
        sMouser object = dynamic_pointer_cast<Mouser>(getObject());
        if(object)
        {
			object->receive(jEventMouse(Mouser::Event::Type::Move, e));
        }
    }
    
    void jObject::mouseEnter(const MouseEvent& e)
    {
        sMouser object = dynamic_pointer_cast<Mouser>(getObject());
        if(object)
        {
            object->receive(jEventMouse(Mouser::Event::Type::Enter, e));
        }
    }
    
    void jObject::mouseExit(const MouseEvent& e)
    {
        sMouser object = dynamic_pointer_cast<Mouser>(getObject());
        if(object)
        {
            object->receive(jEventMouse(Mouser::Event::Type::Leave, e));
        }
    }
    
    void jObject::mouseDoubleClick(const MouseEvent& e)
    {
        sMouser object = dynamic_pointer_cast<Mouser>(getObject());
        if(object)
        {
            object->receive(jEventMouse(Mouser::Event::Type::Move, e));
        }
    }
    
    void jObject::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
    {
        sMouser object = dynamic_pointer_cast<Mouser>(getObject());
        if(object)
        {
            object->receive(jEventMouse(event, wheel));
        }
    }
    
    void jObject::focusGained(FocusChangeType cause)
    {
        sKeyboarder object = dynamic_pointer_cast<Keyboarder>(getObject());
		object->receive(Keyboarder::In);
    }
    
    void jObject::focusLost(FocusChangeType cause)
    {
        sKeyboarder object = dynamic_pointer_cast<Keyboarder>(getObject());
        object->receive(Keyboarder::Out);
    }
    
    bool jObject::keyPressed(const KeyPress& key)
    {
        sKeyboarder object = dynamic_pointer_cast<Keyboarder>(getObject());
        return object->receive(jEventKeyboard(key));
    }
}

#endif
