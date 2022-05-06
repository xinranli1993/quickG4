#include "B2EventData.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4Log.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4Poisson.hh"
#include <cmath>

using namespace std;


B2EventData* B2EventData::me = 0;

B2EventData::B2EventData(){
	fNDep = 0;
	fEventID = -1;
}

B2EventData* B2EventData::Get(){
	if(!me)
	  me = new B2EventData();
	return me;
}
void B2EventData::AddDep(G4double x, G4double y, G4double z, G4double l, G4double theta, G4double t, G4double e, G4double Ek, G4int tag, G4int pid, G4int motherid, G4int trackid){
		if(tag>=3)
		  return;
//	G4cout<<"pushing back: "<<x/um<<" "<<y/um<<" "<<z/um<<" E_pre="<<Ek/keV<<" Edep="<<e/keV<<G4endl;
//	r0_scale = 1;//2.0;
//	w0_scale = 1;

		fDepX_true.push_back(x/um);// + B2IO::Get()->GetDetectorWidth()/um);
		fDepY_true.push_back(y/um);// + B2IO::Get()->GetDetectorWidth()/um);
		fDepZ_true.push_back(z/um);// + B2IO::Get()->GetDetectorHeight()/um);
		fDepE.push_back(e/keV);
		fEk.push_back(Ek/keV);
		fDepT.push_back(t/ns);
		fDepTag.push_back(tag*((pid>=0)?1:-1) + pid*100);
		fDepMotherID.push_back(motherid);
		fDepTrkID.push_back(trackid);
		fNDep++;
	
//	G4cout<< x <<" "<<fDepX_true.size()<<" | ";
}
G4bool B2EventData::GetPrimaryP1(G4double *P1){
	G4bool success = true;
	if(fEventID == -1)
	  success = false;
	else
	  for(int i=0;i<3;i++)
		P1[i]=fPrimaryP1[i];
	return success;
}
//G4bool B2EventData::GetPrimaryP2(G4double *P2){
//	G4bool success = true;
//	if(fEventID == -1)
//	  success = false;
//	else
//	  for(int i=0;i<3;i++)
//		P2[i]=fPrimaryP2[i];
//	return success;
//}
G4bool B2EventData::GetPrimaryPos(G4double *Pos){
	G4bool success = true;
	if(fEventID == -1)
	  success = false;
	else
	  for(int i=0;i<3;i++)
		Pos[i]=fPrimaryPos[i];
	return success;
}
G4bool B2EventData::GetPrimaryT(G4double &t){
	G4bool success = true;
	if(fEventID == -1)
	  success = false;
	else
	  t = fPrimaryT;
	return success;
}

