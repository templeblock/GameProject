#ifndef SCROLL_BAR_HPP
#define SCROLL_BAR_HPP

/// Internal Includes
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			enum class ScrollBarDirection {
				SCROLL_HORIZONTAL,
				SCROLL_VERTICAL
			};

			class ScrollBar : public GuiItem {
			public:

				ScrollBar(GuiInfo& info);
				virtual ~ScrollBar();

				void setAutoScroll(bool autoScroll);

				void setScrollDirection(ScrollBarDirection dir);
				void setNumElements(int32_t num);
				void setMinElements(int32_t num);

				void selectElement(int32_t element);
				int32_t getSelectedElement() const;

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				void updateBarPosition(int mx, int my);

				ScrollBarDirection scrollDir;

				int32_t numElements;
				int32_t minElements;

				int32_t selectedElement;
				bool autoScrollLastElement;

				int scrollbarScreenPosition;
				int scrollbarLastPos;
				int scrollbarThickness;

				bool holdingBar;
				bool forceUpdate;
			};

		}
	}
}

#endif
