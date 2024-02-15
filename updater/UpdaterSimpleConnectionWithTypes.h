/*--------------------------------------------------------------------------------
    ooo      L   attice-based  |
  o\.|./o    e   xtensible     | LeMonADE: An Open Source Implementation of the
 o\.\|/./o   Mon te-Carlo      |           Bond-Fluctuation-Model for Polymers
oo---0---oo  A   lgorithm and  |
 o/./|\.\o   D   evelopment    | Copyright (C) 2024 by
  o/.|.\o    E   nvironment    | LeMonADE Principal Developers (see AUTHORS)
    ooo                        |
----------------------------------------------------------------------------------

This file is part of LeMonADE.

LeMonADE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LeMonADE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LeMonADE.  If not, see <http://www.gnu.org/licenses/>.

--------------------------------------------------------------------------------*/

#ifndef LEMONADE_UPDATER_UpdaterSimpleConnectionWithTypes_H
#define LEMONADE_UPDATER_UpdaterSimpleConnectionWithTypes_H


#include <LeMonADE/updater/moves/MoveLocalBase.h>
#include <LeMonADE/utility/RandomNumberGenerators.h>
#include<LeMonADE/updater/AbstractUpdater.h>
#include <LeMonADE/updater/moves/MoveConnectSc.h>

/**
 * @file
 *
 * @class UpdaterSimpleConnectionWithTypes
 *
 * @brief Simple simulation updater for generic polymers perfoming a type1-type2 reaction.
 *
 * @details It takes the type of move as template argument MoveType
 * and the number of mcs to be executed as argument for the constructor.
 * 
 *
 * @tparam IngredientsType Ingredients class storing all system information( e.g. monomers, bonds, etc).
 * @tparam MoveType name of the specialized move.
 */

template<class IngredientsType,class MoveType, class ConnectionMoveType>
class UpdaterSimpleConnectionWithTypes:public AbstractUpdater
{

public:
  /**
   * @brief Standard Constructor initialized with ref to Ingredients and MCS per cycle
   *
   * @param ing a reference to the IngredientsType - mainly the system
   * @param steps MCS per cycle to performed by execute()
   */
  UpdaterSimpleConnectionWithTypes(IngredientsType& ing, int _type1, int _type2, uint32_t steps = 1 )
  :ingredients(ing),nsteps(steps), type1(_type1), type2(_type2)
  {
      std::cout <<"Connecting reactive monomers with "<<type1 << " and " << type2 << std::endl;
  }

  
 
  /**
   * @brief This checks all used Feature and applies all Feature if all conditions are met.
   *
   * @details This function runs over \a steps MCS and performs the moves.
   * It setting the age of the system and prints a simple simple simulation speed
   * in the number of attempted monomer moves per s (tried and performed monomer moves/s).
   *
   * @return True if function are done.
   */
  bool execute();

  /**
   * @brief This function is called \a once in the beginning of the TaskManager.
   *
   * @details It´s a virtual function for inheritance.
   * Use this function for initializing tasks (e.g. init SDL)
   *
   **/
  virtual void initialize();

  /**
   * @brief This function is called \a once in the end of the TaskManager.
   *
   * @details It´s a virtual function for inheritance.
   * Use this function for cleaning tasks (e.g. destroying arrays, file outut)
   *
   **/
  virtual void cleanup();
  
protected:
  
  //! A reference to the IngredientsType - mainly the system
  IngredientsType& ingredients;
  
  //! Specialized move to be used for the movement of the monomers 
  MoveType move;

  //! Specialized move to be used for the connection between reactive monomers
  ConnectionMoveType connectionMove;  
  
  //! random number generator (seed set in main program)
  RandomNumberGenerators rng;
  
private:

  //! Number of mcs to be executed
  uint32_t nsteps;
  
  //! The reactive type1 only for type1-type2-reaction
  int type1;
  
  //! The reactive type1 only for type1-type2-reaction
  int type2;
  
  //! Simple vector to keep track of connection process
  std::vector<uint32_t> vReactedMonomers;

};
/**Implementation of the member functions
 * @brief 
 */
