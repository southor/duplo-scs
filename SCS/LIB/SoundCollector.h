
#ifndef _SOUNDCOLLECTOR_
#define _SOUNDCOLLECTOR_

#include "Base.h"
#include "MultiTone.h"
#include "Module.h"
#include "Instrument.h"

struct InstrumentUse
{
	Instrument *instr;
	double pan;
	double vol;
	double expr;
	double oct; //octave
};

class InstrumentArray
{
private:
	int maxInst;
	
	//Instrument* Array[16];
	InstrumentUse Array[16];

	int currentInst;

	void ResetInstrumentArray()
	{
		nInst = 0;
		for(int i=0;i<maxInst;i++)
		{
			//Array[i] = {NULL, 0.0, 50.0, 1.0};
			Array[i].instr = NULL;
			Array[i].pan = 0.0;
			Array[i].vol = 1.0;
			Array[i].expr = 1.0;
			Array[i].oct = 1.0;
		}
	}

protected:

	int nInst;

public:
	
	InstrumentArray()
	{
		maxInst = 16;
		ResetInstrumentArray();
		currentInst = 0;
	}

	int InstrumentSize()
	{
		return nInst;
	}

	//bool addInstrument(Instrument *Inst)
	//{
	//	if (nInst < maxInst)
	//	{
	//		Array[nInst] = Inst;
	//		nInst++;
	//		return true;
	//	}
	//	else return false;
	//}
	
	//Instrument *getInstrument(int i)
	//{
	//	if (i < nInst && i >= 0) return Array[i];
	//	else return NULL;
	//}

	int setCurrentInstrumentNumber(int i)
	{
		assert(i >= 0);
		assert(i < 16);
		int oldInst = currentInst;
		currentInst = i;
		if (i >= nInst) nInst = i+1;
		return oldInst;
	}

	int getCurrentInstrumentNumber()
	{
		return currentInst;
	}


	void setInstrumentPan(double pan)
	{
		Array[currentInst].pan = pan;
	}

	void setInstrumentVol(double vol)
	{
		Array[currentInst].vol = vol;
	}

	void setInstrumentExpr(double expr)
	{
		Array[currentInst].expr = expr;
	}

	void setInstrumentOct(double oct)
	{
		Array[currentInst].oct = oct;
	}

	InstrumentUse* getInstrumentUse(int i)
	{
		return Array + i;
	}

	InstrumentUse* getInstrumentUse()
	{
		return Array + currentInst;
	}

	Instrument *getInstrument(int i)
	{		
		return Array[i].instr;
	}

	// @param inst can be NULL which mean no instrument
	void setInstrument(Instrument *inst)
	{
		if (currentInst >= nInst) nInst = currentInst+1;

		Instrument *&oldInst = Array[currentInst].instr;
		if (oldInst) delete oldInst;
		oldInst = inst;
	}

};


class APReference
{
public:
	double *Amp;
	double *Pan;
	InstrumentUse *instUse;

	void Clean()
	{
		Amp = NULL;
		Pan = NULL;
		instUse = NULL;
	}
	
	double getAmp()
	{
		//cout << *Amp << endl;
		if (Amp == NULL) return 0;
		else 
		{
			return *Amp * instUse->vol * instUse->expr;
		}
	}

	double getPan()
	{
		if (Pan == NULL) return 0;
		else 
		{
			return *Pan + instUse->pan;
		}
	}

	//double getOct()
	//{
	//	if (instUse == NULL) return 0;
	//	else return instUse->oct;		
	//}

};


class APArrayClass
{
private:
	int maxAP;
	
	APReference Array[32];
protected:
	int nAP;

	double getAmp(int i)
	{
		
		if (i < nAP && i >= 0)
		{
			//cout << "kasso" << endl;
			return Array[i].getAmp();
			//cout << "pyte" << endl;
		}
		else return 0;
	}
	
	double getPan(int i)
	{
		if (i < nAP && i >= 0)
		{
			return Array[i].getPan();
		}
		else return 0;
	}

public:
	
	APArrayClass()
	{
		maxAP = 32;
		CleanAPArray();
	}

	void CleanAPArray()
	{
		nAP = 0;
		for(int i=0;i<maxAP;i++)
		{
			Array[i].Clean();
		}
	}


	int APSize()
	{
		return nAP;
	}

