#include "strategy.h"
#include "sensors.h"
#include "motors.h"

void scan(){
	
	while(!poll_sensors()){
		
			motorLeft(100,0);
	}

}

void push(int mode){

	while(fwContact()){
		
		motorLeft(100,0);
		motorRight(100,0);
	}
	
	while(rwContact()){
		
		motorLeft(-100,0);
		motorRight(-100,0);
	}

}

void evade(int left){
	

	if(left){
		
		motorLeft(100,1);
		motorRight(50,1);

	}
	else{
		
		motorLeft(50,1);
		motorRight(100,1);
		
	}


}


void attack(int t) {
  
}

//
//move around in pattern 
//seek opponent and destroyyyyyy!!!!!!!!!!!!
//
void pattern(){
  
}

void win(){
  
}

void moveForward() {
  motorLeft(100, 1);
  motorRight(100, 1);
}