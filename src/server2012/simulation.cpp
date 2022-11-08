/**
 * @file simulation.cpp
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

#include "simulation.h"

using namespace std;

Transitions::Transitions(){

}

/**
 * Add new transition to the list with given ID
 * @param transID ID (number) of the Transition
 */
void Transitions::addTrans(QString transID){
	Transition *newTrans = new Transition;

	newTrans->transitionID = transID;

	transList.append(newTrans);
}


/**
 * Set time to wait for transition
 * @param pTrans reference to transition
 * @param ddelay delay
 */
void Transitions::setDelay(Transition &pTrans, int ddelay){
	pTrans.delay = ddelay;
}

/**
 * Add conditions to transitions
 * @param pTrans reference to transition
 * @param condString condition
 */
void Transitions::addCond(Transition &pTrans, QString Op1, QString Op2, QString Rel){
	Condition newCond;
	bool ok;

	newCond.op1 = Op1;
	newCond.op2 = Op2;
	newCond.rel = Rel;

	newCond.op2Const = Op2.toInt(&ok, 10);
	if(ok) newCond.isOp2Const = TRUE;
	else newCond.isOp2Const = FALSE;

	pTrans.conditions.append(newCond);
}

/**
 * Add operations to transitions
 * @param pTrans reference to transition
 * @param operString operation
 */
void Transitions::addOper(Transition &pTrans, QString operString){
	Operation newOper;
	newOper.outputVar = operString.at(0);
	newOper.expression = operString.section('=',1,2);

	pTrans.operations.append(newOper);
}

/**
 * Get pointer to transition with given ID
 * @param transID ID of transition
 */
Transitions::Transition* Transitions::getTrans(QString transID){
	for(int i = 0; i < transList.count(); i++){
		if(transList.at(i)->transitionID == transID)
			return transList.at(i);
	}
	return FALSE;
}

/**
 * Connect input place to transition
 */
void Transitions::connInputPlace(
		Transition* pTrans,
		Places::Place* pPlace,
		QString variable,
		QString weight){

	Edge *newEdge = new Edge;
	bool ok;

	newEdge->constValue = variable.toInt(&ok, 10);
	if(ok) newEdge->constant = TRUE;
	else newEdge->constant = FALSE;

	newEdge->pPlace = pPlace;
	newEdge->weight = weight.toInt(&ok, 10);
	edgeList.append(newEdge);

	pTrans->inputEdges.insert(variable, newEdge);

}

/**
 * Connect output place to transition
 */
void Transitions::connOutputPlace(
		Transition* pTrans,
		Places::Place* pPlace,
		QString variable,
		QString weight){

	Edge *newEdge = new Edge;
	bool ok;

	newEdge->constValue = variable.toInt(&ok, 10);
	if(ok) newEdge->constant = TRUE;
	else newEdge->constant = FALSE;

	newEdge->pPlace = pPlace;
	newEdge->weight = weight.toInt(&ok, 10);
	edgeList.append(newEdge);

	pTrans->outputEdges.insert(variable, newEdge);

}

/**
 * Get number of the edges with given variable
 * @param variable variable on the edge
 */
int Transitions::Transition::getInputVarCount(QString &variable){
	return inputEdges.count(variable);
}

/**
 * Get number of the edges with given variable
 * @param variable variable on the edge
 */
int Transitions::Transition::getOutputVarCount(QString variable){
	return outputEdges.count(variable);
}

/**
 * Check if transition is executable
 * @return TRUE if executable FALSE if not executable
 */
bool Transitions::Transition::isExecutable(){
	QMultiMap<QString, Edge*>::iterator iterEdge = inputEdges.begin();
	/* do cleanup */
	iterEdge = inputEdges.begin();

	while(iterEdge != inputEdges.end()){
		iterEdge.value()->boundedVars.clear();
		++iterEdge;
	}

	iterEdge = outputEdges.begin();

	while(iterEdge != outputEdges.end()){
		iterEdge.value()->boundedVars.clear();
		++iterEdge;
	}

	if(!boundVarsInputEdges()){
		//qDebug() << "No tokens left" << endl;
		return FALSE;
	}
	else{
		if(!filterBoundedVars()){
			//qDebug() << "Filtering variables empty" << endl;
			return FALSE;
		}
		else{
			if(!conditionsPassed()){
				//qDebug() << "Condition did not pass" << endl;
				return FALSE;
			}
		}
	}
	return TRUE;

}

