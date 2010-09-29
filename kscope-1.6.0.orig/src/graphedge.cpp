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

#include <math.h>
#include <stdlib.h>
#include <qpainter.h>
#include "graphedge.h"
#include "graphnode.h"

int GraphEdge::RTTI = 1002;

// Some definitions required by the ConvexHull class
typedef int (*CompFunc)(const void*, const void*);
#define ISLEFT(P0, P1, P2) \
	(P1.x() - P0.x()) * (P2.y() - P0.y()) - \
	(P2.x() - P0.x()) * (P1.y() - P0.y())
#define FARTHEST(P0, P1, P2) \
	((P1.x() - P0.x()) * (P1.x() - P0.x()) - \
	(P1.y() - P0.y()) * (P1.y() - P0.y())) - \
	((P2.x() - P0.x()) * (P2.x() - P0.x()) - \
	(P2.y() - P0.y()) * (P2.y() - P0.y()))


/**
 * An array of QPoint objects that can compute the convex hull surrounding all
 * points in the array.
 * @author Elad Lahav
 */
class ConvexHull : public QPointArray
{
public:
	/**
	 * Class constructor.
	 */
	ConvexHull() : QPointArray() {}
	
	/**
	 * Computes the convex hull of the points stored in the array, using
	 * Graham's scan.
	 * @param	arrHull	Holds the coordinates of the convex hull, upon return
	 */
	void compute(QPointArray& arrHull) {
		uint i, nPivot, nTop;
		
		// Find the pivot point
		nPivot = 0;
		for (i = 1; i < size(); i++) {
			if ((*this)[i].y() < (*this)[nPivot].y()) {
				nPivot = i;
			}
			else if ((*this)[i].y() == (*this)[nPivot].y() &&
				(*this)[i].x() < (*this)[nPivot].x()) {
				nPivot = i;
			}
		}
			
		// Sort points in radial order, relative to the pivot
		s_ptPivot = (*this)[nPivot];
		(*this)[nPivot] = (*this)[0];
		(*this)[0] = s_ptPivot;
		qsort(&(*this).data()[1], (*this).size() - 1, sizeof(QPoint), 
			(CompFunc)compRadial);
		
		// Initialise Graham's scan algorithm
		arrHull.resize(size() + 1);
		arrHull[0] = (*this)[0];
		arrHull[1] = (*this)[1];
		nTop = 1;
		
		// Compute the convex hull
		for (i = 2; i < size();) {
			// TODO: According to the algorithm, the condition should be >0
			// for pushing the point into the stack. For some reason, it works
			// only with <0. Why?
			if (ISLEFT(arrHull[nTop - 1], arrHull[nTop], (*this)[i]) < 0) {
				arrHull[++nTop] = (*this)[i];
				i++;
			}
			else {
				nTop--;
			}
		}
		
		// Close the hull
		arrHull[++nTop] = (*this)[0];
		arrHull.truncate(nTop + 1);
	}
	
private:
	/** The current pivot point, required by compRadial. */
	static QPoint s_ptPivot;
	
	/**
	 * Compares two points based on their angle relative to the current
	 * pivot point.
	 * This function is passed as the comparison function of qsort().
	 * @param	pPt1	A pointer to the first point
	 * @param	pPt2	A pointer to the second point
	 * @return	>0 if the first point is to the left of the second, <0 otherwise
	 */
	static int compRadial(const QPoint* pPt1, const QPoint* pPt2) {
		int nResult;
		
		// Determine which point is to the left of the other. If the angle is
		// the same, the greater point is the one farther from the pivot
		nResult = ISLEFT(s_ptPivot, (*pPt1), (*pPt2));
		if (nResult == 0)
			return FARTHEST(s_ptPivot, (*pPt1), (*pPt2));
			
		return nResult;
	}
};

QPoint ConvexHull::s_ptPivot;

/**
 * Class constructor.
 * @param	pCanvas	The canvas on which to draw the edge
 * @param	pHead	The edge's starting point
 * @param	pTail	The edge's end point
 */
GraphEdge::GraphEdge(QCanvas* pCanvas, GraphNode* pHead, GraphNode* pTail) :
	QCanvasPolygonalItem(pCanvas),
	m_pHead(pHead),
	m_pTail(pTail),
	m_arrPoly(4)
{
}

/**
 * Class destructor.
 */
GraphEdge::~GraphEdge()
{
	// Classes derived from QCanvasPolygonalItem must call hide() in their
	// detructor (according to the Qt documentation)
	hide();
}

/**
 * Calculates the area surrounding the edge, and the bounding rectangle of
 * the edge's polygonal head.
 * The calculated area is used to find items on the canvas and to display
 * tips. The bounding rectangle defines the tip's region (@see QToolTip).
 * TODO: The function assumes that the we can simply append the polygon's
 * array to that of the splines. Is this always the case, or should we sort 
 * the points of the polygon in radial order?
 * @param	arrCurve	The control points of the edge's spline
 * @param	ai			Used to calculate the arrow head polygon
 */
