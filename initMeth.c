/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 - 2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$(C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0

#include "method.h"

/*:=MPB=:=======================================================*
 *
 * Global Function: initMeth
 *
 * Description: This procedure is implicitly called when this
 *	method is selected.
 *
 * Error History:
 *
 * Interface:							*/

void initMeth()
/*:=MPE=:=======================================================*/
{
  int dimRange[2] = { 1,3 };
  int lowMat[3]   = { 8, 8, 8 };
  int upMat[3]    = { 2048, 2048, 2048 };
  int defaultMat[3] = {128, 128, 64};

  DB_MSG(( "-->FLASH:initMeth\n" ));



  /*  initialize local and redirected parameters */

  NAveragesRange();
  InversionTimeRange();
  if(ParxRelsParHasValue("PVM_NMovieFrames") == No)
  {
    PVM_NMovieFrames = 1;
  }
  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
  {
    PVM_NRepetitions = 1;
  }

     //Line ON Enc OFF
  //if(ParxRelsParHasValue("PhaseRes") == No)
 // {
      if(ParxRelsParHasValue("PreBaseLineNum") == No)
      {
        PreBaseLineNum = 10;
      }
      if(ParxRelsParHasValue("StimNum") == No)
      {    
        StimNum = 10;
      }
      if(ParxRelsParHasValue("InterStimNum") == No)
      {
        InterStimNum = 40;
      }
      if(ParxRelsParHasValue("NEpochs") == No)
      {
        NEpochs = 1;
      }
 // }
  /* encoding group */
  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);

  /* Initialisation of rf pulse parameters */


  /*
   * 1: pulses declared in parDefinitions.h
   * in this case - ExcPulse1. We initalise it to default name,
   * 5000.0 Hz, and 30 deg flip angle
   * In addition we declare ExcPulse1Enum ExcPulse1Ampl and ExcPulse1Shape
   * to be handled together with pulse struct ExcPulse1. Only if a double
   * array parameter is provided as shape, the pulse supports calculated
   * shapes.
   */

  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "Calculated",     // default shape
                  3000.0,           // default bandwidth
                  30.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();

  /* Initialisation of nucleus */
  STB_InitNuclei(1);

  /* Initialisation of geometry parameters */
  STB_InitImageGeometry(true);

  /* 1: method specific initialisation */

  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 100.0;
  if(ParxRelsParHasValue("PVM_EchoTime") == No)
    PVM_EchoTime = 4.0;
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;
  if(!ParxRelsParHasValue("GopAdj"))
    GopAdj = Yes;
  IsGrappaAdj = No;

  /* Initialisation of spoilers */
  MRT_InitSpoiler("ReadSpoiler");
  MRT_InitSpoiler("SliceSpoiler");

  if (ParxRelsParHasValue("PVM_MotionSupOnOff") == 0)
     PVM_MotionSupOnOff = Off;

  EffSWhRange();

  /* Initialisation of modules */
  STB_InitFatSupModule();
  STB_InitSatTransModule();
  STB_InitFovSatModule();
  STB_InitFlowSaturationModule();
  STB_InitTriggerModule();
  STB_InitTaggingModule();
  STB_InitEvolutionModule();
  STB_InitSelIrModule();
  STB_InitBlBloodModule();
  if(!ParxRelsParHasValue("RFSpoiling"))
    RFSpoiling=No;
  if(!ParxRelsParHasValue("AngioMode"))
    AngioMode=No;
  STB_InitDummyScans(1000.0);
  STB_InitDriftComp(On);

  /* initialize mapshim parameter group */
  STB_InitMapShim();
  /* initialize startup shim parameter group */
  STB_InitStartupShims();

  /* initialize method ats parameter group */
  STB_AtsInit();

  /* initialization of method specific reconstruction */
  if(ParxRelsParHasValue("RecoMethMode") == No)
    RecoMethMode=Default;
  if(ParxRelsParHasValue("WeightingMode") == No)
    WeightingMode=positive_mask;
  GaussBroadRange();
  MaskWeightRange();

  /* Visibility of parameters */
  ParxRelsParMakeNonEditable({"PVM_EchoPosition"});
  ParxRelsParMakeNonEditable({"PVM_MinEchoTime","PVM_AcquisitionTime"});
  ParxRelsParMakeNonEditable({"EncGradDur"});
  ParxRelsParShowInEditor({"PVM_EncPpiRefScan"});
  /*
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent
   */

  backbone();

  DB_MSG(("<--FLASH:initMeth\n"));
}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
