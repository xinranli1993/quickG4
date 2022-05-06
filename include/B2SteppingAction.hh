//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B2SteppingAction.hh 68058 2013-03-13 14:47:43Z gcosmo $
// 
/// \file B2SteppingAction.hh
/// \brief Definition of the B2SteppingAction class

#ifndef B2SteppingAction_h
#define B2SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class B2DetectorConstruction;
class B2EventAction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track 
/// lengths of charged particles in Absober and Gap layers and
/// updated in B2EventAction.

class B2SteppingAction : public G4UserSteppingAction
{
public:
//  B2SteppingAction(const B2DetectorConstruction* detectorConstruction);
  B2SteppingAction();//const B2DetectorConstruction* detectorConstruction);
                   // B2EventAction* eventAction);
  virtual ~B2SteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
    
private:
  //const B2DetectorConstruction* fDetConstruction;
  //const B2DetectorConstruction* fDetConstruction;
  int trackID;
  int secondaryID;
//  B2EventAction*  fEventAction;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
