/**
 * @file simulation.h
 *
 *  Editor a simulator vysokourovnovych Petriho siti
 *
 *  Created on: Mar 10, 2012
 *  @author Martin Cincala <xcinca00@stud.fit.vutbr.cz>
 *  @version 1.0
 *
 *  @section DESCRIPTION
 *  Implementation of the simulation classes
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <QList>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QMultiMap>
#include <QtAlgorithms>
#include <QTime>

#include <iostream>


const int intMin = -32767;
const int intMax = 32767;

/**
 * Class Places holds list of all places and provides
 * methods for operations with them.
 */
class Places{
private:

	/**
	 * Place in Petri network
	 * Place is represented with ID and list of tokens
	 */
	struct Place{
		/* ID of the place */
		QString placeID;

		/* List of tokens */
		QList<int> tokens;

		/**
		 * Check if place contains tokens
		 * @param minTokens minimum number of tokens
		 * @param minValue min value of the tokens
		 * @param maxValue max value of the tokens
		 * @return TRUE if place contains tokens, FALSE if not
		 */
		int checkToken(int minTokens, int minValue, int maxValue);

		/**
		 * Delete token from the place
		 * @param count number of tokens to delete
		 * @param value value of the token
		 */
		bool delToken(int value, int count);
	};

	/* Class Transitions is friend */
	friend class Transitions;
	friend class WriteXml;

public:
	/**
	 * Add new place to the list with given ID
	 * @param plaID ID (number) of the Place
	 */
	void addPlace(QString plaID);

	/**
	 * Search list of places for place with required ID
	 * @param plaID ID of the place
	 */
	Place* getPlace(QString plaID);

	/**
	 * Add token to the place and sort tokens
	 * @param Place reference to the place
	 * @param value value of the token
	 */
	void addToken(Place &pPlace, int value);

	/**
	 * Check place for tokens
	 * @param pPlace reference to the place
	 * @param value value of the token
	 * @param count number of tokens with the same value
	 */
	bool hasTokens(Place &pPlace, int value, int count);

	/* List of all places */
	QList<Place*> placeList;
};

/**
 * Class Transitions contains list of all transitions
 * and edges with methods.
 */
class Transitions{
private:
	friend class WriteXml;

	/**
	 * Edge from transition to place
	 */
	struct Edge{

		/* Pointer to the connected place */
		Places::Place* pPlace;
		QList<int> boundedVars;
		/* weight of the edge */
		int weight;
		bool constant;
		int constValue;

	};

	/**
	 * Input condition in transition
	 */
	struct Condition{
		/* Operators */
		QString op1;
		QString op2;
		bool isOp2Const;
		int op2Const;

		/* Relation between operators*/
		QString rel;
	};

	/**
	 * Output operation in transition
	 */
	struct Operation{
		QString outputVar;	//variable we want to compute
		QString expression;	//numeric expression


	};

	/**
	 * Transition class - holds ID of every transition,
	 * lists of input and output edges, input
	 * conditions, output operations and waiting time.
	 */
	struct Transition{
		/* ID of transition */
		QString transitionID;

		/* Waiting time */
		int delay;

		/* mapping letter to ID of the edge */
		/* <variable, edgeID> */
		QMultiMap<QString, Edge*> inputEdges;
		QMultiMap<QString, Edge*> outputEdges;

		/* List of input conditions and operations */
		QList<Condition> conditions;
		QList<Operation> operations;

		/* Calculated input variables */
		QMap<QString, QList<int> > inputVarResult;	//TODO: QList pointer would be better

		/*
		 * Get delay of the transition
		 */
		int getDelay(){ return delay; }

		/**
		 * Get number of the edges with given variable
		 * @param variable variable on the edge
		 */
		int getInputVarCount(QString &variable);

		/**
		 * Get number of the edges with given variable
		 * @param variable variable on the edge
		 */
		int getOutputVarCount(QString variable);

		/**
		 * Check if transition is executable
		 * @return TRUE if executable FALSE if not executable
		 */
		bool isExecutable();

		/**
		 * Execute transition - delete and add tokens
		 */
		bool executeTrans();

		/**
		 * Compute operation and return result
		 * @param index index of the operation
		 * @return result of the expression
		 */
		int computeOperation(int index);

		/**
		 * Compute variables on output edges
		 */
		void computeOutputVars();

		/**
		 * Bound variables on input edges
		 * @return TRUE if bound successful, FALSE if not
		 */
		bool boundVarsInputEdges();

		/**
		 * Filter variables (if more edges with the same variable
		 * and append to output variable array
		 */
		bool filterBoundedVars();

		/**
		 * Check if all conditions are true
		 * @return TRUE if all conditions passed, FALSE if not
		 */
		bool conditionsPassed();

		/**
		 * Check all conditions in transition,
		 * if condition is not true, return FALSE
		 * @return TRUE if all conditions PASSED
		 */
		bool checkAllConditions();

		/**
		 * Delete from results all tokens except one
		 * @param index index of variable
		 * @param variable which tokens to delete
		 */
		void deleteAllIndexExcept(QString variable, int index);

		/**
		 * Delete from results all tokens lower than index
		 * @param limit limit of deleted tokens
		 * @param variable which tokens to delete
		 */
		void deleteBoundLowerThan(QString variable, int limit);

		/**
		 * Delete from results all tokens higher than index
		 * @param limit limit of deleted tokens
		 * @param variable which tokens to delete
		 */
		void deleteBoundHigherThan(QString variable, int limit);

		/**
		 * Delete from results all tokens not equal to limit
		 * @param limit limit of deleted tokens
		 * @param variable which tokens to delete
		 */
		void deleteBoundNotEq(QString variable, int limit);

		/**
		 * Delete from results all tokens qhich are not in intersect
		 * @param variable1 tokens to delete
		 * @param variable2 which tokens to delete
		 */
		void deleteNotIntersect(QString variable1, QString variable2);

		/**
		 * Get highest token of the tokens in place
		 * @param variable - requested variable
		 * @return highest token
		 */
		int getHighestToken(QString variable);

		/**
		 * Get lowest tokens fom list of tokens
		 * @param variable pointing to list of tokens
		 * @return lowest token
		 */
		int getLowestToken(QString variable);


	};


public:

	/* List of all edges */
	QVector<Edge*> edgeList;

	/* List of all transitions */
	QList<Transition*> transList;

	/* flag if simulation is initialized */
	bool simInit;

	Transitions();
	void addTrans(QString transID);
	Transition* getTrans(QString transID);
	void setDelay(Transition &pTrans, int ddelay);
	void addCond(Transition &pTrans, QString Op1, QString Op2, QString Rel);
	void addOper(Transition &pTrans, QString operString);
	void connInputPlace(Transition* pTrans, Places::Place* placeID,
			QString variable, QString weight);
	void connOutputPlace(Transition* pTrans, Places::Place* placeID,
			QString variable, QString weight);
};



#endif /* SIMULATION_H_ */
