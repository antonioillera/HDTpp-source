/*
 * HDTRW.cpp
 *
 *  Created on: 11/08/2012
 *      Author: mck
 */

#include "BasicModifiableHDT.hpp"

#include "TripleIDStringIterator.hpp"

namespace hdt {

BasicModifiableHDT::BasicModifiableHDT() {
	createComponents();
}

BasicModifiableHDT::BasicModifiableHDT(HDTSpecification &spec) {
	this->spec = spec;
	createComponents();
}

BasicModifiableHDT::~BasicModifiableHDT() {
    if (header){
        delete header;
        header =NULL;
    }
    if (dictionary){
        delete dictionary;
        dictionary = NULL;
    }
    if (triples){
        delete triples;
        triples = NULL;
    }
}

void BasicModifiableHDT::createComponents() {
	std::string dictType = spec.get("dictionary.type");
	std::string triplesType = spec.get("triples.type");

	// FIXME: SELECT
	header = new PlainHeader();
	dictionary = new PlainDictionary();
	triples = new TriplesList();
}



Header *BasicModifiableHDT::getHeader()
{
    return header;
}

Dictionary *BasicModifiableHDT::getDictionary()
{
    return dictionary;
}

Triples *BasicModifiableHDT::getTriples()
{
    return triples;
}


IteratorTripleString *BasicModifiableHDT::search(const char *subject, const char *predicate, const char *object)
{
	TripleString ts(subject, predicate, object);

	TripleID tid;
	dictionary->tripleStringtoTripleID(ts, tid);

//	cout << "TID: "<< tid.getSubject() << "," << tid.getPredicate() << "," << tid.getObject() << endl;

	IteratorTripleID *iterID = triples->search(tid);
    cout <<"iterID aqui"<<endl;
	TripleIDStringIterator *iterator = new TripleIDStringIterator(dictionary, iterID);
	return iterator;
}

VarBindingString *BasicModifiableHDT::searchJoin(vector<TripleString> &patterns, set<string> &vars)
{
	throw "Not implemented";
}

void BasicModifiableHDT::loadFromRDF(const char *fileName, string baseUri, RDFNotation notation, ProgressListener *listener)
{

}

void BasicModifiableHDT::saveToRDF(RDFSerializer &serializer, ProgressListener *listener)
{
    IteratorTripleString *it = search("","","");
    serializer.serialize(it, listener, this->getTriples()->getNumberOfElements() );
	delete it;
}

void BasicModifiableHDT::loadFromHDT(const char *fileName, ProgressListener *listener) {
	ifstream input(fileName, ios::binary | ios::in);
	if(!input.good()){
		throw "Error opening file to save HDT.";
	}
	this->loadFromHDT(input, listener);
    input.close();
}

void BasicModifiableHDT::mapHDT(const char *fileName, ProgressListener *listener)
{
    throw "Not implemented";
}

void BasicModifiableHDT::loadFromHDT(std::istream & input, ProgressListener *listener)
{
	//header->load(input);
	ControlInformation controlInformation;
	controlInformation.load(input);
	dictionary->load(input, controlInformation);

	controlInformation.clear();
	controlInformation.load(input);
	triples->load(input, controlInformation);
}

void BasicModifiableHDT::loadOrCreateIndex(ProgressListener *listener) {

}

void BasicModifiableHDT::saveIndex(ProgressListener *listener) {

}

void BasicModifiableHDT::saveToHDT(const char *fileName, ProgressListener *listener)
{
	ofstream out(fileName, ios::binary | ios::out);
	if(!out.good()){
		throw "Error opening file to save HDT.";
	}
	this->saveToHDT(out, listener);
	out.close();

	this->fileName = fileName;
}

void BasicModifiableHDT::saveToHDT(std::ostream & output, ProgressListener *listener)
{
	ControlInformation controlInformation;

	//header->save(output);

	cout << "Saving dictionary" << endl;
	StopWatch st;
	dictionary->save(output, controlInformation);
	cout << "Dictionary saved in " << st << endl;

	cout << "Saving triples" << endl;
	st.reset();
	triples->save(output, controlInformation);
	cout << "Triples saved in " << st << endl;

	this->fileName = fileName;
}


void BasicModifiableHDT::insert(TripleString & triple)
{
	TripleID tid;
	dictionary->tripleStringtoTripleID(triple, tid);
	triples->insert(tid);
}


void BasicModifiableHDT::insert(IteratorTripleString *triples)
{
	throw "Not implemented";
}

void BasicModifiableHDT::remove(TripleString & triple)
{
	TripleID tid;
	dictionary->tripleStringtoTripleID(triple, tid);
	triples->remove(tid);

	// Fixme: Need to remove from dictionary?
}

void BasicModifiableHDT::remove(IteratorTripleString *triples)
{
	throw "Not implemented";
}

}
