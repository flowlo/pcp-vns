/// Contains all definitions of units for different neighborhoods of the VNS
#ifndef VNS_PRIV_H
#define VNS_PRIV_H
#include "pcp.hpp"
#include "vns.hpp"
#include <time.h>
#include <utility>
#include <boost/tuple/tuple.hpp>
#include <signal.h>

namespace pcp {
	//! The base class for all neighborhood units
	/*!
		The base class from which all other neighborhood units must inherit.
		Every unit must provide on method to imrove a solution and
		one method to shake a solution.
		\sa changeColor, changeNode, dsatur
	*/
	class VNS_Unit {
		public:
			//! Prototype for an improvement method
			virtual Solution findLocalMin(Solution& curBest) = 0;

			//! Prototype for a shaking method
			virtual Solution shuffleSolution(Solution& cur, int numSteps) = 0;

			//! Prototype destructor
			virtual ~VNS_Unit();

			//! Prototype name
			virtual const std::string getName() = 0;
			
			//! Prototype abbreviation
			static const char getAbbreviation();
	};
	
	//! Implementation of the changeColor neighborhood
	/*!
		changeColor is a simple and fast neighborhood.
		It is based on the idea of recoloring single vertices, and then trying
		to solve the generated conflicts by recoloring neighboring vertices.
	*/
	class changeColor : public VNS_Unit {
		public:
			virtual Solution findLocalMin(Solution& curBest);
			virtual Solution shuffleSolution(Solution& cur, int numSteps);

			virtual const std::string getName();
			static const char getAbbreviation();
	};

	//! Implementation of the changeNode neighborhood
	/*!
		changeNode is a simple neighborhood.
		It is based on the idea of replacing single vertices, and then trying
		to solve the generated conflicts by recoloring the recentyl replaced vertex.
	*/
	class changeNode : public VNS_Unit {
		public:
			virtual Solution findLocalMin(Solution& curBest);
			virtual Solution shuffleSolution(Solution& cur, int numSteps);

			virtual const std::string getName();
			static const char getAbbreviation();
	};

	//! Implementation of the changeAll neighborhood
	/*!
		The changeAll neighborhood is a combination of changeColor and changeNode.
		It is based on the idea of replacing single vertices, recoloring them and
		then trying to solve any remaining conflicts by recoloring neighboring
		vertices.
	*/
	class changeAll : public VNS_Unit {
		public:
			virtual Solution findLocalMin(Solution& curBest);
			virtual Solution shuffleSolution(Solution& cur, int numSteps);

			virtual const std::string getName();
			static const char getAbbreviation();
	};

	//! Implementation of the dsatur neighborhood
	/*!
		dsatur is a very simple and fast neighborhood. It is based on the idea
		of solving a traditional graph coloring problem, by completely ignoring
		the different selections of vertices.
	*/
	class dsatur : public VNS_Unit {
		public:
			virtual Solution findLocalMin(Solution& curBest);
			virtual Solution shuffleSolution(Solution& cur, int numSteps);

			virtual const std::string getName();
			static const char getAbbreviation();
	};
}
#endif