template<class IngredientsType,class MoveType, class ConnectionMoveType>
bool UpdaterSimpleConnectionWithTypes<IngredientsType,MoveType,ConnectionMoveType>::execute()
{
  
// 	time_t startTimer = time(NULL); //in seconds
// 	std::cout<<"connection mcs "<<ingredients.getMolecules().getAge() << " passed time " << ((difftime(time(NULL), startTimer)) ) <<std::endl;

	for(int n=0;n<nsteps;n++)
	{
      // spacial move
      for(size_t m=0;m<ingredients.getMolecules().size();m++)
      {
        move.init(ingredients);
        if(move.check(ingredients)==true)
        {
            move.apply(ingredients);
        }
      }
      //connection move
      for(size_t m=0;m<ingredients.getMolecules().size();m++)
      {
        connectionMove.init(ingredients);
        if ( connectionMove.check(ingredients) ) // this checks only the reactivity and if the bond is possible
        {
            //check if it's a type1-type2 reaction
            if( (ingredients.getMolecules()[connectionMove.getIndex()].getAttributeTag() == type1) && (ingredients.getMolecules()[connectionMove.getPartner()].getAttributeTag() == type2) )
            {
                // check if the monomers have not been connected yet
                if(!ingredients.getMolecules().areConnected(connectionMove.getIndex(), connectionMove.getPartner()))
                {
                    // add the connection information to vector
                    vReactedMonomers.push_back(ingredients.getMolecules().getAge());
                    vReactedMonomers.push_back(std::min(connectionMove.getIndex(),connectionMove.getPartner())); 
                    vReactedMonomers.push_back(std::max(connectionMove.getIndex(),connectionMove.getPartner())); 
                }
                
                connectionMove.apply(ingredients);
            }
            
            //check if it's a type2-type1 reaction
            if( (ingredients.getMolecules()[connectionMove.getIndex()].getAttributeTag() == type2) && (ingredients.getMolecules()[connectionMove.getPartner()].getAttributeTag() == type1) )
            {
                // check if the monomers have not been connected yet
                if(!ingredients.getMolecules().areConnected(connectionMove.getIndex(), connectionMove.getPartner()))
                {
                    // add the connection information to vector
                    vReactedMonomers.push_back(ingredients.getMolecules().getAge());
                    vReactedMonomers.push_back(std::min(connectionMove.getIndex(),connectionMove.getPartner())); 
                    vReactedMonomers.push_back(std::max(connectionMove.getIndex(),connectionMove.getPartner())); 
                }
                
                connectionMove.apply(ingredients);
            }
            
            // ignore type1-type1 or type2-type2 reaction
        }
      }
      //increase time 
      ingredients.modifyMolecules().setAge(ingredients.getMolecules().getAge()+1);
    }
    
    // print out the conversion process so far
    cleanup();
    
    
    return true;
};

template<class IngredientsType,class MoveType, class ConnectionMoveType>
void  UpdaterSimpleConnectionWithTypes<IngredientsType,MoveType,ConnectionMoveType>::initialize()
{
// 	std::cout << "Conversion at time " << ingredients.getMolecules().getAge() << " is "<< ingredients.getConversion()<<std::endl;

	/*if ( ingredients.getNReactiveSites() == 0  )
	{
	  std::stringstream errormessage;
	  errormessage << "UpdaterSimpleConnectionWithTypes::initialize(): The number of possible reactive sites is zero. \n"
		       << "Check if the system is correctly setup with reactivity!";
	  throw std::runtime_error(errormessage.str());
	}*/ 
};

template<class IngredientsType,class MoveType, class ConnectionMoveType>
void  UpdaterSimpleConnectionWithTypes<IngredientsType,MoveType,ConnectionMoveType>::cleanup()
{
    std::cout << "Conversion process with " << (vReactedMonomers.size()/3) << " connections" << std::endl << std::endl;
    std::cout << "Time MonomerID1 MonomerID2" << std::endl << std::endl;
    
    for(size_t i = 0; i < vReactedMonomers.size(); i+=3)
    {
        std::cout << vReactedMonomers.at(i) << " " << vReactedMonomers.at(i+1) << " " << vReactedMonomers.at(i+2) << std::endl;
    }
    
     std::cout << std::endl;
     
};


#endif 	/*LEMONADE_UPDATER_UpdaterSimpleConnectionWithTypes_H*/
