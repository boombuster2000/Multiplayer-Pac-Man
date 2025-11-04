#pragma once
#include <vector>
#include "MenuOption.h"
#include "RenderableObject.h"
#include "DataTypes/Vector2Ex.h"

namespace UIComponents
{

	class Menu : public RenderableObject
	{

	private:
		std::vector<MenuOption> m_options;

		int m_selectedIndex;
		int m_spacing;
		Alignment m_alignment;
		AnchorPoint m_anchorPoint;
		bool m_isUIupdateNeeded;

	public:
		Menu(Vector2Ex<int> anchorPointPosition, AnchorPoint AnchorPoint, Alignment alignment, bool visible, int spacing);
		~Menu() = default;

		void AddOption(MenuOption option);
		void DeleteOption(int index);
		void ClearOptions();

		void SelectNext();
		void SelectPrevious();

		const MenuOption &GetSelectedOption() const;
		int GetSelectedIndex() const;

		bool IsUIUpdateNeeded() const;
		void UpdateOptionsAnchorPointPositions();
		virtual Vector2Ex<int> GetDimensions() const override;
		virtual void Render(Vector2Ex<int> offset = {0, 0}) const override;
	};

}