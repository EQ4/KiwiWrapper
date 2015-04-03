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

#ifndef __DEF_KIWI_GUI_JUCE_LINK__
#define __DEF_KIWI_GUI_JUCE_LINK__

#include "KiwiGuiJuceObject.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  JLINK CONTROLER                                 //
    // ================================================================================ //
    
    //! The jLink is the juce implementation of the link::controller.
    /**
     The jLink overrides
     */
    class jLink  : public LinkView, public Component
    {
    public:
        
        //! The constructor.
        /** You should never have to use this function.
         */
        jLink(sGuiLink link);
        
        //! Destructor.
        /** You should never have to use this function.
         */
        ~jLink();
        
        //! The redraw function that should be override.
        /** The function is called by the link when it should be repainted.
         */
        void redraw() override;
    
        //! The bounds notification function that should be override.
        /** The function is called by the link when its bounds changed.
         */
        void boundsChanged() override;
		
        void paint(Graphics& g) override;
        
		bool hitTest(int x, int y) override
        {
            return false;
        }
    };
    
    typedef shared_ptr<jLink>    sjLink;
    typedef weak_ptr<jLink>      wjLink;


	// ================================================================================ //
	//                                  JLINK CREATOR									//
	// ================================================================================ //

	//! The TempLink is a temporary link.
	/**
	 The TempLink is a temporary link
	 */
	class TempLink : public Component
	{
	private:
		const wObjectView m_attached_object;
		const bool      m_attached_to_outlet;
		const long      m_io_index;
		
		// gui
		Kiwi::Path		m_path;
		Kiwi::Point		m_startpos;
		Kiwi::Point		m_dragpos;
		
	public:
		TempLink(wObjectView attachedObject, long index, bool fromOutlet) :
		m_attached_object(attachedObject),
		m_attached_to_outlet(fromOutlet),
		m_io_index(index)
		{
			;
		}
		
		~TempLink() {}
		
		inline Kiwi::Point getStartCoord() const noexcept
		{
			return m_startpos;
		}
		
		inline Kiwi::Point getEndCoord() const noexcept
		{
			return m_dragpos;
		}
		
		inline bool isAttachedToOutlet() const noexcept
		{
			return m_attached_to_outlet;
		}
		
		inline long getAttachedIOIndex() const noexcept
		{
			return m_io_index;
		}
		
		inline wObjectView getAttachedObject() const noexcept
		{
			return m_attached_object.lock();
		}
		
		/** Call this in your mouseDown event, to initialise a drag.
		 After using this method to initialise the link, repeatedly call dragLink()
		 in your component's mouseDrag callback.
		 */
		void beginLink(const MouseEvent& e)
		{
			jassert (getParentComponent() != nullptr);  // you need to add this to a parent component for it to work!
			
			//setSize (0, 0);
			
			if (e.mods.isShiftDown())
			{
				;
			}
			/*
            sObject object = m_attached_object.lock();
			if(object)
			{
				Object::sController jobject = object->getController();
				if(jobject)
				{
					if (m_attached_to_outlet && m_io_index < object->getNumberOfOutlets())
					{
						m_startpos = jobject->getOutletPosition(m_io_index);
						m_path.moveTo(m_startpos);
						m_path.lineTo(Kiwi::Point());
					}
					else if (!m_attached_to_outlet && m_io_index < object->getNumberOfInlets())
					{
						m_startpos = jobject->getInletPosition(m_io_index);
						m_path.moveTo(m_startpos);
						m_path.lineTo(Kiwi::Point());
					}
				}
			}*/
		}
		
		/** Call this in your mouseDrag event, to update the link's position.
		 This must be repeatedly calling when the mouse is dragged, after you've
		 first initialised the link with beginLink().
		 */
		void dragLink(const MouseEvent& e)
		{
            /*
			juce::Point<int> pos = e.getPosition();
			m_dragpos = Kiwi::Point(pos.x, pos.y);
			
			if (isAttachedToOutlet())
			{
				m_path.setPoint(0, m_startpos);
				m_path.setPoint(1, m_dragpos);
			}
			else
			{
				m_path.setPoint(0, m_dragpos);
				m_path.setPoint(1, m_startpos);
			}
			
			const Kiwi::Rectangle b = m_path.getBounds().expanded(20);
			setBounds(juce::Rectangle<int>(b.x(), b.y(), b.width(), b.height()));
			setVisible(true);
            */
		}
		
		/** Call this in your mouseUp event, after the link has been dragged.
		 */
		void endDrag()
		{
			m_path.clear();
			setVisible (false);
		}
		
		Kiwi::Point getLocalPoint(Kiwi::Point const& point)
		{
            /*
			const Kiwi::Rectangle bounds = m_path.getBounds();
			Kiwi::Point pt = point - bounds.expanded(20).position();
			return pt;
            */
            return Kiwi::Point();
		}
		
		void paint(Graphics& g) override
		{
            /*
			juce::Path p;
			const Kiwi::Point pos = getLocalPoint(m_path.getPoint(0));
			Kiwi::Point pt = pos;
			Kiwi::Point next;
			p.startNewSubPath(pt.x(), pt.y());
			
			for(ulong i = 0; i < m_path.size() - 1; i++)
			{
				double h;
				next = getLocalPoint(m_path.getPoint(i+1));
				
				if(pt.y() <= next.y())
					h = max((next.y() - pt.y()) * 0.5, 15.);
				else
					h = max(15. / (pt.y() - next.y()), 15.);
				
				p.cubicTo(pt.x(), pt.y() + h, next.x(), next.y() - h, next.x(), next.y());
				pt = next;
			}
			
			const Colour color = juce::Colour::fromFloatRGBA(0.42, 0.42, 0.42, 1.);
			g.setColour(color.darker(0.6));
			g.strokePath(p, PathStrokeType(2, PathStrokeType::curved, PathStrokeType::rounded));
			
			g.setColour(color.brighter(0.5));
			g.strokePath(p, PathStrokeType(1, PathStrokeType::curved, PathStrokeType::rounded));
			
			const juce::Rectangle<float> start((long)pos.x(), (long)pos.y(), 0, 0);
			g.setColour(color.contrasting(0.4));
			g.fillEllipse(start.expanded(3.5));
			
			const Kiwi::Point dragpos = getLocalPoint(m_dragpos);
			const juce::Rectangle<float> dragger(dragpos.x(), dragpos.y(), 0, 0);
			g.setColour(color.contrasting(0.4));
			g.fillEllipse(dragger.expanded(4));
            */
		}
		
		/** @internal */
		bool hitTest (int, int) override        { return false; }
	};

	typedef shared_ptr<TempLink> sTempLink;
}


#endif

#endif
