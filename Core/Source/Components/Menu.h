#pragma once
#include "MenuOption.h"
#include "RenderableObject.h"
#include <vector>

namespace Components {

    
	class Menu : RenderableObject
	{

    private:
        std::vector<MenuOption> m_options;

		int m_selectedIndex;
		int m_spacing;
		Alignment m_alignment;
		bool m_isUIupdateNeeded;

	public:

        Menu(Vector2 anchorPointPosition, AnchorPoint AnchorPoint, Alignment alignment, bool visible, int spacing);
		~Menu() = default;
		
		
		void AddOption(MenuOption option);
		void DeleteOption(int index);
		void ClearOptions();

		void SelectNext();
		void SelectPrevious();

		const MenuOption& GetSelectedOption() const;
		int GetSelectedIndex() const;

		bool IsUIUpdateNeeded() const;
		void UpdateOptionsAnchorPointPositions();
		virtual Vector2 GetSize() const override;
		virtual void Render() const override;
	};

}