/**
 * Bound variables on input edges
 * @return TRUE if bound successful, FALSE if not
 */
bool Transitions::Transition::boundVarsInputEdges(){
	QMultiMap<QString, Edge*>::iterator iter = inputEdges.begin();
	int tmpToken = intMin;
	int tmpMinValue = intMin;
	int tmpMaxValue = intMax;

	/* process all input edges */
	while(iter != inputEdges.end()){
		tmpMinValue = intMin;
		tmpMaxValue = intMax;

		/* check for constant on the edge */
		if(iter.value()->constant){
			if((tmpToken = iter.value()->pPlace->checkToken(iter.value()->weight,
					iter.value()->constValue, iter.value()->constValue)) != intMax){
				iter.value()->boundedVars.append(tmpToken);
			}
			else return FALSE;
		}
		else{
			/* not constant - get tokens */
			while((tmpToken = iter.value()->pPlace->checkToken(iter.value()->weight,
					tmpMinValue, tmpMaxValue)) != intMax){

				iter.value()->boundedVars.append(tmpToken);
				tmpMinValue = tmpToken + 1;
			}
			if(tmpMinValue == intMin) return FALSE;
		}

		++iter;
	}
	return TRUE;
}

/**
 * Filter variables (if more edges with the same variable
 * and append to output variable array
 */
bool Transitions::Transition::filterBoundedVars(){

	QList<Edge*> tmpInputPlaceList;
	QList<int> tmpInputVarList;
	QSet<int> tmpInputVarSet1, tmpInputVarSet2, intersectSet;
	QMultiMap<QString, Edge*>::iterator iter = inputEdges.begin();

	while(iter != inputEdges.end()){
		//qDebug() << "iter key count" <<inputEdges.count(iter.key()) << iter.key();

		if(inputEdges.count(iter.key()) == 1){	//variable is only on single edge
			inputVarResult.insert(iter.key(), iter.value()->boundedVars);

		}
		else if(inputEdges.count(iter.key()) > 1){	//variable is on multiple edges
			tmpInputVarSet1 = QSet<int>::fromList(iter.value()->boundedVars);
			tmpInputPlaceList = inputEdges.values(iter.key());
			intersectSet = tmpInputVarSet1;

			/* intersect all places */
			for(int i = 0; i < tmpInputPlaceList.size(); i++){
				tmpInputVarList = tmpInputPlaceList.at(i)->boundedVars;
				tmpInputVarSet2 = QSet<int>::fromList(tmpInputVarList);
				/* intersect two places */
				intersectSet = intersectSet.intersect(tmpInputVarSet2);
				if(intersectSet.empty()) return FALSE;
			}
			/* insert results to map */
			//QList<int> intersecResult = new QList<int>;
			inputVarResult.insert(iter.key(), intersectSet.toList());
		}
		//qDebug() << "filtered bounded vars:" << iter.key() << inputVarResult.find(iter.key()).value();
		++iter;
	}

	return TRUE;
}


/**
 * Check all conditions in transition,
 * if condition is not true, return FALSE
 * @return TRUE if all conditions PASSED
 */
