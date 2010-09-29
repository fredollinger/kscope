/***************************************************************************
 *
 * Copyright (C) 2006 Elad Lahav (elad_lahav@users.sf.net)
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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
#ifndef DOTFRONTEND_H
#define DOTFRONTEND_H

#include <frontend.h>
#include <qpointarray.h>

class GraphWidget;

/**
 * Front-end for executing graphviz's command-line tool.
 * This tool accepts the description of a graph in the 'dot' language, and
 * outputs a set of drawing instructions for the graph.
 * @author Elad Lahav
 */
class DotFrontend : public Frontend
{
	Q_OBJECT

public:
	DotFrontend(GraphWidget*);
	~DotFrontend();

	bool run(const QString&);
	
	static bool verify(const QString&);
	
protected:
	virtual ParseResult parseStdout(QString&, ParserDelim);

private:
	/** The owner graph widget on which to draw. */
	GraphWidget* m_pGraph;
	
	/** State values for the parser state machine. */
	enum ParserState { Graph, GraphScale, GraphWidth, GraphHeight,
		NodeEdgeStop, NodeName, NodeCentreX, NodeCentreY, NodeWidth, NodeHeight,
		EdgeHead, EdgeTail, EdgeCurveSize, EdgeCurveX, EdgeCurveY,
		EndNodeEdge };
	
	/** The current state of the parser state machine. */
	ParserState m_state;
	
	/** The horizontal DPI value of the graph widget. */
	double m_dDpiX;
	
	/** The vertical DPI value of the graph widget. */
	double m_dDpiY;
};

#endif
