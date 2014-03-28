#include "Logging.h"

Logging::Logging(){

	fp = fopen("test.log","w");
	if(fp== NULL){
		//err
	}
}

Logging::~Logging(){
	if(fp!= NULL){
		fclose(fp);
	}
}