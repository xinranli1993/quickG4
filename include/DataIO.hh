#ifndef _DATAIO_HH_
#define _DATAIO_HH_

#include <fstream>
#include <iostream>
#include "G4String.hh"
#include "G4Event.hh"
#include "TTree.h"
#include "TFile.h"
//#include "B2TrackerHit.hh"

using namespace std;


struct myEvent{
	int eventID;
	short trackID;
	double theta; //0-360
	double phi; //0-90, angle to the forward direction.
	double e; //total energy deposition in the tracker.
	double targetE; //total energy deposition in the target.
};

class DataIO {
	private:
		DataIO();
	public: 
		static DataIO* Get();
		virtual ~DataIO() {};

		void OpenRootFile();
		void WriteRootFile();
		void CloseRootFile();
		void FillTTree(const G4Event* event);
		void IncEventID(){fmyEvent.eventID++;};

	private:
		static DataIO* me;
		TFile * foutRootFile;
		TTree * foutTTree;
		G4String foutRootFileName;
		myEvent fmyEvent;

};
#endif
