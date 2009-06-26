/* -------------------------------------------------------------------------- *
 *                                   OpenMM                                   *
 * -------------------------------------------------------------------------- *
 * This is part of the OpenMM molecular simulation toolkit originating from   *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org.               *
 *                                                                            *
 * Portions copyright (c) 2009 Stanford University and the Authors.           *
 * Authors: Mark Friedrichs, Mike Houston                                     *
 * Contributors:                                                              *
 *                                                                            *
 * This program is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
 * -------------------------------------------------------------------------- */

#include <sstream>
#include "BrookBondParameters.h"
#include "openmm/OpenMMException.h"

using namespace OpenMM;
using namespace std;

/** 
 * BrookBondParameters constructor
 * 
 * @param bondName                  bond name
 * @param numberOfParticlesInBond   no. of particles in each bond
 * @param numberOfParametersInBond  no. of parameters in each bond
 * @param numberOfBonds             no. of bonds
 * @param log                       optional log reference
 *
 */

BrookBondParameters::BrookBondParameters( std::string bondName, int numberOfParticlesInBond,
                                          int numberOfParametersInBond, int numberOfBonds, FILE* log = NULL ) :

 _bondName( bondName ), _numberOfParticlesInBond( numberOfParticlesInBond ),
 _numberOfParametersInBond( numberOfParametersInBond ), _numberOfBonds( numberOfBonds ), _log( log ){

// ---------------------------------------------------------------------------------------

   // static const std::string methodName      = "BrookBondParameters::BrookBondParameters";

// ---------------------------------------------------------------------------------------

   // allocate memory for particle indices/parameters

   _bondParameters.resize( numberOfBonds );
   _particleIndices.resize( numberOfBonds );

}   

/** 
 * BrookBondParameters destructor
 * 
 */

BrookBondParameters::~BrookBondParameters( ){

// ---------------------------------------------------------------------------------------

   // static const std::string methodName      = "BrookBondParameters::BrookBondParameters";

// ---------------------------------------------------------------------------------------

}

/** 
 * Get particle indices
 * 
 * @return particle indices
 *
 */
    
const std::vector<std::vector<int>>& BrookBondParameters::getParticleIndices( void ) const {
   return _particleIndices;
}
    
/** 
 * Get bond parameters
 * 
 * @return parameters
 *
 */
    
const std::vector<std::vector<double>>& BrookBondParameters::getBondParameters( void ) const {
   return _bondParameters;
}
    
/** 
 * Get log file reference
 * 
 * @return  log file reference
 *
 */

FILE* BrookBondParameters::getLog( void ) const {
   return _log;
}

/** 
 * Get bond name
 * 
 * @return  bond name
 *
 */
    
std::string BrookBondParameters::getBondName( void ) const {
   return _bondName;
}
 
/** 
 * Set log file reference
 * 
 * @param  log file reference
 *
 * @return  DefaultReturnValue
 *
 */

int BrookBondParameters::setLog( FILE* log ){
   _log = log;
   return DefaultReturnValue;
}

/** 
 * Get number of bonds
 * 
 * @return  numberOfBonds
 *
 */

int BrookBondParameters::getNumberOfBonds( void ) const {
   return _numberOfBonds;
}

/** 
 * Get number of particles in bond
 * 
 * @return  numberOfParticlesInBond
 *
 */

int BrookBondParameters::getNumberOfParticlesInBond( void ) const {
   return _numberOfParticlesInBond;
}

/** 
 * Get number of parameters in bond
 * 
 * @return numberOfParametersInBond 
 *
 */

int BrookBondParameters::getNumberOfParametersInBond( void ) const {
   return _numberOfParametersInBond;
}

/** 
 * Initialize the kernel, setting up the values of all the force field parameters.
 * 
 * @param bondIndex                 bond index to be set
 * @param particleIndices           indices of particles (dimension=numberOfParticlesInBond)
 * @param bondParameters            bond parameters (dimension=numberOfParametersInBond)
 *
 * @return DefaultReturnValue
 *
 * @throw OpenMMException exeception if bond index is invalid
 *
 */
 
int BrookBondParameters::setBond( int bondIndex, int* particleIndices, double* bondParameters ){ 

// ---------------------------------------------------------------------------------------

   static const std::string methodName      = "BrookBondParameters::setBond";
   FILE* log                                = getLog();

   // ---------------------------------------------------------------------------------------

   // validate bond index

   if( bondIndex < 0 ){
      std::stringstream message;
      message << methodName << "BondIndex=" << bondIndex << " is < 0.";
      throw OpenMMException( message.str() );
   }

   if( bondIndex >= getNumberOfBonds() ){
      std::stringstream message;
      message << methodName << "BondIndex=" << bondIndex << " is >= " << getNumberOfBonds() << ".";
      throw OpenMMException( message.str() );
   }

   // load'em up

   int numberOfParticlesInBond = getNumberOfParticlesInBond();
   for( int ii = 0; ii < numberOfParticlesInBond; ii++ ){
      _particleIndices[bondIndex].push_back( particleIndices[ii] );
   }

   int numberOfParametersInBond = getNumberOfParametersInBond();
   for( int ii = 0; ii < numberOfParametersInBond; ii++ ){
//fprintf( stderr, "Param %s %d %e\n", getBondName().c_str(), ii, bondParameters[ii] );
      _bondParameters[bondIndex].push_back( bondParameters[ii] );
   }

   // ---------------------------------------------------------------------------------------

   return DefaultReturnValue;
}

