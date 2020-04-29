#include "button.h"
#include "led_matrix.h"

int main(){

    button_init();
    led_matrix_init();

	int sleep = 0;
	while(1){

        if( button_a_pressed() ) led_matrix_turn_on();
        if( button_b_pressed() ) led_matrix_turn_off();

		sleep = 10000;
		while(--sleep);
	}
	return 0;
}