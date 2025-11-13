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
		float m_spacing;
		Alignment m_alignment;
		AnchorPoint m_anchorPoint;
		bool m_isUIupdateNeeded;

	public:
		Menu(Vector2Ex<float> anchorPointPosition, AnchorPoint AnchorPoint, Alignment alignment, bool visible, float spacing);
		~Menu() = default;

		void AddOption(MenuOption option);
		void DeleteOption(int index);
		void ClearOptions();

		void SelectNext();
		void SelectPrevious();
		void ConfirmSelection();

		const MenuOption &GetSelectedOption() const;
		int GetSelectedIndex() const;

		bool IsUIUpdateNeeded() const;
		void UpdateOptionsAnchorPointPositions();
		virtual Vector2Ex<float> GetDimensions() const override;
		virtual void Render(Vector2Ex<float> offset = {0, 0}) const override;
	};

}