bool Transitions::Transition::checkAllConditions(){
	int tmpLow, tmpHigh;

	/* one loop - check all conditions */
	for(int j = 0; j < conditions.count(); j++){

		/* LOWER */
		if(conditions.at(j).rel == "<"){
			if(conditions.at(j).op2Const){	//op2 is constant
				deleteBoundHigherThan(conditions.at(j).op1, conditions.at(j).op2Const-1);
				if(inputVarResult.value(conditions.at(j).op1).empty())
					return FALSE;
			}
			else{	//op2 is not constant
				tmpHigh = getHighestToken(conditions.at(j).op2);
				deleteBoundHigherThan(conditions.at(j).op1, tmpHigh-1);

				if(inputVarResult.value(conditions.at(j).op1).empty())
					return FALSE;

				tmpLow = getLowestToken(conditions.at(j).op1);
				deleteBoundLowerThan(conditions.at(j).op2, tmpLow+1);

				if(inputVarResult.value(conditions.at(j).op2).empty())
					return FALSE;
			}

		}

		/* LOWER OR EQUAL */
		else if(conditions.at(j).rel == "<="){
			if(conditions.at(j).op2Const){	//op2 is constant
				deleteBoundHigherThan(conditions.at(j).op1, conditions.at(j).op2Const);
				if(inputVarResult.value(conditions.at(j).op1).empty())
					return FALSE;
			}
			else{	//op2 is not constant
				tmpHigh = getHighestToken(conditions.at(j).op2);
				deleteBoundHigherThan(conditions.at(j).op1, tmpHigh);

				if(inputVarResult.value(conditions.at(j).op1).empty())
					return FALSE;

				tmpLow = getLowestToken(conditions.at(j).op1);
				deleteBoundLowerThan(conditions.at(j).op2, tmpLow);

				if(inputVarResult.value(conditions.at(j).op2).empty())
					return FALSE;
			}
		}

		/* HIGHER OR EQUAL */
		else if(conditions.at(j).rel == ">="){
			if(conditions.at(j).op2Const){	//op2 is constant
				deleteBoundLowerThan(conditions.at(j).op1, conditions.at(j).op2Const);
				if(inputVarResult.value(conditions.at(j).op1).empty())
					return FALSE;
			}
			else{	//op2 is not constant
				tmpLow = getLowestToken(conditions.at(j).op2);
				deleteBoundLowerThan(conditions.at(j).op1, tmpLow);

				if(inputVarResult.value(conditions.at(j).op1).empty())
					return FALSE;

				tmpHigh = getHighestToken(conditions.at(j).op1);
				deleteBoundHigherThan(conditions.at(j).op2, tmpHigh);

				if(inputVarResult.value(conditions.at(j).op2).empty())
					return FALSE;
			}
		}

		/* HIGHER */
		else if(conditions.at(j).rel == ">"){
			if(conditions.at(j).op2Const){	//op2 is constant
				deleteBoundLowerThan(conditions.at(j).op1, conditions.at(j).op2Const+1);
				if(inputVarResult.value(conditions.at(j).op1).empty())
					return FALSE;
			}
			else{	//op2 is not constant
				//qDebug() << "inputvarres1:" << inputVarResult;

				tmpLow = getLowestToken(conditions.at(j).op2);
				deleteBoundLowerThan(conditions.at(j).op1, tmpLow + 1);

				if(inputVarResult.value(conditions.at(j).op1).empty())
					return FALSE;

				tmpHigh = getHighestToken(conditions.at(j).op1);
				deleteBoundHigherThan(conditions.at(j).op2, tmpHigh - 1);

				if(inputVarResult.value(conditions.at(j).op2).empty())
					return FALSE;
				//qDebug() << "inputvarres2:" << inputVarResult;
			}
		}

		/* EQUAL */
		else if(conditions.at(j).rel == "=="){
			if(conditions.at(j).op2Const){	//op2 is constant
				deleteBoundNotEq(conditions.at(j).op1, conditions.at(j).op2Const);
				if(inputVarResult.value(conditions.at(j).op1).empty())
					return FALSE;
			}
			else{	//op2 is not constant
				deleteNotIntersect(conditions.at(j).op1, conditions.at(j).op2);

				if(inputVarResult.value(conditions.at(j).op1).empty())
					return FALSE;
				if(inputVarResult.value(conditions.at(j).op2).empty())
					return FALSE;
			}
		}
		else{
			qDebug() << "unknown operator:" << conditions.at(j).rel;
		}
		/* NOT EQUAL */
		//to be added - more complicated...

/*		qDebug() << "bounded vars after conditions:" << conditions.at(j).op1 << inputVarResult.find(conditions.at(j).op1).value();
		if(!conditions.at(j).op2Const)
			qDebug() << "bounded vars after conditions:" << conditions.at(j).op2 << inputVarResult.find(conditions.at(j).op2).value();
		else
			qDebug() << "bounded vars after conditions:" << conditions.at(j).op2Const;*/

	}
	return TRUE;
}


/**
 * Check if all conditions are true
 * @return TRUE if all conditions passed, FALSE if not
 */
bool Transitions::Transition::conditionsPassed(){
	int tmpRand;

	//qDebug() << "starting eval conditions";

	/* check all conditions repeatedly for changes */
	for(int i = 0; i < conditions.count(); i++){
		if(checkAllConditions() == FALSE)
			return FALSE;
	}


	/* final variable binding */
	QMultiMap<QString, QList<int> >::iterator iter = inputVarResult.begin();
	while(iter++ != inputVarResult.end()-1){
		if(iter.value().count() > 1){
			QTime now = QTime::currentTime();
			qsrand(now.msec());
			tmpRand = (qrand() % iter.value().count());
			deleteAllIndexExcept(iter.key(), tmpRand);

			if(checkAllConditions() == FALSE)
				return FALSE;
		}

	}
	return TRUE;
}

