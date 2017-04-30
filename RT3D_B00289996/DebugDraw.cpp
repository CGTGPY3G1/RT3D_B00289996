#include "DebugDraw.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Camera.h"
#include "Transform.h"
namespace B00289996 {
	DebugDraw::DebugDraw() {
		// set ip the shader
		shaderID = rt3d::initShaders("RT3D\\Shaders\\basic.vert", "RT3D\\Shaders\\basic.frag");
	}

	DebugDraw::~DebugDraw() {
	}

	void DebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color) {
		// add the line to the cache
		verts.push_back(from.getX());
		verts.push_back(from.getY());
		verts.push_back(from.getZ());
		verts.push_back(to.getX());
		verts.push_back(to.getY());
		verts.push_back(to.getZ());
		for(size_t i = 0; i < 2; i++) {
			colours.push_back(0.0f);
			colours.push_back(1.0f);
			colours.push_back(0.0f);
		}
		noOfVerts += 2;
	}

	void DebugDraw::drawContactPoint(const btVector3 & pointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color) {
		// add the contact to the cache
		GLfloat x = pointOnB.getX() + normalOnB.getX() * distance, y = pointOnB.getY() + normalOnB.getY() * distance, z = pointOnB.getZ() + normalOnB.getZ() * distance;
		contactVerts.push_back(x);
		contactVerts.push_back(y);
		contactVerts.push_back(z);
		contactColours.push_back(1.0f);
		contactColours.push_back(0.0f);
		contactColours.push_back(0.0f);
		noOfContacts++;

		// add the normal line to the cache
		verts.push_back(x);
		verts.push_back(y);
		verts.push_back(z);
		verts.push_back(x + normalOnB.getX() * 0.5f);
		verts.push_back(y + normalOnB.getY() * 0.5f);
		verts.push_back(z + normalOnB.getZ() * 0.5f);
		verts.push_back(x);
		verts.push_back(y);
		verts.push_back(z);
		verts.push_back(x - normalOnB.getX() * 0.5f);
		verts.push_back(y - normalOnB.getY() * 0.5f);
		verts.push_back(z - normalOnB.getZ() * 0.5f);
		for(size_t i = 0; i < 12; i++) {
			colours.push_back(1.0f);
		}
		noOfVerts += 4;
	}

	void DebugDraw::reportErrorWarning(const char * warningString) {
		std::cout << warningString << std::endl;
	}

	void DebugDraw::draw3dText(const btVector3 & location, const char * textString) {
	}

	void DebugDraw::setDebugMode(int debugMode) {
		this->debugMode = debugMode;
	}

	int DebugDraw::getDebugMode() const {
		return debugMode;
	}

	void DebugDraw::Render() {
		static GLuint id = 0;
		if(noOfVerts != 0 || noOfContacts != 0) {
			glUseProgram(shaderID);
			// update the shader projection transform using the cameras ViewProjection Transform
			Camera::GetMainCamera().lock()->UpdateProjectionUniform(shaderID, true);
			if(noOfVerts != 0) { // render the lines
				if(colliderModelID == 0) colliderModelID = rt3d::createColourMesh(noOfVerts, verts.data(), colours.data());
				else {
					rt3d::updateMesh(colliderModelID, RT3D_VERTEX, verts.data(), verts.size());
					rt3d::updateMesh(colliderModelID, RT3D_COLOUR, colours.data(), colours.size());
				}
				rt3d::drawMesh(colliderModelID, noOfVerts, GL_LINES);
			}
			if(noOfContacts != 0) { // render the points
				if(contactModelID == 0) contactModelID = rt3d::createColourMesh(noOfContacts, contactVerts.data(), contactColours.data());
				else {
					rt3d::updateMesh(contactModelID, RT3D_VERTEX, contactVerts.data(), noOfContacts);
					rt3d::updateMesh(contactModelID, RT3D_COLOUR, contactColours.data(), noOfContacts);
				}
				rt3d::drawMesh(contactModelID, noOfContacts, GL_POINTS);
			}
		}
	}

	void DebugDraw::flushLines() {
		Render();
		verts.clear();
		colours.clear();
		noOfVerts = 0;
		contactVerts.clear();
		contactColours.clear();
		noOfContacts = 0;
	}
}
