#include <LeMonADE/core/Ingredients.h>
#include <LeMonADE/feature/FeatureMoleculesIO.h>
#include <LeMonADE/feature/FeatureAttributes.h>
#include <LeMonADE/feature/FeatureReactiveBonds.h>
#include <LeMonADE/feature/FeatureExcludedVolumeSc.h>

#include <LeMonADE/updater/UpdaterAddLinearChains.h>
#include <LeMonADE/updater/UpdaterSimpleSimulator.h>

#include <LeMonADE/analyzer/AnalyzerWriteBfmFile.h>

#include <LeMonADE/utility/RandomNumberGenerators.h>
#include <LeMonADE/utility/TaskManager.h>

#include "../../updater/UpdaterSimpleConnectionWithTypes.h"

int main(int argc, char* argv[])
{
  // for lattice fraction c=0.5 (dense system) -> 64³/8*c=16384=PHI cubes
  // -> n=254 chains a N=64 monomers with m=127 tetra-functional cross-linker for full conversion 2n=4m and n*N+m=PHI with maximum 4m=508 bond (full conversion)
  //int nChains(254),chainLength(64),type1(1),type2(2),nMCS(100),nRuns(10), nCrosslinks(127), nReactiveMCS(10000), nReactiveRuns(100);
    
  int nChains(10),chainLength(64),type1(1),type2(2),nMCS(100),nRuns(10), nCrosslinks(50), nReactiveMCS(10000), nReactiveRuns(100);
  
  
  typedef LOKI_TYPELIST_4(
    FeatureMoleculesIO,         // file fandling
    FeatureAttributes<>,        // defines types/species (linear=1, cross-links=2)
    FeatureExcludedVolumeSc<>,  // full excluded volume
    FeatureReactiveBonds        // provides reactivity 
) Features;
    
  const uint max_bonds=4;
  typedef ConfigureSystem<VectorInt3,Features,max_bonds> Config;
  typedef Ingredients<Config> IngredientsType;
  IngredientsType ingredients;
    
  RandomNumberGenerators rng;
  rng.seedAll();
  
  ingredients.setBoxX(64);
  ingredients.setBoxY(64);
  ingredients.setBoxZ(64);
  ingredients.setPeriodicX(true);
  ingredients.setPeriodicY(true);
  ingredients.setPeriodicZ(true);
  ingredients.modifyBondset().addBFMclassicBondset();
  ingredients.synchronize();

  // create and equilibrate the system
  TaskManager taskManager;
  taskManager.addUpdater(new UpdaterAddLinearChains<IngredientsType>(ingredients, nChains,chainLength,type1,type1),0);
  taskManager.addUpdater(new UpdaterAddLinearChains<IngredientsType>(ingredients, nCrosslinks,1,type2,type2),0);
 
  taskManager.addUpdater(new UpdaterSimpleSimulator<IngredientsType,MoveLocalSc>(ingredients,nMCS));
  
  taskManager.addAnalyzer(new AnalyzerWriteBfmFile<IngredientsType>("config.bfm",ingredients,AnalyzerWriteBfmFile<IngredientsType>::APPEND));
  
  taskManager.initialize();
  taskManager.run(nRuns);
  taskManager.cleanup();
  
  // check if the system is alright
  if ( ingredients.getMolecules().size() != (nChains*chainLength+nCrosslinks) )
  {
      std::stringstream errormessage;
      errormessage << "The number of monomers is inconsistent. \n"
      << "Check the system's setup !";
      throw std::runtime_error(errormessage.str());
  }
  
  // add the tags for cross-linking
  for(size_t i = 0; i < ingredients.getMolecules().size(); i++)
  {
      if(ingredients.getMolecules().getNumLinks(i) == 1) // only linear chain-ends
      {
          ingredients.modifyMolecules()[i].setReactive(true);
          ingredients.modifyMolecules()[i].setNumMaxLinks(2); // one bond is part of the chain; the other connects to the cross-linker if possible 
      }
      
      if(ingredients.getMolecules()[i].getAttributeTag() == type2) // only cross-linker
      {
          ingredients.modifyMolecules()[i].setReactive(true);
          ingredients.modifyMolecules()[i].setNumMaxLinks(4); // tetra-functional cross-linker
      }
  }
  
  ingredients.modifyMolecules().setAge(0); // reset the clock to 0
  
  ingredients.synchronize(); // check the system
  
  // perform the cross-linking: only between type1 and type2 avoiding multiple bond between the same monomers
  TaskManager taskManager2;
  //taskManager2.addUpdater(new UpdaterSimpleSimulator<IngredientsType,MoveLocalSc>(ingredients,nMCS));
  taskManager2.addUpdater(new UpdaterSimpleConnectionWithTypes<IngredientsType,MoveLocalSc,MoveConnectSc>(ingredients, type1, type2, nReactiveMCS)); // move the system and connect the reactive monomers if possible
  taskManager2.addAnalyzer(new AnalyzerWriteBfmFile<IngredientsType>("config2.bfm",ingredients,AnalyzerWriteBfmFile<IngredientsType>::APPEND));
  
  taskManager2.initialize();
  taskManager2.run(nReactiveRuns);
  taskManager2.cleanup();
  
  // done
  return 0;
}
