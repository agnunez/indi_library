/*******************************************************************************
  Copyright(c) 2018 Leone Di Mario  <leone DOT dimario AT gmail DOT com>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License version 2 as published by the Free Software Foundation.
 .
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.
 .
 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory>
#include <wiringPi.h>
#include <string.h>

#include "rpi_focus.h"

std::unique_ptr<FocusRpi> focusRpi(new FocusRpi());

#define MAX_STEPS 20000 // maximum steps focuser
#define DIR 5	// GPIO24
#define STEP 4	// GPIO23

void ISPoll(void *p);


void ISInit()
{
   static int isInit = 0;

   if (isInit == 1)
       return;
   if(focusRpi.get() == 0)
   {
       isInit = 1;
       focusRpi.reset(new FocusRpi());
   }
}

void ISGetProperties(const char *dev)
{
        ISInit();
        focusRpi->ISGetProperties(dev);
}

void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int num)
{
        ISInit();
        focusRpi->ISNewSwitch(dev, name, states, names, num);
}

void ISNewText(	const char *dev, const char *name, char *texts[], char *names[], int num)
{
        ISInit();
        focusRpi->ISNewText(dev, name, texts, names, num);
}

void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int num)
{
        ISInit();
        focusRpi->ISNewNumber(dev, name, values, names, num);
}

void ISNewBLOB (const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[], char *formats[], char *names[], int n)
{
  INDI_UNUSED(dev);
  INDI_UNUSED(name);
  INDI_UNUSED(sizes);
  INDI_UNUSED(blobsizes);
  INDI_UNUSED(blobs);
  INDI_UNUSED(formats);
  INDI_UNUSED(names);
  INDI_UNUSED(n);
}

void ISSnoopDevice (XMLEle *root)
{
    ISInit();
    focusRpi->ISSnoopDevice(root);
}

FocusRpi::FocusRpi()
{
	setVersion(1,1);
}

FocusRpi::~FocusRpi()
{

}

const char * FocusRpi::getDefaultName()
{
        return (char *)"LDM Focuser";
}

bool FocusRpi::Connect()
{
    setenv("WIRINGPI_GPIOMEM", "1", 1);
    // putenv ("WIRINGPI_GPIOMEM=1") ;
    if (wiringPiSetup())
    {
		IDMessage(getDeviceName(), "Problem initiating LDM Focuser.");
		return false;
	}

    // init GPIOs
    // Set gpios to output mode
    pinMode (DIR, OUTPUT) ;
    pinMode (STEP, OUTPUT) ;
	
    IDMessage(getDeviceName(), "LDM Focuser connected successfully.");
    return true;
}

bool FocusRpi::Disconnect()
{
	// park focuser
	if ( FocusParkingS[0].s == ISS_ON )
	{
		IDMessage(getDeviceName(), "LDM Focuser is parking...");	
		MoveAbsFocuser(FocusAbsPosN[0].min);
	}
		
	IDMessage(getDeviceName(), "LDM Focuser disconnected successfully.");

    pinMode (DIR, INPUT) ; 
    pinMode (STEP, INPUT) ; 

    return true;
}

bool FocusRpi::initProperties()
{
    INDI::Focuser::initProperties();
	
    IUFillNumber(&MotorDelayN[0],"MOTOR_DELAY","milliseconds","%0.0f",1,50,1,2);
    IUFillNumberVector(&MotorDelayNP,MotorDelayN,1,getDeviceName(),"MOTOR_CONFIG","Step Delay",OPTIONS_TAB,IP_RW,0,IPS_OK);

    IUFillNumber(&FocusAbsPosN[0],"FOCUS_ABSOLUTE_POSITION","Ticks","%0.0f",0,MAX_STEPS,(int)MAX_STEPS/100,0);
    IUFillNumberVector(&FocusAbsPosNP,FocusAbsPosN,1,getDeviceName(),"ABS_FOCUS_POSITION","Position",MAIN_CONTROL_TAB,IP_RW,0,IPS_OK);

    IUFillNumber(&FocusRelPosN[0],"FOCUS_RELATIVE_POSITION","Ticks","%0.0f",0,(int)MAX_STEPS/100,(int)MAX_STEPS/1000,(int)MAX_STEPS/1000);
    IUFillNumberVector(&FocusRelPosNP,FocusRelPosN,1,getDeviceName(),"REL_FOCUS_POSITION","Relative",MAIN_CONTROL_TAB,IP_RW,60,IPS_OK);

    IUFillSwitch(&FocusMotionS[0],"FOCUS_INWARD","Focus In",ISS_OFF);
    IUFillSwitch(&FocusMotionS[1],"FOCUS_OUTWARD","Focus Out",ISS_ON);
    IUFillSwitchVector(&FocusMotionSP,FocusMotionS,2,getDeviceName(),"FOCUS_MOTION","Direction",MAIN_CONTROL_TAB,IP_RW,ISR_ATMOST1,60,IPS_OK);
	
    IUFillNumber(&PresetN[0], "Preset 1", "", "%0.0f", 0, MAX_STEPS, (int)(MAX_STEPS/100), 0);
    IUFillNumber(&PresetN[1], "Preset 2", "", "%0.0f", 0, MAX_STEPS, (int)(MAX_STEPS/100), 0);
    IUFillNumber(&PresetN[2], "Preset 3", "", "%0.0f", 0, MAX_STEPS, (int)(MAX_STEPS/100), 0);
    IUFillNumberVector(&PresetNP, PresetN, 3, getDeviceName(), "Presets", "Presets", OPTIONS_TAB, IP_RW, 0, IPS_IDLE);

    IUFillSwitch(&PresetGotoS[0], "Preset 1", "Preset 1", ISS_OFF);
    IUFillSwitch(&PresetGotoS[1], "Preset 2", "Preset 2", ISS_OFF);
    IUFillSwitch(&PresetGotoS[2], "Preset 3", "Preset 3", ISS_OFF);
    IUFillSwitchVector(&PresetGotoSP, PresetGotoS, 3, getDeviceName(), "Presets Goto", "Goto", MAIN_CONTROL_TAB,IP_RW,ISR_1OFMANY,60,IPS_OK);

    IUFillNumber(&FocusBacklashN[0], "FOCUS_BACKLASH_VALUE", "Steps", "%0.0f", 0, (int)(MAX_STEPS/100), (int)(MAX_STEPS/10000), 0);
    IUFillNumberVector(&FocusBacklashNP, FocusBacklashN, 1, getDeviceName(), "FOCUS_BACKLASH", "Backlash", OPTIONS_TAB, IP_RW, 0, IPS_IDLE);

    IUFillSwitch(&FocusResetS[0],"FOCUS_RESET","Reset",ISS_OFF);
    IUFillSwitchVector(&FocusResetSP,FocusResetS,1,getDeviceName(),"FOCUS_RESET","Position Reset",OPTIONS_TAB,IP_RW,ISR_1OFMANY,60,IPS_OK);

    IUFillSwitch(&FocusParkingS[0],"FOCUS_PARKON","Enable",ISS_OFF);
    IUFillSwitch(&FocusParkingS[1],"FOCUS_PARKOFF","Disable",ISS_OFF);
    IUFillSwitchVector(&FocusParkingSP,FocusParkingS,2,getDeviceName(),"FOCUS_PARK","Parking Mode",OPTIONS_TAB,IP_RW,ISR_1OFMANY,60,IPS_OK);

    // set capabilities
    //SetFocuserCapability(FOCUSER_CAN_ABS_MOVE | FOCUSER_CAN_REL_MOVE);

    return true;
}

void FocusRpi::ISGetProperties (const char *dev)
{
    INDI::Focuser::ISGetProperties(dev);

    /* Add debug controls so we may debug driver if necessary */
    addDebugControl();

    return;
}

