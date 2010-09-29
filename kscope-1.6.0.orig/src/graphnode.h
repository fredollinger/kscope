/***************************************************************************
 *
 * Copyright (C) 2005 Elad Lahav (elad_lahav@users.sourceforge.net)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***************************************************************************/

#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <qcanvas.h>
#include <qdict.h>
#include "graphedge.h"

/**
 * A canvas item that draws the name of a function insider a filled rectangle.
 * This item represents a function in the call graph.
 * @author Elad Lahav
 */
class GraphNode : public QCanvasPolygon
{
public:
	GraphNode(QCanvas* pCanvas, const QString&, bool bMultiCall = false);
	~GraphNode();

	GraphEdge* addOutEdge(GraphNode*);
	void dfs();
	void removeOutEdges();
	void removeInEdges();
	void getFirstNeighbour(GraphNode*&, bool&);
	
	/**
	 * @param	rect	The bounding rectangle of the node
	 */
	void setRect(const QRect& rect);
	
	/**
	 * @param	font	The font to use for drawing the text
	 */
	void setFont(const QFont& font) { m_font = font; }
	
	/**
	 * @return	The name of the function
	 */
	const QString& getFunc() const { return m_sFunc; }
	
	/**
	 * @return	true for a multiple-call node, false otherwise
	 */
	bool isMultiCall() { return m_bMultiCall; }
	
	/**
	 * @return	The set of outgoing edges
	 */
	QDict<GraphEdge>& getOutEdges() { return m_dictOutEdges; }
	
	/**
	 * @return	true if this node was already visited during the current DFS,
	 *			false otherwise
	 */
	bool dfsVisited() { return m_bDfsFlag; }
	
	/**
	 * Clears the 'DFS-visited' flag, in preparation for the next DFS.
	 */
	void dfsReset() { m_bDfsFlag = false; }
	
	/** Identifies this class among other QCanvasItem classes. */
	static int RTTI;

	/**
	 * @return	The class identifier
	 */
	virtual int rtti() const { return RTTI; }
	
protected:
	virtual void drawShape(QPainter&);
	
private:
	/** Function name. */
	QString m_sFunc;

	/** A list of outgoing edges indexed by destination. */
	QDict<GraphEdge> m_dictOutEdges;

	/** A list of incoming edges indexed by destination. */
	QDict<GraphEdge> m_dictInEdges;

	/** The bounding rectangle for the node. */
	QRect m_rect;

	/** The font to use for drawing the text. */
	QFont m_font;
	
	/** true for a multiple-call node, false otherwise. */
	bool m_bMultiCall;
	
	/** Determines whether this node was visited during a depth-first 
		search. */
	bool m_bDfsFlag;
};

#endif
