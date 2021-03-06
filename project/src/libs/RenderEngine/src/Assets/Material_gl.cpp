#include "Material_gl.hpp"

void Material_gl::init(unsigned int nrTextures) {
	numberOfTextures = nrTextures;

	textures = new ITexture*[numberOfTextures];
	bindLocations = new int[numberOfTextures];

	for (unsigned int i = 0; i < numberOfTextures; i++) {
		textures[i] = nullptr;
		bindLocations[i] = 0;
	}
}

void Material_gl::release() {
	delete textures;
	delete bindLocations;

	delete this;
}

void Material_gl::setTexture(unsigned int textureIndex, ITexture * texture) {
	if (textureIndex > numberOfTextures)
		textures[textureIndex] = texture;
}

void Material_gl::useMaterial() {
	for (unsigned int i = 0; i < numberOfTextures; i++) {
		if (textures[i]) {
			glActiveTexture(GL_TEXTURE0 + i);
			textures[i]->bind();
			bindLocations[i] = i;
		}
	}
}

int Material_gl::getTextureCount() const {
	return numberOfTextures;
}

int Material_gl::getTextureBindLocation(unsigned int textureIndex) const {
	if (textureIndex > numberOfTextures)
		return bindLocations[textureIndex];
	return 0;
}