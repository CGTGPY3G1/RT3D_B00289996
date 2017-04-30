#pragma once
#ifndef B00289996_DEBUG_DRAW_H
#define B00289996_DEBUG_DRAW_H

#include <LinearMath/btIDebugDraw.h>
#include "RT3D\rt3d.h"
#include <vector>
namespace B00289996 {
	/// <summary>
	/// btIDebugDraw Implementation for OpenGL 3.3 using rt3d 
	/// </summary>
	/// <seealso cref="btIDebugDraw" />
	class DebugDraw : public btIDebugDraw {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="DebugDraw"/> class.
		/// </summary>
		DebugDraw();
		/// <summary>
		/// Finalizes an instance of the <see cref="DebugDraw"/> class.
		/// </summary>
		~DebugDraw();
		/// <summary>
		/// Caches lines. Used by Bullet Physics to draw colliders
		/// </summary>
		/// <param name="from">the start point.</param>
		/// <param name="to">the end point.</param>
		/// <param name="color">The colour (Not Used).</param>
		void drawLine(const btVector3 &from, const btVector3 &to,
					  const btVector3 &color);
		/// <summary>
		/// Caches contact points. Used by Bullet Physics to draw contact points
		/// </summary>
		/// <param name="pointOnB">The contact point.</param>
		/// <param name="normalOnB">The collision normal.</param>
		/// <param name="distance">The penetration depth (Not Used).</param>
		/// <param name="lifeTime">The life time (Not Used).</param>
		/// <param name="color">The colour (Not Used).</param>
		void drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB,
							  btScalar distance, int lifeTime,
							  const btVector3 &color);
		/// <summary>
		/// Reports the error warning.
		/// </summary>
		/// <param name="warningString">The warning string.</param>
		void reportErrorWarning(const char *warningString);
		/// <summary>
		/// Draws 3D text (Not Implemented).
		/// </summary>
		/// <param name="location">The location.</param>
		/// <param name="textString">The text string.</param>
		void draw3dText(const btVector3 &location, const char *textString);
		/// <summary>
		/// Sets the debug mode.
		/// </summary>
		/// <param name="debugMode">The debug mode.</param>
		void setDebugMode(int debugMode);
		/// <summary>
		/// Gets the debug mode.
		/// </summary>
		/// <returns> the debug mode </returns>
		int getDebugMode() const;
		/// <summary>
		/// Renders the cached data
		/// </summary>
		void Render();
		/// <summary>
		/// Flushes the cached data.
		/// </summary>
		void flushLines()override;
	protected:
		int debugMode;
		size_t maxlines;
		GLuint texture = 0, shaderID = 0, contactModelID = 0, colliderModelID, noOfVerts = 0, noOfContacts = 0;
		std::vector<GLfloat> verts, contactVerts, normalVerts;
		std::vector<GLfloat> colours, contactColours, normalColours;
		rt3d::materialStruct material = {
			{0.0f, 1.0f, 0.0f, 1.0f}, // ambient
			{0.0f, 1.0f, 0.0f, 1.0f}, // diffuse
			{0.2f, 0.2f, 0.2f, 1.0f}, // specular
			1.0f  // shininess
		};
	};
}
#endif // !B00289996_DEBUG_DRAW_H