/**
 * Delete from results all tokens except one
 * @param index index of variable
 * @param variable which tokens to delete
 */
void Transitions::Transition:: deleteAllIndexExcept(QString variable, int index){
	QMultiMap<QString, QList<int> >::iterator iter = inputVarResult.find(variable);
	for(int i = 0; i < iter.value().size(); i++){
		if(i != index)
			iter.value().removeAt(i);
	}
}


/**
 * Delete from results all tokens lower than index
 * @param limit limit of deleted tokens
 * @param variable which tokens to delete
 */
void Transitions::Transition::deleteBoundLowerThan(QString variable, int limit){
	QMultiMap<QString, QList<int> >::iterator iter = inputVarResult.find(variable);

	for(int i = 0; i < iter.value().size();){
		if(iter.value().at(i) < limit){
			iter.value().removeAt(i);
		}
		else i++;
	}
}

/**
 * Delete from results all tokens higher than index
 * @param limit limit of deleted tokens
 * @param variable which tokens to delete
 */
void Transitions::Transition::deleteBoundHigherThan(QString variable, int limit){
	QMultiMap<QString, QList<int> >::iterator iter = inputVarResult.find(variable);
	for(int i = 0; i < iter.value().size(); i++){
		if(iter.value().at(i) > limit){
			iter.value().removeAt(i);
		}
	}
}

/**
 * Delete from results all tokens not equal to limit
 * @param limit limit of deleted tokens
 * @param variable which tokens to delete
 */
void Transitions::Transition::deleteBoundNotEq(QString variable, int limit){
	QMultiMap<QString, QList<int> >::iterator iter = inputVarResult.find(variable);
	for(int i = 0; i < iter.value().size(); i++){
		if(iter.value().at(i) != limit){
			iter.value().removeAt(i);
		}
	}
}

/**
 * Delete from results all tokens qhich are not in intersect
 * @param variable1 tokens to delete
 * @param variable2 which tokens to delete
 */
void Transitions::Transition::deleteNotIntersect(QString variable1, QString variable2){
	QMultiMap<QString, QList<int> >::iterator iter1 = inputVarResult.find(variable1);
	QMultiMap<QString, QList<int> >::iterator iter2 = inputVarResult.find(variable2);

	QSet<int> tmpSet1 = iter1.value().toSet();
	QSet<int> tmpSet2 = iter2.value().toSet();

	tmpSet1 = tmpSet1.intersect(tmpSet2);

	iter1.value() = tmpSet1.toList();
	iter2.value() = tmpSet1.toList();
}

/**
 * Get highest token of the tokens in place
 * @param variable - requested variable
 * @return highest token
 */
int Transitions::Transition::getHighestToken(QString variable){
	QMultiMap<QString, QList<int> >::iterator iter = inputVarResult.find(variable);
	int tmpMax = iter.value().at(0);

	for(int i = 1; i < iter.value().size(); i++){
		if(iter.value().at(i) > tmpMax){
			tmpMax = iter.value().at(i);
		}
	}
	return tmpMax;
}


/**
 * Get lowest tokens fom list of tokens
 * @param variable pointing to list of tokens
 * @return lowest token
 */
int Transitions::Transition::getLowestToken(QString variable){
	QMultiMap<QString, QList<int> >::iterator iter = inputVarResult.find(variable);
	if(iter.value().empty())
		qDebug() << "getLowestToken iter value empty";
	int tmpMin = iter.value().first();

	for(int i = 1; i < iter.value().size(); i++){
		if(iter.value().at(i) < tmpMin){
			tmpMin = iter.value().at(i);
		}
	}
	return tmpMin;
}

/**
 * Execute transition - delete and add tokens
 */
bool Transitions::Transition::executeTrans(){
	QMultiMap<QString, Edge*>::iterator iterEdge = inputEdges.begin();
	QMap<QString, QList<int> >::iterator iterInputVars;
	QString var;

	//pWriteXml->newXmlDoc();

	/* delete tokens from all input edges */
	while(iterEdge != inputEdges.end()){
		/* find edge variable in results */
		var = iterEdge.key();
		iterInputVars = inputVarResult.find(var);

		/* delete tokens from place */
		if(!iterEdge.value()->pPlace->delToken(iterInputVars.value().at(0),
				iterEdge.value()->weight)){

			qDebug() << "error while deleting tokens";
			return FALSE;
		}

		//pWriteXml->appendPlace();

		++iterEdge;
	}


	/* compute output variables */
	computeOutputVars();

	return TRUE;
}

