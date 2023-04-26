/*
 * flo_edge.h
 *
 *  Created on: Apr 14, 2023
 *      Author: aditya
 */

#ifndef INC_FLO_EDGE_H_
#define INC_FLO_EDGE_H_

#define PUSH_BUTTON_PRESS_DURATION_MS 1000
#define BOOT_POWER_BUTTON_HOLD_TIME_MS 6000
#define FASTBOOT_TO_BOOT_POWER_BUTTON_HOLD_TIME_MS 7500
#define FASTBOOT_POWER_BUTTON_HOLD_TIME_MS 6500
#define SHUTDOWN_POWER_BUTTON_HOLD_TIME_MS 2000
#define EXTENDED_SHUTDOWN_DURATION_MS 8000
#define TIMEOUT_DURATION_MS 45000

#define BLINK_DELAY 50


void press_vp(){
	HAL_GPIO_WritePin(GPIOB, VOLUME_UP_Pin, GPIO_PIN_SET);
}
void release_vp(){
	HAL_GPIO_WritePin(GPIOB, VOLUME_UP_Pin, GPIO_PIN_RESET);
}
void press_vm(){
	HAL_GPIO_WritePin(GPIOB, VOLUME_DOWN_Pin, GPIO_PIN_SET);
}
void release_vm(){
	HAL_GPIO_WritePin(GPIOB, VOLUME_DOWN_Pin, GPIO_PIN_RESET);
}
void press_p(){
	HAL_GPIO_WritePin(GPIOB, POWER_Pin, GPIO_PIN_SET);
}
void release_p(){
	HAL_GPIO_WritePin(GPIOB, POWER_Pin, GPIO_PIN_RESET);
}

/* state 1 means on 0 means off*/
void set_debug_led(int state){
	if(state == 0){
		// Turn off debug led
		HAL_GPIO_WritePin(GPIOB, DEBUG_LED_Pin, GPIO_PIN_RESET);
	}else if(state == 1){
		// Turn on debug led
		HAL_GPIO_WritePin(GPIOB, DEBUG_LED_Pin, GPIO_PIN_SET);
	}
}

/*
 * red led is on if r is 1 and off if 0
 * green led is on if g is 1 and off if 0
 * blue led is on if b is 1 and off if 0
 */
void set_rgb_led(int r, int g, int b){
	if(r == 0){
		HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_RESET);
	}else if(r == 1){
		HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, GPIO_PIN_SET);
	}

	if(g == 0){
		HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_RESET);
	}else if(g == 1){
		HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, GPIO_PIN_SET);
	}

	if(b == 0){
		HAL_GPIO_WritePin(GPIOB, BLUE_LED_Pin, GPIO_PIN_RESET);
	}else if(b == 1){
		HAL_GPIO_WritePin(GPIOB, BLUE_LED_Pin, GPIO_PIN_SET);
	}
}

/* returns 1 if button is pressed else returns 0*/
int read_push_button(){
	GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, BOOT_BUTTON_Pin);
	int push_button = 0;
	if(state == GPIO_PIN_RESET){
		push_button = 1;
	}else if(state == GPIO_PIN_SET){
		push_button = 0;
	}
	return push_button;
}

/* returns 0 or 1 based on state*/
int read_boot_mode(){
	GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, BOOT_MODE_Pin);
	int boot_mode = 0;
	if(state == GPIO_PIN_RESET){
		boot_mode = 0;
	}else if(state == GPIO_PIN_SET){
		boot_mode = 1;
	}
	return boot_mode;
}

/* returns 1 if haptic is on and 0 if off*/
int read_haptic(){
	GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, HAPTICS_FB_Pin);
	int haptic = 0;
	if(state == GPIO_PIN_RESET){
		haptic = 0;
	}else if(state == GPIO_PIN_SET){
		haptic = 1;
	}
	return haptic;
}

void flo_edge_init(){
	release_vp();
	release_vm();
	release_p();
	set_debug_led(0);
	set_rgb_led(1, 0, 0);
}

