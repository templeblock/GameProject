#include "MirrorMesh.hpp"
#include "../Graphics.hpp"

#include <RenderEngine/IRenderEngine.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace Engine {
	namespace Graphics {
		namespace Mesh {

			MirrorMesh::MirrorMesh() : mesh(nullptr), pos(0), normal(0) {
				mesh = gRenderEngine->createMesh();
				mesh->init(MeshPrimitiveType::TRIANGLE);
			}

			MirrorMesh::~MirrorMesh() {
				if (mesh) {
					mesh->release();
				}
			}

			void MirrorMesh::setSize(glm::vec2 s) {
				size = s;


				/*
				float vertices[6][9] = {
				{ xpos,     ypos - h, 0,   texX1, texY1, color.r, color.g, color.b, color.a },
				{ xpos,     ypos, 0,       texX1, texY2, color.r, color.g, color.b, color.a },
				{ xpos + w, ypos, 0,       texX2, texY2, color.r, color.g, color.b, color.a },

				{ xpos,     ypos - h, 0,   texX1, texY1, color.r, color.g, color.b, color.a },
				{ xpos + w, ypos, 0,       texX2, texY2, color.r, color.g, color.b, color.a },
				{ xpos + w, ypos - h, 0,   texX2, texY1, color.r, color.g, color.b, color.a }
				};
				*/

				rotMat = glm::mat4();

				glm::vec3 n2(0, 0, -1);

				const glm::vec3 a = normal;
				const glm::vec3 b = n2;
				glm::vec3 v = glm::cross(b, a);
				float angle = acos(glm::dot(b, a) / (glm::length(b) * glm::length(a)));

				rotMat = glm::rotate(rotMat, -angle, v) * glm::transpose(glm::translate(rotMat, pos));

				glm::vec2 s2 = size / 2.0F;

				float vertex[6][5]{
					{ 0 - s2.x, 0 - s2.y, 0, 0, 0 },
					{ 0 - s2.x, 0 + s2.y, 0, 0, 0 },
					{ 0 + s2.x, 0 + s2.y, 0, 0, 0 },

					{ 0 - s2.x, 0 - s2.y, 0, 0, 0 },
					{ 0 + s2.x, 0 + s2.y, 0, 0, 0 },
					{ 0 + s2.x, 0 - s2.y, 0, 0, 0 },
				};

				mesh->setMeshData(vertex, sizeof(vertex), MeshDataLayout::VERT_UV);

			}

			void MirrorMesh::setMirrorPosition(glm::vec3 p) {
				pos = p;
			}

			void MirrorMesh::setMirrorNormal(glm::vec3 n) {
				normal = n;
			}

			glm::mat4 MirrorMesh::reflectionMatrix() {

				glm::mat4 mat(1.0F);

				float a = normal.x;
				float b = normal.y;
				float c = normal.z;
				float d = glm::distance(glm::vec3(), pos);

				mat[0][0] = 1.0F - 2.0F * a*a;	mat[0][1] = -2.0F * a*b;	    mat[0][2] = -2.0F * a*c;	    mat[0][3] = -2.0F * a*d;
				mat[1][0] = -2.0F * a*b;       mat[1][1] = 1.0F - 2.0F * b*b;	mat[1][2] = -2.0F * b*c;	    mat[1][3] = -2.0F * b*d;
				mat[2][0] = -2.0F * a*c;	    mat[2][1] = -2.0F * b*c;	    mat[2][2] = 1.0F - 2.0F * c*c;	mat[2][3] = -2.0F * c*d;
				mat[3][0] = 0.0f;	        mat[3][1] = 0.0f;    	    mat[3][2] = 0.0f;	        mat[3][3] = 1.0f;

				return mat;
			}

			glm::mat4 MirrorMesh::modelMatrix() {
				return rotMat;
			}

			void MirrorMesh::render(bool writeDepth) {
				if (writeDepth == false) {
					gRenderEngine->setStencilTest(true);
					gRenderEngine->stencilFunc(FuncConstants::ALWAYS, 0x01, 0xFF);
					gRenderEngine->stencilOp(StencilOp::KEEP, StencilOp::KEEP, StencilOp::REPLACE);
					gRenderEngine->stencilMask(0xFF);
					gRenderEngine->clearStencil();
					gRenderEngine->depthMask(false);
				}
				//gRenderEngine->colorMask(false, false, false, false);
				//glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
				//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
				//glStencilMask(0xFF); // Write to stencil buffer
				//glDepthMask(GL_FALSE); // Don't write to depth buffer
				//glClear(GL_STENCIL_BUFFER_BIT);

				mesh->bind();
				mesh->render();
				
				if (writeDepth == false) {

					gRenderEngine->setStencilTest(false);
					gRenderEngine->stencilMask(0x00);
					gRenderEngine->depthMask(true);
					//gRenderEngine->colorMask(true, true, true, true);
				}
			}

		}
	}
}