/**
 * Compute variables on output edges
 */
void Transitions::Transition::computeOutputVars(){
	//qDebug() << "computing output variables";
	int result;
	QString outVar;

	for(int i = 0; i < operations.size(); i++){
		result = computeOperation(i);
		outVar = operations.at(i).outputVar;
		for(int j = 0; j < outputEdges.value(outVar)->weight; j++){
			outputEdges.value(outVar)->pPlace->tokens.append(result);
			//qDebug() << "output var:" << outVar << "value:" << result;
		}
	}
}

/**
 * Compute operation and return result
 * @param index index of the operation
 */
int Transitions::Transition::computeOperation(int index){
	QString num;
	QChar lastOper = QChar('+');
	int numToInt = 0;;
	int result = 0;
	bool ok;
	for(int i = 0; i < operations.at(index).expression.size(); i++){
		if((operations.at(index).expression.at(i) == QChar('+')) ||
				operations.at(index).expression.at(i) == QChar('-')){
			if(lastOper == QChar('+')){
				numToInt = num.toInt(&ok, 10);
				if(ok)
					result = result + numToInt;
				else
					result = result + inputVarResult.value(num).at(0);
			}
			else{
				numToInt = num.toInt(&ok, 10);
				if(ok)
					result = result - numToInt;
				else
					result = result - inputVarResult.value(num).at(0);
			}

			lastOper = operations.at(index).expression.at(i);
			num.clear();

		}
		else{
			num.append(operations.at(index).expression.at(i));
		}
	}

	if(lastOper == QChar('+')){
		numToInt = num.toInt(&ok, 10);
		if(ok)
			result = result + numToInt;
		else
			result = result + inputVarResult.value(num).at(0);
	}
	else{
		numToInt = num.toInt(&ok, 10);
		if(ok)
			result = result - numToInt;
		else
			result = result - inputVarResult.value(num).at(0);
	}
	num.clear();

	return result;
}

/**
 * Add new place to the list with given ID
 * @param plaID ID (number) of the Place
 */
void Places::addPlace(QString plaID){
	Place *newPlace = new Place;

	newPlace->placeID = plaID;

	placeList.append(newPlace);
}

/**
 * Search list of places for place with required ID
 * @param plaID ID of the place
 */
Places::Place* Places::getPlace(QString plaID){
	for(int i = 0; i < placeList.count(); i++){
		if(placeList.at(i)->placeID == plaID)
			return placeList.at(i);
	}
	return FALSE;
}


/**
 * Add token to the place and sort tokens
 * @param Place reference to the place
 * @param value value of the token
 */
void Places::addToken(Place &pPlace, int value){
	pPlace.tokens.append(value);
	qSort(pPlace.tokens.begin(), pPlace.tokens.end());
}

/**
 * Delete token from the place
 * @param count number of tokens to delete
 * @param value value of the token
 */
bool Places::Place::delToken(int value, int count){
	/* remove first occurence of value in the list */
	for(int i = 0; i < count; i++){
		//qDebug() << "tokens:" << tokens << "token value" << value << "count" << count;
		if(!tokens.removeOne(value)) return FALSE;
	}
	return TRUE;
}

/**
 * Check place for tokens
 * @param pPlace reference to the place
 * @param value value of the token
 * @param count number of tokens with the same value
 */
bool Places::hasTokens(Place &pPlace, int value, int count = 1){
	int counter = 0;
	for(int i = 0; i < pPlace.tokens.count(); i++){
		if(pPlace.tokens.at(i) == value)
			counter++;
	}

	if(counter < count) return FALSE;

	return TRUE;
}

/**
 * Check if place contains tokens
 * @param minTokens minimum number of tokens
 * @param minValue min value of the tokens
 * @param maxValue max value of the tokens
 * @return TRUE if place contains tokens, FALSE if not
 */
int Places::Place::checkToken(int minTokens,
		int minValue, int maxValue){

	int counter = 0;
	int tmpToken = intMin;

	for(int i = 0; i < tokens.count(); i++){
		if(counter >= minTokens) break;
		else if((tokens.at(i) >= minValue) && (tokens.at(i) <= maxValue)){
			counter++;
			if(tmpToken != tokens.at(i)){
				counter = 1;
				tmpToken = tokens.at(i);
			}
		}
	}

	return (counter >= minTokens) ? tmpToken : intMax;
}