bool FocusRpi::updateProperties()
{

    INDI::Focuser::updateProperties();

    if (isConnected())
    {
	deleteProperty(FocusSpeedNP.name);
        defineNumber(&FocusAbsPosNP);
	defineNumber(&FocusRelPosNP);
        defineSwitch(&FocusMotionSP);
	defineNumber(&FocusBacklashNP);
	defineSwitch(&FocusParkingSP);
	defineSwitch(&FocusResetSP);
	defineSwitch(&PresetGotoSP);
	defineNumber(&PresetNP);
	defineNumber(&MotorDelayNP);
    }
    else
    {
        deleteProperty(FocusAbsPosNP.name);
	deleteProperty(FocusRelPosNP.name);
        deleteProperty(FocusMotionSP.name);
	deleteProperty(FocusBacklashNP.name);
	deleteProperty(FocusParkingSP.name);
	deleteProperty(FocusResetSP.name);
	deleteProperty(PresetGotoSP.name);	        
	deleteProperty(PresetNP.name);
	deleteProperty(MotorDelayNP.name);
    }

    return true;
}

bool FocusRpi::ISNewNumber (const char *dev, const char *name, double values[], char *names[], int n)
{
	// first we check if it's for our device
	if(strcmp(dev,getDeviceName())==0)
	{

        // handle focus absolute position
        if (!strcmp(name, FocusAbsPosNP.name))
        {
	    int newPos = (int) values[0];
            if ( MoveAbsFocuser(newPos) == IPS_OK )
            {
               IUUpdateNumber(&FocusAbsPosNP,values,names,n);
               FocusAbsPosNP.s=IPS_OK;
               IDSetNumber(&FocusAbsPosNP, NULL);
            }
            return true;
        }        

        // handle focus relative position
        if (!strcmp(name, FocusRelPosNP.name))
        {
	    IUUpdateNumber(&FocusRelPosNP,values,names,n);
			
			//FOCUS_INWARD
            if ( FocusMotionS[0].s == ISS_ON )
			MoveRelFocuser(FOCUS_INWARD, FocusRelPosN[0].value);

			//FOCUS_OUTWARD
            if ( FocusMotionS[1].s == ISS_ON )
			MoveRelFocuser(FOCUS_OUTWARD, FocusRelPosN[0].value);

			FocusRelPosNP.s=IPS_OK;
			IDSetNumber(&FocusRelPosNP, NULL);
			return true;
        }

        // handle focus timer
        if (!strcmp(name, FocusTimerNP.name))
        {
            IUUpdateNumber(&FocusTimerNP,values,names,n);

			//FOCUS_INWARD
            if ( FocusMotionS[0].s == ISS_ON )
			MoveFocuser(FOCUS_INWARD, 0, FocusTimerN[0].value);

			//FOCUS_OUTWARD
            if ( FocusMotionS[1].s == ISS_ON )
			MoveFocuser(FOCUS_OUTWARD, 0, FocusTimerN[0].value);

			FocusTimerNP.s=IPS_OK;
			IDSetNumber(&FocusTimerNP, NULL);
			return true;
        }

        // handle focus backlash
        if (!strcmp(name, FocusBacklashNP.name))
        {
            IUUpdateNumber(&FocusBacklashNP,values,names,n);
            FocusBacklashNP.s=IPS_OK;
            IDSetNumber(&FocusBacklashNP, "LDM Focuser backlash set to %d", (int) FocusBacklashN[0].value);
            return true;
        }
        
        // handle motor delay
        if (!strcmp(name, MotorDelayNP.name))
        {
            IUUpdateNumber(&MotorDelayNP,values,names,n);
            MotorDelayNP.s=IPS_OK;
            IDSetNumber(&MotorDelayNP, "LDM Focuser motor delay set to %d", (int) MotorDelayN[0].value);
            return true;
        }

	}
    return INDI::Focuser::ISNewNumber(dev,name,values,names,n);
}