	bool addAP(double *Amp, double *Pan, InstrumentUse *instUse)
	{
		if(nAP < maxAP)
		{
			//Array[nAP] = {Amp, Pan, instUse};
			Array[nAP].Amp = Amp;
			Array[nAP].Pan = Pan;
			Array[nAP].instUse = instUse;
			nAP++;
			return true;
		}
		else return false;
	}

};




class SoundCollector : public APArrayClass, public InstrumentArray, public Instrument
{
private:
	double SSPos;

	double Pan;
	
	//double Expr;

protected:

	//APArrayClass APArray;
	//InstrumentArray IArray;

public:
	double SS;

	double LAmp;
	double RAmp;

	double Vel;
	double AfterTouchLevel;

	//double Oct;

	SoundCollector() : APArrayClass(), InstrumentArray(), Instrument()
	{
		LAmp = 0;
		RAmp = 0;
		Pan = 0;
		Vel = 50.0;
		//Expr = 1.0;
		//Oct = 0;
		
		SSPos = 0;
		SS = 1.0/500;
		
		Playing = false;
	}

	void NoteOn()
	{
		bool foundInstrument = false;
		InstrumentUse *IU;
		for(int i=0;i < nInst;i++)
		{
			IU = getInstrumentUse(i);
			if (IU == NULL) continue;
			Instrument *I = IU->instr;
			if (I)
			{
				I->Fre = Fre * IU->oct;
				I->Duration = Duration;
				I->NoteOn();
				foundInstrument = true;
			}
		}
		
		if (foundInstrument)
		{
			SSPos = 0;
			Playing = true;
		}
	}

	
	bool Run()
	{
		Playing = false;
		for(int i=0;i < nInst;i++)
		{
			Instrument *inst = getInstrument(i);
			if (inst) Playing = (getInstrument(i)->Run() || Playing);
		}
		
		double AmpIn, PanIn;
		LAmp = 0;
		RAmp = 0;


		//cout << "fibonacki" << endl;

		for(int j=0;j<nAP;j++)
		{
			AmpIn = getAmp(j);
			PanIn = getPan(j);
			
			
			
			LAmp = LAmp + LVolFunc(AmpIn, Limit(Pan+PanIn, -1.0, 1.0));
			RAmp = RAmp + RVolFunc(AmpIn, Limit(Pan+PanIn, -1.0, 1.0));
			
			//cout << LAmp << "     " << RAmp << endl;
		}
		LAmp = LAmp * Vel * AfterTouchLevel; // added
		RAmp = RAmp * Vel * AfterTouchLevel; // added
		
		if (SSPos < SS)
		{
			LAmp = LAmp*(SSPos/SS);
			RAmp = RAmp*(SSPos/SS);
			
			SSPos = SSPos + TimeStep;
		}
		

		return Playing;
	}
	
	void NoteOff()
	{
		for(int i=0;i < nInst;i++)
		{
			Instrument * I = getInstrument(i);
			if (I) I->NoteOff();
		}
	}

	void AfterTouch()
	{
		
	}

	//void setPan(double Pan)
	//{
	//	if (getCurrentInst() == 0) this->Pan = Pan;			
	//	else setInstrumentPan(Pan);
	//}

	//void setVol(double Vol)
	//{
	//	if (getCurrentInst() == 0) this->Vol = Vol;			
	//	else setInstrumentVol(Vol);
	//}

	//void setExpr(double Expr)
	//{
	//	if (getCurrentInst() == 0) this->Expr = Expr;			
	//	else setInstrumentExpr(Expr);
	//}

	void End()
	{
		for(int i=0;i < nInst;i++)
		{
			getInstrument(i)->End();
		}
	}

	//void deleteAllInstruments()
	//{
	//	Instrument *I;
	//	for(int i=0;i < nInst;i++)
	//	{
	//		I = getInstrument(i);
	//		delete I;
	//	}
	//	CleanInstrumentArray();
	//}

	void setFre(double Fre)
	{
		Instrument *I;
		InstrumentUse *IU;
		this->Fre = Fre;
		
		for(int i=0;i < nInst;i++)
		{
			IU = getInstrumentUse(i);
			if (IU == NULL) continue;
			Instrument *I = IU->instr;
			if (I) I->Fre = Fre * IU->oct;
		}
	}
	

};

#endif