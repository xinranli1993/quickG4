#ifndef _B2EVENTDATA_HH_
#define _B2EVENTDATA_HH_

#include <iostream>
#include <vector>
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
using namespace std;

class B2EventData {
  private:
	B2EventData();
  public:
	virtual ~B2EventData(){}
	static B2EventData* Get();
//	void AddScatter(G4double n, G4double e);
//	void AddIonization(G4double n, G4double e, G4double emax);
	void AddDep(G4double x, G4double y, G4double z, G4double l, G4double theta, G4double t, G4double e, G4double Ek, G4int tag, G4int pid, G4int motherid, G4int trackid);
	G4int GetNDep(){return fNDep;}
	G4int GetEventID(){return fEventID;}
	G4bool GetPrimaryP1(G4double *P1);
//	G4bool GetPrimaryP2(G4double *P2);
	G4bool GetPrimaryPos(G4double *Pos);
	G4bool GetPrimaryT(G4double &t);
	G4bool GetX_true(unsigned int size, G4double *x_true);
	G4bool GetY_true(unsigned int size, G4double *y_true);
	G4bool GetZ_true(unsigned int size, G4double *z_true);
	G4bool GetDepE(unsigned int size, G4double *depE, G4double *Ek, G4double *totalDepE, G4bool* isPhoton, int* pid);
//	G4bool GetCharge(unsigned int size, G4int *charge, G4int* totC);
//	G4bool GetTotCharge(G4int *totcharge);
	G4bool GetDepT(unsigned int size, G4double *depT);
	G4bool GetDepTag(unsigned int size, G4int *depTag);
	G4bool GetDepMotherID(unsigned int size, G4int *depMotherID);
	G4bool GetDepTrkID(unsigned int size, G4int *depTrkID);
	
	void SetEventID(G4int eventID){fEventID = eventID;}
	inline void SetPrimaryP1(G4double P1x, G4double P1y, G4double P1z);
//	inline void SetPrimaryP2(G4double P2x, G4double P2y, G4double P2z);
	inline void SetPrimaryPos(G4double Posx, G4double Posy, G4double Posz);
	void SetPrimaryT(G4double t){fPrimaryT = t;}

	void ClearAll();


	
  private:
	static B2EventData *me;
	G4int fEventID;
	unsigned int fNDep;
	G4double fPrimaryP1[3];
//	G4double fPrimaryP2[3];
	G4double fPrimaryPos[3];
	G4double fPrimaryT;
	vector<double> fDepX_true;
	vector<double> fDepY_true;
	vector<double> fDepZ_true;
	vector<double> fDepE;
//	vector<int>	   fDepC;
//	vector<int>	   fDepC_EScan[8];
	vector<double> fEk;
	vector<double> fDepT;
	vector<int> fDepTag;
	vector<int> fDepMotherID;
	vector<int> fDepTrkID;
//	int fNSc;//nscatter per step.
//	double fSc[200][2];//n.de
//	int fNIon;//nscatter per step.
//	double fIon[200][3];//n.de(Emin).Emax
//	const double alpha_mu = 1.6e-19/8.854187e-12/6.3;
//	const G4double w0		  = 0.004*keV;
//	double w0_scale;
//	double r0_scale;
//	double Efield;
//	double mobility;
//	double a0; // modified recombination probability constant.
//	int ftotCharge[20][20][8];
//	double fSensitiveR2;
//	double DoRec(G4double l, G4double theta, G4double e, G4double Ek, G4int pid);
};
	inline void B2EventData::SetPrimaryP1(G4double P1x, G4double P1y, G4double P1z){
		fPrimaryP1[0] = P1x/keV;
		fPrimaryP1[1] = P1y/keV;
		fPrimaryP1[2] = P1z/keV;
	}
//	inline void DBEventData::SetPrimaryP2(G4double P2x, G4double P2y, G4double P2z){
//		fPrimaryP2[0] = P2x/keV;
//		fPrimaryP2[1] = P2y/keV;
//		fPrimaryP2[2] = P2z/keV;
//	}
	inline void B2EventData::SetPrimaryPos(G4double Posx, G4double Posy, G4double Posz){
		fPrimaryPos[0] = Posx/um;
		fPrimaryPos[1] = Posy/um;
		fPrimaryPos[2] = Posz/um;
	}
#endif