bool FocusRpi::ISNewSwitch (const char *dev, const char *name, ISState *states, char *names[], int n)
{
	// first we check if it's for our device
    if (!strcmp(dev, getDeviceName()))
    {
		
        // handle focus motion in and out
        if (!strcmp(name, FocusMotionSP.name))
        {
            IUUpdateSwitch(&FocusMotionSP, states, names, n);

			//FOCUS_INWARD
            if ( FocusMotionS[0].s == ISS_ON )
				MoveRelFocuser(FOCUS_INWARD, FocusRelPosN[0].value);

			//FOCUS_OUTWARD
            if ( FocusMotionS[1].s == ISS_ON )
				MoveRelFocuser(FOCUS_OUTWARD, FocusRelPosN[0].value);

            FocusMotionS[0].s = ISS_OFF;
            FocusMotionS[1].s = ISS_OFF;

	    FocusMotionSP.s = IPS_OK;
            IDSetSwitch(&FocusMotionSP, NULL);
            return true;
        }

        // handle focus presets
        if (!strcmp(name, PresetGotoSP.name))
        {
            IUUpdateSwitch(&PresetGotoSP, states, names, n);

			//Preset 1
            if ( PresetGotoS[0].s == ISS_ON )
				MoveAbsFocuser(PresetN[0].value);

			//Preset 2
            if ( PresetGotoS[1].s == ISS_ON )
				MoveAbsFocuser(PresetN[1].value);

			//Preset 2
            if ( PresetGotoS[2].s == ISS_ON )
				MoveAbsFocuser(PresetN[2].value);

			PresetGotoS[0].s = ISS_OFF;
			PresetGotoS[1].s = ISS_OFF;
			PresetGotoS[2].s = ISS_OFF;
			PresetGotoSP.s = IPS_OK;
            IDSetSwitch(&PresetGotoSP, NULL);
            return true;
        }
                
        // handle focus reset
        if(!strcmp(name, FocusResetSP.name))
        {
			IUUpdateSwitch(&FocusResetSP, states, names, n);

            if ( FocusResetS[0].s == ISS_ON && FocusAbsPosN[0].value == FocusAbsPosN[0].min  )
            {
				FocusAbsPosN[0].value = (int)MAX_STEPS/100;
				IDSetNumber(&FocusAbsPosNP, NULL);
				MoveAbsFocuser(0);
			}
            FocusResetS[0].s = ISS_OFF;
            IDSetSwitch(&FocusResetSP, NULL);
			return true;
		}

        // handle parking mode
        if(!strcmp(name, FocusParkingSP.name))
        {
			IUUpdateSwitch(&FocusParkingSP, states, names, n);
			IDSetSwitch(&FocusParkingSP, NULL);
			return true;
		}

        // handle focus abort - TODO
        if (!strcmp(name, AbortSP.name))
        {
            IUUpdateSwitch(&AbortSP, states, names, n);
            AbortS[0].s = ISS_OFF;
			AbortSP.s = IPS_OK;
            IDSetSwitch(&AbortSP, NULL);
            return true;
        }
    }
    return INDI::Focuser::ISNewSwitch(dev,name,states,names,n);
}