/* 
 * Format line
 *
 * @param tab         tab
 * @param description description
 * @param value       value
 *
 * @return string containing contents
 *
 * */

std::string BrookBondParameters::_getLine( const std::string& tab,
                                           const std::string& description,
                                           const std::string& value ) const {

// ---------------------------------------------------------------------------------------

   static const std::string methodName      = "BrookStreamInternal::_getLine";
   static const unsigned int MAX_LINE_CHARS = 256;
   char line[MAX_LINE_CHARS];

// ---------------------------------------------------------------------------------------

   std::stringstream message;
   memset( line, ' ', MAX_LINE_CHARS );  
#ifdef _MSC_VER
   (void) sprintf_s( line, MAX_LINE_CHARS, "%s %-40s %s", tab.c_str(), description.c_str(), value.c_str() );
#else
   (void) sprintf( line, "%s %-40s %s", tab.c_str(), description.c_str(), value.c_str() );
#endif
   message << std::string( line ) << std::endl;

   return message.str();

}

/* 
 * Get contents of object
 *
 * @param level   level of dump
 *
 * @return string containing contents
 *
 * */

std::string BrookBondParameters::getContentsString( int level ) const {

// ---------------------------------------------------------------------------------------

   static const std::string methodName      = "BrookBondParameters::getContentsString";

   static const unsigned int MAX_LINE_CHARS = 1024;
   char value[MAX_LINE_CHARS];
   static const char* Set                   = "Set";
   static const char* NotSet                = "Not set";

// ---------------------------------------------------------------------------------------

   std::stringstream message;
   std::string tab   = "   ";

#ifdef _MSC_VER
#define LOCAL_SPRINTF(a,b,c) sprintf_s( (a), MAX_LINE_CHARS, (b), (c) );   
#define LOCAL_2_SPRINTF(a,b,c,d) sprintf_s( (a), MAX_LINE_CHARS, (b), (c), (d) );   
#else
#define LOCAL_SPRINTF(a,b,c) sprintf( (a), (b), (c) );   
#define LOCAL_2_SPRINTF(a,b,c,d) sprintf( (a), (b), (c), (d) );   
#endif

   (void) LOCAL_SPRINTF( value, "%s", getBondName().c_str() );
   message << _getLine( tab, "Bond name:", value ); 

   (void) LOCAL_SPRINTF( value, "%d", getNumberOfBonds() );
   message << _getLine( tab, "Number of bonds:", value ); 

   (void) LOCAL_SPRINTF( value, "%d", getNumberOfParticlesInBond() );
   message << _getLine( tab, "Particles/bond:", value ); 

   (void) LOCAL_SPRINTF( value, "%d", getNumberOfParametersInBond() );
   message << _getLine( tab, "Parameters/bond:", value ); 

   message << "Bonds:"  << std::endl; 
   for( int ii = 0; ii < getNumberOfBonds(); ii++ ){
      const static size_t descriptionSz = 1024;
      char description[1024];
      char buffer[1024];
      (void) LOCAL_SPRINTF( description, "%6d [", ii );

#ifdef _MSC_VER

      // particle indices

      for( int jj = 0; jj < getNumberOfParticlesInBond(); jj++ ){
         (void) LOCAL_SPRINTF( buffer, "%6d ", _particleIndices[ii][jj] );
         (void) strcat_s( description, descriptionSz, buffer );
      }
      (void) strcat_s( description, descriptionSz, "] [" );

      // parameters

      for( int jj = 0; jj < getNumberOfParametersInBond(); jj++ ){
         (void) LOCAL_SPRINTF( buffer, "%18.10e ", _bondParameters[ii][jj] );
         (void) strcat_s( description, descriptionSz, buffer );
      }

      (void) strcat_s( description, descriptionSz, "]" );
#else

      // particle indices

      for( int jj = 0; jj < getNumberOfParticlesInBond(); jj++ ){
         (void) LOCAL_SPRINTF( buffer, "%6d ", _particleIndices[ii][jj] );
         (void) strcat( description, buffer );
      }
      (void) strcat( description, "] [" );

      // parameters

      for( int jj = 0; jj < getNumberOfParametersInBond(); jj++ ){
         (void) LOCAL_SPRINTF( buffer, "%18.10e ", _bondParameters[ii][jj] );
         (void) strcat( description, buffer );
      }
      (void) strcat( description, "]" );
#endif
      message << _getLine( tab, "", description );
   }
 
#undef LOCAL_SPRINTF
#undef LOCAL_2_SPRINTF

   return message.str();
}
