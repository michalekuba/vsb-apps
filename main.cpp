#include "mbed.h"
#define T 20
int led_while = 0;
DigitalIn g_but_PTC9( PTC9);
DigitalIn g_but_PTC10( PTC10 );
DigitalIn g_but_PTC11( PTC11 );
DigitalIn g_but_PTC12( PTC12 );
class PWMLED{
	public:
		Ticker m_ticker;
		DigitalOut m_led;
		uint8_t m_jas_proc;   // jas v %
		uint32_t m_T1;        // nebo cas T0
		uint32_t m_ticks;
	PWMLED( DigitalOut t_led_pin ) : m_led( t_led_pin ){
	// inicializace promennych
		this->m_jas_proc = 0;
		this->m_T1 = 0;
		this->m_ticks = 0;
		this->m_ticker.attach( callback( this, &PWMLED::pwm ), 1ms );
	}
    void nastav_jas_proc( uint8_t t_jas_proc ){
    	if (t_jas_proc <= 100){
            this->m_jas_proc = t_jas_proc;
            this->m_T1 = (T * this->m_jas_proc) / 100;
    	}
    }
    void pwm(){
    	// ovladani m_led
		if ( this->m_ticks < this->m_T1 ){	// time to switch on?
			this->m_led = 1;				// LED On
		} else {
			this->m_led = 0;
		}
		this->m_ticks++;						// milliseconds counter
		if ( this->m_ticks >= T ){			// End of periode?
			this->m_ticks = 0;				// start periode again			// LED Off
		}
    }
};
PWMLED g_red_led_leva[] = { { PTC0 }, { PTC1 }, { PTC2 }, { PTC3 } };
PWMLED g_red_led_prava[] = { { PTC4 }, { PTC5 }, { PTC7 }, { PTC8 } };
PWMLED g_rgb_led[] = { { PTB2 }, { PTB3 }, { PTB9 }, { PTB10 }, { PTB11 }, { PTB18 }, { PTB19 }, { PTB20 }, { PTB23 } };
class TickerBlinkr{
	public:
		Ticker blinkr_ticker;
		Ticker svetlo_ticker;
		bool sviti_svetla = false;
		int pocet_leva = 0;
		int wait_leva = 0;
		bool sviti_leva = false;
		int pocet_prava = 0;
		int wait_prava = 0;
		bool sviti_prava = false;
	TickerBlinkr(){
		this->blinkr_ticker.attach( callback(this, &TickerBlinkr::blinkr ), 15ms);
		this->svetlo_ticker.attach( callback(this, &TickerBlinkr::ledky), 20ms);
	}
	void blinkr(){
		if (!g_but_PTC9){
			this->sviti_leva = true;
		}
		if (!g_but_PTC12){
			this->sviti_prava = true;
		}
		if (this->pocet_leva < 3 && this->sviti_leva){
			if (g_red_led_leva[0].m_jas_proc < 100){
				g_red_led_leva[0].nastav_jas_proc(g_red_led_leva[0].m_jas_proc + 5);
			} else if (g_red_led_leva[1].m_jas_proc < 100){
				g_red_led_leva[1].nastav_jas_proc(g_red_led_leva[1].m_jas_proc + 5);
			} else if (g_red_led_leva[2].m_jas_proc < 100){
				g_red_led_leva[2].nastav_jas_proc(g_red_led_leva[2].m_jas_proc + 5);
			} else if (g_red_led_leva[3].m_jas_proc < 100){
				g_red_led_leva[3].nastav_jas_proc(g_red_led_leva[3].m_jas_proc + 5);
			} else if (this->wait_leva < 300) {
				this->wait_leva += 50;
			} else {
				this->pocet_leva += 1;
				this->wait_leva = 0;
				g_red_led_leva[0].nastav_jas_proc(0);
				g_red_led_leva[1].nastav_jas_proc(0);
				g_red_led_leva[2].nastav_jas_proc(0);
				g_red_led_leva[3].nastav_jas_proc(0);
			}
		} else {
			this->pocet_leva = 0;
			this->sviti_leva = false;
		}
		if (this->pocet_prava < 3 && this->sviti_prava){
			if (g_red_led_prava[3].m_jas_proc < 100){
				g_red_led_prava[3].nastav_jas_proc(g_red_led_prava[3].m_jas_proc + 5);
			} else if (g_red_led_prava[2].m_jas_proc < 100){
				g_red_led_prava[2].nastav_jas_proc(g_red_led_prava[2].m_jas_proc + 5);
			} else if (g_red_led_prava[1].m_jas_proc < 100){
				g_red_led_prava[1].nastav_jas_proc(g_red_led_prava[1].m_jas_proc + 5);
			} else if (g_red_led_prava[0].m_jas_proc < 100){
				g_red_led_prava[0].nastav_jas_proc(g_red_led_prava[0].m_jas_proc + 5);
			} else if (this->wait_prava < 300) {
				this->wait_prava += 50;
			} else {
				this->pocet_prava += 1;
				this->wait_prava = 0;
				g_red_led_prava[3].nastav_jas_proc(0);
				g_red_led_prava[2].nastav_jas_proc(0);
				g_red_led_prava[1].nastav_jas_proc(0);
				g_red_led_prava[0].nastav_jas_proc(0);
			}
		} else {
			this->pocet_prava = 0;
			this->sviti_prava = false;
		}
		return;
	}
	void nastav_svetla(int kraj, int prostr, bool sviti_svetla){
		g_rgb_led[0].nastav_jas_proc(kraj);
		g_rgb_led[1].nastav_jas_proc(kraj);
		g_rgb_led[2].nastav_jas_proc(kraj);
		// 2. RGB brzdové světlo
		g_rgb_led[3].nastav_jas_proc(prostr);
		// 3. RGB kraj světel
		g_rgb_led[6].nastav_jas_proc(kraj);
		g_rgb_led[7].nastav_jas_proc(kraj);
		g_rgb_led[8].nastav_jas_proc(kraj);
		this->sviti_svetla = sviti_svetla;
		// deinicializace
		if (sviti_svetla == false){
			g_red_led_leva[0].nastav_jas_proc(kraj);
			g_red_led_leva[1].nastav_jas_proc(kraj);
			g_red_led_leva[2].nastav_jas_proc(kraj);
			g_red_led_leva[3].nastav_jas_proc(kraj);
			g_red_led_prava[0].nastav_jas_proc(kraj);
			g_red_led_prava[1].nastav_jas_proc(kraj);
			g_red_led_prava[2].nastav_jas_proc(kraj);
			g_red_led_prava[3].nastav_jas_proc(kraj);
			this->sviti_leva = sviti_svetla;
			this->sviti_prava = sviti_svetla;
			this->wait_leva = 0;
			this->wait_prava = 0;
			this->pocet_leva = 0;
			this->pocet_prava = 0;
		}
		return;
	}
	void ledky(){
		if(!g_but_PTC11){
			g_rgb_led[3].nastav_jas_proc(100);
		} else if (g_but_PTC11){
			if(sviti_svetla){
				g_rgb_led[3].nastav_jas_proc(5);
			} else {
				g_rgb_led[3].nastav_jas_proc(0);
			}
		}
		if(!g_but_PTC10 && !g_but_PTC11){
			nastav_svetla(20, 5, true);
		}
		if (!g_but_PTC10 && g_but_PTC11){
			nastav_svetla(0, 0, false);
		}
		return;
	}
};
int main(){
	TickerBlinkr t;
    while ( 1 ) __WFI();
}