#include "mbed.h"

#define T 20

/*
DigitalOut g_led_1(PTC0, 1);
DigitalOut g_led_2(PTC1, 0);
DigitalOut g_led_3(PTC2, 0);
DigitalOut g_led_4(PTC3, 0);
DigitalOut g_led_5(PTC4, 0);
DigitalOut g_led_6(PTC5, 0);
DigitalOut g_led_7(PTC7, 0);
DigitalOut g_led_8(PTC8, 0);
*/

DigitalIn g_but_1(PTC9);
DigitalIn g_but_2(PTC10);
DigitalIn g_but_3(PTC11);
DigitalIn g_but_4(PTC12);

class PWMLED{
public:
    Ticker m_ticker;
    DigitalOut m_led;
    uint8_t m_jas_proc;
    uint32_t m_T1;
    uint32_t m_ticks;

    PWMLED(PinName t_led_pin) : m_led(t_led_pin){
    	m_jas_proc = 0;
    	m_T1 = 0;
    	m_ticks = 0;
        m_ticker.attach(callback(this, &PWMLED::pwm), 1ms);
    }

    void nastav_jas_proc(uint8_t t_jas_proc){
        m_jas_proc = t_jas_proc;
        m_T1 = T*t_jas_proc/100;
    }

    void pwm(){
    	if(m_ticks < m_T1){
    		m_led = 1;
    	}

    	else{
    		m_led = 0;
    	}

        m_ticks++;

        if(m_ticks >= T){
            m_ticks = 0;
        }
    }
};

PWMLED g_red_led[] = {{PTC0}, {PTC1}, {PTC2}, {PTC3}, {PTC4}, {PTC5}, {PTC7}, {PTC8}};
PWMLED g_rgb_led[] = {{PTB2}, {PTB3}, {PTB9}, {PTB10}, {PTB11}, {PTB18}, {PTB19}, {PTB20}, {PTB23}};

class Had{
public:
	enum smer{vlevo, vpravo};

	Ticker t;
	bool spusteno;
	smer s;
	int delkaHada;
	int pocetLed;

	Had(){
		t.attach(callback(this, &Had::pohyb), 100ms);
		spusteno = false;
		s = vlevo;
		delkaHada = 0;
		pocetLed = -1;
	}

	void pohyb(){
		if(!spusteno){

			g_rgb_led[0].nastav_jas_proc(10);

			if(!g_but_2){
				if(delkaHada < 8){
					g_rgb_led[delkaHada + 1].nastav_jas_proc(10);
					delkaHada++;
				}
			}

			else if(!g_but_1){
				if(delkaHada > -1){
					g_rgb_led[delkaHada + 1].nastav_jas_proc(0);
					delkaHada--;
				}
			}

			else if(!g_but_3){
				if(pocetLed < delkaHada){
					g_red_led[pocetLed + 1].nastav_jas_proc(10);
					pocetLed++;
				}
			}
		}
	}
};

int main(){
    Had n;

    while(1) __WFI();
}