G4bool B2EventData::GetX_true(unsigned int size, G4double *x_true){
	G4bool success = true;
	if(fDepX_true.size()!=fNDep){
		G4cout<<"deposit x_true data missing!!"<<fDepX_true.size()<<" "<<size<<G4endl;
		success = false;
	}else if(fDepX_true.size() > size){
		G4cout<<"larger container needed for x_true!"<<G4endl;
		success = false;
	}else{
		for(unsigned int i=0;i<fDepX_true.size();i++)
		  x_true[i]=fDepX_true[i];
	}
	return success;
}
G4bool B2EventData::GetY_true(unsigned int size, G4double *y_true){
	G4bool success = true;
	if(fDepY_true.size()!=fNDep){
		G4cout<<"deposit y_true data missing!!"<<G4endl;
		success = false;
	}else if(fDepY_true.size() > size){
		G4cout<<"larger container needed for y_true!"<<G4endl;
		success = false;
	}else{
		for(unsigned  int i=0;i<fDepY_true.size();i++)
		  y_true[i]=fDepY_true[i];
	}
	return success;
}
G4bool B2EventData::GetZ_true(unsigned int size, G4double *z_true){
	G4bool success = true;
	if(fDepZ_true.size()!=fNDep){
		G4cout<<"deposit z_true data missing!!"<<G4endl;
		success = false;
	}else if(fDepZ_true.size() > size){
		G4cout<<"larger container needed for z_true!"<<G4endl;
		success = false;
	}else{
		for(unsigned int i=0;i<fDepZ_true.size();i++)
		  z_true[i]=fDepZ_true[i];
	}
	return success;
}
G4bool B2EventData::GetDepE(unsigned int size, G4double *depE, G4double *Ek, G4double *totalDepE, G4bool* isPhoton, int* pid){
	G4bool success = true;
	if(fDepE.size()!=fNDep){
		G4cout<<"deposit depE data missing!!"<<G4endl;
		success = false;
	}else if(fDepE.size() > size){
		G4cout<<"larger container needed for depE!"<<G4endl;
		success = false;
	}else{
		for(int i=0; i<4; i++)
		  totalDepE[i] = 0;
		//totDepE=0;
		//totDepE_source=0;
		for(unsigned int i=0;i<fDepE.size();i++){
			Ek[i]=fEk[i];
		  depE[i]=fDepE[i];
		  int tag = (fDepTag[i]<0? -fDepTag[i] : fDepTag[i]) %100;
		  totalDepE[tag]+=fDepE[i];
		  //if(tag==1)totDepE+=fDepE[i];
		  //else if(tag==2){totDepE_source+=fDepE[i];totDepE+=fDepE[i];}
		  isPhoton[i] = (fDepTag[i]/100)==22;
		  pid[i] = (fDepTag[i]<0? -1 : 1) 
				  *((fDepTag[i]<0? -fDepTag[i] : fDepTag[i])/100);
		}
	}
	return success;
}
G4bool B2EventData::GetDepT(unsigned int size, G4double *depT){
	G4bool success = true;
	if(fDepT.size()!=fNDep){
		G4cout<<"deposit depT data missing!!"<<G4endl;
		success = false;
	}else if(fDepT.size() > size){
		G4cout<<"larger container needed for depT!"<<G4endl;
		success = false;
	}else{
		for(unsigned int i=0;i<fDepT.size();i++)
		  depT[i]=fDepT[i];
	}
	return success;
}
G4bool B2EventData::GetDepTag(unsigned int size, G4int *depTag){
	G4bool success = true;
	if(fDepT.size()!=fNDep){
		G4cout<<"deposit depTag data missing!!"<<G4endl;
		success = false;
	}else if(fDepT.size() > size){
		G4cout<<"larger container needed for depTag!"<<G4endl;
		success = false;
	}else{
		for(unsigned int i=0;i<fDepTag.size();i++)
		  depTag[i]=fDepTag[i];
	}
	return success;
}
G4bool B2EventData::GetDepMotherID(unsigned int size, G4int *depMotherID){
	G4bool success = true;
	if(fDepMotherID.size()!=fNDep){
		G4cout<<"deposit depTag data missing!!"<<G4endl;
		success = false;
	}else if(fDepMotherID.size() > size){
		G4cout<<"larger container needed for depTag!"<<G4endl;
		success = false;
	}else{
		for(unsigned int i=0;i<fDepMotherID.size();i++)
		  depMotherID[i]=fDepMotherID[i];
	}
	return success;
}
G4bool B2EventData::GetDepTrkID(unsigned int size, G4int *depTrkID){
	G4bool success = true;
	if(fDepTrkID.size()!=fNDep){
		G4cout<<"deposit depTag data missing!!"<<G4endl;
		success = false;
	}else if(fDepTrkID.size() > size){
		G4cout<<"larger container needed for depTag!"<<G4endl;
		success = false;
	}else{
		for(unsigned int i=0;i<fDepTrkID.size();i++)
		  depTrkID[i]=fDepTrkID[i];
	}
	return success;
}
void B2EventData::ClearAll(){
	fEventID = -1;
	fNDep = 0;
	for(int i=0;i<3;i++){
		fPrimaryP1[i] = 0;
		fPrimaryPos[i] = 0;
	}
	fPrimaryT = 0;
	fDepX_true.clear();
	fDepY_true.clear();
	fDepZ_true.clear();
	fDepE.clear();
	fEk.clear();
	fDepT.clear();
	fDepTag.clear();
	fDepMotherID.clear();
	fDepTrkID.clear();
}
