/*
 * File: PlainTriples.hpp
 * Last modified: $Date: 2012-08-21 21:15:01 +0100 (mar, 21 ago 2012) $
 * Revision: $Revision: 250 $
 * Last modified by: $Author: mario.arias $
 *
 * Copyright (C) 2012, Mario Arias, Javier D. Fernandez, Miguel A. Martinez-Prieto
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 * Contacting the authors:
 *   Mario Arias:               mario.arias@gmail.com
 *   Javier D. Fernandez:       jfergar@infor.uva.es
 *   Miguel A. Martinez-Prieto: migumar2@infor.uva.es
 *
 */

#ifndef PLAINTRIPLES_
#define PLAINTRIPLES_

#include <Triples.hpp>
#include <HDTSpecification.hpp>

#include "TripleIterators.hpp"

#include "../sequence/LogSequence2.hpp"

namespace hdt {

class PlainTriples : public Triples {

private:
	ControlInformation controlInformation;
	HDTSpecification spec;
	TripleComponentOrder order;
	IntSequence *streamX, *streamY, *streamZ;

public:
	PlainTriples();
	PlainTriples(HDTSpecification &spec);
	~PlainTriples();

	/**
	 * Returns a vector of triples matching the pattern
	 *
	 * @param triple
	 * @return
	 */
	IteratorTripleID *search(TripleID &triple);
    IteratorTripleID *searchJoin(TripleID &a, TripleID &b, unsigned short conditions);

	/**
	 * Calculates the cost to retrieve a specific pattern
	 *
	 * @param triple
	 * @return
	 */
	float cost(TripleID &triple);

	/**
	 * Returns the number of triples
	 *
	 * @return
	 */
	unsigned int getNumberOfElements();

    size_t size();

	/**
	 * Saves the triples
	 *
	 * @param output
	 * @return
	 */
	void save(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener = NULL);

    /**
     * Loads triples from a file
     *
     * @param input
     * @return
     */
	void load(std::istream &input, ControlInformation &controlInformation, ProgressListener *listener = NULL);

	size_t load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener=NULL);

	void load(ModifiableTriples &triples, ProgressListener *listener = NULL);

	void generateIndex(ProgressListener *listener);

	void saveIndex(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener);
	void loadIndex(std::istream &input, ControlInformation &controlInformation, ProgressListener *listener);

    size_t loadIndex(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener);

	void populateHeader(Header &header, string rootNode);

	string getType();

	TripleComponentOrder getOrder();

	friend class PlainTriplesIterator;
};

class PlainTriplesIterator : public IteratorTripleID {
private:
	TripleID pattern, returnTriple;
	TripleComponentOrder order;
	PlainTriples *triples;
	int64_t pos;

	void updateOutput();
public:
	PlainTriplesIterator(PlainTriples *triples, TripleID &pattern, TripleComponentOrder order);
	bool hasNext();
	TripleID *next();
	bool hasPrevious();
	TripleID *previous();
	void goToStart();
};


class ComponentIterator : public IteratorUInt {
private:
	TripleComponentRole role;
	IteratorTripleID *it;

public:
	ComponentIterator(IteratorTripleID *iterator, TripleComponentRole component);

	bool hasNext();
	unsigned int next();
	void goToStart();
};

}

#endif /* PLAINTRIPLES_ */
