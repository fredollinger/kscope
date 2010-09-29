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

#include <qpainter.h>
#include <qfontmetrics.h>
#include "graphnode.h"

int GraphNode::RTTI = 1001;

/**
 * Class constructor.
 * @param	pCanvas		The owner canvas
 * @param	sFunc		The node's function
 * @param	bMultiCall	Whether this node represents multiple calls
 */
GraphNode::GraphNode(QCanvas* pCanvas, const QString& sFunc, bool bMultiCall) : 
	QCanvasPolygon(pCanvas),
	m_sFunc(sFunc),
	m_bMultiCall(bMultiCall),
	m_bDfsFlag(false)
{
	// Every node deletes its out-edges only
	m_dictOutEdges.setAutoDelete(true);
}

/**
 * Class destructor.
 */
GraphNode::~GraphNode()
{
}

/**
 * Finds an edge leaving this node and reaching the given node.
 * If such an edge does not exist, a new one is created.
 * @param	pTail	The destination node
 * @return	The edge that ends at the given node
 */
GraphEdge* GraphNode::addOutEdge(GraphNode* pTail)
{
	GraphEdge* pEdge;
	
	// Look for the edge
	if ((pEdge = m_dictOutEdges.find(pTail->getFunc())) == NULL) {
		// Create a new edge
		pEdge = new GraphEdge(canvas(), this, pTail);
		m_dictOutEdges.insert(pTail->getFunc(), pEdge);
		pTail->m_dictInEdges.replace(m_sFunc, pEdge);
	}
	
	// Return the new/constructed edge
	return pEdge;
}

/**
 * Performs a weak depth-first-search on the graph.
 * The search continues along all edges, both incoming and outgoing.
 */
void GraphNode::dfs()
{
	// Stop if this node is already marked
	if (m_bDfsFlag)
		return;
		
	// Mark the node as visited
	m_bDfsFlag = true;
		
	// Continue along outgoing edges
	QDictIterator<GraphEdge> itrOut(m_dictOutEdges);
	for (; itrOut.current(); ++itrOut)
		(*itrOut)->getTail()->dfs();
		
	// Continue along incoming edges
	QDictIterator<GraphEdge> itrIn(m_dictInEdges);
	for (; itrIn.current(); ++itrIn)
		(*itrIn)->getHead()->dfs();
}
	
/**
 * Deletes all outgoing edges.
 * Uses the auto-delete property of the dictionary.
 */
void GraphNode::removeOutEdges()
{
	m_dictOutEdges.clear();
}

/**
 * Deletes all incoming edges.
 * To avoid double deletions, the function lets the head node of the edge remove
 * it.
 */
void GraphNode::removeInEdges()
{
	QDictIterator<GraphEdge> itr(m_dictInEdges);
	
	// Delete edges through their head nodes
	for (; itr.current(); ++itr)
		(*itr)->getHead()->m_dictOutEdges.remove(m_sFunc);
	
	// remove edges from the local dictionary (will not delete them)
	m_dictInEdges.clear();
}

/**
 * Returns the first found node connected to this one.
 * This function is used with multi-call nodes for retrieving the parent node.
 * @param	pNode
 * @param	bCalled
 */
void GraphNode::getFirstNeighbour(GraphNode*& pNode, bool& bCalled)
{
	QDictIterator<GraphEdge> itrIn(m_dictInEdges);
	QDictIterator<GraphEdge> itrOut(m_dictOutEdges);
	
	if (itrIn.current()) {
		pNode = itrIn.current()->getHead();
		bCalled = false;
	}
	else if (itrOut.current()) {
		pNode = itrOut.current()->getTail();
		bCalled = true;
	}
	else {
		pNode = NULL;
	}
}

/**
 * Modifies the bounding rectangle of the node.
 * @param	rect	The new coordinates to set
 */
void GraphNode::setRect(const QRect& rect)
{
	QPointArray arr(4);
	
	m_rect = rect;
	
	arr.setPoint(0, m_rect.topLeft());
	arr.setPoint(1, m_rect.topRight());
	arr.setPoint(2, m_rect.bottomRight());
	arr.setPoint(3, m_rect.bottomLeft());
	setPoints(arr);
}

/**
 * Draws the node.
 * @param	painter	Used for drawing the item on the canvas view
 */
void GraphNode::drawShape(QPainter& painter)
{
	const QPen& pen = painter.pen();
	const QFont& font = painter.font();
	
	// Draw the rectangle
	painter.setPen(QPen(Qt::black));
	painter.drawRect(m_rect);
	
	// Draw the text
	painter.setPen(pen);
	painter.setFont(m_font);
	if (m_bMultiCall)
		painter.drawText(m_rect, Qt::AlignCenter, "...");
	else
		painter.drawText(m_rect, Qt::AlignCenter, m_sFunc);
	
	painter.setFont(font);
}