void GraphEdge::setPoints(const QPointArray& arrCurve, const ArrowInfo& ai)
{
	ConvexHull ch;
	uint i;
	int nXpos, nYpos;

	// Redraw an existing edge
	if (m_arrArea.size() > 0)
		invalidate();

	// Store the point array for drawing
	m_arrCurve = arrCurve;

	// Calculate the arrowhead's polygon
	makeArrowhead(ai);

	// Compute the convex hull of the edge
	ch.resize(m_arrCurve.size() + m_arrPoly.size() - 2);
	ch.putPoints(0, m_arrCurve.size() - 1, m_arrCurve);
	ch.putPoints(m_arrCurve.size() - 1, m_arrPoly.size() - 1, m_arrPoly);	
	ch.compute(m_arrArea);
	
	// Calculate the head's bounding rectangle
	m_rcTip = QRect(m_arrPoly[0], m_arrPoly[0]);
	for (i = 1; i < m_arrPoly.size(); i++) {
		nXpos = m_arrPoly[i].x();
		if (nXpos < m_rcTip.left())
			m_rcTip.setLeft(nXpos);
		else if (nXpos > m_rcTip.right())
			m_rcTip.setRight(nXpos);
		
		nYpos = m_arrPoly[i].y();
		if (nYpos < m_rcTip.top())
			m_rcTip.setTop(nYpos);
		else if (nYpos > m_rcTip.bottom())
			m_rcTip.setBottom(nYpos);
	}
}

/**
 * Sets the call information associated with this edge.
 * @param	sFile	The call's file path
 * @param	sLine	The call's line number
 * @param	sText	The call's text
 */
void GraphEdge::setCallInfo(const QString& sFile, const QString& sLine,
	const QString& sText)
{
	m_sFile = sFile;
	m_sLine = sLine;
	m_sText = sText;
} 

/**
 * Constructs a tool-tip string for this edge.
 * @return	The tool-tip text
 */
QString GraphEdge::getTip() const
{
	QString sTip;
	
	sTip = m_sText + "<br><i>" + m_sFile + "</i>:" + m_sLine;
	return sTip;
}

/**
 * Draws the spline as a sequence of cubic Bezier curves.
 * @param	painter	Used for drawing the item on the canvas view
 */
void GraphEdge::drawShape(QPainter& painter)
{
	uint i;
	
	// Draw the polygon
	painter.drawConvexPolygon(m_arrPoly);
	
	// Draw the Bezier curves
	for (i = 0; i < m_arrCurve.size() - 1; i += 3)
		painter.drawCubicBezier(m_arrCurve, i);
		
#if 0
	// Draws the convex hull of the edge
	QPen pen = painter.pen();
	QBrush br = painter.brush();
	painter.setPen(QPen(QColor(255, 0, 0)));
	painter.setBrush(QBrush());
	painter.drawPolygon(m_arrArea);
	painter.setPen(pen);
	painter.setBrush(br);
#endif
}

/**
 * Computes the coordinates of the edge's arrow head, based on its curve.
 * @param	ai	Pre-computed values used for all edges
 */
void GraphEdge::makeArrowhead(const ArrowInfo& ai)
{
	QPoint ptLast, ptPrev;
	double dX1, dY1, dX0, dY0, dX, dY, dDeltaX, dDeltaY, dNormLen;
	
	// The arrowhead follows the line from the second last to the last points
	// in the curve
	ptLast = m_arrCurve[m_arrCurve.size() - 1];
	ptPrev = m_arrCurve[m_arrCurve.size() - 2];
	
	// The first and last points of the polygon are the end of the curve
	m_arrPoly.setPoint(0, ptLast.x(), ptLast.y());
	m_arrPoly.setPoint(3, ptLast.x(), ptLast.y());
	
	// Convert integer points to double precision values
	dX1 = (double)ptLast.x();
	dY1 = (double)ptLast.y();
	dX0 = (double)ptPrev.x();
	dY0 = (double)ptPrev.y();
	
	// The values (x1-x0), (y1-y0) and sqrt(1 + tan(theta)^2) are useful
	dDeltaX = dX1 - dX0;
	dDeltaY = dY1 - dY0;
	
	// The normalised length of the arrow's sides
	dNormLen = ai.m_dLength / sqrt(dDeltaX * dDeltaX + dDeltaY * dDeltaY);
	
	// Compute the other two points
	dX = dX1 - ((dDeltaX - ai.m_dTan * dDeltaY) / ai.m_dSqrt) * dNormLen;
	dY = dY1 - ((dDeltaY + ai.m_dTan * dDeltaX) / ai.m_dSqrt) * dNormLen;
	m_arrPoly.setPoint(1, (int)dX, (int)dY);
	
	dX = dX1 - ((dDeltaX + ai.m_dTan * dDeltaY) / ai.m_dSqrt) * dNormLen;
	dY = dY1 - ((dDeltaY - ai.m_dTan * dDeltaX) / ai.m_dSqrt) * dNormLen;
	m_arrPoly.setPoint(2, (int)dX, (int)dY);
}
