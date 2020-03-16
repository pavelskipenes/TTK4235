#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "ubit.h"

#include "bluetooth.h"

int main(){
	ubit_uart_init();

	uint32_t err_code = 0;

	err_code = bluetooth_init();
	ubit_uart_print("BLE Enable error code: %d\n\r", err_code);

	return 0;
}