void boot_main(){
	// 1. Phone on
	int rgb_led_state = 0;
	unsigned long t = HAL_GetTick();
	unsigned long t_haptic = HAL_GetTick();
	while(1){
		if(read_push_button() != 1){
			t = HAL_GetTick();
			set_debug_led(0);
		}else{
			set_debug_led(1);
		}

		if(HAL_GetTick() - t_haptic > TIMEOUT_DURATION_MS){
			break;
		}

		int haptic = read_haptic();
		if(haptic == 1){
			t_haptic = HAL_GetTick();
			set_rgb_led(0, 1, 0);
			if(HAL_GetTick() - t > PUSH_BUTTON_PRESS_DURATION_MS){
				set_debug_led(0);
				break;
			}
		}else{
			if(rgb_led_state == 0){
				rgb_led_state = 1;
			}else{
				rgb_led_state = 0;
			}
			set_rgb_led(rgb_led_state, 0, 0);
		}
		HAL_Delay(BLINK_DELAY);
	}

	// 2. Red blinking until power button press
	t = HAL_GetTick();
	press_p();
	rgb_led_state = 0;
	while(HAL_GetTick() - t < SHUTDOWN_POWER_BUTTON_HOLD_TIME_MS){
		if(rgb_led_state == 0){
			rgb_led_state = 1;
		}else{
			rgb_led_state = 0;
		}
		set_rgb_led(rgb_led_state, 0, 0);
		HAL_Delay(BLINK_DELAY);
	}
	release_p();

	// 3. Shutting down
	while(1){
		int haptic = read_haptic();
		if(haptic == 0){
			set_rgb_led(1, 0, 0);
			break;
		}else{
			if(rgb_led_state == 0){
				rgb_led_state = 1;
			}else{
				rgb_led_state = 0;
			}
			set_rgb_led(rgb_led_state, 0, 0);
		}
		HAL_Delay(BLINK_DELAY);
	}

	// 4. Wait for EXTENDED_SHUTDOWN_DURATION
	t = HAL_GetTick();
	rgb_led_state = 0;
	while(HAL_GetTick() - t < EXTENDED_SHUTDOWN_DURATION_MS){
		if(rgb_led_state == 0){
			rgb_led_state = 1;
		}else{
			rgb_led_state = 0;
		}
		set_rgb_led(rgb_led_state, 0, 0);
		HAL_Delay(BLINK_DELAY);
	}
}

void fastboot_main(){
	// Fastboot mode
	set_rgb_led(0, 0, 1);
	unsigned long t = HAL_GetTick();
	unsigned long t_push_button = HAL_GetTick();
	while(1){
		if(read_push_button() != 1){
			t_push_button = HAL_GetTick();
			set_debug_led(0);
		}else{
			set_debug_led(1);
		}

		if(HAL_GetTick() - t_push_button > PUSH_BUTTON_PRESS_DURATION_MS){
			set_debug_led(0);

			// Red blinking until power button press
			t = HAL_GetTick();
			press_p();
			int rgb_led_state = 0;
			while(HAL_GetTick() - t < FASTBOOT_TO_BOOT_POWER_BUTTON_HOLD_TIME_MS){
				if(rgb_led_state == 0){
					rgb_led_state = 1;
				}else{
					rgb_led_state = 0;
				}
				set_rgb_led(rgb_led_state, 0, 0);
				HAL_Delay(BLINK_DELAY);
			}
			release_p();

			break;
		}

		int haptic = read_haptic();
		if(haptic == 0){
			t = HAL_GetTick();
		}
		if(HAL_GetTick() - t > 1000){
			break;
		}
		HAL_Delay(BLINK_DELAY);
	}
}

void flo_edge_main(){
	// 1. Wait for boot signal
	unsigned long t = HAL_GetTick();
	while(1){
		if(HAL_GetTick() - t > PUSH_BUTTON_PRESS_DURATION_MS){
			set_debug_led(0);
			break;
		}
		if(read_push_button() != 1){
			t = HAL_GetTick();
			set_debug_led(0);
		}else{
			set_debug_led(1);
		}
		HAL_Delay(1);
	}

	// 2. Check whether to boot in normal or fastboot mode
	if(read_boot_mode() == 0){ // normal mode
		// Red blinking until power button press
		t = HAL_GetTick();
		press_p();
		int rgb_led_state = 0;
		while(HAL_GetTick() - t < BOOT_POWER_BUTTON_HOLD_TIME_MS){
			if(rgb_led_state == 0){
				rgb_led_state = 1;
			}else{
				rgb_led_state = 0;
			}
			set_rgb_led(rgb_led_state, 0, 0);
			HAL_Delay(BLINK_DELAY);
		}
		release_p();

		boot_main();
	}else if(read_boot_mode() == 1){// fastboot mode
		// Red blinking until power and v- button press
		t = HAL_GetTick();
		press_p();
		press_vm();
		int rgb_led_state = 0;
		while(HAL_GetTick() - t < FASTBOOT_POWER_BUTTON_HOLD_TIME_MS){
			if(rgb_led_state == 0){
				rgb_led_state = 1;
			}else{
				rgb_led_state = 0;
			}
			set_rgb_led(rgb_led_state, 0, 0);
			HAL_Delay(BLINK_DELAY);
		}
		release_p();
		release_vm();

		fastboot_main();
		boot_main();
	}
}

#endif /* INC_FLO_EDGE_H_ */
