/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#define T 20
/*
DigitalOut g_led_1( PTC0);
DigitalOut g_led_2( PTC1);
DigitalOut g_led_3( PTC2);
DigitalOut g_led_4( PTC3);
DigitalOut g_led_5( PTC4);
DigitalOut g_led_6( PTC5);
DigitalOut g_led_7( PTC7);
DigitalOut g_led_8( PTC8);
*/
DigitalIn but_1( PTC9 );
DigitalIn but_2( PTC10 );
DigitalIn but_3( PTC11 );
DigitalIn but_4( PTC12 );


class PWMLED
{
public:
    Ticker m_ticker;
    DigitalOut m_led;
    uint8_t m_jas_proc;   // jas v %
    uint32_t m_T1;        // nebo cas T0
    uint32_t m_ticks;

  PWMLED( PinName t_led_pin ) : m_led( t_led_pin )
    {
    // inicializace promennych
	  m_jas_proc = 0;		// max jas10
	  m_T1 = 0;
	  m_ticks = 0;
      m_ticker.attach( callback( this, &PWMLED::pwm ), 1ms );
    }

    void nastav_jas_proc( uint8_t t_jas_proc )
    {
        m_jas_proc = t_jas_proc;
        m_T1 = vypocti_jas( t_jas_proc, T);
    }

    uint32_t vypocti_jas( uint8_t t_jas, int t){
    	uint32_t jas = (( t *t_jas) / 100);
    	return jas;
    }

    void pwm()
    {
    // ovladani m_led
    	if ( m_ticks < m_T1 ){
    		m_led = 1;
    	}
    	else {
        	m_led = 0;
    	}
    	m_ticks++;



        if ( m_ticks >= T ){
            m_ticks = 0;
        }
    }

};

PWMLED led[] = { { PTC0 }, { PTC1 }, { PTC2 }, { PTC3 }, { PTC4 }, { PTC5 }, { PTC7 }, { PTC8 } };
PWMLED rgb[] = { {PTB2}, {PTB3}, {PTB9}, {PTB10}, {PTB11}, {PTB18}, {PTB19}, {PTB20}, {PTB23} };

class Had{
	public:
		Ticker t_had;
		bool moving;
	Had(){
		t_had.attach( callback(this, &Had::move_snake ), 50ms);
	}

	void move_snake(){
		if ( !but_1 ){
			moving = true;

			if ( moving ){
				for( int i=0; i<9; i++){
					if ( i < 8){
					led[ i ].nastav_jas_proc( led[i+1].m_jas_proc );
					}
					else if ( i == 8 ){
						led[ i ].nastav_jas_proc( led[ 0 ].m_jas_proc );
					}
				}
			}
		}
		else if ( !but_1 ){
			moving = false;
		}

		return;
	}


};

class ZmenJas{
	public:
		Ticker t_jas;
	ZmenJas(){
		this->t_jas.attach( callback(this, &ZmenJas::Zmena ), 50ms);
	}

	void Zmena(){
		if ( !but_2 ){
			if ( led[ 2 ].m_jas_proc < 100 ){
				led[ 2 ].nastav_jas_proc( led[ 2 ].m_jas_proc + 5 );
			}
			else {
				led[ 2 ].nastav_jas_proc( 0 );
			}

		}
		else if ( !but_3 ){
			if ( led[ 2 ].m_jas_proc > 0 ){
				led[ 2 ].nastav_jas_proc( led[ 2 ].m_jas_proc - 5 );
			}
			else {
				led[ 2 ].nastav_jas_proc( 100 );
			}

		}
		return;
	}
};

class ZmenJasAuto{
	public:
		Ticker t_jas_auto;
		bool zmacknuto;

	ZmenJasAuto(){
		this->t_jas_auto.attach( callback(this, &ZmenJasAuto::ZmenaAuto ), 50ms);
	}

	void ZmenaAuto(){
		if ( !but_1 ){
			zmacknuto = true;
		}
		if ( !but_4 ){
			zmacknuto = false;
		}
		if ( zmacknuto ){

			if ( led[ 4 ].m_jas_proc < 100 ){
				led[ 4 ].nastav_jas_proc( led[ 4 ].m_jas_proc + 5 );
			}
			else {
				led[ 4 ].nastav_jas_proc( 0 );
			}

		}
		else if ( zmacknuto == false ){

			if ( led[ 4 ].m_jas_proc > 0 ){
				led[ 4 ].nastav_jas_proc( led[ 4 ].m_jas_proc - 5 );
			}
			else {
				led[ 4 ].nastav_jas_proc( 100 );
			}

		}
		return;
	}
};

class Nadrz{
	public:
		Ticker t_nadrz;
		//Ticker t_napust;
		bool system_off = false;
		bool vylivam = false;
		int i = 1;
		int i_rgb = 0;
		int idx = 0;

	Nadrz(){
		this->t_nadrz.attach( callback(this, &Nadrz::System ), 100ms);
		//this->t_napust.attach( callback(this, &Nadrz::Napust ), 100ms);
		//this->t_hladina.attach( callback(this, &Nadrz::System ), 1500ms);
	}

	void System(){
		if ( system_off == false){
			rgb[ 4 ].nastav_jas_proc( 10 );				// system off

		}
		if ( system_off == false){
			led[ i ].nastav_jas_proc( 10 );
			if ( !but_4 ){ 								// posun doprava
				led[ i ].nastav_jas_proc( 0 );
				if ( i<7){
					i++;
				} else{
					i=1;
				}
				led[ i ].nastav_jas_proc( 10 );
			}
			else if ( !but_3 ){								// posun doleva
				led[ i ].nastav_jas_proc( 0 );
				if ( i>1){
					i--;
				} else{
					i=7;
				}
				led[ i ].nastav_jas_proc( 10 );
				}
		}
		if ( !but_3 && !but_4){
			system_off = true;
			for (i_rgb = 0; i_rgb<3; i_rgb++){
				rgb[ i_rgb ].nastav_jas_proc( 20 );
			}

		}

		if ( system_off == true){

			if ( idx < i && vylivam == false){
				if ( led[ idx ].m_jas_proc < 100){
					led[ idx ].nastav_jas_proc( led[ idx ].m_jas_proc + 10 );
				}
				else{
					idx++;
				}
			}
			if ( !but_1 || vylivam == true ){
				vylivam = true;
				if ( led[ idx-1 ].m_jas_proc > 0){
					led[ idx-1 ].nastav_jas_proc( led[ idx-1 ].m_jas_proc - 10 );
				}
				else{
					idx--;
				}
				if (idx == 0 && system_off == true){
					vylivam = false;
				}
			}
		}


		if ( !but_2 ){
			system_off = false;
			idx = 0;
			for (int c = 0; c<8; c++){
				led[ c ].nastav_jas_proc( 0 );
			}
			for (int e = 0; e<3; e++){
				rgb[ e ].nastav_jas_proc( 0 );
			}
			led[ i ].nastav_jas_proc( 10 );
			rgb[ 4 ].nastav_jas_proc( 10 );
		}
		return;
	}



};

int main()
{

	//Had had;
	//ZmenJas t;
	//ZmenJasAuto a;
	Nadrz b;

    while ( 1 ) __WFI();
}
