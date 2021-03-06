#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "headers/modes.h"
#include "headers/elevator.h"
#include "headers/hardware.h"
#include "headers/interface.h"
#include "headers/modeSelector.h"

static void sigHandler(int sig) {

	switch(sig){
		case SIGSEGV:
			printf("[Warning]: Received Segmentation fault.\n");
			hardware_command_movement(HARDWARE_MOVEMENT_STOP);
			exit(1);
			break;
		default:
			printf("[Error]: Received signal %d, Terminating elevator\n", sig);
			hardware_command_movement(HARDWARE_MOVEMENT_STOP);
			exit(0);
	}
}

void startUp(Elevator* elevator) {
	// connect to hardware
	if (hardware_init()) {
		fprintf(stderr, "[Error]: Unable to initialize hardware\n");
		exit(1);
	}

	// clear old and random data
	elevator->status = UNKNOWN;
	elevator->direction = NONE;
	elevator->emergencyState = false;
	elevatorStop();
	clearAllOrders(elevator);
	closeDoor();

	// crash handling
	printf("[Info]: To terminate program run: kill -15 %d\n", getpid());
	signal(SIGTERM, sigHandler);
	signal(SIGSEGV, sigHandler);

	// find floor
	updateLastFloor(elevator);
	if (!atSomeFloor()) {
		elevatorMoveUp();
		while (!atSomeFloor()) {
			updateLastFloor(elevator);
		}
	}

	printf("[Info]: Elevator started\n");
	elevatorStop();
}

void idle(Elevator* elevator) {
	while (!elevator->hasOrders) {
		if(readStop() || orderAt(elevator, elevator->lastKnownFloor)){
			return;
		}

		updateOrders(elevator);
	}
}

void running(Elevator* elevator) {
	while (elevator->hasOrders) {
		// update sensors and set direction
		updateLastFloor(elevator);
		updateOrders(elevator);
		findTargetFloor(elevator);

		// wait until a floor with orders is reached
		while(!atTargetFloor(elevator)){

			elevator->direction == UP ? elevatorMoveUp() : elevatorMoveDown();

			updateOrders(elevator);
			updateLastFloor(elevator);

			if(readStop()){
				return;
			}

			findTargetFloor(elevator);
			updateDirection(elevator);
		}

		elevatorStop();
		return;
	}
}

void serveFloor(Elevator* elevator){
	updateLastFloor(elevator);
	clearAllOrdersAtThisFloor(elevator);
	elevatorStop();
	openDoor();

	// start a timer
	time_t startTime = clock() / CLOCKS_PER_SEC;
	time_t start = startTime;
	time_t endTime = startTime + DOOR_OPEN_TIME;

	while (startTime < endTime){
		// reset timer on obstructions
		if(readObstruction() || readStop() || orderAt(elevator, elevator->lastKnownFloor)){
			if(readStop()){
				clearAllOrders(elevator);
			}
			endTime = clock() / CLOCKS_PER_SEC + DOOR_OPEN_TIME;
			clearAllOrdersAtThisFloor(elevator);
		}

		updateOrders(elevator);
		startTime = clock()/ CLOCKS_PER_SEC;
	}
	printf("[Info]: The door was open for %d seconds\n", (int)(endTime - start));
	closeDoor();
	return;

}

void emergency(Elevator* elevator) {
	// Note: active only between floors
	elevator->emergencyState = true;
	clearAllOrders(elevator);
	elevatorStop();

	while(readStop()){

	}
	return;
}