bool FocusRpi::ISSnoopDevice (XMLEle *root)
{
    return INDI::Focuser::ISSnoopDevice(root);
}

bool FocusRpi::saveConfigItems(FILE *fp)
{
    IUSaveConfigNumber(fp, &FocusRelPosNP);
    IUSaveConfigNumber(fp, &PresetNP);
    IUSaveConfigNumber(fp, &FocusBacklashNP);
    IUSaveConfigSwitch(fp, &FocusParkingSP);
    IUSaveConfigNumber(fp, &MotorDelayNP);

    if ( FocusParkingS[0].s == ISS_ON )
		IUSaveConfigNumber(fp, &FocusAbsPosNP);

    return true;
}

IPState FocusRpi::MoveFocuser(FocusDirection dir, int speed, int duration)
{
    int ticks = (int) ( duration / MotorDelayN[0].value );
    return 	MoveRelFocuser( dir, ticks);
}


IPState FocusRpi::MoveRelFocuser(FocusDirection dir, int ticks)
{
    int targetTicks = FocusAbsPosN[0].value + (ticks * (dir == FOCUS_INWARD ? -1 : 1));
    return MoveAbsFocuser(targetTicks);
}

IPState FocusRpi::MoveAbsFocuser(int targetTicks)
{
    if (targetTicks < FocusAbsPosN[0].min || targetTicks > FocusAbsPosN[0].max)
    {
        IDMessage(getDeviceName(), "Requested position is out of range.");
        return IPS_ALERT;
    }
    	
    if (targetTicks == FocusAbsPosN[0].value)
    {
        // IDMessage(getDeviceName(), "LDM Focuser already in the requested position.");
        return IPS_OK;
    }

	// set focuser busy
	FocusAbsPosNP.s = IPS_BUSY;
	IDSetNumber(&FocusAbsPosNP, NULL);

	// set full step size
	SetSpeed(1);
	
	// check last motion direction for backlash triggering
	int lastdir = digitalRead(DIR);

    // set direction
    const char* direction;    
    if (targetTicks > FocusAbsPosN[0].value)
    {
		// OUTWARD
		digitalWrite(DIR, LOW);
		direction = " outward ";
	}
    else
	{
		// INWARD
		digitalWrite(DIR, HIGH);
		direction = " inward ";
	}

    IDMessage(getDeviceName() , "LDM Focuser is moving %s", direction);

	// if direction changed do backlash adjustment
	if ( digitalRead(DIR) != lastdir && FocusAbsPosN[0].value != 0 && FocusBacklashN[0].value != 0 )
	{
		IDMessage(getDeviceName() , "LDM Focuser backlash compensation by %0.0f steps...", FocusBacklashN[0].value);	
		for ( int i = 0; i < FocusBacklashN[0].value; i++ )
		{
			// step on
			digitalWrite(STEP, HIGH);
			// wait
			usleep(MotorDelayN[0].value * 1000);
			// step off
			digitalWrite(STEP, LOW);
			// wait 
			usleep(MotorDelayN[0].value * 1000);
		}
	}

	// process targetTicks
    int ticks = abs(targetTicks - FocusAbsPosN[0].value);

    for ( int i = 0; i < ticks; i++ )
    {
        // step on
	digitalWrite(STEP, HIGH);
        // wait
	usleep(MotorDelayN[0].value * 1000);
        // step off
	digitalWrite(STEP, LOW);
        // wait 
        usleep(MotorDelayN[0].value * 1000);

		// INWARD - count down
		if ( digitalRead(DIR) == HIGH )
			FocusAbsPosN[0].value -= 1;

		// OUTWARD - count up
		if ( digitalRead(DIR) == LOW )
			FocusAbsPosN[0].value += 1;

		IDSetNumber(&FocusAbsPosNP, NULL);
    }

    // update abspos value and status
    IDSetNumber(&FocusAbsPosNP, "LDM Focuser moved to position %0.0f", FocusAbsPosN[0].value);
	FocusAbsPosNP.s = IPS_OK;
	IDSetNumber(&FocusAbsPosNP, NULL);
	
    return IPS_OK;
}

bool FocusRpi::SetSpeed(int speed)
{
    switch(speed)
    {
    case 1:	// 1:1
        break;
    default:	// 1:1
        break;
    }
	return true;
}

