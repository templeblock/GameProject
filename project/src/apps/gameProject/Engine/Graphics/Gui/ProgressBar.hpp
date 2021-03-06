#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

/// Internal Includes
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class ProgressBar : public GuiItem {

			public:

				ProgressBar(GuiInfo& info);
				ProgressBar(GuiInfo& info, uint32_t _min, uint32_t _max, uint32_t _value);

				void setValue(uint32_t _value);
				void setRange(uint32_t _min, uint32_t _max);

				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				uint32_t min;
				uint32_t max;
				uint32_t value;

			};
		}
	}
}

#endif
